#ifndef ANIMMESHH
#define ANIMMESHH

#include "Mesh.h"
#include <d3dx9.h>
#include <string>
using namespace std;


class CMeshContainer;


//==============================================================
// フレームとメッシュコンテナを読み込むために使用する
// ID3DXAllocateHierarchyの派生クラス
class CAllocateHierarchy: public ID3DXAllocateHierarchy {
public:
	STDMETHOD(CreateFrame)(
		THIS_ LPCTSTR name, LPD3DXFRAME *new_frame);
	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR name, 
		CONST D3DXMESHDATA *mesh_data,
		CONST D3DXMATERIAL *materials, 
		CONST D3DXEFFECTINSTANCE *effect_instances,
		DWORD num_materials, 
		CONST DWORD *adjacency, 
		LPD3DXSKININFO skin_info, 
		LPD3DXMESHCONTAINER *new_mesh_container);
	STDMETHOD(DestroyFrame)(
		THIS_ LPD3DXFRAME frame_to_tree);
	STDMETHOD(DestroyMeshContainer)(
		THIS_ LPD3DXMESHCONTAINER mesh_container_base);
};


//==============================================================
// フレーム
class CFrame : public D3DXFRAME {
private:

	// メッシュコンテナ、兄弟フレーム、子フレーム
	CMeshContainer* MeshContainer;
	CFrame* Sibling;
	CFrame* Child;

	// 合成済み変形行列
	D3DXMATRIXA16 CombinedTransMatrix;

public:

	// コンストラクタ、デストラクタ
	CFrame(LPCTSTR name);
	~CFrame();

	// 行列の取得
	LPD3DXMATRIXA16 GetCombinedTransMatrix() {
		return &CombinedTransMatrix;
	}
	
	// フレームの検索
	CFrame* Find(LPCTSTR name);

	// ボーン変形行列の初期化
	void InitBoneTransMatrices(CFrame* root);

	// 変形行列の更新
	void UpdateMatrices(LPD3DXMATRIX parent_mat);

	// 描画
	void Draw();
};


//==============================================================
// 頂点処理のモード
typedef enum {
	VP_HLSL, VP_INDEXED, VP_SOFTWARE
} VPMODE;


//==============================================================
// メッシュコンテナ
class CMeshContainer : public D3DXMESHCONTAINER {
private:

	// メッシュ
	ID3DXMesh* Mesh;

	// マテリアルの数、マテリアル、テクスチャ
	DWORD NumMaterials;
	D3DMATERIAL9* Materials;
	LPDIRECT3DTEXTURE9* Textures;

	// スキン情報
	LPD3DXSKININFO SkinInfo;

	// ボーンの数、オフセット行列、トランスフォーム行列
	DWORD NumBones;
	D3DXMATRIX* BoneOffsetMatrices;
	D3DXMATRIX** BoneTransMatrices;

	// ボーン行列パレットの数、同時に影響するボーンの最大数
    DWORD NumPaletteEntries;
	DWORD NumMaxInfls;

	// ボーンの組み合わせの数、ボーンの組み合わせ
	DWORD NumBoneCombinations;
	LPD3DXBUFFER BoneCombinations;

	// 頂点処理のモード
	VPMODE VPMode;

public:

	// コンストラクタ
	CMeshContainer(
		THIS_ LPCTSTR name,
		CONST D3DXMESHDATA *mesh_data,
		CONST D3DXMATERIAL *materials,
		DWORD num_materials,
		LPD3DXSKININFO skin_info,
		CONST DWORD *adjacency
	);

	// デストラクタ
	~CMeshContainer();

	// 次のメッシュコンテナを取得
	CMeshContainer* Next() {
		return (CMeshContainer*)pNextMeshContainer;
	}

	// ボーン変形行列の初期化
	void InitBoneTransMatrices(CFrame* root);

	// 描画
	void Draw(CFrame* frame);
};


//==============================================================
// アニメーション対応メッシュ
class CAnimMesh {
private:

	// デバイス
	LPDIRECT3DDEVICE9 Device;

	// ルートのフレーム
	CFrame* FrameRoot;

	// アニメーションコントローラ
	LPD3DXANIMATIONCONTROLLER AnimController;

public:

	// コンストラクタ，デストラクタ
	CAnimMesh(LPDIRECT3DDEVICE9 device);
	~CAnimMesh();

	// ファイルの読み込み
	void LoadFromFile(string file);

	// アニメーション
	double GetAnimTime();
	void AdvanceAnimTime(double time);
	void SetAnimSet(DWORD track, DWORD set);
	void SetAnimEnabled(DWORD track, bool enabled);
	void SetAnimWeight(DWORD track, float weight);
	void SetAnimSpeed(DWORD track, float speed);
	DWORD GetAnimNumTracks();
	DWORD GetAnimSetCount();

	// フレームの取得
	CFrame* GetFrameRoot() { return FrameRoot; }

	// 描画
	void Draw(LPD3DXMATRIX mat);
	void Draw();
	void Draw(
		float x, float y, float z, 
		float sx, float sy, float sz, 
		float tx, float ty, float tz, TURN_ORDER order);
};


#endif
