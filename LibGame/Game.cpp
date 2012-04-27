#pragma warning(disable:4786)
#pragma warning(disable:4819)
#pragma warning(disable:4311)

#include "global.h"
#include "Game.h"
#include "CHWResource.h"

#include "..\LibUtil\LibUtil\LibUtil.h"
#include <map>
#include <strstream>
using namespace std;





//==============================================================
//==============================================================
// ファイル名からパスを取り出して返す
static string ExtractFilePath(string s) {
	int pos=s.rfind("\\");
	if (pos==string::npos) return "";
	return s.substr(0, pos+1);
}

// 実行ファイルのパスを返す（末尾に\が付く）
static string GetExePath() {
	char buf[MAX_PATH+1];
	GetModuleFileName(NULL, buf, MAX_PATH);
	return ExtractFilePath(buf);
}




CGameSystem::CGameSystem(const char* app_name, bool zbuffer, bool use_menu)
:	m_nWindowWidth(800),	m_nWindowHeight(600), 
	m_nFullScreenWidth(800), m_nFullScreenHeight(600), 
	m_bFullScreen(false),	m_nRefreshRate(60),
	m_bUseMenu(use_menu)
{
	// アプリケーション名，実行ファイルのパス、ヘルプのＵＲＬ
	m_AppName=app_name;
	m_ExePath=GetExePath();


}

void CGameSystem::AnalyzeGameOption(){
	// コマンドラインオプションの解釈
	for (int i=1; i<__argc; i++) {
		char* s=__argv[i];
		if (strcmp("-w", s)==0 && ++i<__argc) {
			m_nWindowWidth=m_nFullScreenWidth=atoi(__argv[i]);
		} else
		if (strcmp("-h", s)==0 && ++i<__argc) {
			m_nWindowHeight=m_nFullScreenHeight=atoi(__argv[i]);
		} else
		if (strcmp("-ww", s)==0 && ++i<__argc) {
			m_nWindowWidth=atoi(__argv[i]);
		} else
		if (strcmp("-wh", s)==0 && ++i<__argc) {
			m_nWindowHeight=atoi(__argv[i]);
		} else
		if (strcmp("-fw", s)==0 && ++i<__argc) {
			m_nFullScreenWidth=atoi(__argv[i]);
		} else
		if (strcmp("-fh", s)==0 && ++i<__argc) {
			m_nFullScreenHeight=atoi(__argv[i]);
		}

		if (strcmp("-r", s)==0 && ++i<__argc) {
			m_nRefreshRate=atoi(__argv[i]);
		}
		if (strcmp("-f", s)==0) {
			m_bFullScreen=true;
		}
	}
}


void CGameSystem::CreateGameWindow( bool fixed_size){

	// ウィンドウの作成
	long style=WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX;
	RECT r={0, 0, m_nWindowWidth, m_nWindowHeight};
	AdjustWindowRect(&r, style, false);
	m_HWnd=CreateWindow(
		m_AppName.c_str(), m_AppName.c_str(), style, 
		0, 0, r.right-r.left, r.bottom-r.top, 
		GetDesktopWindow(), NULL, m_hInstance, NULL);
	HWndCGameMap.insert(
		THWndCGameMap::value_type(m_HWnd, this));

	//resource
	CHWResourceRef::GetInstance()->AddHWResourceWnd(m_HWnd,m_nWindowWidth,m_nWindowHeight);

	// グラフィックスの作成
	m_pGraphics=new CGraphics(m_HWnd);
	if (!m_pGraphics) {
		string s=
			"このプログラムにはDirectX 9.0以上が必要です。\n"
			"DirectXの最新版、および最新のドライバが\n"
			"インストールされているかどうかご確認ください。\n";
		if (!fixed_size) {
			s+=
				"また、起動時のオプションで解像度を変更してみてください。\n\n"
				"ウィンドウ時800x600、フルスクリーン1024x768の場合：\n"
				"-w 800 -h 600 -fw 1024 -fh 768";
		}
		MessageBox(m_HWnd, s.c_str(), m_AppName.c_str(), MB_OK);
		exit(1);
	}
	m_pGraphics->Clear();
	m_pGraphics->Present();
	if (m_bFullScreen) ResetScreen();

	// 入力の初期化
	m_pInput=new CInput(m_HWnd);
	m_Mouse.X=m_Mouse.Y=0;
	m_Mouse.LButton=m_Mouse.MButton=m_Mouse.RButton=false;

	// 時間
	m_nLastPerfCounter.QuadPart=0;
	m_nLastTickCount=0;
	m_nElapsed=0;

	// メニュー
	if (m_bUseMenu) {
		m_pMenuBar=CreateMenu();
		SetMenu(m_HWnd, m_pMenuBar);
	}

}

//==============================================================
// ＜CGameSystem＞デストラクタ
CGameSystem::~CGameSystem() {
	delete m_pInput;
	delete m_pGraphics;
}




