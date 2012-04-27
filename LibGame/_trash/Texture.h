#ifndef TEXTUREH
#define TEXTUREH

#include "..\..\LibUtil\LibUtil\Archive.h"
#include <d3dx9.h>
#include <string>
using namespace std;


//==============================================================
// テクスチャ
class CTexture {
protected:

	// Direct3Dデバイス
	LPDIRECT3DDEVICE9 Device;

	// テクスチャ
	LPDIRECT3DTEXTURE9 Texture;

	// サイズ
	int TextureW, TextureH, OriginalW, OriginalH;

public:

	// コンストラクタ、デストラクタ
	CTexture(LPDIRECT3DDEVICE9 device, int w, int h);
	CTexture(LPDIRECT3DDEVICE9 device, string file, CArchiveLoader* archive);
	~CTexture();

	// サイズ
	int GetTextureWidth() { return TextureW; }
	int GetTextureHeight() { return TextureH; }
	int GetOriginalWidth() { return OriginalW; }
	int GetOriginalHeight() { return OriginalH; }

	// テクスチャ
	LPDIRECT3DTEXTURE9 GetTexture() { return Texture; }

	// 描画
	static void DrawRect(
		LPDIRECT3DDEVICE9 device, 
		float x0, float y0, D3DCOLOR c0, float u0, float v0, 
		float x1, float y1, D3DCOLOR c1, float u1, float v1,
		float x2, float y2, D3DCOLOR c2, float u2, float v2,
		float x3, float y3, D3DCOLOR c3, float u3, float v3
	);
	static void DrawRect(
		LPDIRECT3DDEVICE9 device, 
		float x, float y, float dx, float dy, 
		float u, float v, float du, float dv, 
		D3DCOLOR diffuse
	);
	static void DrawRect(
		LPDIRECT3DDEVICE9 device, 
		float x, float y, float dx, float dy, 
		D3DCOLOR diffuse
	);
	void Draw(
		float x0, float y0, D3DCOLOR c0, float u0, float v0, 
		float x1, float y1, D3DCOLOR c1, float u1, float v1,
		float x2, float y2, D3DCOLOR c2, float u2, float v2,
		float x3, float y3, D3DCOLOR c3, float u3, float v3
	);
	void Draw(
		float x, float y, float dx, float dy, 
		float u, float v, float du, float dv, 
		D3DCOLOR diffuse
	);
	void Draw(float x, float y, D3DCOLOR diffuse) {
		Draw(x, y, (float)OriginalW, (float)OriginalH, 0, 0, 1, 1, diffuse);
	}

	// 塗りつぶし
	void Fill(D3DXVECTOR4* color);
};


#endif

