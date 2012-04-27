#ifndef __CHWRESOURCEDATA_WIN32__H__
#define __CHWRESOURCEDATA_WIN32__H__

#include <windows.h>
#include <d3dx9.h>

#ifndef DIRECTINPUT_VERSION
	#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

typedef struct
{
	// インスタンス
	HINSTANCE   m_hInstance;

	// アクセラレータキー
	HACCEL	m_HAccel;

	// Direct3Dインタフェース、デバイス
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pDevice;

	// DirectInputインタフェース
	LPDIRECTINPUT8 m_pDI;

}HWCommonResourceDataWin32;

typedef struct
{	
	// ウィンドウハンドル
	HWND	m_HWnd;

	// 画面サイズ、リフレッシュレート
	mS32	m_nWindowWidth,		m_nWindowHeight;

	mS32	m_nRefreshRate;
	mBool	m_bFullScreen;

}HWResourceDataWin32;


#endif