//==============================================================
// ＜CGame＞時刻のリセット
void CGameSystem::ResetTime() {
	LARGE_INTEGER freq, count;
	if (QueryPerformanceFrequency(&freq) && QueryPerformanceCounter(&count)) {
		m_nLastPerfCounter=count;
	} else {
		m_nLastTickCount=GetTickCount();
	}
}

// 画面のリセット
void CGameSystem::ResetScreen() {
	if (m_bFullScreen) {
		m_pGraphics->SetWidth(m_nFullScreenWidth);
		m_pGraphics->SetHeight(m_nFullScreenHeight);
		m_pGraphics->SetRefreshRate(m_nRefreshRate);
	} else {
		m_pGraphics->SetWidth(m_nWindowWidth);
		m_pGraphics->SetHeight(m_nWindowHeight);
		m_pGraphics->SetRefreshRate(0);
	}
	m_pGraphics->SetFullScreen(m_bFullScreen);
	OnLostDevice();
	if (m_pGraphics->ResetDevice()) OnResetDevice();
}


//==============================================================
// ＜CGame＞ゲームの実行
void CGameSystem::MainLoop() {

	// デバイスリセット後の処理を呼び出す
	OnResetDevice();

	// ダミーのメッセージハンドラを，
	// 正式なメッセージハンドラに置き換える
	SetWindowLong(m_HWnd, GWL_WNDPROC, (long)::WndProc);
	
	// メッセージループ
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ResetTime();

	while (msg.message!=WM_QUIT) {

		
		GameMainLoop(msg);
	}

	// メッセージハンドラをダミーに戻す
	SetWindowLong(m_HWnd, GWL_WNDPROC, (long)::DummyWndProc);
	
	// 終了処理
	UnregisterClass(m_AppName.c_str(), GetModuleHandle(NULL));
}
//==============================================================
//==============================================================
//==============================================================
// ＜CGame＞コンストラクタ
CGame::CGame(const char* app_name)
:	CGameSystem(app_name, true, false), 
	m_FPS(60), m_bDropFrames(true), m_bPauseInTheBackground(true),
	m_bPause(false), m_bMenuPause(false), m_bConfirmExit(true)
{

}

//==============================================================
// ＜CGame＞デストラクタ
CGame::~CGame() {
}


void CGame::AnalyzeGameOption(){
	CGameSystem::AnalyzeGameOption();

	// コマンドラインオプションの解釈
	for (int i=1; i<__argc; i++) {
		char* s=__argv[i];
		if (strcmp("-nce", s)==0) {
			m_bConfirmExit=false;
		}
	}
}




void CGame::GameMainLoop(MSG msg){

	// バックグラウンド時のメッセージ処理
	if (m_bPauseInTheBackground && m_HWnd!=GetForegroundWindow()) {
		GetMessage(&msg, NULL, 0U, 0U);
		if (!TranslateAccelerator(m_HWnd, m_HAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		ResetTime();
		return;
	}

	// フォアグラウンド時のメッセージ処理
	if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
		if (!TranslateAccelerator(m_HWnd, m_HAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return;
	}

	// フルスクリーン時のメニュー表示
	if (m_bUseMenu) {
		if (m_pGraphics->IsFullScreen()) {
			POINT p;
			GetCursorPos(&p);
			bool prev=m_bMenuPause;
			m_bMenuPause=(p.y<GetSystemMetrics(SM_CYMENU));
			if (!prev && m_bMenuPause) {
				m_pGraphics->PresentGDISurface();
				DrawMenuBar(m_HWnd);
			}
		} else {
			m_bMenuPause=false;
		}
	}

	// 一時停止
	if (m_bPause || m_bMenuPause) return;

	// 時間調整
	LARGE_INTEGER freq, count;
	DWORD tick;
	if (QueryPerformanceFrequency(&freq) && 
		QueryPerformanceCounter(&count)) {
		m_nElapsed+=m_FPS*(count.QuadPart-m_nLastPerfCounter.QuadPart)/freq.QuadPart;
		m_nLastPerfCounter=count;
	} else {
		tick=GetTickCount();
		m_nElapsed+=m_FPS*(tick-m_nLastTickCount)/1000;
		m_nLastTickCount=tick;
	}



	// 移動と描画
	if (m_pGraphics->GetRefreshRate()==m_FPS || m_nElapsed>=1.0) {
		if (m_bDropFrames) {
			for (; m_nElapsed>=1.0; m_nElapsed-=1.0){
				Move();
			}
		} else {
			m_nElapsed=0;
			Move();
		}
		DrawScene();
	}
}


// 描画
void CGame::DrawScene() {

	// デバイスが消失していたら再構築する
	switch (m_pGraphics->GetDevice()->TestCooperativeLevel()) {
		case D3DERR_DEVICELOST:
			Sleep(50);
			return;
		case D3DERR_DEVICENOTRESET:
			OnLostDevice();
			if (m_pGraphics->ResetDevice()) OnResetDevice();
			return;
	}

	// 描画開始
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();
	m_pGraphics->BeginScene();

	Draw();

	// 描画完了
	m_pGraphics->EndScene();
	m_pGraphics->Present();
}

