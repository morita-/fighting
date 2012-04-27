

#include "..\LibGame\global.h"

#include "..\LibGame\CTexture.h"
#include "CTexture_win32.h"


#if	PLATFORM_CHECK( PLATFORM_WIN32 )


#include <d3dx9.h>


//==============================================================
// 頂点形式

// 構造体
struct VERTEX
{
	// 変換済み頂点
	FLOAT X, Y, Z, RHW;

	// ディフューズ色
	D3DCOLOR Diffuse;

	// テクスチャ座標
	FLOAT U, V;
};

// FVF
#define D3DFVF_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//==============================================================
// テクスチャ

// コンストラクタ（サイズ）
CTextureWin32::CTextureWin32(LPDIRECT3DDEVICE9 device, int w, int h)
:	m_pDevice(device)
	, m_pTexture(NULL)
{
	memset(&m_stTexture,0,sizeof(m_stTexture));

	m_stTexture.nWidth = m_stTexture.nOrgWidth = w;
	m_stTexture.nHeight = m_stTexture.nHeight = h;
	
	m_pDevice->CreateTexture(
		w, h, 1, D3DUSAGE_RENDERTARGET, 
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture, NULL);
}

// コンストラクタ（ロード）
CTextureWin32::CTextureWin32(LPDIRECT3DDEVICE9 device, string file, CArchiveLoader* archive)
:	m_pDevice(device)
	, m_pTexture(NULL)
{
	memset(&m_stTexture,0,sizeof(m_stTexture));

	D3DXIMAGE_INFO info;
	if (archive) {
		ARCHIVE_ENTRY* e=archive->Find(file);
		if (e) {
			D3DXCreateTextureFromFileInMemoryEx(
				m_pDevice, e->Data, e->Size, 
				D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
				D3DX_DEFAULT, D3DX_DEFAULT, 0, 
				&info, NULL, &m_pTexture);
		}
	} else {
		D3DXCreateTextureFromFileEx(
			m_pDevice, file.c_str(),			
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
			D3DX_DEFAULT, D3DX_DEFAULT, 0, 
			&info, NULL, &m_pTexture);
	}
	if (m_pTexture) {
		D3DSURFACE_DESC desc;
		m_pTexture->GetLevelDesc(0, &desc);
		m_stTexture.nWidth=desc.Width;
		m_stTexture.nHeight=desc.Height;
		m_stTexture.nOrgWidth=info.Width;
		m_stTexture.nOrgHeight=info.Height;
	}
	else{
		mASSERT(mFalse);
	}
}

// デストラクタ
CTextureWin32::~CTextureWin32() {
	if (m_pTexture) {
		m_pTexture->Release();
		m_pTexture=NULL;
	}
}

// 矩形の描画
void DrawRect(
	LPDIRECT3DDEVICE9 device,
	float x0, float y0, D3DCOLOR c0, float u0, float v0, 
	float x1, float y1, D3DCOLOR c1, float u1, float v1,
	float x2, float y2, D3DCOLOR c2, float u2, float v2,
	float x3, float y3, D3DCOLOR c3, float u3, float v3
) {

	// 頂点座標の設定
	VERTEX vertex[4]={
		{x0, y0, 0, 1, c0, u0, v0},
		{x1, y1, 0, 1, c1, u1, v1}, 
		{x2, y2, 0, 1, c2, u2, v2},
		{x3, y3, 0, 1, c3, u3, v3}
	};

	// 描画
	device->SetFVF(D3DFVF_VERTEX);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX));	
}

// 矩形の描画
void DrawRect(
	LPDIRECT3DDEVICE9 device,
	float x, float y, float dx, float dy, 
	float u, float v, float du, float dv, 
	D3DCOLOR diffuse
) {

	// 頂点座標の設定
	VERTEX vertex[4]={
		{x, y, 0, 1, diffuse, u, v},
		{x+dx, y, 0, 1, diffuse, u+du, v},
		{x, y+dy, 0, 1, diffuse, u, v+dv},
		{x+dx, y+dy, 0, 1, diffuse, u+du, v+dv}
	};

	// 描画
	device->SetFVF(D3DFVF_VERTEX);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX));	
}

// 矩形の描画
void DrawRect(
	LPDIRECT3DDEVICE9 device,
	float x, float y, float dx, float dy, 
	D3DCOLOR diffuse
) {
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	DrawRect(device, x, y, dx, dy, 0, 0, 0, 0, diffuse);
}

// テクスチャの描画
void CTextureWin32::Draw(
	float x0, float y0, D3DCOLOR c0, float u0, float v0, 
	float x1, float y1, D3DCOLOR c1, float u1, float v1,
	float x2, float y2, D3DCOLOR c2, float u2, float v2,
	float x3, float y3, D3DCOLOR c3, float u3, float v3
) {
	LPDIRECT3DDEVICE9 device=m_pDevice;
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTexture(0, m_pTexture);
	DrawRect(
		device,
		x0, y0, c0, u0, v0, 
		x1, y1, c1, u1, v1, 
		x2, y2, c2, u2, v2, 
		x3, y3, c3, u3, v3
	);
}

// テクスチャの描画
void CTextureWin32::Draw(
	float x, float y, float dx, float dy, 
	float u, float v, float du, float dv, 
	D3DCOLOR diffuse
) {
	LPDIRECT3DDEVICE9 device=m_pDevice;
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTexture(0, m_pTexture);
	DrawRect(device, x, y, dx, dy, u, v, du, dv, diffuse);
}

// テクスチャ塗りつぶし用の関数
static VOID WINAPI FillTexture(
	D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, 
	const D3DXVECTOR2* pTexelSize, LPVOID pData
) {
	*pOut=*(D3DXVECTOR4*)pData;
}

// 塗りつぶし
void CTextureWin32::Fill(D3DXVECTOR4* color) {
	D3DXFillTexture(m_pTexture, FillTexture, color);
}




#endif