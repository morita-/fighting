#ifndef __PADTYPE__H__
#define __PADTYPE__H__

///�p�b�h�^�C�v
typedef enum
{
	ePadType_Pad=0,	//�p�b�h
	ePadType_Key,	//�L�[�{�[�h
	ePadType_Mouse,	//�}�E�X
	ePadType_Max
}PadType;

///ePadType_Pad�^�C�v���
enum{

	ePAD_Btn_U=0,	
	ePAD_Btn_D,		
	ePAD_Btn_L,		
	ePAD_Btn_R,		

	ePAD_Btn_A,		
	ePAD_Btn_B,		
	ePAD_Btn_X,		
	ePAD_Btn_Y,

	ePAD_Btn_L1,		
	ePAD_Btn_L2,	
	ePAD_Btn_L3,	
	ePAD_Btn_R1,		
	ePAD_Btn_R2,		
	ePAD_Btn_R3,

	ePAD_Btn_START,
	ePAD_Btn_SELECT,
	ePAD_Btn_Max,
	
	ePAD_Stk_L_U = ePAD_Btn_Max,
	ePAD_Stk_L_D,
	ePAD_Stk_L_L,
	ePAD_Stk_L_R,
	
	ePAD_Stk_R_U,
	ePAD_Stk_R_D,
	ePAD_Stk_R_L,
	ePAD_Stk_R_R,

	ePAD_BtnStk_Max
};
///ePadType_Pad�^�C�v���
#define PAD_Btn_U		(1<< ePAD_Btn_U)
#define PAD_Btn_D		(1<< ePAD_Btn_D)
#define PAD_Btn_L		(1<< ePAD_Btn_L)
#define PAD_Btn_R		(1<< ePAD_Btn_R)

#define PAD_Btn_A		(1<< ePAD_Btn_A)
#define PAD_Btn_B		(1<< ePAD_Btn_B)
#define PAD_Btn_X		(1<< ePAD_Btn_X)
#define PAD_Btn_Y		(1<< ePAD_Btn_Y)

#define PAD_Btn_L1		(1<< ePAD_Btn_L1)
#define PAD_Btn_L2		(1<< ePAD_Btn_L2)
#define PAD_Btn_L3		(1<< ePAD_Btn_L3)
#define PAD_Btn_R1		(1<< ePAD_Btn_R1)
#define PAD_Btn_R2		(1<< ePAD_Btn_R2)
#define PAD_Btn_R3		(1<< ePAD_Btn_R3)

#define PAD_Btn_START	(1<< ePAD_Btn_START)
#define PAD_Btn_SELECT	(1<< ePAD_Btn_SELECT)

#define PAD_Stk_L_U		(1<< ePAD_Stk_L_U)
#define PAD_Stk_L_D		(1<< ePAD_Stk_L_D)
#define PAD_Stk_L_L		(1<< ePAD_Stk_L_L)
#define PAD_Stk_L_R		(1<< ePAD_Stk_L_R)

#define PAD_Stk_R_U		(1<< ePAD_Stk_R_U)
#define PAD_Stk_R_D		(1<< ePAD_Stk_R_D)
#define PAD_Stk_R_L		(1<< ePAD_Stk_R_L)
#define PAD_Stk_R_R		(1<< ePAD_Stk_R_R)

///ePadType_Key�^�C�v���
enum
{
	ePAD_Key_Enter,		// Enter �L�[ 
	ePAD_Key_Escape,	// Esc �L�[ 
	ePAD_Key_Space,		// Space �L�[ 

	ePAD_Key_Left,		// �� �L�[ 
	ePAD_Key_Up,		// �� �L�[ 
	ePAD_Key_Right,		// �� �L�[ 
	ePAD_Key_Down,		// �� �L�[ 

	ePAD_Key_D0,		// 0 �L�[ 
	ePAD_Key_D1,		// 1 �L�[ 
	ePAD_Key_D2,		// 2 �L�[ 
	ePAD_Key_D3,		// 3 �L�[ 
	ePAD_Key_D4,		// 4 �L�[ 
	ePAD_Key_D5,		// 5 �L�[ 
	ePAD_Key_D6,		// 6 �L�[ 
	ePAD_Key_D7,		// 7 �L�[ 
	ePAD_Key_D8,		// 8 �L�[ 
	ePAD_Key_D9,		// 9 �L�[ 

