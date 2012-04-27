#include "Main.h"
#include "Versus.h"



//=========================================================================
// ���C�����[�`��

// �R�}���h���C���I�v�V����
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
	///////////////�V�X�e���I�v�V����
	if (ArgExist("-shader"))	gpGame->SetShaderEnable(true); 
	else						gpGame->SetShaderEnable(false);

	//////////////�Q�[���I�v�V����
	gpGame->AnalyzeGameOption();

	//////////////�E�B���h�E�쐬
	gpGame->CreateGameWindow();

	//////////////�O���t�B�b�N�C�j�V�����C�Y
	gpGame->GameGraphicInitialize();

	//////////////�Q�[���̃C�j�V�����C�Y
	gpGame->InitializeGame();

}


// ���C�����[�`��
INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, INT) {

	gpGame = CreateGame();

	ApplicationInit();

	gpGame->MainLoop();
	
	delete gpGame;
	return 0;
}


