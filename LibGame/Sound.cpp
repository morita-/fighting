#include "Sound.h"
#include "..\DXUT\Core\DXUT.h"
#include "..\LibUtil\LibUtil\LibUtil.h"
#include <windows.h>
#include <cguid.h>
#include <math.h>
/*

//==============================================================
// �T�E���h�֘A�̋��ʃf�[�^
class CSoundCommon {
public:
	IDirectMusicLoader8* Loader;
	unsigned int NumSounds;
	CSoundCommon();
	~CSoundCommon();
};
static CSoundCommon* SoundCommon=NULL;

// �R���X�g���N�^
CSoundCommon::CSoundCommon()
:	Loader(NULL), NumSounds(0)
{
	CoInitialize(NULL);
	CoCreateInstance(
		CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
		IID_IDirectMusicLoader8, (void**)&Loader);
}

// ���ʃf�[�^�̃f�X�g���N�^
CSoundCommon::~CSoundCommon() {
	if (Loader) Loader->Release();
	CoUninitialize();
}

*/
//==============================================================
// ���ʉ��f�[�^�̃N���X

// �R���X�g���N�^
CSfx::CSfx(string file, CArchiveLoader* archive)
//:	Segment(NULL), MemData(NULL)
{
/*	if (!SoundCommon) SoundCommon=new CSoundCommon();
	SoundCommon->NumSounds++;

	// ���[�_�[�̊m�F
	if (!SoundCommon->Loader) return;

	// �t�@�C�����̉��H
	WCHAR wc_file[MAX_PATH+1];
	if (!MultiByteToWideChar(CP_ACP, 0, file.c_str(), -1, wc_file, MAX_PATH)) return;
	
	// �����f�[�^�̃��[�h
	if (archive) {
		ARCHIVE_ENTRY* e=archive->Find(file);
		if (!e) return;
		
		// GetObject�ɂ͈ꎞ�������̈��n�����Ƃ��ł��Ȃ����߁A
		// �������̈���m�ۂ��ăf�[�^���R�s�[����
		MemData=new BYTE[e->Size];
		memcpy(MemData, e->Data, e->Size);
		
		DMUS_OBJECTDESC desc;
		desc.dwSize=sizeof(DMUS_OBJECTDESC);
		desc.guidClass=CLSID_DirectMusicSegment;
		desc.dwValidData=DMUS_OBJ_CLASS|DMUS_OBJ_MEMORY;
		desc.pbMemData=MemData;
		desc.llMemLength=e->Size;
		SoundCommon->Loader->GetObject(
			&desc, IID_IDirectMusicSegment8, (LPVOID*)&Segment);
	} else {
		SoundCommon->Loader->LoadObjectFromFile(
			CLSID_DirectMusicSegment, IID_IDirectMusicSegment8,
			wc_file, (LPVOID*)&Segment);
	}
	*/
}

// �f�X�g���N�^
CSfx::~CSfx()
{
/*	// �Z�O�����g�̉��
	if (Segment) Segment->Release();
	if (MemData) delete[] MemData;
	
	// ���[�_�[�̍폜
	SoundCommon->NumSounds--;
	if (!SoundCommon->NumSounds) {
		delete SoundCommon;
		SoundCommon=NULL;
	}*/
}

// �Đ��񐔂̐ݒ�
void CSfx::SetRepeats(unsigned int repeats) {
/*	if (!Segment) return;
	repeats=
		repeats<0 ? DMUS_SEG_REPEAT_INFINITE : 
		repeats>0 ? (DWORD)repeats-1 : 0;
	Segment->SetRepeats(repeats);	
	*/
}


//==============================================================
// ���ʉ��Đ��̃N���X

// �R���X�g���N�^
CSfxPlayer::CSfxPlayer()
{
	m_pDS=NULL;
	m_pDSListener=NULL;
	memset(&m_dsBufferParams,0,sizeof(DS3DBUFFER));               // 3D buffer properties
	memset(&m_dsListenerParams,0,sizeof(DS3DLISTENER));             // Listener properties


	m_pDS3DBuffer = NULL;

/*	if (!SoundCommon) SoundCommon=new CSoundCommon();

	// �p�t�H�[�}���X�̍쐬
	CoCreateInstance(
		CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
		IID_IDirectMusicPerformance8, (void**)&Performance);

	// �p�t�H�[�}���X�̏������A�I�[�f�B�I�p�X�̎擾
	if (!Performance) return;
	Performance->InitAudio(
		NULL, NULL, NULL, 
		use3d?DMUS_APATH_DYNAMIC_3D:DMUS_APATH_DYNAMIC_STEREO,
		channels,
		DMUS_AUDIOF_ALL, NULL);
	Performance->GetDefaultAudioPath(&AudioPath);
	
	// DirectSound3D�o�b�t�@�̎擾
	if (!AudioPath) return;
	if (use3d) {
		AudioPath->GetObjectInPath(
			DMUS_PCHANNEL_ALL, DMUS_PATH_BUFFER, 0, 
			GUID_NULL, 0, IID_IDirectSound3DBuffer, 
			(LPVOID*)&DS3DBuffer);
	}
	*/
}

