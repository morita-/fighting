#pragma warning(disable:4819)

#include "Graphics.h"
#include <d3dx9.h>


//==============================================================
// コンストラクタ
CGraphics::CGraphics(HWND hwnd)
:	m_HWnd(hwnd), m_bFullScreen(false), m_nRefreshRate(60),
	m_pDevice(NULL), m_bIsGDISurface(true),
	m_DepthStencilFormat(D3DFMT_D16)
{
	// ウィンドウスタイルの保存
	m_WindowStyle=GetWindowLong(hwnd, GWL_STYLE);

	// クライアント領域のサイズを取得
	RECT r;
	GetClientRect(hwnd, &r);
	m_nWidth=r.right-r.left;
	m_nHeight=r.bottom-r.top;

	// Direct3Dインタフェースの作成
	m_pD3D=Direct3DCreate9(D3D_SDK_VERSION);

	// Direct3Dデバイスの作成
	ResetDevice();
}


//==============================================================
// デストラクタ
CGraphics::~CGraphics() {
	if (m_pDevice)	m_pDevice->Release();
	if (m_pD3D)		m_pD3D->Release();
}


//==============================================================
// デバイス

// デバイスのリセット
bool CGraphics::ResetDevice() {

	// ウィンドウスタイルとサイズの設定：
	// デバイスリセットより先に行わないとウィンドウサイズが変わる
	if (m_bFullScreen) {
		SetWindowLong(m_HWnd, GWL_STYLE, WS_VISIBLE);
	} else {
		SetWindowLong(m_HWnd, GWL_STYLE, m_WindowStyle);
		RECT r={0, 0, m_nWidth, m_nHeight};
		AdjustWindowRect(&r, m_WindowStyle, GetMenu(m_HWnd)!=NULL);
		SetWindowPos(m_HWnd, HWND_NOTOPMOST, 
			0, 0, r.right-r.left, r.bottom-r.top, SWP_SHOWWINDOW);
	}

	// デバイスのパラメータ設定
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow=m_HWnd;
	d3dpp.BackBufferWidth=m_nWidth;
	d3dpp.BackBufferHeight=m_nHeight;
	d3dpp.Windowed=!m_bFullScreen;
	d3dpp.FullScreen_RefreshRateInHz=m_bFullScreen?m_nRefreshRate:0;
	d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat=D3DFMT_A8R8G8B8;
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	d3dpp.EnableAutoDepthStencil=TRUE;
	d3dpp.AutoDepthStencilFormat=m_DepthStencilFormat;

	// デバイスの作成またはリセット
	// GeForce系でスキニングを正常に動作させるには
	// D3DCREATE_MIXED_VERTEXPROCESSINGにする必要がある
	if (!m_pDevice) {
		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_HWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &m_pDevice))
		) 
		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_HWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice))
		)
		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_HWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice))
		)
		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_HWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice))
		) return false;
	} else {
		if (FAILED(m_pDevice->Reset(&d3dpp))) return false;
	}

	// 幅と高さの取得
	LPDIRECT3DSURFACE9 back_buffer;
	if (SUCCEEDED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer))) {
		D3DSURFACE_DESC desc;
		back_buffer->GetDesc(&desc);
		m_nWidth=desc.Width;
		m_nHeight=desc.Height;
		back_buffer->Release();
	} else {
		m_nWidth=m_nHeight=0;
	}

	// リフレッシュレートの取得
	D3DDISPLAYMODE mode;
	if (SUCCEEDED(m_pDevice->GetDisplayMode(0, &mode))) {
		m_nRefreshRate=mode.RefreshRate;
	} else {
		m_nRefreshRate=0;
	}

	m_bIsGDISurface=true;
	return true;
}


//==============================================================
// 画面（バックバッファ）の消去
void CGraphics::Clear(D3DCOLOR color) {
	m_pDevice->Clear(0, NULL, 
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, color, 1, 0 );
}


//==============================================================
// 画面の更新
bool CGraphics::Present() {
	if (FAILED(m_pDevice->Present(0, 0, 0, 0))) return false;
	m_bIsGDISurface=!m_bIsGDISurface;
	return true;
}


//==============================================================
// ライトの初期化
void CGraphics::SetLight(
	int id, D3DLIGHTTYPE type, 
	D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXCOLOR color, 
	float range
) {
	// ライトの作成
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type=type;
	light.Position=pos;
	D3DXVec3Normalize(&dir, &dir);
	light.Direction=dir;
	light.Diffuse=color, 
	light.Range=range;

	// ライトの設定
	m_pDevice->SetLight(id, &light);
	m_pDevice->LightEnable(id, TRUE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CGraphics::SetLight(
	int id, D3DLIGHTTYPE type, 
	float x, float y, float z,
	float range
) {
	SetLight(
		id, type, 
		D3DXVECTOR3(x, y, z), D3DXVECTOR3(x, y, z), 
		D3DXCOLOR(1, 1, 1, 1), range
	);
}

//==============================================================
// アルファ加算モードの設定
void CGraphics::SetAddAlpha(bool add_alpha) {
	if (add_alpha) {
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	} else {
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//==============================================================
// エフェクトのロード
ID3DXEffect* CGraphics::LoadEffect(string file) {
	ID3DXEffect* effect=NULL;
	LPD3DXBUFFER error;

	// エフェクトファイルの存在を確認する
	WIN32_FIND_DATA fd;
	if (FindFirstFile(file.c_str(), &fd)==INVALID_HANDLE_VALUE) {
		MessageBox(NULL, file.c_str(), "Shader File Not Found", MB_OK);
		PostQuitMessage(1);
		return NULL;
	}

	// エフェクトファイルのロード
	D3DXCreateEffectFromFile(
		m_pDevice, file.c_str(), 
		NULL, NULL, 0, NULL, &effect, &error);

	// コンパイルエラーの場合はエラーを表示して終了
	if (!effect) {
		MessageBox(
			NULL, 
			error?(LPCTSTR)error->GetBufferPointer():NULL, 
			"Shader Compile Error", MB_OK);
		PostQuitMessage(1);
		return NULL;
	}
	return effect;
}

//==============================================================
// テクスチャのロード
LPDIRECT3DTEXTURE9 CGraphics::LoadTexture(string file) {
	LPDIRECT3DTEXTURE9 texture=NULL;
	D3DXCreateTextureFromFile(m_pDevice, file.c_str(), &texture);
	return texture;
}

