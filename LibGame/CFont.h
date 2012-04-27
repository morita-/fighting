#ifndef __CFONT__H__
#define __CFONT__H__

#include <string>
using namespace std;

#include "typedef.h"

class CFontBase
{
public:
	CFontBase(void * pFile=NULL){}
	~CFontBase(){}

	// ï`âÊ
	virtual void DrawChar(unsigned char c, float x, float y, mColor32 color) = 0;
	virtual void DrawChar(unsigned char c, float x, float y, mColor32 color, mColor32 shade) = 0;
	virtual void DrawText(string s, float x, float y, mColor32 color) = 0;
	virtual void DrawText(string s, float x, float y, mColor32 color, mColor32 shade) = 0;
	virtual void DrawText(string s, float x, float y, float sx, float sy, mColor32 color) = 0;

	// ÉTÉCÉY
	virtual float GetTextW(string s) = 0;
	virtual float GetTextH() = 0;


};

//////////////////////////////////////////////////////////////
#include "..\LibGameWin32\CFont_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	
	typedef CFontWin32 CFontRef;
	
#else
	
	typedef CFontBase CFontRef;
		
#endif


//////////////////////////////////////////////////////////////




#endif
