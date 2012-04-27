#ifndef __CTEXZTRUE_WIN32__H__
#define __CTEXZTRUE_WIN32__H__

#include "..\LibGame\platform.h"
#if	PLATFORM_CHECK( PLATFORM_WIN32 )

#include <d3dx9.h>
#include <string>
#include "..\LibGame\\global.h"

#include "..\LibUtil\LibUtil\Archive.h"

class CTextureBase;

//==============================================================
// テクスチャ
class CTextureWin32 : CTextureBase
{
protected:

	// Direct3Dデバイス
	LPDIRECT3DDEVICE9 m_pDevice;

	// テクスチャ
	LPDIRECT3DTEXTURE9 m_pTexture;

	// サイズ
	struct
	{
		mS32 nWidth;
		mS32 nHeight;
		mS32 nOrgWidth;
		mS32 nOrgHeight;
	}m_stTexture;


public:

	// コンストラクタ、デストラクタ
	CTextureWin32(LPDIRECT3DDEVICE9 device, int w, int h);
	CTextureWin32(LPDIRECT3DDEVICE9 device, string file, CArchiveLoader* archive);
	~CTextureWin32();

	// サイズ
	int GetTextureWidth() { return m_stTexture.nWidth; }
	int GetTextureHeight() { return m_stTexture.nHeight; }
	int GetOriginalWidth() { return m_stTexture.nOrgWidth; }
	int GetOriginalHeight() { return m_stTexture.nOrgHeight; }

	// テクスチャ
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }

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
		Draw(x, y, (float)m_stTexture.nOrgWidth, (float)m_stTexture.nOrgHeight, 0, 0, 1, 1, diffuse);
	}

	// 塗りつぶし
	void Fill(D3DXVECTOR4* color);
};



#endif //PLATFORM_CHECK( PLATFORM_WIN32 )
#endif //__CTEXZTRUE_WIN32__H__