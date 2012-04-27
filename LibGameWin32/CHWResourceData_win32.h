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
	// �C���X�^���X
	HINSTANCE   m_hInstance;

	// �A�N�Z�����[�^�L�[
	HACCEL	m_HAccel;

	// Direct3D�C���^�t�F�[�X�A�f�o�C�X
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pDevice;

	// DirectInput�C���^�t�F�[�X
	LPDIRECTINPUT8 m_pDI;

}HWCommonResourceDataWin32;

typedef struct
{	
	// �E�B���h�E�n���h��
	HWND	m_HWnd;

	// ��ʃT�C�Y�A���t���b�V�����[�g
	mS32	m_nWindowWidth,		m_nWindowHeight;

	mS32	m_nRefreshRate;
	mBool	m_bFullScreen;

}HWResourceDataWin32;


#endif
