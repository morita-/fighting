#ifndef GRAPHICSH
#define GRAPHICSH

#include <d3dx9.h>
#include <string>
using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif


//==============================================================
// �O���t�B�b�N�X�`��@�\���Ǘ�����N���X
class CGraphics {
protected:

	// Direct3D�C���^�t�F�[�X�A�f�o�C�X
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pDevice;

	// �E�B���h�E�n���h��
	HWND m_HWnd;
	
	// ���̃E�B���h�E�X�^�C��
	DWORD m_WindowStyle;

	// ���A����
	int m_nWidth, m_nHeight;

	// �t���X�N���[�����[�h�Ȃ��true
	bool m_bFullScreen;

	// ���t���b�V�����[�g(Hz)
	int m_nRefreshRate;

	// GDI�ɂ��`�悪�\�ȃT�[�t�F�X���ǂ���
	bool m_bIsGDISurface;

	D3DFORMAT m_DepthStencilFormat;

public:

	// �R���X�g���N�^�C�f�X�g���N�^
	CGraphics(HWND hwnd);
	~CGraphics();

	// �f�o�C�X�̃��Z�b�g
	void SetWidth(int w) { m_nWidth=w; }
	void SetHeight(int h) { m_nHeight=h; }
	void SetFullScreen(bool b) { m_bFullScreen=b; }
	void SetRefreshRate(int r) { m_nRefreshRate=r; }
	void SetDepthStencilFormat(D3DFORMAT format) { m_DepthStencilFormat=format; }
	bool ResetDevice();
	bool DeviceNotReset();

	// �V�[���`��̊J�n�C�I��
	bool BeginScene() { return SUCCEEDED(m_pDevice->BeginScene()); }
	void EndScene() { m_pDevice->EndScene(); }

	// ��ʏ����C��ʃt���b�v
	void Clear(D3DCOLOR color=D3DCOLOR_ARGB(255, 0, 0, 0));
	bool Present();

	// �e����̎擾
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
	HWND GetHWnd() { return m_HWnd; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }
	bool IsFullScreen() { return m_bFullScreen; }
	int GetRefreshRate() { return m_nRefreshRate; }

	// GDI�ɂ��`�悪�\�ȃT�[�t�F�X�ɂ���
	void PresentGDISurface() { if (!m_bIsGDISurface) Present(); }

	// ���C�g�̐ݒ�
	void SetLight(
		int id, D3DLIGHTTYPE type, 
		D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXCOLOR color, 
		float range
	);
	void SetLight(
		int id, D3DLIGHTTYPE type, 
		float x, float y, float z,
		float range=1000
	);

	// �A���t�@���Z���[�h�̎w��
	void SetAddAlpha(bool add_alpha);

	// �G�t�F�N�g�̃��[�h
	ID3DXEffect* LoadEffect(string file);
	
	// �e�N�X�`���̃��[�h
	LPDIRECT3DTEXTURE9 LoadTexture(string file);
};


#endif