	ePAD_Key_A,			// A �L�[ 
	ePAD_Key_B,			// B �L�[ 
	ePAD_Key_C,			// C �L�[ 
	ePAD_Key_D,			// D �L�[ 
	ePAD_Key_E,			// E �L�[ 
	ePAD_Key_F,			// F �L�[ 
	ePAD_Key_G,			// G �L�[ 
	ePAD_Key_H,			// H �L�[ 
	ePAD_Key_I,			// I �L�[ 
	ePAD_Key_J,			// J �L�[ 
	ePAD_Key_K,			// K �L�[ 
	ePAD_Key_L,			// L �L�[ 
	ePAD_Key_M,			// M �L�[ 
	ePAD_Key_N,			// N �L�[ 
	ePAD_Key_O,			// O �L�[ 
	ePAD_Key_P,			// P �L�[ 
	ePAD_Key_Q,			// Q �L�[ 
	ePAD_Key_R,			// R �L�[ 
	ePAD_Key_S,			// S �L�[ 
	ePAD_Key_T,			// T �L�[ 
	ePAD_Key_U,			// U �L�[ 
	ePAD_Key_V,			// V �L�[ 
	ePAD_Key_W,			// W �L�[ 
	ePAD_Key_X,			// X �L�[ 
	ePAD_Key_Y,			// Y �L�[ 
	ePAD_Key_Z,			// Z �L�[ 

	ePAD_Key_NumPad0, 		// ���l�L�[�p�b�h�� 0 �L�[ 
	ePAD_Key_NumPad1, 		// ���l�L�[�p�b�h�� 1 �L�[ 
	ePAD_Key_NumPad2, 		// ���l�L�[�p�b�h�� 2 �L�[ 
	ePAD_Key_NumPad3, 		// ���l�L�[�p�b�h�� 3 �L�[ 
	ePAD_Key_NumPad4, 		// ���l�L�[�p�b�h�� 4 �L�[ 
	ePAD_Key_NumPad5, 		// ���l�L�[�p�b�h�� 5 �L�[ 
	ePAD_Key_NumPad6, 		// ���l�L�[�p�b�h�� 6 �L�[ 
	ePAD_Key_NumPad7, 		// ���l�L�[�p�b�h�� 7 �L�[ 
	ePAD_Key_NumPad8, 		// ���l�L�[�p�b�h�� 8 �L�[ 
	ePAD_Key_NumPad9, 		// ���l�L�[�p�b�h�� 9 �L�[ 

	ePAD_Key_F1, 			// F1 �L�[ 
	ePAD_Key_F2, 			// F2 �L�[ 
	ePAD_Key_F3, 			// F3 �L�[ 
	ePAD_Key_F4, 			// F4 �L�[ 
	ePAD_Key_F5, 			// F5 �L�[ 
	ePAD_Key_F6, 			// F6 �L�[ 
	ePAD_Key_F7, 			// F7 �L�[ 
	ePAD_Key_F8, 			// F8 �L�[ 
	ePAD_Key_F9, 			// F9 �L�[ 
	ePAD_Key_F10, 			// F10 �L�[ 
	ePAD_Key_F11, 			// F11 �L�[ 
	ePAD_Key_F12, 			// F12 �L�[ 

	ePAD_Key_Back,			// BackSpace �L�[ 
	ePAD_Key_Tab,			// Tab �L�[ 
	ePAD_Key_Pause,			// Pause �L�[ 
	ePAD_Key_CapsLock,		// CapsLock �L�[ 

	ePAD_Key_PageUp,		// PageUp �L�[ 
	ePAD_Key_PageDown,		// PageDown �L�[ 
	ePAD_Key_End,			// End �L�[ 
	ePAD_Key_Home,			// Home �L�[ 

	ePAD_Key_Insert,		// Ins �L�[ 
	ePAD_Key_Delete,		// Del �L�[ 

	ePAD_Key_NumLock, 		// NumLock �L�[ 
	ePAD_Key_Scroll, 		// ScrollLock �L�[ 
	ePAD_Key_NumEnter, 		// NumEnter �L�[ 

