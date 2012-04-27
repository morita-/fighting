#pragma warning(disable:4996)
#pragma warning(disable:4995)

#include "Media.h"
#include <stdlib.h>


//==============================================================
// �R���X�g���N�^
CMedia::CMedia()
:	Graph(NULL), Control(NULL), Event(NULL), 
	Seeking(NULL), Audio(NULL)
{
	// COM���C�u�����̏�����
	CoInitialize(NULL);
}


//==========================================================
// �f�X�g���N�^
CMedia::~CMedia() {
	CleanUp();
	CoUninitialize();
}


//==========================================================
// �擾���������̉��
void CMedia::CleanUp() {
	if (Seeking) { Seeking->Release(); Seeking=NULL; }
	if (Event)   { Event->Release();   Event=NULL;   }
	if (Control) { Control->Release(); Control=NULL; }
	if (Graph)   { Graph->Release();   Graph=NULL;   }
	if (Audio)   { Audio->Release();   Audio=NULL;   }
}


//==============================================================
// �t�@�C���̃��[�h
bool CMedia::LoadFromFile(string file_name) {

	// �����̉��
	CleanUp();

	// �C���^�[�t�F�C�X�̎擾
	// �iGraphBuilder�CMediaControl�CMediaEvent�CMediaSeeking�j
	if (FAILED(CoCreateInstance(
			CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
			IID_IGraphBuilder, (void **)&Graph)) ||
		FAILED(Graph->QueryInterface(
			IID_IMediaControl, (void **)&Control)) ||
		FAILED(Graph->QueryInterface(
			IID_IMediaEvent, (void **)&Event)) ||
		FAILED(Graph->QueryInterface(
			IID_IMediaSeeking, (void **)&Seeking)) ||
		FAILED(Graph->QueryInterface(
			IID_IBasicAudio, (void **)&Audio))
	) return false;
	
	// �t�@�C������Unicode�ɕϊ��F
	// mbstowcs�֐��ł͂��܂������Ȃ��B
	wchar_t fn[MAX_PATH];
	MultiByteToWideChar(
		0, 0, file_name.c_str(), -1, fn, MAX_PATH);

	// �O���t�̍쐬
	if (FAILED(Graph->RenderFile(fn, NULL))) return false;

	return true;
}


//==============================================================
// ��~
void CMedia::Stop() {
	if (!Control) return;
	Control->Stop();
	SetPos(0);
}


//==============================================================
// �Đ��C�ꎞ��~

// �Đ�
void CMedia::Play() {
	if (!Control) return;
	Control->Run();
}

// �ꎞ��~
void CMedia::Pause() {
	if (!Control) return;
	Control->Pause();
}


//==============================================================
// �Đ����I��������ǂ���
bool CMedia::IsFinished() {
	if (!Event) return true;
	long code;
	return SUCCEEDED(Event->WaitForCompletion(0, &code));
}


//==============================================================
// �Đ����̈ʒu�C�S�̂̒����i�b�P�ʁj
float CMedia::GetPos() {
	if (!Seeking) return 0;
	LONGLONG current, length;
	Seeking->GetPositions(&current, &length);
	double d=(double)current;
	return (float)(d/10000000);
}

float CMedia::GetLength() {
	if (!Seeking) return 0;
	LONGLONG current, length;
	Seeking->GetPositions(&current, &length);
	double d=(double)length;
	return (float)(d/10000000);
}

//==============================================================
// �Đ��ʒu�̕ύX

// ��Έʒu
void CMedia::SetPos(float pos) {
	if (!Seeking) return;
	LONGLONG current=(LONGLONG)((double)pos*10000000);
	Seeking->SetPositions(
		&current, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning);
}

// ���Έʒu
void CMedia::MovePos(float offset) {
	SetPos(GetPos()+offset);
}

//==============================================================
// �{�����[���i0.0�`1.0�j

float CMedia::GetVolume() {
	if (!Audio) return 0;
	long l;
	Audio->get_Volume(&l);
	return ((float)l+10000)/10000;
}

void CMedia::SetVolume(float vol) {
	if (!Audio) return;
	long l=(long)(vol*10000-10000);
	if (l<-10000) l=-10000; else if (l>0) l=0;
	Audio->put_Volume(l);
}

//==============================================================
// �o�����X�i-1.0�`1.0�j

float CMedia::GetBalance() {
	if (!Audio) return 0;
	long l;
	Audio->get_Balance(&l);
	return (float)l/10000;
}

void CMedia::SetBalance(float bal) {
	if (!Audio) return;
	long l=(long)(bal*10000);
	if (l<-10000) l=-10000; else if (l>10000) l=10000;
	Audio->put_Balance(l);
}


