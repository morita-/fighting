#ifndef __CFIGHTING_GAME__H__
#define __CFIGHTING_GAME__H__

#include "..\..\LibGame\LibGame.h"
#include "..\..\LibUtil\LibUtil\LibUtil.h"
#include "Model.h"


// 各種パラメータ
#define MODEL_COUNT 2
#define ANIM_TIME_STEP 100
#define TEX_SIZE 2048

// ファイル名
extern const char* gMODEL_FILE_NAME[];
extern const char* gANIM_FILE_NAME[];

// アニメーションID
enum ANIM_ID {

	// 移動
	ANIM_2, ANIM_5, 
	ANIM_4, ANIM_6, 
	ANIM_44, ANIM_66, 
	ANIM_121, ANIM_323, 
	ANIM_RUN, 

	// ガード、攻撃
	ANIM_2G, ANIM_2K, ANIM_2P, 
	ANIM_5G, ANIM_5K, ANIM_5P, 
	ANIM_3K, ANIM_5PP, ANIM_3236P, 

	// ダメージ
	ANIM_DAMAGE2LOW0, ANIM_DAMAGE2MIDDLE0, 
	ANIM_DAMAGE5LOW0, ANIM_DAMAGE5MIDDLE0, ANIM_DAMAGE5HIGH0, 

	// 吹き飛び、ダウン、ダウン攻撃
	ANIM_BLOW20, ANIM_BLOW50, ANIM_DOWN0, 
	ANIM_D8P, ANIM_DAMAGE_DOWN0, 

	// ガード成功
	ANIM_GUARD2, ANIM_GUARD5, 

	// 投げ
	ANIM_5PG, ANIM_THROWN5PG, 

	// 起きあがり、起きあがり攻撃
	ANIM_STANDUP2, ANIM_STANDUP2K, ANIM_STANDUP5, ANIM_STANDUP5K, 

	ANIM_COUNT
};

// ゲーム本体（共通）
class CFightingGame : public CGameRef {
protected:
	bool m_bPaused;
	bool m_bPrevInput;
	bool m_bShaderEnabled;

	// フォント
	CFontRef* m_pFont;

	// テクスチャ
	CTextureRef*	m_pDummyTexture;
	bool		m_bUseIntTexture;
	LPDIRECT3DTEXTURE9 m_pShadowTexture;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;


	// モデル、アニメーション
	vector<CModel*>			 m_pModel;
	vector<CModelAnimation*> m_pAnimation;

	// 表情テクスチャ
	IDirect3DTexture9* m_pFaceNormal;
	IDirect3DTexture9* m_pFaceClosed;
	IDirect3DTexture9* m_pFacePain;

	// エフェクト
	ID3DXEffect* m_pEffect;

public:

	CFightingGame();

	void GameGraphicInitialize();
	virtual void InitializeGame(){};

	virtual void OnLostDevice();
	virtual void OnResetDevice();

	void SetShaderEnable(bool sw){m_bShaderEnabled=sw;}

	bool IsPaused(){return m_bPaused;}
	bool IsPrevInput(){return m_bPrevInput;}
	bool IsShaderEnabled(){return m_bShaderEnabled;}

	// フォント
	CFontRef* GetFont(){return m_pFont;}

	// テクスチャ
	CTextureRef*	GetDummyTexture(){return m_pDummyTexture;}
	bool		IsUseIntTexture(){return m_bUseIntTexture;}
	LPDIRECT3DTEXTURE9 GetShadowTextue(){return m_pShadowTexture;}
	LPDIRECT3DSURFACE9 GetShadowDepthStencil(){return m_pShadowDepthStencil;}

	// 表情テクスチャ
	IDirect3DTexture9* GetFaceTexNormal(){return m_pFaceNormal;}
	IDirect3DTexture9* GetFaceTexClose(){return m_pFaceClosed;}
	IDirect3DTexture9* GetFaceTexPain(){return m_pFacePain;}

	// エフェクト
	ID3DXEffect* GetEffect(){return m_pEffect;}

	// 乱数
	CRand m_Rand;

};

extern CFightingGame* gpGame;
CFightingGame* NewFightingGameViewer(bool anim);
CFightingGame* NewFightingGameVersus(bool cpu);

// 色
#define COL_WHITE D3DCOLOR_ARGB(255, 255, 255, 255)
#define COL_LGRAY D3DCOLOR_ARGB(255, 192, 192, 192)
#define COL_MGRAY D3DCOLOR_ARGB(255, 128, 128, 128)
#define COL_DGRAY D3DCOLOR_ARGB(255, 64, 64, 64)
#define COL_BLACK D3DCOLOR_ARGB(255, 0, 0, 0)
#define COL_SHADE D3DCOLOR_ARGB(128, 0, 0, 0)

#endif