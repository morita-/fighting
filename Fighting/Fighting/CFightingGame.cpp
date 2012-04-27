#include "CFightingGame.h"

//=========================================================================
// �萔�A�ϐ�

// �Q�[���{��
CFightingGame* gpGame;

// ���b�V���t�@�C����
const char* gMODEL_FILE_NAME[]={
	"00_Mesh1P.x", "00_Mesh2P.x"
};

// �A�j���[�V�����t�@�C����
const char* gANIM_FILE_NAME[]={

	// �ړ�
	"00_2.x", "00_5.x", 
	"00_4.x", "00_6.x", 
	"00_44.x", "00_66.x", 
	"00_121.x", "00_323.x", 
	"00_Run.x", 

	// �K�[�h�A�U��
	"00_2G.x", "00_2K.x", "00_2P.x", 
	"00_5G.x", "00_5K.x", "00_5P.x", 
	"00_3K.x", "00_5PP.x", "00_3236P.x", 

	// �_���[�W
	"00_Damage2Low0.x", "00_Damage2Middle0.x", 
	"00_Damage5Low0.x", "00_Damage5Middle0.x", "00_Damage5High0.x", 

	// ������сA�_�E���A�_�E���U��
	"00_Blow20.x", "00_Blow50.x", "00_Down0.x", 
	"00_D8P.x", "00_DamageDown0.x", 

	// �K�[�h����
	"00_Guard2.x", "00_Guard5.x", 

	// ����
	"00_5P+G.x", "00_Thrown(00_P+G).x", 

	// �N��������A�N��������U��
	"00_Standup2.x", "00_Standup2K.x", "00_Standup5.x", "00_Standup5K.x", 
};

// �A�j���[�V�����t�@�C�����i���E�t�]�j
const char* gANIM_FILE_NAME_R[]={

	// �ړ�
	"00R_2.x", "00R_5.x", 
	"00R_4.x", "00R_6.x", 
	"00R_44.x", "00R_66.x", 
	"00R_121.x", "00R_323.x", 
	"00R_Run.x", 

	// �K�[�h�A�U��
	"00R_2G.x", "00R_2K.x", "00R_2P.x", 
	"00R_5G.x", "00R_5K.x", "00R_5P.x", 
	"00R_3K.x", "00R_5PP.x", "00R_3236P.x", 

	// �_���[�W
	"00R_Damage2Low0.x", "00R_Damage2Middle0.x", 
	"00R_Damage5Low0.x", "00R_Damage5Middle0.x", "00R_Damage5High0.x", 

	// ������сA�_�E���A�_�E���U��
	"00R_Blow20.x", "00R_Blow50.x", "00R_Down0.x", 
	"00R_D8P.x", "00R_DamageDown0.x", 

	// �K�[�h����
	"00R_Guard2.x", "00R_Guard5.x", 

	// ����
	"00R_5P+G.x", "00R_Thrown(00_5P+G).x", 

	// �N��������A�N��������U��
	"00R_Standup2.x", "00R_Standup2K.x", "00R_Standup5.x", "00R_Standup5K.x", 
};

//=========================================================================
// �T�u���[�`��

// �G�t�F�N�g�̃��[�h
static ID3DXEffect* LoadEffect(IDirect3DDevice9* device, string file) {
	ID3DXEffect* effect=NULL;
	LPD3DXBUFFER error;

	// �G�t�F�N�g�t�@�C���̑��݂��m�F����
	if (!FileExists(file)) {
		MessageBox(
			NULL, file.c_str(), 
			"�G�t�F�N�g�t�@�C����������܂���B", MB_OK);
		PostQuitMessage(1);
		return NULL;
	}

	// �G�t�F�N�g�t�@�C���̃��[�h
	D3DXCreateEffectFromFile(
		device, file.c_str(), 
		NULL, NULL, 0, NULL, &effect, &error);

	// �R���p�C���G���[�̏ꍇ�̓G���[��\�����ďI��
	if (!effect) {
		MessageBox(
			NULL, 
			error?(LPCTSTR)error->GetBufferPointer():NULL, 
			"�G�t�F�N�g�t�@�C���̃R���p�C���G���[�B", MB_OK);
		PostQuitMessage(1);
		return NULL;
	}

	return effect;
}


//=========================================================================
// �Q�[���{�́i���ʁj

// �R���X�g���N�^
CFightingGame::CFightingGame()
:	CGame("Fighter"), 
	m_bPaused(false), m_bPrevInput(false), m_bShaderEnabled(false),
	m_pShadowTexture(NULL), m_pShadowDepthStencil(NULL)
{
}

