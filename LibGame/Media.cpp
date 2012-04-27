#pragma warning(disable:4996)
#pragma warning(disable:4995)

#include "Media.h"
#include <stdlib.h>


//==============================================================
// コンストラクタ
CMedia::CMedia()
:	Graph(NULL), Control(NULL), Event(NULL), 
	Seeking(NULL), Audio(NULL)
{
	// COMライブラリの初期化
	CoInitialize(NULL);
}


//==========================================================
// デストラクタ
CMedia::~CMedia() {
	CleanUp();
	CoUninitialize();
}


//==========================================================
// 取得した資源の解放
void CMedia::CleanUp() {
	if (Seeking) { Seeking->Release(); Seeking=NULL; }
	if (Event)   { Event->Release();   Event=NULL;   }
	if (Control) { Control->Release(); Control=NULL; }
	if (Graph)   { Graph->Release();   Graph=NULL;   }
	if (Audio)   { Audio->Release();   Audio=NULL;   }
}


//==============================================================
// ファイルのロード
bool CMedia::LoadFromFile(string file_name) {

	// 資源の解放
	CleanUp();

	// インターフェイスの取得
	// （GraphBuilder，MediaControl，MediaEvent，MediaSeeking）
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
	
	// ファイル名をUnicodeに変換：
	// mbstowcs関数ではうまく動かない。
	wchar_t fn[MAX_PATH];
	MultiByteToWideChar(
		0, 0, file_name.c_str(), -1, fn, MAX_PATH);

	// グラフの作成
	if (FAILED(Graph->RenderFile(fn, NULL))) return false;

	return true;
}


//==============================================================
// 停止
void CMedia::Stop() {
	if (!Control) return;
	Control->Stop();
	SetPos(0);
}


//==============================================================
// 再生，一時停止

// 再生
void CMedia::Play() {
	if (!Control) return;
	Control->Run();
}

// 一時停止
void CMedia::Pause() {
	if (!Control) return;
	Control->Pause();
}


//==============================================================
// 再生が終わったかどうか
bool CMedia::IsFinished() {
	if (!Event) return true;
	long code;
	return SUCCEEDED(Event->WaitForCompletion(0, &code));
}


//==============================================================
// 再生中の位置，全体の長さ（秒単位）
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
// 再生位置の変更

// 絶対位置
void CMedia::SetPos(float pos) {
	if (!Seeking) return;
	LONGLONG current=(LONGLONG)((double)pos*10000000);
	Seeking->SetPositions(
		&current, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning);
}

// 相対位置
void CMedia::MovePos(float offset) {
	SetPos(GetPos()+offset);
}

//==============================================================
// ボリューム（0.0～1.0）

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
// バランス（-1.0～1.0）

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


