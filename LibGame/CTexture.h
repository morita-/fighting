#ifndef	__CTECTRUE__H__
#define	__CTECTRUE__H__



class CTextureBase
{
public:
	CTextureBase(){}
	~CTextureBase(){}


};

//////////////////////////////////////////////////////////////
#include "..\LibGameWin32\CTexture_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	
	typedef CTextureWin32 CTextureRef;
	
#else
	
	typedef CTextureBase CTextureRef;
		
#endif




#endif