// �O���t�B�b�N�C�j�V�����C�Y
void CFightingGame::GameGraphicInitialize()
{
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();

	// �w���v�A�I���A�J�[�\���Ɋւ���ݒ�
	m_HelpURL=m_ExePath+"..\\Help\\index.html";
	m_bConfirmExit=false;
	ShowCursor(TRUE);
	
	m_FPS=60;
	m_bPauseInTheBackground=true;
	m_bDropFrames=false;
	
	// �O���t�B�b�N
	m_pGraphics->SetDepthStencilFormat(D3DFMT_D24S8);
	m_pGraphics->ResetDevice();

	// �t�H���g
	static const char fontchar[]="~|0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,.(){}<>!?+-x*/='\\\"#$%&@[]_";
	m_pFont=new CFontRef(m_pGraphics->GetDevice(), m_ExePath+"..\\Font\\font.png", NULL, fontchar, 16.0f/256, 32.0f/256, 16, 32, 2, 2);

	// �G�t�F�N�g
	m_pEffect=LoadEffect(device, "..\\Effect\\Fighting.fx");

	// ���f��
	for (int i=0; i<MODEL_COUNT; i++) {
		m_pModel.push_back(new CModel(device, gMODEL_FILE_NAME[i], false));
	}
	
	// �A�j���[�V����
	for (int i=0; i<ANIM_COUNT; i++) {
		m_pAnimation.push_back((new CModel(device, gANIM_FILE_NAME[i], true))->GetAnimation(0));
		for (int j=0; j<MODEL_COUNT; j++) {
			m_pModel[j]->AddAnimation(m_pAnimation[i]);
		}
	}
	for (int i=0; i<ANIM_COUNT; i++) {
		m_pAnimation.push_back((new CModel(device, gANIM_FILE_NAME_R[i], true))->GetAnimation(0));
		for (int j=0; j<MODEL_COUNT; j++) {
			m_pModel[j]->AddAnimation(m_pAnimation[ANIM_COUNT+i]);
		}
	}

	// �e�N�X�`��
	m_pFaceNormal=GetTexture(device, "Face_C.dds");
	m_pFaceClosed=GetTexture(device, "Face_C(Closed).dds");
	m_pFacePain=GetTexture(device, "Face_C(Pain).dds");

	m_bPrevInput=false;
}

//=========================================================================
// �f�o�C�X

// �f�o�C�X���X�g
void CFightingGame::OnLostDevice() {
	m_pEffect->OnLostDevice();
	SAFE_RELEASE(m_pShadowTexture);
	SAFE_RELEASE(m_pShadowDepthStencil);
	delete m_pDummyTexture;
}

// �f�o�C�X���Z�b�g
void CFightingGame::OnResetDevice() {
	LPDIRECT3DDEVICE9 device=m_pGraphics->GetDevice();

	// �G�t�F�N�g
	m_pEffect->OnResetDevice();

	// ���ʐݒ�
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// �V�F�[�_�p�ݒ�
	D3DXVECTOR4 light(1, -1, 1, 0);
	D3DXVec4Normalize(&light, &light);
	m_pEffect->SetVector("Light", &light);
	D3DXCOLOR ambient(0.5f, 0.5f, 0.5f, 1);
	m_pEffect->SetVector("Ambient", (D3DXVECTOR4*)&ambient);
	
	// �Œ�p�C�v���C���p�ݒ�
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(128, 128, 128, 255));
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphics->SetLight(0, D3DLIGHT_DIRECTIONAL, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, -1, 1), D3DXCOLOR(1, 1, 1, 1), 1000);

	// �_�~�[�̃e�N�X�`��
	m_pDummyTexture=new CTextureRef(device, 1, 1);
	D3DXVECTOR4 color(1, 1, 1, 1);
	m_pDummyTexture->Fill(&color);

	// �V���h�E�}�b�v�̍쐬
	if (!m_pShadowTexture) {

		// ���������e�N�X�`��
		m_bUseIntTexture=false;
		device->CreateTexture(
			TEX_SIZE, TEX_SIZE, 1, 
			D3DUSAGE_RENDERTARGET, D3DFMT_R32F, 
			D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
		if (!m_pShadowTexture) {
			device->CreateTexture(
				TEX_SIZE, TEX_SIZE, 1, 
				D3DUSAGE_RENDERTARGET, D3DFMT_R16F, 
				D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
			if (!m_pShadowTexture) {
				device->CreateTexture(
					TEX_SIZE, TEX_SIZE, 1, 
					D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, 
					D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
		
				// �����e�N�X�`��
				if (!m_pShadowTexture) {
					m_bUseIntTexture=true;
					device->CreateTexture(
						TEX_SIZE, TEX_SIZE, 1, 
						D3DUSAGE_RENDERTARGET, D3DFMT_G16R16, 
						D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
					if (!m_pShadowTexture) {
						device->CreateTexture(
							TEX_SIZE, TEX_SIZE, 1, 
							D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16, 
							D3DPOOL_DEFAULT, &m_pShadowTexture, NULL);
					}
				}
			}
		}
	}

	// �[�x�o�b�t�@�̍쐬
	if (!m_pShadowDepthStencil) {
		device->CreateDepthStencilSurface(
			TEX_SIZE, TEX_SIZE, D3DFMT_D16, 
			D3DMULTISAMPLE_NONE, 0, TRUE, 
			&m_pShadowDepthStencil, NULL);
	}
}