// �f�X�g���N�^
CSfxPlayer::~CSfxPlayer()
{
/*	if (DS3DBuffer) DS3DBuffer->Release();
	if (Performance) {
		Performance->CloseDown();
		Performance->Release();
	}*/
    SAFE_RELEASE( m_pDS3DBuffer );

	SAFE_RELEASE( m_pDSListener );
    SAFE_RELEASE( m_pDS );

}

// �C�j�V�����C�Y
HRESULT	CSfxPlayer::Initialize(HWND hwnd,DWORD dwCoopLevel){
	HRESULT             hr;

    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
        return DXUT_ERR( L"DirectSoundCreate8", hr );

    // Set DirectSound coop level
    if( FAILED( hr = m_pDS->SetCooperativeLevel( hwnd, dwCoopLevel ) ) )
        return DXUT_ERR( L"SetCooperativeLevel", hr );

    return S_OK;
}

//�v���C�}���o�b�t�@�[�쐬
HRESULT CSfxPlayer::SetPrimaryBufferFormat( DWORD dwPrimaryChannels,
										    DWORD dwPrimaryFreq,
									        DWORD dwPrimaryBitRate )
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;

    // Get the primary buffer
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;

    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
        return DXUT_ERR( L"CreateSoundBuffer", hr );

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
    wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM;
    wfx.nChannels       = (WORD) dwPrimaryChannels;
    wfx.nSamplesPerSec  = (DWORD) dwPrimaryFreq;
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate;
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
        return DXUT_ERR( L"SetFormat", hr );

    SAFE_RELEASE( pDSBPrimary );

    return S_OK;
}

HRESULT CSfxPlayer::Set3DListener()
{
    HRESULT             hr;

    // Get the 3D listener, so we can control its params
    hr = Get3DListenerInterface( &m_pDSListener );

    if( FAILED(hr) )
    {
        return DXUT_ERR( L"Get3DListenerInterface", hr );
    }

    // Get listener parameters
    m_dsListenerParams.dwSize = sizeof(DS3DLISTENER);
    m_pDSListener->GetAllParameters( &m_dsListenerParams );
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::Get3DListenerInterface()
// Desc: Returns the 3D listener interface associated with primary buffer.
//-----------------------------------------------------------------------------
HRESULT CSfxPlayer::Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener )
{
    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( ppDSListener == NULL )
        return E_INVALIDARG;
    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;

    *ppDSListener = NULL;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
        return DXUT_ERR( L"CreateSoundBuffer", hr );

    if( FAILED( hr = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener,
                                                  (VOID**)ppDSListener ) ) )
    {
        SAFE_RELEASE( pDSBPrimary );
        return DXUT_ERR( L"QueryInterface", hr );
    }

    // Release the primary buffer, since it is not need anymore
    SAFE_RELEASE( pDSBPrimary );

    return S_OK;
}


// 3D��Ԃɂ�����Đ��ʒu�̐ݒ�
void CSfxPlayer::Set3DPosition(float x, float y, float z) {
//	if (!DS3DBuffer) return;
//	DS3DBuffer->SetPosition(x, y, z, DS3D_IMMEDIATE );
}

// �{�����[���̐ݒ�i0.0�`1.0�j
void CSfxPlayer::SetVolume(float volume) {
//	if (!AudioPath) return;
//	volume=100*20*log10(volume);
//	long v=0;
	
//	if (volume<-9600) v=-9600; else if (volume>0) v=0; else v=(long)volume;
//	AudioPath->SetVolume(v, 0);
}

// �_�E�����[�h
void CSfxPlayer::Download(CSfx* pSE) {
//	sound->Segment->Download(Performance);
}

// �Đ�
void CSfxPlayer::Play(CSfx* pSE) {
/*	if (!sound->Segment || !Performance || !AudioPath) return;
	Performance->PlaySegmentEx(
		sound->Segment, NULL, NULL, DMUS_SEGF_SECONDARY, 
		0, NULL, NULL, AudioPath);
*/
}

// ��~
void CSfxPlayer::Stop(CSfx* pSE) {
//	if (!Performance) return;
//	Performance->Stop(sound->Segment, NULL, 0, 0);
}

// �Đ������ǂ���
bool CSfxPlayer::IsPlaying(CSfx* pSE) {
//	if (!Performance) return false;
//	return Performance->IsPlaying(sound->Segment, NULL)==S_OK;
	return false;
}

