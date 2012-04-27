
#include "..\LibGame\global.h"

#include "..\LibGame\CPad.h"
#include "CPad_win32.h"

#include "..\LibGame\CHWResource.h"
#include "CHWResource_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )


//////////////////////////////////////////////////////
///
CPadCommonWin32::CPadCommonWin32(){
	m_hDI=NULL;
}
CPadCommonWin32::~CPadCommonWin32(){
	ReleaseDevice();
}

void CPadCommonWin32::CreateDevice(){
	HRESULT hr;

	// DirectInputインタフェースの作成
	hr = DirectInput8Create( GetModuleHandle(NULL), 
							 DIRECTINPUT_VERSION, 
							 IID_IDirectInput8, 
							 (void**)&m_hDI, 
							 NULL);
	if (FAILED(hr))return;



}

void CPadCommonWin32::ReleaseDevice(){
	if (m_hDI) m_hDI->Release();
	m_hDI=NULL;
}
void CPadCommonWin32::Update(){
}

//////////////////////////////////////////////////////
///
CKeyDataWin32::CKeyDataWin32(CPadControl* pCtrl,mS32 no,PadType type) : CKeyData(pCtrl,no,type)
{
	m_pCtrl = pCtrl;
	m_hKeyboard=NULL;

	ClearState();

}
CKeyDataWin32::~CKeyDataWin32(){
	Release();
}



void CKeyDataWin32::Create(){
	HRESULT hr;

	LPDIRECTINPUT8 hDI = m_pCtrl->GetInputDevice();
	if(!hDI){
		Release();
		return;
	}

	// キーボードデバイスの取得と初期化
	hr = hDI->CreateDevice( GUID_SysKeyboard, &m_hKeyboard, NULL );
	if (FAILED(hr)){
		Release();
		return; 
	}

	hr = m_hKeyboard->SetDataFormat( &c_dfDIKeyboard );
	if (FAILED(hr)){
		Release();
		return; 
	}

	const HWResourceDataRef* pRef = CHWResourceRef::GetInstance()->GetHWResourceData();
	hr = m_hKeyboard->SetCooperativeLevel( pRef->m_HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)){
		Release();
		return; 
	}

}
void CKeyDataWin32::Release(){
	if (m_hKeyboard) {
		m_hKeyboard->Unacquire();
		m_hKeyboard->Release();
	}

	m_hKeyboard = NULL;
}

