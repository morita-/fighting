#include "CFightingGame.h"

//=========================================================================
// 定数、変数

// ゲーム本体
CFightingGame* gpGame;

// メッシュファイル名
const char* gMODEL_FILE_NAME[]={
	"00_Mesh1P.x", "00_Mesh2P.x"
};

// アニメーションファイル名
const char* gANIM_FILE_NAME[]={

	// 移動
	"00_2.x", "00_5.x", 
	"00_4.x", "00_6.x", 
	"00_44.x", "00_66.x", 
	"00_121.x", "00_323.x", 
	"00_Run.x", 

	// ガード、攻撃
	"00_2G.x", "00_2K.x", "00_2P.x", 
	"00_5G.x", "00_5K.x", "00_5P.x", 
	"00_3K.x", "00_5PP.x", "00_3236P.x", 

	// ダメージ
	"00_Damage2Low0.x", "00_Damage2Middle0.x", 
	"00_Damage5Low0.x", "00_Damage5Middle0.x", "00_Damage5High0.x", 

	// 吹き飛び、ダウン、ダウン攻撃
	"00_Blow20.x", "00_Blow50.x", "00_Down0.x", 
	"00_D8P.x", "00_DamageDown0.x", 

	// ガード成功
	"00_Guard2.x", "00_Guard5.x", 

	// 投げ
	"00_5P+G.x", "00_Thrown(00_P+G).x", 

	// 起きあがり、起きあがり攻撃
	"00_Standup2.x", "00_Standup2K.x", "00_Standup5.x", "00_Standup5K.x", 
};

// アニメーションファイル名（左右逆転）
const char* gANIM_FILE_NAME_R[]={

	// 移動
	"00R_2.x", "00R_5.x", 
	"00R_4.x", "00R_6.x", 
	"00R_44.x", "00R_66.x", 
	"00R_121.x", "00R_323.x", 
	"00R_Run.x", 

	// ガード、攻撃
	"00R_2G.x", "00R_2K.x", "00R_2P.x", 
	"00R_5G.x", "00R_5K.x", "00R_5P.x", 
	"00R_3K.x", "00R_5PP.x", "00R_3236P.x", 

	// ダメージ
	"00R_Damage2Low0.x", "00R_Damage2Middle0.x", 
	"00R_Damage5Low0.x", "00R_Damage5Middle0.x", "00R_Damage5High0.x", 

	// 吹き飛び、ダウン、ダウン攻撃
	"00R_Blow20.x", "00R_Blow50.x", "00R_Down0.x", 
	"00R_D8P.x", "00R_DamageDown0.x", 

	// ガード成功
	"00R_Guard2.x", "00R_Guard5.x", 

	// 投げ
	"00R_5P+G.x", "00R_Thrown(00_5P+G).x", 

	// 起きあがり、起きあがり攻撃
	"00R_Standup2.x", "00R_Standup2K.x", "00R_Standup5.x", "00R_Standup5K.x", 
};

//=========================================================================
// サブルーチン

// エフェクトのロード
static ID3DXEffect* LoadEffect(IDirect3DDevice9* device, string file) {
	ID3DXEffect* effect=NULL;
	LPD3DXBUFFER error;

	// エフェクトファイルの存在を確認する
	if (!FileExists(file)) {
		MessageBox(
			NULL, file.c_str(), 
			"エフェクトファイルが見つかりません。", MB_OK);
		PostQuitMessage(1);
		return NULL;
	}

	// エフェクトファイルのロード
	D3DXCreateEffectFromFile(
		device, file.c_str(), 
		NULL, NULL, 0, NULL, &effect, &error);

	// コンパイルエラーの場合はエラーを表示して終了
	if (!effect) {
		MessageBox(
			NULL, 
			error?(LPCTSTR)error->GetBufferPointer():NULL, 
			"エフェクトファイルのコンパイルエラー。", MB_OK);
		PostQuitMessage(1);
		return NULL;
	}

	return effect;
}


//=========================================================================
// ゲーム本体（共通）

// コンストラクタ
CFightingGame::CFightingGame()
:	CGame("Fighter"), 
	m_bPaused(false), m_bPrevInput(false), m_bShaderEnabled(false),
	m_pShadowTexture(NULL), m_pShadowDepthStencil(NULL)
{
}

