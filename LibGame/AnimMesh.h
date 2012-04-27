#ifndef ANIMMESHH
#define ANIMMESHH

#include "Mesh.h"
#include <d3dx9.h>
#include <string>
using namespace std;


class CMeshContainer;


//==============================================================
// �t���[���ƃ��b�V���R���e�i��ǂݍ��ނ��߂Ɏg�p����
// ID3DXAllocateHierarchy�̔h���N���X
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
// �t���[��
class CFrame : public D3DXFRAME {
private:

	// ���b�V���R���e�i�A�Z��t���[���A�q�t���[��
	CMeshContainer* MeshContainer;
	CFrame* Sibling;
	CFrame* Child;

	// �����ςݕό`�s��
	D3DXMATRIXA16 CombinedTransMatrix;

public:

	// �R���X�g���N�^�A�f�X�g���N�^
	CFrame(LPCTSTR name);
	~CFrame();

	// �s��̎擾
	LPD3DXMATRIXA16 GetCombinedTransMatrix() {
		return &CombinedTransMatrix;
	}
	
	// �t���[���̌���
	CFrame* Find(LPCTSTR name);

	// �{�[���ό`�s��̏�����
	void InitBoneTransMatrices(CFrame* root);

	// �ό`�s��̍X�V
	void UpdateMatrices(LPD3DXMATRIX parent_mat);

	// �`��
	void Draw();
};


//==============================================================
// ���_�����̃��[�h
typedef enum {
	VP_HLSL, VP_INDEXED, VP_SOFTWARE
} VPMODE;


//==============================================================
// ���b�V���R���e�i
class CMeshContainer : public D3DXMESHCONTAINER {
private:

	// ���b�V��
	ID3DXMesh* Mesh;

	// �}�e���A���̐��A�}�e���A���A�e�N�X�`��
	DWORD NumMaterials;
	D3DMATERIAL9* Materials;
	LPDIRECT3DTEXTURE9* Textures;

	// �X�L�����
	LPD3DXSKININFO SkinInfo;

	// �{�[���̐��A�I�t�Z�b�g�s��A�g�����X�t�H�[���s��
	DWORD NumBones;
	D3DXMATRIX* BoneOffsetMatrices;
	D3DXMATRIX** BoneTransMatrices;

	// �{�[���s��p���b�g�̐��A�����ɉe������{�[���̍ő吔
    DWORD NumPaletteEntries;
	DWORD NumMaxInfls;

	// �{�[���̑g�ݍ��킹�̐��A�{�[���̑g�ݍ��킹
	DWORD NumBoneCombinations;
	LPD3DXBUFFER BoneCombinations;

	// ���_�����̃��[�h
	VPMODE VPMode;

public:

	// �R���X�g���N�^
	CMeshContainer(
		THIS_ LPCTSTR name,
		CONST D3DXMESHDATA *mesh_data,
		CONST D3DXMATERIAL *materials,
		DWORD num_materials,
		LPD3DXSKININFO skin_info,
		CONST DWORD *adjacency
	);

	// �f�X�g���N�^
	~CMeshContainer();

	// ���̃��b�V���R���e�i���擾
	CMeshContainer* Next() {
		return (CMeshContainer*)pNextMeshContainer;
	}

	// �{�[���ό`�s��̏�����
	void InitBoneTransMatrices(CFrame* root);

	// �`��
	void Draw(CFrame* frame);
};


//==============================================================
// �A�j���[�V�����Ή����b�V��
class CAnimMesh {
private:

	// �f�o�C�X
	LPDIRECT3DDEVICE9 Device;

	// ���[�g�̃t���[��
	CFrame* FrameRoot;

	// �A�j���[�V�����R���g���[��
	LPD3DXANIMATIONCONTROLLER AnimController;

public:

	// �R���X�g���N�^�C�f�X�g���N�^
	CAnimMesh(LPDIRECT3DDEVICE9 device);
	~CAnimMesh();

	// �t�@�C���̓ǂݍ���
	void LoadFromFile(string file);

	// �A�j���[�V����
	double GetAnimTime();
	void AdvanceAnimTime(double time);
	void SetAnimSet(DWORD track, DWORD set);
	void SetAnimEnabled(DWORD track, bool enabled);
	void SetAnimWeight(DWORD track, float weight);
	void SetAnimSpeed(DWORD track, float speed);
	DWORD GetAnimNumTracks();
	DWORD GetAnimSetCount();

	// �t���[���̎擾
	CFrame* GetFrameRoot() { return FrameRoot; }

	// �`��
	void Draw(LPD3DXMATRIX mat);
	void Draw();
	void Draw(
		float x, float y, float z, 
		float sx, float sy, float sz, 
		float tx, float ty, float tz, TURN_ORDER order);
};


#endif