	ePAD_Key_LShiftKey,		// ���� Shift �L�[ 
	ePAD_Key_RShiftKey,		// �E�� Shift �L�[ 
	ePAD_Key_LControlKey, 	// ���� Ctrl �L�[ 
	ePAD_Key_RControlKey, 	// �E�� Ctrl �L�[ 
	ePAD_Key_LAlt,			// ���� Alt �L�[ 
	ePAD_Key_RAlt,			// �E�� Alt �L�[ 

	ePAD_Key_MAX
};


#define PAD_Key_Enter		(1<<ePAD_Key_Enter)	// Enter �L�[ 
#define PAD_Key_Escape		(1<<ePAD_Key_Escape)	// Esc �L�[ 
#define PAD_Key_Space		(1<<ePAD_Key_Space)	// Space �L�[ 

#define PAD_Key_Left		(1<<ePAD_Key_Left)	// �� �L�[ 
#define PAD_Key_Up			(1<<ePAD_Key_Up)	// �� �L�[ 
#define PAD_Key_Right		(1<<ePAD_Key_Right)	// �� �L�[ 
#define PAD_Key_Down		(1<<ePAD_Key_Down)	// �� �L�[ 

#define PAD_Key_D0			(1<<ePAD_Key_D0)	// 0 �L�[ 
#define PAD_Key_D1			(1<<ePAD_Key_D1)	// 1 �L�[ 
#define PAD_Key_D2			(1<<ePAD_Key_D2)	// 2 �L�[ 
#define PAD_Key_D3			(1<<ePAD_Key_D3)	// 3 �L�[ 
#define PAD_Key_D4			(1<<ePAD_Key_D4)	// 4 �L�[ 
#define PAD_Key_D5			(1<<ePAD_Key_D5)	// 5 �L�[ 
#define PAD_Key_D6			(1<<ePAD_Key_D6)	// 6 �L�[ 
#define PAD_Key_D7			(1<<ePAD_Key_D7)	// 7 �L�[ 
#define PAD_Key_D8			(1<<ePAD_Key_D8)	// 8 �L�[ 
#define PAD_Key_D9			(1<<ePAD_Key_D9)	// 9 �L�[ 

#define PAD_Key_A			(1<<ePAD_Key_A)	// A �L�[ 
#define PAD_Key_B			(1<<ePAD_Key_B)	// B �L�[ 
#define PAD_Key_C			(1<<ePAD_Key_C)	// C �L�[ 
#define PAD_Key_D			(1<<ePAD_Key_D)	// D �L�[ 
#define PAD_Key_E			(1<<ePAD_Key_E)	// E �L�[ 
#define PAD_Key_F			(1<<ePAD_Key_F)	// F �L�[ 
#define PAD_Key_G			(1<<ePAD_Key_G)	// G �L�[ 
#define PAD_Key_H			(1<<ePAD_Key_H)	// H �L�[ 
#define PAD_Key_I			(1<<ePAD_Key_I)	// I �L�[ 
#define PAD_Key_J			(1<<ePAD_Key_J)	// J �L�[ 
#define PAD_Key_K			(1<<ePAD_Key_K)	// K �L�[ 
#define PAD_Key_L			(1<<ePAD_Key_L)	// L �L�[ 
#define PAD_Key_M			(1<<ePAD_Key_M)	// M �L�[ 
#define PAD_Key_N			(1<<ePAD_Key_N)	// N �L�[ 
#define PAD_Key_O			(1<<ePAD_Key_O)	// O �L�[ 
#define PAD_Key_P			(1<<ePAD_Key_P)	// P �L�[ 
#define PAD_Key_Q			(1<<ePAD_Key_Q)	// Q �L�[ 
#define PAD_Key_R			(1<<ePAD_Key_R)	// R �L�[ 
#define PAD_Key_S			(1<<ePAD_Key_S)	// S �L�[ 
#define PAD_Key_T			(1<<ePAD_Key_T)	// T �L�[ 
#define PAD_Key_U			(1<<ePAD_Key_U)	// U �L�[ 
#define PAD_Key_V			(1<<ePAD_Key_V)	// V �L�[ 
#define PAD_Key_W			(1<<ePAD_Key_W)	// W �L�[ 
#define PAD_Key_X			(1<<ePAD_Key_X)	// X �L�[ 
#define PAD_Key_Y			(1<<ePAD_Key_Y)	// Y �L�[ 
#define PAD_Key_Z			(1<<ePAD_Key_Z)	// Z �L�[ 

