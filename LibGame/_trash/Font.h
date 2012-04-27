#ifndef FONTH
#define FONTH

#include "..\..\LibUtil\LibUtil\Archive.h"
#include "CTexture.h"
#include <string>
using namespace std;


//==============================================================
// フォント
class CFont {
protected:

	// 画像
	CTextureRef* Texture;

	// 各文字の座標
	float X[256], Y[256];

	// テクスチャ上のサイズ、表示サイズ、影の位置
	float SW, SH, DW, DH, ShadeX, ShadeY;

public:

	// コンストラクタ
	CFont(LPDIRECT3DDEVICE9 device, 
		string file, CArchiveLoader* archive, 
		string chars, float sw, float sh, float dw, float dh, 
		float shadex, float shadey);
	
	// 描画
	void DrawChar(unsigned char c, float x, float y, D3DCOLOR color);
	void DrawChar(unsigned char c, float x, float y, D3DCOLOR color, D3DCOLOR shade);
	void DrawText(string s, float x, float y, D3DCOLOR color);
	void DrawText(string s, float x, float y, D3DCOLOR color, D3DCOLOR shade);
	void DrawText(string s, float x, float y, float sx, float sy, D3DCOLOR color);

	// サイズ
	float GetTextW(string s) { return s.length()*DW; }
	float GetTextH() { return DH; }
};




#endif