// グラフィックイニシャライズ
void CFightingGame::GameGraphicInitialize()
{
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();

	// ヘルプ、終了、カーソルに関する設定
	m_HelpURL=m_ExePath+"..\\Help\\index.html";
	m_bConfirmExit=false;
	ShowCursor(TRUE);
	
	m_FPS=60;
	m_bPauseInTheBackground=true;
	m_bDropFrames=false;
	
	// グラフィック
	m_pGraphics->SetDepthStencilFormat(D3DFMT_D24S8);
	m_pGraphics->ResetDevice();

	// フォント
	static const char fontchar[]="~|0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,.(){}<>!?+-x*/='\\\"#$%&@[]_";
	m_pFont=new CFontRef(m_pGraphics->GetDevice(), m_ExePath+"..\\Font\\font.png", NULL, fontchar, 16.0f/256, 32.0f/256, 16, 32, 2, 2);

	// エフェクト
	m_pEffect=LoadEffect(device, "..\\Effect\\Fighting.fx");

	// モデル
	for (int i=0; i<MODEL_COUNT; i++) {
		m_pModel.push_back(new CModel(device, gMODEL_FILE_NAME[i], false));
	}
	
	// アニメーション
	for (int i=0; i<ANIM_COUNT; i++) {
		m_pAnimation.push_back((new CModel(device, gANIM_FILE_NAME[i], true))->GetAnimation(0));
		for (int j=0; j<MODEL_COUNT; j++) {
			m_pModel[j]->AddAnimation(m_pAnimation[i]);
		}
	}
	for (int i=0; i<ANIM_COUNT; i++) {
		m_pAnimation.push_back((new CModel(device, gANIM_FILE_NAME_R[i], true))->GetAnimation(0));
		for (int j=0; j<MODEL_COUNT; j++) {
			m_pModel[j]->AddAnimation(m_pAnimation[ANIM_COUNT+i]);
		}
	}

	// テクスチャ
	m_pFaceNormal=GetTexture(device, "Face_C.dds");
	m_pFaceClosed=GetTexture(device, "Face_C(Closed).dds");
	m_pFacePain=GetTexture(device, "Face_C(Pain).dds");

	m_bPrevInput=false;
}

//=========================================================================
// デバイス

// デバイスロスト
void CFightingGame::OnLostDevice() {
	m_pEffect->OnLostDevice();
	SAFE_RELEASE(m_pShadowTexture);
	SAFE_RELEASE(m_pShadowDepthStencil);
	delete m_pDummyTexture;
}

// デバイスリセット
void CFightingGame::OnResetDevice() {
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();

	// エフェクト
	m_pEffect->OnResetDevice();

	// 共通設定
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// シェーダ用設定
	D3DXVECTOR4 light(1, -1, 1, 0);
	D3DXVec4Normalize(&light, &light);
	m_pEffect->SetVector("Light", &light);
	D3DXCOLOR ambient(0.5f, 0.5f, 0.5f, 1);
	m_pEffect->SetVector("Ambient", (D3DXVECTOR4*)&ambient);
	
	// 固定パイプライン用設定
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(128, 128, 128, 255));
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphics->SetLight(0, D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, -1, 1), D3DXCOLOR(1, 1, 1, 1), 1000);

	// ダミーのテクスチャ
	m_pDummyTexture=new CTextureRef(device, 1, 1);
	D3DXVECTOR4 color(1, 1, 1, 1);
	m_pDummyTexture->Fill(&color);

	// シャドウマップの作成
	if (!m_pShadowTexture) {

		// 浮動小数テクスチャ
		m_bUseIntTexture=false;
		device->CreateTexture(
			TEX_SIZE, TEX_SIZE, 1, 
			D3DUSAGE_RENDERTARGET, D3DFMT_R32F, 
			D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
		if (!m_pShadowTexture) {
			device->CreateTexture(
				TEX_SIZE, TEX_SIZE, 1, 
				D3DUSAGE_RENDERTARGET, D3DFMT_R16F, 
				D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
			if (!m_pShadowTexture) {
				device->CreateTexture(
					TEX_SIZE, TEX_SIZE, 1, 
					D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, 
					D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
		
				// 整数テクスチャ
				if (!m_pShadowTexture) {
					m_bUseIntTexture=true;
					device->CreateTexture(
						TEX_SIZE, TEX_SIZE, 1, 
						D3DUSAGE_RENDERTARGET, D3DFMT_G16R16, 
						D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
					if (!m_pShadowTexture) {
						device->CreateTexture(
							TEX_SIZE, TEX_SIZE, 1, 
							D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16, 
							D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
					}
				}
			}
		}
	}

	// 深度バッファの作成
	if (!m_pShadowDepthStencil) {
		device->CreateDepthStencilSurface(
			TEX_SIZE, TEX_SIZE, D3DFMT_D16, 
			D3DMULTISAMPLE_NONE, 0, TRUE, 
			&m_pShadowDepthStencil, NULL);
	}
}
