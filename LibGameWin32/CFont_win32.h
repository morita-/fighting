#ifndef __CFONT_WIN32__H__
#define __CFONT_WIN32__H__

#include "..\LibGame\platform.h"
#if	PLATFORM_CHECK( PLATFORM_WIN32 )

#include "..\LibGame\CTexture.h"

class CFontBase;

class CFontWin32 : public CFontBase
{
	// 画像
	CTextureRef* m_pTextrue;

	// 各文字の座標
	float X[256], Y[256];

	// テクスチャ上のサイズ、表示サイズ、影の位置
	float SW, SH, DW, DH, ShadeX, ShadeY;

public:
	CFontWin32(char* pFile);
	CFontWin32(LPDIRECT3DDEVICE9 device, 
		string file, CArchiveLoader* archive, 
		string chars, float sw, float sh, float dw, float dh, 
		float shadex, float shadey);

	~CFontWin32();

	// 描画
	void DrawChar(unsigned char c, float x, float y, mColor32 color);
	void DrawChar(unsigned char c, float x, float y, mColor32 color, mColor32 shade);
	void DrawText(string s, float x, float y, mColor32 color);
	void DrawText(string s, float x, float y, mColor32 color, mColor32 shade);
	void DrawText(string s, float x, float y, float sx, float sy, mColor32 color);

	// サイズ
	float GetTextW(string s){
		return s.length()*DW;
	}
	float GetTextH(){
		return DH;
	}


};



#endif
#endif
