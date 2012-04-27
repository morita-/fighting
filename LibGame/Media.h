#ifndef SHOWH
#define SHOWH

#include <DShow.h>
#include <string>
using namespace std;


//==============================================================
// 動画と音声の再生
class CMedia {
private:

	// グラフビルダ（メディアソースやデコーダの管理）
	// メディアコントロール（再生や停止）
	// メディアイベント（再生時のイベント処理）
	// メディアシーキング（再生位置の取得や指定）
	IGraphBuilder* Graph;
	IMediaControl* Control;
	IMediaEvent* Event;
	IMediaSeeking* Seeking;
	IBasicAudio* Audio;

    void CleanUp();

public:

	// コンストラクタ，デストラクタ
	CMedia();
	~CMedia();
	
	// 再生，一時停止，停止
	bool LoadFromFile(string file_name);
	void Play();
	void Pause();
	void Stop();

	// ボリューム（0.0～1.0）
	float GetVolume();
	void SetVolume(float vol);

	// バランス（-1.0～1.0）
	float GetBalance();
	void SetBalance(float bal);

	// 再生が終了したかどうか
	bool IsFinished();

	// 再生中の位置，全体の長さ（秒単位）
	float GetPos();
	float GetLength();
	void MovePos(float offset);
	void SetPos(float pos);
};


#endif

