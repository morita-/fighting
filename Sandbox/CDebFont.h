#ifndef __CDEBFONT__H__
#define __CDEBFONT__H__

#include "..\LibGame\platform.h"
#include "..\LibGame\CFont.h"

class CDebFont	: public CFontRef
{
public:
	CDebFont(char* pFile);
	~CDebFont();


};

#endif
