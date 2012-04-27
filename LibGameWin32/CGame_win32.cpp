
#include "..\LibGame\global.h"

#include "..\LibGame\Game.h"

#include "CGame_win32.h"

//==============================================================
// HWNDとCGameインスタンスを関連づけるマップ
typedef map<HWND, CGameSystemWin32*, less<HWND> > THWndCGameMap;
static THWndCGameMap HWndCGameMap;


//==============================================================
// 空のメッセージハンドラ：
// グラフィックスなどの初期化前にはこれを実行する。
static LRESULT WINAPI DummyWndProc(
	HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
) {
	return DefWindowProc( hwnd, msg, wparam, lparam );
}


//==============================================================
// メッセージハンドラ
static LRESULT WINAPI WndProc(
	HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
) {
	THWndCGameMap::iterator iter=HWndCGameMap.find(hwnd);
	if (iter==HWndCGameMap.end())
		return DefWindowProc( hwnd, msg, wparam, lparam );
	CGameSystemWin32* game=iter->second;
	return game->WndProc(msg, wparam, lparam);
}


//==============================================================
static const char* copyright=
	"Copyright(C)";

CGameSystemWin32::CGameSystemWin32(const char* app_name, bool zbuffer, bool use_menu)
{
	m_HelpURL="";

	// アイコンの読み込み
	HINSTANCE hinst=GetModuleHandle(NULL);
	HICON icon;
	string icon_file=m_ExePath+"game.ico";
	if (FileExists(icon_file)) {
		icon=(HICON)LoadImage(
			hinst, icon_file.c_str(), 
			IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	} else {
		icon=LoadIcon(hinst, MAKEINTRESOURCE(1));
	}

	// ウィンドウクラスの登録：
	// ここではダミーのメッセージハンドラを登録する
	WNDCLASSEX wc={
		sizeof(WNDCLASSEX), CS_CLASSDC, DummyWndProc, 0L, 0L,
		hinst, icon, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
		app_name, NULL
	};
	RegisterClassEx(&wc);

	m_hInstance = wc.hInstance;

	// アクセラレータキーの設定：
	// [ESC] : 終了
	// [Alt]+[Enter] : 画面モード切り替え
	// [F1] : ヘルプ
	// [Shift]+[F1]：バージョン表示
	ACCEL accel[]={
		{FVIRTKEY, VK_ESCAPE, IDC_EXIT},
		{FALT|FVIRTKEY, VK_RETURN, IDC_TOGGLEFULLSCREEN},
		{FVIRTKEY, VK_F1, IDC_HELPURL},
		{FSHIFT|FVIRTKEY, VK_F1, IDC_VERSION}
	};
	m_HAccel=CreateAcceleratorTable(
		accel, sizeof(accel)/sizeof(ACCEL));

	CHWResourceRef::CreateInstance();
	CHWResourceRef::GetInstance()->SetHWResourceInstance(m_hInstance);
	CHWResourceRef::GetInstance()->SetHWResourceAccel(m_HAccel);
}

LRESULT WINAPI CGameSystemWin32::WndProc(
	UINT msg, WPARAM wparam, LPARAM lparam
) {
	switch(msg) {
	// 描画
	case WM_PAINT:
		DrawScene();
		break;

	// タイトルバーのダブルクリック
	case WM_NCLBUTTONDBLCLK:
		ToggleFullScreen();
		return 0;


	// システムコマンド
	case WM_SYSCOMMAND:
		switch (wparam) {
		// 最大化時には画面モード切り替え
		case SC_MAXIMIZE:
			ToggleFullScreen();
			return 0;					

		// スクリーンセーバとモニタ電源Offの防止
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;

		// メニュー
		case SC_KEYMENU:
			m_pGraphics->PresentGDISurface();
			DrawMenuBar(m_HWnd);
			return DefWindowProc(m_HWnd, msg, wparam, lparam);
		}
		break;

	// 各種のコマンド
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		// 終了
		case IDC_EXIT:
			SendMessage(m_HWnd, WM_CLOSE, 0, 0);
			return 0;

		// Alt+Returnで画面モード切り替え
		case IDC_TOGGLEFULLSCREEN:
			ToggleFullScreen();
			return 0;
		default:break;
		}
		break;

	// 終了
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc(m_HWnd, msg, wparam, lparam);
}

// ウィンドウモードとフルスクリーンモードの切り替え
void CGameSystemWin32::ToggleFullScreen() {
	m_bFullScreen=!m_pGraphics->IsFullScreen();
	ResetScreen();
}






LRESULT WINAPI CGameWin32::WndProc(
	UINT msg, WPARAM wparam, LPARAM lparam
) {

	ostrstream oss;
	switch(msg) {
	// マウス
	case WM_MOUSEMOVE:
		m_Mouse.X=LOWORD(lparam);
		m_Mouse.Y=HIWORD(lparam);
		break;
	case WM_LBUTTONDOWN: m_Mouse.LButton=true; break;
	case WM_LBUTTONUP: m_Mouse.LButton=false; break;
	case WM_MBUTTONDOWN: m_Mouse.MButton=true; break;
	case WM_MBUTTONUP: m_Mouse.MButton=false; break;
	case WM_RBUTTONDOWN: m_Mouse.RButton=true; break;
	case WM_RBUTTONUP: m_Mouse.RButton=false; break;

	// 終了
	case WM_CLOSE:
		if (!GetConfirmExit() ||
			MessageBox(m_HWnd, "終了してもよろしいですか？", "終了確認", MB_OKCANCEL)==IDOK) {
			DestroyWindow(m_HWnd);
		}
		return 0;

	// 各種のコマンド
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		// ヘルプ
		case IDC_HELPURL:
			if (m_HelpURL!="") {
				ShellExecute(m_HWnd, NULL, m_HelpURL.c_str(),
					NULL, NULL, SW_SHOWNORMAL);					
				return 0;
			}

		// バージョン表示
		case IDC_VERSION:
			oss<<"「"<<m_AppName<<"」\n\n";
			oss<<"上記ソフトウェアの著作権は松浦健一郎／司ゆき（ひぐぺん工房）が所有しています。\n";
			oss<<"Copyright(C) 2004 Ken-ichiro Matsuura & Yuki Tsukasa (HigPen Works).\n";
			oss<<"All Rights Reserved.\n\n";
			oss<<"[OK]を押すと下記のサポートWebサイトが開きます。\n";
			oss<<"（http://cgi32.plala.or.jp/higpen/gate.shtml）";
			oss<<ends;
			if (MessageBox(m_HWnd, oss.str(), "バージョン情報", MB_OKCANCEL)==IDOK) {
				ShellExecute(m_HWnd, NULL, 
					"http://cgi32.plala.or.jp/higpen/gate.shtml", 
					NULL, NULL, SW_SHOWNORMAL);
			}
			return 0;
		}
		break;
	default:break;
	}
	return CGameSystem::WndProc( msg, wparam, lparam);
}




//==============================================================
// メニュー
HMENU CGameWin32::AddMenu(string text) {
	HMENU menu=CreatePopupMenu();
	AppendMenu(m_pMenuBar, MF_ENABLED | MF_STRING | MF_POPUP, (UINT)menu, text.c_str());
	DrawMenuBar(m_HWnd);
	return menu;
}
HMENU CGameWin32::AddSubMenu(HMENU menu, string text) {
	HMENU sub_menu=CreatePopupMenu();
	AppendMenu(menu, MF_ENABLED | MF_STRING | MF_POPUP, (UINT)sub_menu, text.c_str());
	return sub_menu;
}
void CGameWin32::AddMenuItem(HMENU menu, string text, int id) {
	AppendMenu(menu, MF_ENABLED | MF_STRING, id, text.c_str());
}
void CGameWin32::AddMenuSeparator(HMENU menu) {
	AppendMenu(menu, MF_ENABLED | MF_SEPARATOR, 0, NULL);
}
void CGameWin32::CheckMenuItem(HMENU menu, int id, bool check) {
	MENUITEMINFO mii;
	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_STATE;
	mii.fState=check?MFS_CHECKED:MFS_UNCHECKED;
	SetMenuItemInfo(menu, id, FALSE, &mii);
}


