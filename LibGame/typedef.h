#ifndef __TYPEDEF__H__
#define __TYPEDEF__H__

typedef unsigned long long	mU64;
typedef signed long long	mS64;

typedef unsigned long		mU32;
typedef signed long			mS32;

typedef unsigned short		mU16;
typedef signed short		mS16;

typedef unsigned char		mU8;
typedef signed char			mS8;

typedef double		mF64;
typedef float		mF32;

typedef char	mChar;
typedef bool	mBool;


#define mTrue	(1)
#define mFalse	(0)


///未定義の設定
#ifndef NULL
	#define NULL (0)
#endif

///マルチタイプのフラグ設定
#define Mlt_FLAG_HD(n)		(n/32)
#define Mlt_FLAG_NO(n)		(n%32)
#define Mlt_FLAG_SFT_NO(n)	(1<<Mlt_FLAG_NO(n))
#define Mlt_FLAG_VAL(n)		((Mlt_FLAG_HD(n)<<28)|Mlt_FLAG_SFT_NO(n))


typedef struct{
	mS32	val[32/4];

	mBool Get(mS32 no){
		return (val[Mlt_FLAG_HD(no)]&(Mlt_FLAG_SFT_NO(no)))? mTrue:mFalse;
	}
	void  Set(mS32 no){
		val[Mlt_FLAG_HD(no)]|=(Mlt_FLAG_SFT_NO(no));
	}
}mFlag32x;

//プラットフォーム別タイプ定義
#include "platform.h"
#if	PLATFORM_CHECK( PLATFORM_WIN32 )
	#include <windows.h>
	#include <D3d9types.h>

	typedef D3DCOLOR mColor32;

	#include <assert.h>
	#define mASSERT(t)	assert(t)
	#define mASSERT_STR(t,s) {if(t){printf("%s",s);assert(t);}}

#else

	typedef struct
	{
		mS8 a,r,g,b;
	}mColor32;

	#define mASSERT(t)	assert(t)
	#define mASSERT_STR(t,s) {if(t){printf("%s",s);assert(t);}}

#endif // PLATFORM_CHECK( PLATFORM_WIN32 )


#endif // __TYPEDEF__H__
