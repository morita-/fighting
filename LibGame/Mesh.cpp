#pragma warning(disable:4819)

#include "Mesh.h"
#include "..\LibUtil\LibUtil\LibUtil.h"


//==============================================================
// �R���X�g���N�^�C�f�X�g���N�^

// �R���X�g���N�^
CMesh::CMesh(LPDIRECT3DDEVICE9 device, string file, CArchiveLoader* archive)
:	Device(device),
	Mesh(NULL), NumMaterials(0), 
	Materials(NULL), Textures(NULL),
	ColorMultiplier(1, 1, 1, 1), ColorAddition(0, 0, 0, 0)
{
	string path=ExtractFilePath(file);

	// �t�@�C����ǂݍ���
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
	
	// �}�e���A���ƃe�N�X�`���̂��߂̔z����m�ۂ���
	D3DXMATERIAL* materials;
	if (buf_materials) {
		materials=(D3DXMATERIAL*)buf_materials->GetBufferPointer();
		Materials=new D3DMATERIAL9[NumMaterials];
		Textures=new LPDIRECT3DTEXTURE9[NumMaterials];
	}

	// �}�e���A�����R�s�[���C�e�N�X�`����ǂݍ���
	for (DWORD i=0; i<NumMaterials; i++ ) {

		// �}�e���A�����R�s�[����
		Materials[i]=materials[i].MatD3D;

		// �A���r�G���g�F��ݒ肷��
		Materials[i].Ambient=Materials[i].Diffuse;

		// �e�N�X�`����ǂݍ���
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
	// �œK��
	DWORD* adjacency=new DWORD[Mesh->GetNumFaces()*3];
	HRESULT result;
	result=Mesh->GenerateAdjacency(0, adjacency);
	result=Mesh->OptimizeInplace(D3DXMESHOPT_STRIPREORDER, adjacency, NULL, NULL, NULL);
	delete adjacency;
*/

	// ��ЂÂ�
	if (buf_materials) buf_materials->Release();
}

// �f�X�g���N�^
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
// �`��

// ���b�V���̕`��
void CMesh::Draw() {

	// ���b�V���̕`��
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

// �ϊ��s����w�肵�ĕ`��
void CMesh::Draw(const D3DXMATRIX& mat) {
	Device->SetTransform(D3DTS_WORLD, &mat);
	Draw();
}

// ���W�A�T�C�Y�A��]���w�肵�ĕ`��
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
	
	// �g��k��
	D3DXMATRIX mat, mat0;
	D3DXMatrixScaling(&mat, scale.x, scale.y, scale.z);

	// ��]
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

	// ���W
	D3DXMatrixTranslation(&mat0, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mat, &mat, &mat0);
	
	// �`��
	Draw(mat);
}


//==============================================================
// �`��i�Â��֐��j

// �A���t�@���w�肵�ĕ`��
void CMesh::Draw(float alpha) {
	SetColor(D3DXCOLOR(1, 1, 1, alpha), D3DXCOLOR(0, 0, 0, 0));
	Draw();
}

// �ϊ��s��A�A���t�@���w�肵�ĕ`��
void CMesh::Draw(LPD3DXMATRIX mat, float alpha) {
	SetColor(D3DXCOLOR(1, 1, 1, alpha), D3DXCOLOR(0, 0, 0, 0));
	Draw(*mat);
}

// ���W�A�T�C�Y�A��]�A�A���t�@���w�肵�ĕ`��
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

// �A���t�@���Z���[�h�̎w��
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
