#ifndef __PADTYPE__H__
#define __PADTYPE__H__

///パッドタイプ
typedef enum
{
	ePadType_Pad=0,	//パッド
	ePadType_Key,	//キーボード
	ePadType_Mouse,	//マウス
	ePadType_Max
}PadType;

///ePadType_Padタイプ情報
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
///ePadType_Padタイプ情報
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

///ePadType_Keyタイプ情報
enum
{
	ePAD_Key_Enter,		// Enter キー 
	ePAD_Key_Escape,	// Esc キー 
	ePAD_Key_Space,		// Space キー 

	ePAD_Key_Left,		// ← キー 
	ePAD_Key_Up,		// ↑ キー 
	ePAD_Key_Right,		// → キー 
	ePAD_Key_Down,		// ↓ キー 

	ePAD_Key_D0,		// 0 キー 
	ePAD_Key_D1,		// 1 キー 
	ePAD_Key_D2,		// 2 キー 
	ePAD_Key_D3,		// 3 キー 
	ePAD_Key_D4,		// 4 キー 
	ePAD_Key_D5,		// 5 キー 
	ePAD_Key_D6,		// 6 キー 
	ePAD_Key_D7,		// 7 キー 
	ePAD_Key_D8,		// 8 キー 
	ePAD_Key_D9,		// 9 キー 

	ePAD_Key_A,			// A キー 
	ePAD_Key_B,			// B キー 
	ePAD_Key_C,			// C キー 
	ePAD_Key_D,			// D キー 
	ePAD_Key_E,			// E キー 
	ePAD_Key_F,			// F キー 
	ePAD_Key_G,			// G キー 
	ePAD_Key_H,			// H キー 
	ePAD_Key_I,			// I キー 
	ePAD_Key_J,			// J キー 
	ePAD_Key_K,			// K キー 
	ePAD_Key_L,			// L キー 
	ePAD_Key_M,			// M キー 
	ePAD_Key_N,			// N キー 
	ePAD_Key_O,			// O キー 
	ePAD_Key_P,			// P キー 
	ePAD_Key_Q,			// Q キー 
	ePAD_Key_R,			// R キー 
	ePAD_Key_S,			// S キー 
	ePAD_Key_T,			// T キー 
	ePAD_Key_U,			// U キー 
	ePAD_Key_V,			// V キー 
	ePAD_Key_W,			// W キー 
	ePAD_Key_X,			// X キー 
	ePAD_Key_Y,			// Y キー 
	ePAD_Key_Z,			// Z キー 

	ePAD_Key_NumPad0, 		// 数値キーパッドの 0 キー 
	ePAD_Key_NumPad1, 		// 数値キーパッドの 1 キー 
	ePAD_Key_NumPad2, 		// 数値キーパッドの 2 キー 
	ePAD_Key_NumPad3, 		// 数値キーパッドの 3 キー 
	ePAD_Key_NumPad4, 		// 数値キーパッドの 4 キー 
	ePAD_Key_NumPad5, 		// 数値キーパッドの 5 キー 
	ePAD_Key_NumPad6, 		// 数値キーパッドの 6 キー 
	ePAD_Key_NumPad7, 		// 数値キーパッドの 7 キー 
	ePAD_Key_NumPad8, 		// 数値キーパッドの 8 キー 
	ePAD_Key_NumPad9, 		// 数値キーパッドの 9 キー 

	ePAD_Key_F1, 			// F1 キー 
	ePAD_Key_F2, 			// F2 キー 
	ePAD_Key_F3, 			// F3 キー 
	ePAD_Key_F4, 			// F4 キー 
	ePAD_Key_F5, 			// F5 キー 
	ePAD_Key_F6, 			// F6 キー 
	ePAD_Key_F7, 			// F7 キー 
	ePAD_Key_F8, 			// F8 キー 
	ePAD_Key_F9, 			// F9 キー 
	ePAD_Key_F10, 			// F10 キー 
	ePAD_Key_F11, 			// F11 キー 
	ePAD_Key_F12, 			// F12 キー 

	ePAD_Key_Back,			// BackSpace キー 
	ePAD_Key_Tab,			// Tab キー 
	ePAD_Key_Pause,			// Pause キー 
	ePAD_Key_CapsLock,		// CapsLock キー 

	ePAD_Key_PageUp,		// PageUp キー 
	ePAD_Key_PageDown,		// PageDown キー 
	ePAD_Key_End,			// End キー 
	ePAD_Key_Home,			// Home キー 

	ePAD_Key_Insert,		// Ins キー 
	ePAD_Key_Delete,		// Del キー 

	ePAD_Key_NumLock, 		// NumLock キー 
	ePAD_Key_Scroll, 		// ScrollLock キー 
	ePAD_Key_NumEnter, 		// NumEnter キー 

	ePAD_Key_LShiftKey,		// 左の Shift キー 
	ePAD_Key_RShiftKey,		// 右の Shift キー 
	ePAD_Key_LControlKey, 	// 左の Ctrl キー 
	ePAD_Key_RControlKey, 	// 右の Ctrl キー 
	ePAD_Key_LAlt,			// 左の Alt キー 
	ePAD_Key_RAlt,			// 右の Alt キー 

