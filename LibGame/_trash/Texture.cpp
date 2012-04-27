#pragma warning(disable:4819)

#include "Texture.h"
#include <d3dx9.h>


//==============================================================
// ���_�`��

// �\����
struct VERTEX
{
	// �ϊ��ςݒ��_
	FLOAT X, Y, Z, RHW;

	// �f�B�t���[�Y�F
	D3DCOLOR Diffuse;

	// �e�N�X�`�����W
	FLOAT U, V;
};

// FVF
#define D3DFVF_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//==============================================================
// �e�N�X�`��

// �R���X�g���N�^�i�T�C�Y�j
CTexture::CTexture(LPDIRECT3DDEVICE9 device, int w, int h)
:	Device(device), TextureW(w), TextureH(h), OriginalW(w), OriginalH(h), Texture(NULL)
{
	device->CreateTexture(
		w, h, 1, D3DUSAGE_RENDERTARGET, 
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &Texture, NULL);
}

// �R���X�g���N�^�i���[�h�j
CTexture::CTexture(LPDIRECT3DDEVICE9 device, string file, CArchiveLoader* archive)
:	Device(device), TextureW(0), TextureH(0), OriginalW(0), OriginalH(0), Texture(NULL)
{
	D3DXIMAGE_INFO info;
	if (archive) {
		ARCHIVE_ENTRY* e=archive->Find(file);
		if (e) {
			D3DXCreateTextureFromFileInMemoryEx(
				Device, e->Data, e->Size, 
				D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
				D3DX_DEFAULT, D3DX_DEFAULT, 0, 
				&info, NULL, &Texture);
		}
	} else {
		D3DXCreateTextureFromFileEx(
			device, file.c_str(), 
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
			D3DX_DEFAULT, D3DX_DEFAULT, 0, 
			&info, NULL, &Texture);
	}
	if (Texture) {
		D3DSURFACE_DESC desc;
		Texture->GetLevelDesc(0, &desc);
		TextureW=desc.Width;
		TextureH=desc.Height;
		OriginalW=info.Width;
		OriginalH=info.Height;
	}
}

// �f�X�g���N�^
CTexture::~CTexture() {
	if (Texture) {
		Texture->Release();
		Texture=NULL;
	}
}

// ��`�̕`��
void CTexture::DrawRect(
	LPDIRECT3DDEVICE9 device, 
	float x0, float y0, D3DCOLOR c0, float u0, float v0, 
	float x1, float y1, D3DCOLOR c1, float u1, float v1,
	float x2, float y2, D3DCOLOR c2, float u2, float v2,
	float x3, float y3, D3DCOLOR c3, float u3, float v3
) {
	// ���_���W�̐ݒ�
	VERTEX vertex[4]={
		{x0, y0, 0, 1, c0, u0, v0},
		{x1, y1, 0, 1, c1, u1, v1}, 
		{x2, y2, 0, 1, c2, u2, v2},
		{x3, y3, 0, 1, c3, u3, v3}
	};

	// �`��
	device->SetFVF(D3DFVF_VERTEX);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX));	
}

// ��`�̕`��
void CTexture::DrawRect(
	LPDIRECT3DDEVICE9 device, 
	float x, float y, float dx, float dy, 
	float u, float v, float du, float dv, 
	D3DCOLOR diffuse
) {
	// ���_���W�̐ݒ�
	VERTEX vertex[4]={
		{x, y, 0, 1, diffuse, u, v},
		{x+dx, y, 0, 1, diffuse, u+du, v},
		{x, y+dy, 0, 1, diffuse, u, v+dv},
		{x+dx, y+dy, 0, 1, diffuse, u+du, v+dv}
	};

	// �`��
	device->SetFVF(D3DFVF_VERTEX);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(VERTEX));	
}

// ��`�̕`��
void CTexture::DrawRect(
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

// �e�N�X�`���̕`��
void CTexture::Draw(
	float x0, float y0, D3DCOLOR c0, float u0, float v0, 
	float x1, float y1, D3DCOLOR c1, float u1, float v1,
	float x2, float y2, D3DCOLOR c2, float u2, float v2,
	float x3, float y3, D3DCOLOR c3, float u3, float v3
) {
	LPDIRECT3DDEVICE9 device=Device;
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTexture(0, Texture);
	DrawRect(
		device, 
		x0, y0, c0, u0, v0, 
		x1, y1, c1, u1, v1, 
		x2, y2, c2, u2, v2, 
		x3, y3, c3, u3, v3
	);
}

// �e�N�X�`���̕`��
void CTexture::Draw(
	float x, float y, float dx, float dy, 
	float u, float v, float du, float dv, 
	D3DCOLOR diffuse
) {
	LPDIRECT3DDEVICE9 device=Device;
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTexture(0, Texture);
	DrawRect(device, x, y, dx, dy, u, v, du, dv, diffuse);
}

// �e�N�X�`���h��Ԃ��p�̊֐�
static VOID WINAPI FillTexture(
	D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, 
	const D3DXVECTOR2* pTexelSize, LPVOID pData
) {
	*pOut=*(D3DXVECTOR4*)pData;
}

// �h��Ԃ�
void CTexture::Fill(D3DXVECTOR4* color) {
	D3DXFillTexture(Texture, FillTexture, color);
}


