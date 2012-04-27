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

	// �O���t�B�b�N�X
	CGraphics* m_pGraphics;

	// ����
	CInput*		m_pInput;
	MOUSE_STATE m_Mouse;

	// ���s�t�@�C���̃p�X�A�A�v���P�[�V������
	string m_ExePath, m_AppName;

	// ���x����
	LARGE_INTEGER	m_nLastPerfCounter;
	DWORD			m_nLastTickCount;
	double			m_nElapsed;

	// ��ʃT�C�Y�A���t���b�V�����[�g
	int		m_nWindowWidth, m_nWindowHeight;
	int		m_nFullScreenWidth, m_nFullScreenHeight;
	int		m_nRefreshRate;
	bool	m_bFullScreen;

	// ���j���[
	bool m_bUseMenu;

	void ResetTime();
	virtual	void DrawScene(){}

	// ��ʃ��[�h�̐؂�ւ�
	void ResetScreen();

	// Direct3D���\�[�X�̊Ǘ�
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

public:
	CGameSystem(const char* app_name, bool zbuffer, bool use_menu);
	~CGameSystem();

	// �I�v�V�����ݒ�
	virtual void AnalyzeGameOption();

	// �E�B���h�E�쐬
	virtual void CreateGameWindow( bool fixed_size=true);

	// �e����̎擾
	CGraphics*	GetGraphics() { return m_pGraphics; }
	CInput*		GetInput() { return m_pInput; }
	const MOUSE_STATE& GetMouseState() { return m_Mouse; }



	// �Q�[���̎��s
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



// �Q�[���{�̂̃N���X
class CGame  :public CGameSystemRef
{
protected:
	////////////////////////////////////////
	// �ꎞ��~
	bool	m_bPause, m_bMenuPause;

	// �I���m�F
	bool m_bConfirmExit;

	void DrawScene();

protected:
	// �e��ݒ�
	double	m_FPS;
	bool	m_bDropFrames, m_bPauseInTheBackground;


	// �I���m�F
	bool GetConfirmExit(){return m_bConfirmExit;}
	void SetConfirmExit(bool ce) { m_bConfirmExit=ce; }


public:

	// �R���X�g���N�^�C�f�X�g���N�^
	CGame(const char* app_name);
	~CGame();

	// �I�v�V�����ݒ�
	virtual void AnalyzeGameOption();


	void GameMainLoop(MSG msg);

	// �ړ��ƕ`��F
	// �T�u�N���X�ŃI�[�o�[���C�h����
	virtual void Move() {}
	virtual void Draw() {}


};


#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	
	typedef CGameWin32 CGameRef;
	
#else
	
	typedef CGame CGameRef;
		
#endif



#endif