	ePAD_Key_MAX
};


#define PAD_Key_Enter		(1<<ePAD_Key_Enter)	// Enter キー 
#define PAD_Key_Escape		(1<<ePAD_Key_Escape)	// Esc キー 
#define PAD_Key_Space		(1<<ePAD_Key_Space)	// Space キー 

#define PAD_Key_Left		(1<<ePAD_Key_Left)	// ← キー 
#define PAD_Key_Up			(1<<ePAD_Key_Up)	// ↑ キー 
#define PAD_Key_Right		(1<<ePAD_Key_Right)	// → キー 
#define PAD_Key_Down		(1<<ePAD_Key_Down)	// ↓ キー 

#define PAD_Key_D0			(1<<ePAD_Key_D0)	// 0 キー 
#define PAD_Key_D1			(1<<ePAD_Key_D1)	// 1 キー 
#define PAD_Key_D2			(1<<ePAD_Key_D2)	// 2 キー 
#define PAD_Key_D3			(1<<ePAD_Key_D3)	// 3 キー 
#define PAD_Key_D4			(1<<ePAD_Key_D4)	// 4 キー 
#define PAD_Key_D5			(1<<ePAD_Key_D5)	// 5 キー 
#define PAD_Key_D6			(1<<ePAD_Key_D6)	// 6 キー 
#define PAD_Key_D7			(1<<ePAD_Key_D7)	// 7 キー 
#define PAD_Key_D8			(1<<ePAD_Key_D8)	// 8 キー 
#define PAD_Key_D9			(1<<ePAD_Key_D9)	// 9 キー 

#define PAD_Key_A			(1<<ePAD_Key_A)	// A キー 
#define PAD_Key_B			(1<<ePAD_Key_B)	// B キー 
#define PAD_Key_C			(1<<ePAD_Key_C)	// C キー 
#define PAD_Key_D			(1<<ePAD_Key_D)	// D キー 
#define PAD_Key_E			(1<<ePAD_Key_E)	// E キー 
#define PAD_Key_F			(1<<ePAD_Key_F)	// F キー 
#define PAD_Key_G			(1<<ePAD_Key_G)	// G キー 
#define PAD_Key_H			(1<<ePAD_Key_H)	// H キー 
#define PAD_Key_I			(1<<ePAD_Key_I)	// I キー 
#define PAD_Key_J			(1<<ePAD_Key_J)	// J キー 
#define PAD_Key_K			(1<<ePAD_Key_K)	// K キー 
#define PAD_Key_L			(1<<ePAD_Key_L)	// L キー 
#define PAD_Key_M			(1<<ePAD_Key_M)	// M キー 
#define PAD_Key_N			(1<<ePAD_Key_N)	// N キー 
#define PAD_Key_O			(1<<ePAD_Key_O)	// O キー 
#define PAD_Key_P			(1<<ePAD_Key_P)	// P キー 
#define PAD_Key_Q			(1<<ePAD_Key_Q)	// Q キー 
#define PAD_Key_R			(1<<ePAD_Key_R)	// R キー 
#define PAD_Key_S			(1<<ePAD_Key_S)	// S キー 
#define PAD_Key_T			(1<<ePAD_Key_T)	// T キー 
#define PAD_Key_U			(1<<ePAD_Key_U)	// U キー 
#define PAD_Key_V			(1<<ePAD_Key_V)	// V キー 
#define PAD_Key_W			(1<<ePAD_Key_W)	// W キー 
#define PAD_Key_X			(1<<ePAD_Key_X)	// X キー 
#define PAD_Key_Y			(1<<ePAD_Key_Y)	// Y キー 
#define PAD_Key_Z			(1<<ePAD_Key_Z)	// Z キー 

#define PAD_Key_NumPad0 		(1<<ePAD_Key_NumPad0)		// 数値キーパッドの 0 キー 
#define PAD_Key_NumPad1 		(1<<ePAD_Key_NumPad1)		// 数値キーパッドの 1 キー 
#define PAD_Key_NumPad2 		(1<<ePAD_Key_NumPad2)		// 数値キーパッドの 2 キー 
#define PAD_Key_NumPad3 		(1<<ePAD_Key_NumPad3)		// 数値キーパッドの 3 キー 
#define PAD_Key_NumPad4 		(1<<ePAD_Key_NumPad4)		// 数値キーパッドの 4 キー 
#define PAD_Key_NumPad5 		(1<<ePAD_Key_NumPad5)		// 数値キーパッドの 5 キー 
#define PAD_Key_NumPad6 		(1<<ePAD_Key_NumPad6)		// 数値キーパッドの 6 キー 
#define PAD_Key_NumPad7 		(1<<ePAD_Key_NumPad7)		// 数値キーパッドの 7 キー 
#define PAD_Key_NumPad8 		(1<<ePAD_Key_NumPad8)		// 数値キーパッドの 8 キー 
#define PAD_Key_NumPad9 		(1<<ePAD_Key_NumPad9)		// 数値キーパッドの 9 キー 

