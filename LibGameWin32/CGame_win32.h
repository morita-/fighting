#ifndef __GAME_WIN32__H__
#define __GAME_WIN32__H__

#include "..\LibGame\platform.h"
#if	PLATFORM_CHECK( PLATFORM_WIN32 )

#include "..\LibGame\global.h"

class CGameSystem;
class CGameSystemWin32 : public CGameSystem
{
protected:

	//�w���v��URL
	string m_HelpURL;

	// ��ʃ��[�h�̐؂�ւ�
	void ToggleFullScreen();

public:
	CGameSystemWin32(const char* app_name, bool zbuffer, bool use_menu);
	~CGameSystemWin32();


	// ���b�Z�[�W�n���h��
	virtual LRESULT WINAPI WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

};

class CGame;
class CGameWin32 : public CGame
{
protected:
	HMENU m_pMenuBar;

public:
	CGameWin32();
	~CGameWin32();

	// ���b�Z�[�W�n���h��
	virtual LRESULT WINAPI WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	// ���j���[
	HMENU GetMenuBar() { return m_pMenuBar; }
	HMENU AddMenu(string text);
	HMENU AddSubMenu(HMENU menu, string text);
	void AddMenuItem(HMENU menu, string text, int id);
	void AddMenuSeparator(HMENU menu);
	void CheckMenuItem(HMENU menu, int id, bool check);

};


#endif
#endif
