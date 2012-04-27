#pragma once

#pragma warning(disable:4244 4786 4819 4996)
#include <d3dx9.h>
#include <vector>
using namespace std;

class CModelFrame {
public:
	D3DXMATRIXA16 m_mTransformMatrix;
	D3DXMATRIXA16 m_mOffsetMatrix;
	D3DXMATRIXA16 m_mCombinedMatrix;
	D3DXMATRIXA16 m_mSkinningMatrix;
	D3DXMATRIXA16 m_mSmootherMatrix;
	char*					m_pName;
	vector<CModelFrame*>	m_Child;
	size_t					m_Index;

	CModelFrame(vector<CModelFrame*>* frame);
	void Animate(D3DXMATRIXA16* parent);
};

class CModelSubset {
public:
	DWORD m_nMaterialIndex;
	DWORD m_nFaceIndex;
	DWORD m_nFaceCount;
};

class CModelMaterial {
public:
	D3DXVECTOR4			m_Color;
	IDirect3DTexture9*	m_pTexture;	
};

class CModelAnimationKey {
public:
	vector<float> m_Time;
	vector<D3DXMATRIXA16*> m_Matrix;
	char*  m_pFrameName;
	size_t m_FrameIndex;
};

class CModelAnimation {
public:
	char* m_pName;
	vector<CModelAnimationKey*> m_Key;
	float m_fTime;
	float m_fWeight;

	float GetMaxTime() { return m_Key.front()->m_Time.back(); }
	bool IsOver() { return m_fTime>=GetMaxTime(); }
	CModelAnimation() {}
	CModelAnimation(const CModelAnimation* anim);
};

class CModel {
public:
	IDirect3DDevice9*		m_pDevice;
	IDirect3DVertexBuffer9* m_pOriginalVertexBuffer;
	IDirect3DVertexBuffer9* m_pAnimatedVertexBuffer;
	DWORD					m_nVertexCount;
	IDirect3DIndexBuffer9*  m_pIndexBuffer;
	DWORD					m_nFaceCount;
	vector<CModelFrame*>		m_Frame;
	vector<CModelSubset*>		m_Subset;
	vector<CModelMaterial*>		m_Material;
	vector<CModelAnimation*>	m_Animation;
	float						m_fSmootherWeight;

	CModel(IDirect3DDevice9* device, const char* file_name, bool anim_only);
	void AnimateFrame(D3DXMATRIXA16* world);
	void AnimateVertex();
	void Draw(ID3DXEffect* effect, D3DXMATRIXA16* view, D3DXMATRIXA16* proj);
	void Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj);

	CModelFrame* FindFrame(const char* name);
	DWORD GetFrameCount() { return (DWORD)m_Frame.size(); }
	CModelFrame* GetFrame(DWORD index) { return m_Frame[index]; }

	CModelAnimation* FindAnimation(const char* name);
	void AddAnimation(CModelAnimation* anim);
	void ResetAnimationWeight();
	DWORD GetAnimationCount() { return (DWORD)m_Animation.size(); }
	CModelAnimation* GetAnimation(DWORD index) { return m_Animation[index]; }

	void SaveSmoother();

//	void ReverseAnimationX(int anim_index);
};

extern IDirect3DTexture9* GetTexture(IDirect3DDevice9* device, const char* file_name);