#define PAD_Key_NumPad0 		(1<<ePAD_Key_NumPad0)		// ���l�L�[�p�b�h�� 0 �L�[ 
#define PAD_Key_NumPad1 		(1<<ePAD_Key_NumPad1)		// ���l�L�[�p�b�h�� 1 �L�[ 
#define PAD_Key_NumPad2 		(1<<ePAD_Key_NumPad2)		// ���l�L�[�p�b�h�� 2 �L�[ 
#define PAD_Key_NumPad3 		(1<<ePAD_Key_NumPad3)		// ���l�L�[�p�b�h�� 3 �L�[ 
#define PAD_Key_NumPad4 		(1<<ePAD_Key_NumPad4)		// ���l�L�[�p�b�h�� 4 �L�[ 
#define PAD_Key_NumPad5 		(1<<ePAD_Key_NumPad5)		// ���l�L�[�p�b�h�� 5 �L�[ 
#define PAD_Key_NumPad6 		(1<<ePAD_Key_NumPad6)		// ���l�L�[�p�b�h�� 6 �L�[ 
#define PAD_Key_NumPad7 		(1<<ePAD_Key_NumPad7)		// ���l�L�[�p�b�h�� 7 �L�[ 
#define PAD_Key_NumPad8 		(1<<ePAD_Key_NumPad8)		// ���l�L�[�p�b�h�� 8 �L�[ 
#define PAD_Key_NumPad9 		(1<<ePAD_Key_NumPad9)		// ���l�L�[�p�b�h�� 9 �L�[ 

#define PAD_Key_F1 			(1<<ePAD_Key_F1)	// F1 �L�[ 
#define PAD_Key_F2 			(1<<ePAD_Key_F2)	// F2 �L�[ 
#define PAD_Key_F3 			(1<<ePAD_Key_F3)	// F3 �L�[ 
#define PAD_Key_F4 			(1<<ePAD_Key_F4)	// F4 �L�[ 
#define PAD_Key_F5 			(1<<ePAD_Key_F5)	// F5 �L�[ 
#define PAD_Key_F6 			(1<<ePAD_Key_F6)	// F6 �L�[ 
#define PAD_Key_F7 			(1<<ePAD_Key_F7)	// F7 �L�[ 
#define PAD_Key_F8 			(1<<ePAD_Key_F8)	// F8 �L�[ 
#define PAD_Key_F9 			(1<<ePAD_Key_F9)	// F9 �L�[ 
#define PAD_Key_F10 		(1<<ePAD_Key_F10)	// F10 �L�[ 
#define PAD_Key_F11 		(1<<ePAD_Key_F11)	// F11 �L�[ 
#define PAD_Key_F12 		(1<<ePAD_Key_F12)	// F12 �L�[ 
#define PAD_Key_F13 		(1<<ePAD_Key_F13)	// F13 �L�[ 
#define PAD_Key_F14 		(1<<ePAD_Key_F14)	// F14 �L�[ 
#define PAD_Key_F15 		(1<<ePAD_Key_F15)	// F15 �L�[ 
#define PAD_Key_F16 		(1<<ePAD_Key_F16)	// F16 �L�[ 
#define PAD_Key_F17 		(1<<ePAD_Key_F17)	// F17 �L�[ 
#define PAD_Key_F18 		(1<<ePAD_Key_F18)	// F18 �L�[ 
#define PAD_Key_F19 		(1<<ePAD_Key_F19)	// F19 �L�[ 
#define PAD_Key_F20 		(1<<ePAD_Key_F20)	// F20 �L�[ 
#define PAD_Key_F21 		(1<<ePAD_Key_F21)	// F21 �L�[ 
#define PAD_Key_F22 		(1<<ePAD_Key_F22)	// F22 �L�[ 
#define PAD_Key_F23 		(1<<ePAD_Key_F23)	// F23 �L�[ 
#define PAD_Key_F24 		(1<<ePAD_Key_F24)	// F24 �L�[ 

#define PAD_Key_Back		(1<<ePAD_Key_Back)	// BackSpace �L�[ 
#define PAD_Key_Tab			(1<<ePAD_Key_Tab)	// Tab �L�[ 
#define PAD_Key_Pause		(1<<ePAD_Key_Pause)	// Pause �L�[ 
#define PAD_Key_CapsLock	(1<<ePAD_Key_CapsLock)	// CapsLock �L�[ 

