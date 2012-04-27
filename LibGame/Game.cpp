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
// �t�@�C��������p�X�����o���ĕԂ�
static string ExtractFilePath(string s) {
	int pos=s.rfind("\\");
	if (pos==string::npos) return "";
	return s.substr(0, pos+1);
}

// ���s�t�@�C���̃p�X��Ԃ��i������\���t���j
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
	// �A�v���P�[�V�������C���s�t�@�C���̃p�X�A�w���v�̂t�q�k
	m_AppName=app_name;
	m_ExePath=GetExePath();


}

void CGameSystem::AnalyzeGameOption(){
	// �R�}���h���C���I�v�V�����̉���
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

	// �E�B���h�E�̍쐬
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

	// �O���t�B�b�N�X�̍쐬
	m_pGraphics=new CGraphics(m_HWnd);
	if (!m_pGraphics) {
		string s=
			"���̃v���O�����ɂ�DirectX 9.0�ȏオ�K�v�ł��B\n"
			"DirectX�̍ŐV�ŁA����эŐV�̃h���C�o��\n"
			"�C���X�g�[������Ă��邩�ǂ������m�F���������B\n";
		if (!fixed_size) {
			s+=
				"�܂��A�N�����̃I�v�V�����ŉ𑜓x��ύX���Ă݂Ă��������B\n\n"
				"�E�B���h�E��800x600�A�t���X�N���[��1024x768�̏ꍇ�F\n"
				"-w 800 -h 600 -fw 1024 -fh 768";
		}
		MessageBox(m_HWnd, s.c_str(), m_AppName.c_str(), MB_OK);
		exit(1);
	}
	m_pGraphics->Clear();
	m_pGraphics->Present();
	if (m_bFullScreen) ResetScreen();

	// ���͂̏�����
	m_pInput=new CInput(m_HWnd);
	m_Mouse.X=m_Mouse.Y=0;
	m_Mouse.LButton=m_Mouse.MButton=m_Mouse.RButton=false;

	// ����
	m_nLastPerfCounter.QuadPart=0;
	m_nLastTickCount=0;
	m_nElapsed=0;

	// ���j���[
	if (m_bUseMenu) {
		m_pMenuBar=CreateMenu();
		SetMenu(m_HWnd, m_pMenuBar);
	}

}

//==============================================================
// ��CGameSystem���f�X�g���N�^
CGameSystem::~CGameSystem() {
	delete m_pInput;
	delete m_pGraphics;
}




//==============================================================
// ��CGame�������̃��Z�b�g
void CGameSystem::ResetTime() {
	LARGE_INTEGER freq, count;
	if (QueryPerformanceFrequency(&freq) && QueryPerformanceCounter(&count)) {
		m_nLastPerfCounter=count;
	} else {
		m_nLastTickCount=GetTickCount();
	}
}

// ��ʂ̃��Z�b�g
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
// ��CGame���Q�[���̎��s
void CGameSystem::MainLoop() {

	// �f�o�C�X���Z�b�g��̏������Ăяo��
	OnResetDevice();

	// �_�~�[�̃��b�Z�[�W�n���h�����C
	// �����ȃ��b�Z�[�W�n���h���ɒu��������
	SetWindowLong(m_HWnd, GWL_WNDPROC, (long)::WndProc);
	
	// ���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ResetTime();

	while (msg.message!=WM_QUIT) {

		
		GameMainLoop(msg);
	}

	// ���b�Z�[�W�n���h�����_�~�[�ɖ߂�
	SetWindowLong(m_HWnd, GWL_WNDPROC, (long)::DummyWndProc);
	
	// �I������
	UnregisterClass(m_AppName.c_str(), GetModuleHandle(NULL));
}
//==============================================================
//==============================================================
//==============================================================
// ��CGame���R���X�g���N�^
CGame::CGame(const char* app_name)
:	CGameSystem(app_name, true, false), 
	m_FPS(60), m_bDropFrames(true), m_bPauseInTheBackground(true),
	m_bPause(false), m_bMenuPause(false), m_bConfirmExit(true)
{

}

//==============================================================
// ��CGame���f�X�g���N�^
CGame::~CGame() {
}


void CGame::AnalyzeGameOption(){
	CGameSystem::AnalyzeGameOption();

	// �R�}���h���C���I�v�V�����̉���
	for (int i=1; i<__argc; i++) {
		char* s=__argv[i];
		if (strcmp("-nce", s)==0) {
			m_bConfirmExit=false;
		}
	}
}




void CGame::GameMainLoop(MSG msg){

	// �o�b�N�O���E���h���̃��b�Z�[�W����
	if (m_bPauseInTheBackground && m_HWnd!=GetForegroundWindow()) {
		GetMessage(&msg, NULL, 0U, 0U);
		if (!TranslateAccelerator(m_HWnd, m_HAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		ResetTime();
		return;
	}

	// �t�H�A�O���E���h���̃��b�Z�[�W����
	if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
		if (!TranslateAccelerator(m_HWnd, m_HAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return;
	}

	// �t���X�N���[�����̃��j���[�\��
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

	// �ꎞ��~
	if (m_bPause || m_bMenuPause) return;

	// ���Ԓ���
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



	// �ړ��ƕ`��
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


// �`��
void CGame::DrawScene() {

	// �f�o�C�X���������Ă�����č\�z����
	switch (m_pGraphics->GetDevice()->TestCooperativeLevel()) {
		case D3DERR_DEVICELOST:
			Sleep(50);
			return;
		case D3DERR_DEVICENOTRESET:
			OnLostDevice();
			if (m_pGraphics->ResetDevice()) OnResetDevice();
			return;
	}

	// �`��J�n
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();
	m_pGraphics->BeginScene();

	Draw();

	// �`�抮��
	m_pGraphics->EndScene();
	m_pGraphics->Present();
}

