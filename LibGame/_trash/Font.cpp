#pragma warning(disable:4819)
#pragma warning(disable:4267)

#include "Font.h"


//==============================================================
// コンストラクタ
CFont::CFont(
	LPDIRECT3DDEVICE9 device, 
	string file, CArchiveLoader* archive, 
	string chars, float sw, float sh, float dw, float dh, 
	float shadex, float shadey
)
:	SW(sw), SH(sh), DW(dw), DH(dh), ShadeX(shadex), ShadeY(shadey)
{
	// 画像の作成とロード
	Texture=new CTextureRef(device, file, archive);

	// 各文字の座標の初期化
	int i, n, cn=(int)(1/SW);
	for (i=0; i<256; i++) X[i]=Y[i]=-1;
	for (i=0, n=chars.length(); i<n; i++) {
		unsigned int c=chars[i];
		X[c]=(i%cn)*SW;
		Y[c]=(i/cn)*SH;
	}
}


//==============================================================
// 描画

// 影なし描画（文字）
void CFont::DrawChar(
	unsigned char c, float x, float y, D3DCOLOR color
) {
	if (X[c]>=0 && Y[c]>=0) {
		Texture->Draw(x, y, DW, DH, X[c], Y[c], SW, SH, color);
	}
}

// 影あり描画（文字）
void CFont::DrawChar(
	unsigned char c, float x, float y,  D3DCOLOR color, D3DCOLOR shade
) {
	DrawChar(c, x+ShadeX, y+ShadeY, shade);
	DrawChar(c, x, y, color);
}

// 影なし描画（文字列）
void CFont::DrawText(
	string s, float x, float y, D3DCOLOR color
) {
	float dx=x;
	for (int i=0, n=s.length(); i<n; i++) {
		unsigned int c=s[i];
		if (X[c]>=0 && Y[c]>=0) {
			Texture->Draw(dx, y, DW, DH, X[c], Y[c], SW, SH, color);
		}
		dx+=DW;
	}
}

// 影あり描画（文字列）
void CFont::DrawText(
	string s, float x, float y, D3DCOLOR color, D3DCOLOR shade
) {
	DrawText(s, x+ShadeX, y+ShadeY, shade);
	DrawText(s, x, y, color);
}

// 影なし描画（文字列、拡大縮小）
void CFont::DrawText(
	string s, float x, float y, float sx, float sy, D3DCOLOR color
) {
	float dx=x, dw=DW*sx, dh=DH*sy;
	for (int i=0, n=s.length(); i<n; i++) {
		unsigned int c=s[i];
		if (X[c]>=0 && Y[c]>=0) {
			Texture->Draw(dx, y, dw, dh, X[c], Y[c], SW, SH, color);
		}
		dx+=dw;
	}
}

