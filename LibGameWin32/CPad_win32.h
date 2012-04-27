#ifndef __CPAD_WIN32__H__
#define __CPAD_WIN32__H__

#include "..\LibGame\platform.h"
#if	PLATFORM_CHECK( PLATFORM_WIN32 )

/** //////////////////////////////////////////////////////////////
 *  @brief	���̓C���^�[�t�F�[�X�p�N���X
 *		 �@	�v���b�g�t�H�[���w�iWin32)
 *
 */ //////////////////////////////////////////////////////////////

#ifndef DIRECTINPUT_VERSION
	#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include <vector>
using namespace std;

///////////////////////////////////////////
///@brief win32�p�C���^�[�t�F�[�X���N���X
///
class CPadCommon;
class CPadCommonWin32 : public CPadCommon
{
	LPDIRECTINPUT8 m_hDI;

public:
	CPadCommonWin32();
	virtual ~CPadCommonWin32();

	virtual void CreateDevice();
	virtual void ReleaseDevice();
	virtual void Update();

	LPDIRECTINPUT8 GetInputDevice(){
		return m_hDI;
	}
};

///////////////////////////////////////////
//win32�p�p�b�h�N���X
class CPadData;
class CPadDataWin32 : public CPadData
{
public:
	CPadDataWin32(CPadControl* pCtrl,mS32 no,PadType type);
	~CPadDataWin32();
};

///////////////////////////////////////////
//win32�p�L�[�{�[�h�N���X
class CKeyData;
class CPadControl;
class CKeyDataWin32 : public CKeyData
{
	CPadControl*			m_pCtrl;
	LPDIRECTINPUTDEVICE8	m_hKeyboard;

	mS32					m_State;

	#define WIN32_KEY_STATE_MAX	(256)
	mS8						m_key_state[WIN32_KEY_STATE_MAX];
	

	void ClearState();
	void FreeResources();
public:
	CKeyDataWin32(CPadControl* pCtrl,mS32 no,PadType type);
	~CKeyDataWin32();

	void Create();
	void Release();
	void Update();

	PadState GetPadState();

	KeyData* GetKeyData();
};

///////////////////////////////////////////
//win32�p�}�E�X�N���X
class CMusData;
class CMusDataWin32 : public CMusData
{
public:
	CMusDataWin32(CPadControl* pCtrl,mS32 no,PadType type);
	~CMusDataWin32();

};

#endif
#endif