void CKeyDataWin32::Update(){


	ClearState();
		
	// キーボード
	if (!m_hKeyboard) {
		return;
	}

	#define KEYDOWN(state,key)				((state[key]&0x80)!=0)				//0x80は押されているときにフラグが立つ
	#define KEYPRESS(onflg,prestate,key)	(onflg && prestate[key]&0x80==0)	//0x80は押されているときにフラグが立つ


	char key_state[WIN32_KEY_STATE_MAX];
	HRESULT hrAcq,hrSts;

	hrAcq = m_hKeyboard->Acquire();
	hrSts = m_hKeyboard->GetDeviceState( sizeof(key_state),key_state);

	if ( FAILED(hrAcq) ){
		return;
	}

	switch( hrSts ){
	case DIERR_INPUTLOST:
	case DIERR_NOTACQUIRED:
		break;
	default:break;
	}

	if ( FAILED(hrSts) ) {
		return;
	}

//	KEYDOWN(key_state,	DIK_RETURN)?									m_KeyData.k_flg[ePAD_Key_Enter	]  = KEY_DATA_FLG_ON;
//	KEYPRESS(m_KeyData.k_flg[ePAD_Key_Enter	],m_key_state,DIK_RETURN)?	m_KeyData.k_flg[ePAD_Key_Enter	] |= KEY_DATA_FLG_PRESS;

	#define KEYFLGSET(state,prestate,flg,key,flgkey)								\
				if( (state[key]&0x80)!=0 ){											\
					flg[flgkey] = KEY_DATA_FLG_ON;									\
					if( (prestate[key]&0x80)==0 )flg[flgkey] = KEY_DATA_FLG_ON;		\
				}

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_RETURN,		ePAD_Key_Enter);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_ESCAPE,		ePAD_Key_Escape);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_SPACE,		ePAD_Key_Space);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_LEFT,		ePAD_Key_Left);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_UP,			ePAD_Key_Up);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_RIGHT,		ePAD_Key_Right);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_DOWN,		ePAD_Key_Down);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_0,			ePAD_Key_D0);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_1,			ePAD_Key_D1);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_2,			ePAD_Key_D2);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_3,			ePAD_Key_D3);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_4,			ePAD_Key_D4);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_5,			ePAD_Key_D5);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_6,			ePAD_Key_D6);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_7,			ePAD_Key_D7);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_8,			ePAD_Key_D8);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_9,			ePAD_Key_D9);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_A,			ePAD_Key_A);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_B,			ePAD_Key_B);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_C,			ePAD_Key_C);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_D,			ePAD_Key_D);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_E,			ePAD_Key_E);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F,			ePAD_Key_F);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_G,			ePAD_Key_G);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_H,			ePAD_Key_H);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_I,			ePAD_Key_I);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_J,			ePAD_Key_J);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_K,			ePAD_Key_K);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_L,			ePAD_Key_L);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_M,			ePAD_Key_M);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_N,			ePAD_Key_N);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_O,			ePAD_Key_O);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_P,			ePAD_Key_P);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_Q,			ePAD_Key_Q);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_R,			ePAD_Key_R);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_S,			ePAD_Key_S);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_T,			ePAD_Key_T);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_U,			ePAD_Key_U);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_V,			ePAD_Key_V);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_W,			ePAD_Key_W);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_X,			ePAD_Key_X);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_Y,			ePAD_Key_Y);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_Z,			ePAD_Key_Z);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD0,	ePAD_Key_NumPad0);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD1,	ePAD_Key_NumPad1);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD2,	ePAD_Key_NumPad2);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD3,	ePAD_Key_NumPad3);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD4,	ePAD_Key_NumPad4);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD5,	ePAD_Key_NumPad5);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD6,	ePAD_Key_NumPad6);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD7,	ePAD_Key_NumPad7);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD8,	ePAD_Key_NumPad8);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPAD9,	ePAD_Key_NumPad9);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F1,			ePAD_Key_F1);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F2,			ePAD_Key_F2);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F3,			ePAD_Key_F3);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F4,			ePAD_Key_F4);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F5,			ePAD_Key_F5);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F6,			ePAD_Key_F6);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F7,			ePAD_Key_F7);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F8,			ePAD_Key_F8);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F9,			ePAD_Key_F9);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F10,		ePAD_Key_F10);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F11,		ePAD_Key_F11);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_F12,		ePAD_Key_F12);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_BACK,		ePAD_Key_Back);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_TAB,		ePAD_Key_Tab);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_PAUSE,		ePAD_Key_Pause);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_CAPSLOCK,	ePAD_Key_CapsLock);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_PGUP,		ePAD_Key_PageUp);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_PGDN,		ePAD_Key_PageDown);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_END,		ePAD_Key_End);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_HOME,		ePAD_Key_Home);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_INSERT,		ePAD_Key_Insert);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_DELETE,		ePAD_Key_Delete);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMLOCK,	ePAD_Key_NumLock);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_SCROLL,		ePAD_Key_Scroll);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_NUMPADENTER,ePAD_Key_NumEnter);

	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_LSHIFT,		ePAD_Key_LShiftKey);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_RSHIFT,		ePAD_Key_RShiftKey);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_LCONTROL,	ePAD_Key_LControlKey);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_RCONTROL,	ePAD_Key_RControlKey);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_LALT,		ePAD_Key_LAlt);
	KEYFLGSET(key_state,m_key_state,m_KeyData.k_flg,DIK_RALT,		ePAD_Key_RAlt);

	memcpy(m_key_state,key_state,sizeof(mS8)*WIN32_KEY_STATE_MAX);
	m_State = PAD_CONNECT;

}

void CKeyDataWin32::ClearState(){
	memset(&m_KeyData,0,sizeof(m_KeyData));
	m_State = PAD_DISCONNECT;
}


PadState CKeyDataWin32::GetPadState(){
	return m_State;
}

KeyData* CKeyDataWin32::GetKeyData(){
	return &m_KeyData;
}


#endif