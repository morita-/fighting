#ifndef __CHWRESOURCEDATA__H__
#define __CHWRESOURCEDATA__H__

typedef struct
{
	mS32 pad;
}HWCommonResourceData;

typedef struct
{
	mS32 pad;
}HWResourceData;

//////////////////////////////////////////////////////////////
#include "..\LibGameWin32\CHWResourceData_win32.h"

#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	typedef HWCommonResourceDataWin32	HWCommonResourceDataRef;
	typedef HWResourceDataWin32			HWResourceDataRef;
#else
	typedef HWCommonResourceData		HWCommonResourceDataRef;
	typedef HWResourceData				HWResourceDataRef;
#endif

#endif