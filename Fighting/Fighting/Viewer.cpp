#include "Main.h"

// ゲーム本体（ビュワー）
class CFightingGameViewer : public CFightingGame {
public:
	int m_nModelIndex;
	int m_nAnimIndex;
	int m_nTime;
	float m_fYaw;
	bool m_bAnimEnabled;

	CFightingGameViewer(bool anim);
	virtual void Move();
	virtual void Draw();
};

// ファクトリ
CFightingGame* NewFightingGameViewer(bool anim) {
	return new CFightingGameViewer(anim);
}

// コンストラクタ
CFightingGameViewer::CFightingGameViewer(bool anim)
:	m_nModelIndex(0), m_nAnimIndex(0), m_fYaw(0), m_nTime(0), m_bAnimEnabled(anim)
{}

// 移動
void CFightingGameViewer::Move() {

	// 入力
	m_pInput->UpdateState();
	const CInputState* is=m_pInput->GetState(0);

	// ステージの動作
	if (is->Left)  m_fYaw+=0.02f;
	if (is->Right) m_fYaw-=0.02f;

	if (!m_bPrevInput) {
		if (is->Up) {
			m_nAnimIndex++;
			m_nTime=0;
		}
		if (is->Down) {
			m_nAnimIndex--;
			m_nTime=0;
		}
		m_nAnimIndex=(m_nAnimIndex+ANIM_COUNT)%ANIM_COUNT;

		if (is->Button[0]) m_nModelIndex++;
		m_nModelIndex=(m_nModelIndex+MODEL_COUNT)%MODEL_COUNT;

		if (is->Button[1]) m_bShaderEnabled=!m_bShaderEnabled;
		if (is->Button[3]) m_bPaused=!m_bPaused;
	}

	if (m_bAnimEnabled) {
		m_pModel[m_nModelIndex]->ResetAnimationWeight();
		CModelAnimation* anim=m_pModel[m_nModelIndex]->GetAnimation(m_nAnimIndex);
		anim->m_fTime=m_nTime*ANIM_TIME_STEP;
		anim->m_fWeight=1;
		if (!m_bPaused || !m_bPrevInput && is->Button[2]) {
			m_nTime++;
			m_nTime%=(int)anim->GetMaxTime()/ANIM_TIME_STEP+1;
		}
	}

	// 前回の入力
	m_bPrevInput=is->Up||is->Down||is->Button[0]||is->Button[1]||is->Button[2]||is->Button[3];
}

// 描画
void CFightingGameViewer::Draw() {
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();
	m_pGraphics->Clear(COL_WHITE);

	// テキスト
	string s;
	s+="FILE=";
	s+=ToUpper(gANIM_FILE_NAME[m_nAnimIndex]);
	s+=" / ";
	s+="TIME=";
	s+=ToStr(m_nTime);
	s+=" / ";
	s+="SHADER=";
	s+=m_bShaderEnabled?"YES":"NO";
	m_pFont->DrawText(s, 0, 0, COL_BLACK);

	// ワールド行列
	D3DXMATRIXA16 world;
	D3DXMatrixRotationY(&world, m_fYaw);

	// ビュー行列
	D3DXMATRIXA16 view;
	D3DXVECTOR3 from(0, 2, -4);
	D3DXVECTOR3 lookat(0, 1, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&view, &from, &lookat, &up);
	
	// 射影行列
	D3DXMATRIXA16 proj;
	float w=m_pGraphics->GetWidth(), h=m_pGraphics->GetHeight();
	D3DXMatrixPerspectiveLH(&proj, 1, h/w, 1, 1000);

	// モデルの描画
	m_pModel[m_nModelIndex]->AnimateFrame(&world);
	m_pModel[m_nModelIndex]->AnimateVertex();
	if (m_bShaderEnabled) {
		m_pEffect->SetTechnique("Basic");
		m_pModel[m_nModelIndex]->Draw(m_pEffect, &view, &proj);
	} else {
		m_pModel[m_nModelIndex]->Draw(&view, &proj);
	}
}


