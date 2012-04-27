
#include "..\LibGame\global.h"


#include "..\LibGame\CFont.h"
#include "CFont_win32.h"


#include "..\LibGame\CHWResource.h"
#include "CHWResource_win32.h"



#if	PLATFORM_CHECK( PLATFORM_WIN32 )


CFontWin32::CFontWin32(char* pFile) : CFontBase()
{
	CHWResourceWin32* pHW=CHWResourceWin32::GetInstanceN();
	LPDIRECT3DDEVICE9 pDevice = pHW->GetHWResourceDevice();
//	CArchiveLoader* pArchive = CArchiveLoader::GetInstanceN(); 
//	m_pTextrue = new CTextureRef(pDevice, pFile, pArchive);
	m_pTextrue = new CTextureRef(pDevice, pFile, NULL);

}

CFontWin32::CFontWin32(LPDIRECT3DDEVICE9 device, 
	string file, CArchiveLoader* archive, 
	string chars, float sw, float sh, float dw, float dh, 
	float shadex, float shadey) 
:	SW(sw), SH(sh), DW(dw), DH(dh), ShadeX(shadex), ShadeY(shadey), CFontBase()
{
	// ‰æ‘œ‚Ìì¬‚Æƒ[ƒh
	m_pTextrue=new CTextureRef(device, file, archive);

	// Še•¶š‚ÌÀ•W‚Ì‰Šú‰»
	int i, n, cn=(int)(1/SW);
	for (i=0; i<256; i++) X[i]=Y[i]=-1;
	for (i=0, n=chars.length(); i<n; i++) {
		unsigned int c=chars[i];
		X[c]=(i%cn)*SW;
		Y[c]=(i/cn)*SH;
	}

}



CFontWin32::~CFontWin32(){
	M_DELETE( m_pTextrue );
}



//==============================================================
// •`‰æ

// ‰e‚È‚µ•`‰æi•¶šj
void CFontWin32::DrawChar(
	unsigned char c, float x, float y, mColor32 color
) {
	if (X[c]>=0 && Y[c]>=0) {
		m_pTextrue->Draw(x, y, DW, DH, X[c], Y[c], SW, SH, color);
	}
}

// ‰e‚ ‚è•`‰æi•¶šj
void CFontWin32::DrawChar(
	unsigned char c, float x, float y,  mColor32 color, mColor32 shade
) {
	DrawChar(c, x+ShadeX, y+ShadeY, shade);
	DrawChar(c, x, y, color);
}

// ‰e‚È‚µ•`‰æi•¶š—ñj
void CFontWin32::DrawText(
	string s, float x, float y, mColor32 color
) {
	float dx=x;
	for (int i=0, n=s.length(); i<n; i++) {
		unsigned int c=s[i];
		if (X[c]>=0 && Y[c]>=0) {
			m_pTextrue->Draw(dx, y, DW, DH, X[c], Y[c], SW, SH, color);
		}
		dx+=DW;
	}
}

// ‰e‚ ‚è•`‰æi•¶š—ñj
void CFontWin32::DrawText(
	string s, float x, float y, mColor32 color, mColor32 shade
) {
	DrawText(s, x+ShadeX, y+ShadeY, shade);
	DrawText(s, x, y, color);
}

// ‰e‚È‚µ•`‰æi•¶š—ñAŠg‘åk¬j
void CFontWin32::DrawText(
	string s, float x, float y, float sx, float sy, mColor32 color
) {
	float dx=x, dw=DW*sx, dh=DH*sy;
	for (int i=0, n=s.length(); i<n; i++) {
		unsigned int c=s[i];
		if (X[c]>=0 && Y[c]>=0) {
			m_pTextrue->Draw(dx, y, dw, dh, X[c], Y[c], SW, SH, color);
		}
		dx+=dw;
	}
}



#endif