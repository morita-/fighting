#pragma warning(disable:4819)

#include "Mesh.h"
#include "..\LibUtil\LibUtil\LibUtil.h"


//==============================================================
// コンストラクタ，デストラクタ

// コンストラクタ
CMesh::CMesh(LPDIRECT3DDEVICE9 device, string file, CArchiveLoader* archive)
:	Device(device),
	Mesh(NULL), NumMaterials(0), 
	Materials(NULL), Textures(NULL),
	ColorMultiplier(1, 1, 1, 1), ColorAddition(0, 0, 0, 0)
{
	string path=ExtractFilePath(file);

	// ファイルを読み込む
	LPD3DXBUFFER buf_materials=NULL;
	if (archive) {
		ARCHIVE_ENTRY* e=archive->Find(file);
		if (e) {
			D3DXLoadMeshFromXInMemory(
				e->Data, e->Size, D3DXMESH_SYSTEMMEM, 
				Device, NULL, &buf_materials, NULL, 
				&NumMaterials, &Mesh);
		}
	} else {
		D3DXLoadMeshFromX(
			file.c_str(), D3DXMESH_SYSTEMMEM, 
			Device, NULL, &buf_materials, NULL, 
			&NumMaterials, &Mesh);
	}
	
	// マテリアルとテクスチャのための配列を確保する
	D3DXMATERIAL* materials;
	if (buf_materials) {
		materials=(D3DXMATERIAL*)buf_materials->GetBufferPointer();
		Materials=new D3DMATERIAL9[NumMaterials];
		Textures=new LPDIRECT3DTEXTURE9[NumMaterials];
	}

	// マテリアルをコピーし，テクスチャを読み込む
	for (DWORD i=0; i<NumMaterials; i++ ) {

		// マテリアルをコピーする
		Materials[i]=materials[i].MatD3D;

		// アンビエント色を設定する
		Materials[i].Ambient=Materials[i].Diffuse;

		// テクスチャを読み込む
		Textures[i]=NULL;
		char* texture_file=materials[i].pTextureFilename;
		if (texture_file && lstrlen(texture_file)>0) {
			if (archive) {
				ARCHIVE_ENTRY* e=archive->Find(path+texture_file);
				if (!e) continue;
				D3DXCreateTextureFromFileInMemory(
					Device, e->Data, e->Size, &Textures[i]);
			} else {
				D3DXCreateTextureFromFile(
					Device, (path+texture_file).c_str(), &Textures[i]);
			}
		}
	}

/*
	// 最適化
	DWORD* adjacency=new DWORD[Mesh->GetNumFaces()*3];
	HRESULT result;
	result=Mesh->GenerateAdjacency(0, adjacency);
	result=Mesh->OptimizeInplace(D3DXMESHOPT_STRIPREORDER, adjacency, NULL, NULL, NULL);
	delete adjacency;
*/

	// 後片づけ
	if (buf_materials) buf_materials->Release();
}

// デストラクタ
CMesh::~CMesh() {
	if (Textures) {
		for (DWORD i=0; i<NumMaterials; i++) {
			if (Textures[i]) Textures[i]->Release();
		}
		delete[] Textures;
	}
	if (Materials) delete[] Materials;
	if (Mesh) Mesh->Release();
}


//==============================================================
// 描画

// メッシュの描画
void CMesh::Draw() {

	// メッシュの描画
	D3DMATERIAL9 mat;
	for (DWORD i=0; i<NumMaterials; i++) {
		mat=Materials[i];
		D3DXCOLOR* col;
		
		#define COLOR_OPERATION(TARGET) \
			col=(D3DXCOLOR*)&mat.TARGET;\
			D3DXColorModulate(col, col, &ColorMultiplier);\
			D3DXColorAdd(col, col, &ColorAddition);
		COLOR_OPERATION(Diffuse);
		COLOR_OPERATION(Ambient);
		COLOR_OPERATION(Specular);
		COLOR_OPERATION(Emissive);
		Device->SetMaterial(&mat);
		Device->SetTexture(0, Textures[i]);
		Mesh->DrawSubset(i);
	}	
}

// 変換行列を指定して描画
void CMesh::Draw(const D3DXMATRIX& mat) {
	Device->SetTransform(D3DTS_WORLD, &mat);
	Draw();
}

// 座標、サイズ、回転を指定して描画
#define TURN_X \
	D3DXMatrixRotationX(&mat0, D3DX_PI*2*turn.x);\
	D3DXMatrixMultiply(&mat, &mat, &mat0);

#define TURN_Y \
	D3DXMatrixRotationY(&mat0, D3DX_PI*2*turn.y);\
	D3DXMatrixMultiply(&mat, &mat, &mat0);

#define TURN_Z \
	D3DXMatrixRotationZ(&mat0, D3DX_PI*2*turn.z);\
	D3DXMatrixMultiply(&mat, &mat, &mat0);

void CMesh::Draw(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& scale, 
	const D3DXVECTOR3& turn, TURN_ORDER order) {
	
	// 拡大縮小
	D3DXMATRIX mat, mat0;
	D3DXMatrixScaling(&mat, scale.x, scale.y, scale.z);

	// 回転
	switch (order) {
		case TO_X: TURN_X break;
		case TO_Y: TURN_Y break;
		case TO_Z: TURN_Z break;
		case TO_XY: TURN_X TURN_Y break;
		case TO_XZ: TURN_X TURN_Z break;
		case TO_YX: TURN_Y TURN_X break;
		case TO_YZ: TURN_Y TURN_Z break;
		case TO_ZX: TURN_Z TURN_X break;
		case TO_ZY: TURN_Z TURN_Y break;
		case TO_XYZ: TURN_X TURN_Y TURN_Z break;
		case TO_XZY: TURN_X TURN_Z TURN_Y break;
		case TO_YXZ: TURN_Y TURN_X TURN_Z break;
		case TO_YZX: TURN_Y TURN_Z TURN_X break;
		case TO_ZXY: TURN_Z TURN_X TURN_Y break;
		case TO_ZYX: TURN_Z TURN_Y TURN_X break;
	}

	// 座標
	D3DXMatrixTranslation(&mat0, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mat, &mat, &mat0);
	
	// 描画
	Draw(mat);
}


//==============================================================
// 描画（古い関数）

// アルファを指定して描画
void CMesh::Draw(float alpha) {
	SetColor(D3DXCOLOR(1, 1, 1, alpha), D3DXCOLOR(0, 0, 0, 0));
	Draw();
}

// 変換行列、アルファを指定して描画
void CMesh::Draw(LPD3DXMATRIX mat, float alpha) {
	SetColor(D3DXCOLOR(1, 1, 1, alpha), D3DXCOLOR(0, 0, 0, 0));
	Draw(*mat);
}

// 座標、サイズ、回転、アルファを指定して描画
void CMesh::Draw(
	float x, float y, float z, 
	float sx, float sy, float sz, 
	float tx, float ty, float tz, TURN_ORDER order,
	float alpha, bool alpha_add
) {
	SetColor(D3DXCOLOR(1, 1, 1, alpha), D3DXCOLOR(0, 0, 0, 0));
	SetAddAlpha(alpha_add);
	Draw(
		D3DXVECTOR3(x, y, z), D3DXVECTOR3(sx, sy, sz), 
		D3DXVECTOR3(tx, ty, tz), order);
}

// アルファ加算モードの指定
void CMesh::SetAddAlpha(bool add_alpha) {
	if (add_alpha) {
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	} else {
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}