#define PAD_Key_F1 			(1<<ePAD_Key_F1)	// F1 キー 
#define PAD_Key_F2 			(1<<ePAD_Key_F2)	// F2 キー 
#define PAD_Key_F3 			(1<<ePAD_Key_F3)	// F3 キー 
#define PAD_Key_F4 			(1<<ePAD_Key_F4)	// F4 キー 
#define PAD_Key_F5 			(1<<ePAD_Key_F5)	// F5 キー 
#define PAD_Key_F6 			(1<<ePAD_Key_F6)	// F6 キー 
#define PAD_Key_F7 			(1<<ePAD_Key_F7)	// F7 キー 
#define PAD_Key_F8 			(1<<ePAD_Key_F8)	// F8 キー 
#define PAD_Key_F9 			(1<<ePAD_Key_F9)	// F9 キー 
#define PAD_Key_F10 		(1<<ePAD_Key_F10)	// F10 キー 
#define PAD_Key_F11 		(1<<ePAD_Key_F11)	// F11 キー 
#define PAD_Key_F12 		(1<<ePAD_Key_F12)	// F12 キー 
#define PAD_Key_F13 		(1<<ePAD_Key_F13)	// F13 キー 
#define PAD_Key_F14 		(1<<ePAD_Key_F14)	// F14 キー 
#define PAD_Key_F15 		(1<<ePAD_Key_F15)	// F15 キー 
#define PAD_Key_F16 		(1<<ePAD_Key_F16)	// F16 キー 
#define PAD_Key_F17 		(1<<ePAD_Key_F17)	// F17 キー 
#define PAD_Key_F18 		(1<<ePAD_Key_F18)	// F18 キー 
#define PAD_Key_F19 		(1<<ePAD_Key_F19)	// F19 キー 
#define PAD_Key_F20 		(1<<ePAD_Key_F20)	// F20 キー 
#define PAD_Key_F21 		(1<<ePAD_Key_F21)	// F21 キー 
#define PAD_Key_F22 		(1<<ePAD_Key_F22)	// F22 キー 
#define PAD_Key_F23 		(1<<ePAD_Key_F23)	// F23 キー 
#define PAD_Key_F24 		(1<<ePAD_Key_F24)	// F24 キー 

#define PAD_Key_Back		(1<<ePAD_Key_Back)	// BackSpace キー 
#define PAD_Key_Tab			(1<<ePAD_Key_Tab)	// Tab キー 
#define PAD_Key_Pause		(1<<ePAD_Key_Pause)	// Pause キー 
#define PAD_Key_CapsLock	(1<<ePAD_Key_CapsLock)	// CapsLock キー 

#define PAD_Key_PageUp		(1<<ePAD_Key_PageUp)	// PageUp キー 
#define PAD_Key_PageDown	(1<<ePAD_Key_PageDown)	// PageDown キー 
#define PAD_Key_End			(1<<ePAD_Key_End)	// End キー 
#define PAD_Key_Home		(1<<ePAD_Key_Home)	// Home キー 

#define PAD_Key_Insert		(1<<ePAD_Key_Insert)	// Ins キー 
#define PAD_Key_Delete		(1<<ePAD_Key_Delete)	// Del キー 

#define PAD_Key_NumLock 	(1<<ePAD_Key_NumLock)	// NumLock キー 
#define PAD_Key_Scroll 		(1<<ePAD_Key_Scroll)	// ScrollLock キー 
#define PAD_Key_NumEnter 	(1<<ePAD_Key_NumEnter) 		// NumEnter キー 


#define PAD_Key_LShiftKey		(1<<ePAD_Key_LShiftKey)		// 左の Shift キー 
#define PAD_Key_RShiftKey		(1<<ePAD_Key_RShiftKey)		// 右の Shift キー 
#define PAD_Key_LControlKey 	(1<<ePAD_Key_LControlKey)		// 左の Ctrl キー 
#define PAD_Key_RControlKey 	(1<<ePAD_Key_RControlKey)		// 右の Ctrl キー 
#define PAD_Key_LMenu			(1<<ePAD_Key_LMenu)		// 左の Alt キー 
#define PAD_Key_RMenu			(1<<ePAD_Key_RMenu)		// 右の Alt キー 

///ePadType_Mouseタイプ情報
#define PAD_Mus_LButton 		(1<<0)		// マウスの左ボタン 
#define PAD_Mus_RButton 		(1<<1)		// マウスの右ボタン 
#define PAD_Mus_MButton 		(1<<2)		// マウスの中央ボタン (3 ボタン マウスの場合) 
#define PAD_Mus_XButton1 		(1<<3)		// x マウスの 1 番目のボタン (5 ボタン マウスの場合) 
#define PAD_Mus_XButton2 		(1<<4)		// x マウスの 2 番目のボタン (5 ボタン マウスの場合) 


///パッドデータ構造体
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

///キーデータ構造体
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

///マウスデータ構造体
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



///パッド状況
#define PAD_DISCONNECT	0
#define PAD_CONNECT		1

typedef mS32	PadState;

#endif