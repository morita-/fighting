#ifndef GAMEH
#define GAMEH


#include "csystem.h"

#include "Graphics.h"
#include "Input.h"
#include <vector>
#include <string>
using namespace std;


#define IDC_EXIT 0
#define IDC_TOGGLEFULLSCREEN 1
#define IDC_HELPURL 2
#define IDC_VERSION 3


class CGameSystem {
protected:

	// グラフィックス
	CGraphics* m_pGraphics;

	// 入力
	CInput*		m_pInput;
	MOUSE_STATE m_Mouse;

	// 実行ファイルのパス、アプリケーション名
	string m_ExePath, m_AppName;

	// 速度調整
	LARGE_INTEGER	m_nLastPerfCounter;
	DWORD			m_nLastTickCount;
	double			m_nElapsed;

	// 画面サイズ、リフレッシュレート
	int		m_nWindowWidth, m_nWindowHeight;
	int		m_nFullScreenWidth, m_nFullScreenHeight;
	int		m_nRefreshRate;
	bool	m_bFullScreen;

	// メニュー
	bool m_bUseMenu;

	void ResetTime();
	virtual	void DrawScene(){}

	// 画面モードの切り替え
	void ResetScreen();

	// Direct3Dリソースの管理
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

public:
	CGameSystem(const char* app_name, bool zbuffer, bool use_menu);
	~CGameSystem();

	// オプション設定
	virtual void AnalyzeGameOption();

	// ウィンドウ作成
	virtual void CreateGameWindow( bool fixed_size=true);

	// 各種情報の取得
	CGraphics*	GetGraphics() { return m_pGraphics; }
	CInput*		GetInput() { return m_pInput; }
	const MOUSE_STATE& GetMouseState() { return m_Mouse; }



	// ゲームの実行
	void	MainLoop();
	
	//
	virtual void GameMainLoop(MSG msg){}

};


//////////////////////////////////////////////////////////////
#include "..\LibGameWin32\CGame_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	
	typedef CGameSystemWin32 CGameSystemRef;
	
#else
	
	typedef CGameSystem CGameSystemRef;
		
#endif



// ゲーム本体のクラス
class CGame  :public CGameSystemRef
{
protected:
	////////////////////////////////////////
	// 一時停止
	bool	m_bPause, m_bMenuPause;

	// 終了確認
	bool m_bConfirmExit;

	void DrawScene();

protected:
	// 各種設定
	double	m_FPS;
	bool	m_bDropFrames, m_bPauseInTheBackground;


	// 終了確認
	bool GetConfirmExit(){return m_bConfirmExit;}
	void SetConfirmExit(bool ce) { m_bConfirmExit=ce; }


public:

	// コンストラクタ，デストラクタ
	CGame(const char* app_name);
	~CGame();

	// オプション設定
	virtual void AnalyzeGameOption();


	void GameMainLoop(MSG msg);

	// 移動と描画：
	// サブクラスでオーバーライドする
	virtual void Move() {}
	virtual void Draw() {}


};


#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	
	typedef CGameWin32 CGameRef;
	
#else
	
	typedef CGame CGameRef;
		
#endif



#endif