#define PAD_Key_PageUp		(1<<ePAD_Key_PageUp)	// PageUp �L�[ 
#define PAD_Key_PageDown	(1<<ePAD_Key_PageDown)	// PageDown �L�[ 
#define PAD_Key_End			(1<<ePAD_Key_End)	// End �L�[ 
#define PAD_Key_Home		(1<<ePAD_Key_Home)	// Home �L�[ 

#define PAD_Key_Insert		(1<<ePAD_Key_Insert)	// Ins �L�[ 
#define PAD_Key_Delete		(1<<ePAD_Key_Delete)	// Del �L�[ 

#define PAD_Key_NumLock 	(1<<ePAD_Key_NumLock)	// NumLock �L�[ 
#define PAD_Key_Scroll 		(1<<ePAD_Key_Scroll)	// ScrollLock �L�[ 
#define PAD_Key_NumEnter 	(1<<ePAD_Key_NumEnter) 		// NumEnter �L�[ 


#define PAD_Key_LShiftKey		(1<<ePAD_Key_LShiftKey)		// ���� Shift �L�[ 
#define PAD_Key_RShiftKey		(1<<ePAD_Key_RShiftKey)		// �E�� Shift �L�[ 
#define PAD_Key_LControlKey 	(1<<ePAD_Key_LControlKey)		// ���� Ctrl �L�[ 
#define PAD_Key_RControlKey 	(1<<ePAD_Key_RControlKey)		// �E�� Ctrl �L�[ 
#define PAD_Key_LMenu			(1<<ePAD_Key_LMenu)		// ���� Alt �L�[ 
#define PAD_Key_RMenu			(1<<ePAD_Key_RMenu)		// �E�� Alt �L�[ 

///ePadType_Mouse�^�C�v���
#define PAD_Mus_LButton 		(1<<0)		// �}�E�X�̍��{�^�� 
#define PAD_Mus_RButton 		(1<<1)		// �}�E�X�̉E�{�^�� 
#define PAD_Mus_MButton 		(1<<2)		// �}�E�X�̒����{�^�� (3 �{�^�� �}�E�X�̏ꍇ) 
#define PAD_Mus_XButton1 		(1<<3)		// x �}�E�X�� 1 �Ԗڂ̃{�^�� (5 �{�^�� �}�E�X�̏ꍇ) 
#define PAD_Mus_XButton2 		(1<<4)		// x �}�E�X�� 2 �Ԗڂ̃{�^�� (5 �{�^�� �}�E�X�̏ꍇ) 


///�p�b�h�f�[�^�\����
typedef enum{
	ePadTypePad_PS2,
	ePadTypePad_MAX
}ePadTypePad;
#define PAD_Pad_ENABLE_ANALOG		(1<<0)
#define PAD_Pad_ENABLE_ANALOG2		(1<<1)
#define PAD_Pad_ENABLE_ANALOG_BTN	(1<<2)
typedef struct
{
	mF32	x,y;
}PadData_Alg;
typedef struct
{
	ePadTypePad	type;
	mS32		flag;

	mS32		p_on;
	mS32		p_press;

	mF32		pa_on[ePAD_Btn_Max];
	PadData_Alg alg;
	PadData_Alg alg2;
}PadData;

///�L�[�f�[�^�\����
typedef enum{
	ePadTypeKey_Normal,
	ePadTypeKey_MAX
}ePadTypeKey;
#define KEY_DATA_FLG_ON		(1<<0)	//on
#define KEY_DATA_FLG_PRESS	(1<<1)	//press
typedef struct
{
	ePadTypeKey	type;
	mS32		flag;

	mU8			k_flg[ePAD_Key_MAX];
}KeyData;

///�}�E�X�f�[�^�\����
typedef enum{
	ePadTypeMus_Normal,
	ePadTypeMus_MAX
}ePadTypeMus;
#define MUS_DATA_FLAG_CLICK		(1<<0)
#define MUS_DATA_FLAG_DBL_CLICK	(1<<1)
#define MUS_DATA_FLAG_RELEASE	(1<<2)
typedef struct
{
	ePadTypeMus	type;
	mS32		flag;

	mF32		x,y;
	mS32		c_flag;
}MusData;



///�p�b�h��
#define PAD_DISCONNECT	0
#define PAD_CONNECT		1

typedef mS32	PadState;

#endif