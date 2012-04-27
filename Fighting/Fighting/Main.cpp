#include "Main.h"
#include "Versus.h"



//=========================================================================
// メインルーチン

// コマンドラインオプション
static bool ArgExist(string option) {
	for (int i=1; i<__argc; i++) {
		if (__argv[i]==option) return true;
	}
	return false;
}


CFightingGame* CreateGame(){
	if (ArgExist("-mesh"))		return NewFightingGameViewer(false);
	if (ArgExist("-anim"))		return NewFightingGameViewer(true);
	if (ArgExist("-versus"))	return NewFightingGameVersus(false);
	if (ArgExist("-cpu"))		return NewFightingGameVersus(true); 
		
	return NewFightingGameVersus(true);

}
void ApplicationInit(){
	///////////////システムオプション
	if (ArgExist("-shader"))	gpGame->SetShaderEnable(true); 
	else						gpGame->SetShaderEnable(false);

	//////////////ゲームオプション
	gpGame->AnalyzeGameOption();

	//////////////ウィンドウ作成
	gpGame->CreateGameWindow();

	//////////////グラフィックイニシャライズ
	gpGame->GameGraphicInitialize();

	//////////////ゲームのイニシャライズ
	gpGame->InitializeGame();

}


// メインルーチン
INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, INT) {

	gpGame = CreateGame();

	ApplicationInit();

	gpGame->MainLoop();
	
	delete gpGame;
	return 0;
}


