#ifndef GRAPHICSH
#define GRAPHICSH

#include <d3dx9.h>
#include <string>
using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif


//==============================================================
// グラフィックス描画機能を管理するクラス
class CGraphics {
protected:

	// Direct3Dインタフェース、デバイス
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pDevice;

	// ウィンドウハンドル
	HWND m_HWnd;
	
	// 元のウィンドウスタイル
	DWORD m_WindowStyle;

	// 幅、高さ
	int m_nWidth, m_nHeight;

	// フルスクリーンモードならばtrue
	bool m_bFullScreen;

	// リフレッシュレート(Hz)
	int m_nRefreshRate;

	// GDIによる描画が可能なサーフェスかどうか
	bool m_bIsGDISurface;

	D3DFORMAT m_DepthStencilFormat;

public:

	// コンストラクタ，デストラクタ
	CGraphics(HWND hwnd);
	~CGraphics();

	// デバイスのリセット
	void SetWidth(int w) { m_nWidth=w; }
	void SetHeight(int h) { m_nHeight=h; }
	void SetFullScreen(bool b) { m_bFullScreen=b; }
	void SetRefreshRate(int r) { m_nRefreshRate=r; }
	void SetDepthStencilFormat(D3DFORMAT format) { m_DepthStencilFormat=format; }
	bool ResetDevice();
	bool DeviceNotReset();

	// シーン描画の開始，終了
	bool BeginScene() { return SUCCEEDED(m_pDevice->BeginScene()); }
	void EndScene() { m_pDevice->EndScene(); }

	// 画面消去，画面フリップ
	void Clear(D3DCOLOR color=D3DCOLOR_ARGB(255, 0, 0, 0));
	bool Present();

	// 各種情報の取得
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
	HWND GetHWnd() { return m_HWnd; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }
	bool IsFullScreen() { return m_bFullScreen; }
	int GetRefreshRate() { return m_nRefreshRate; }

	// GDIによる描画が可能なサーフェスにする
	void PresentGDISurface() { if (!m_bIsGDISurface) Present(); }

	// ライトの設定
	void SetLight(
		int id, D3DLIGHTTYPE type, 
		D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXCOLOR color, 
		float range
	);
	void SetLight(
		int id, D3DLIGHTTYPE type, 
		float x, float y, float z,
		float range=1000
	);

	// アルファ加算モードの指定
	void SetAddAlpha(bool add_alpha);

	// エフェクトのロード
	ID3DXEffect* LoadEffect(string file);
	
	// テクスチャのロード
	LPDIRECT3DTEXTURE9 LoadTexture(string file);
};


#endif



