#ifndef LIBGAME_SOUNDH
#define LIBGAME_SOUNDH


#include "..\LibUtil\LibUtil\Archive.h"
//#include <dmusicc.h>
//#include <dmusici.h>
#include <dsound.h>
#include <vector>
#include <string>
using namespace std;


// 効果音データのクラス
class CSfx {
friend class CSfxPlayer;
protected:

//	IDirectMusicSegment8* Segment;
	BYTE* MemData;

public:

	CSfx(string file, CArchiveLoader* archive);
	~CSfx();
	void SetRepeats(unsigned int repeats);

};

#define DEF_SFX_DS_PRIO		(DSSCL_PRIORITY)

#define DEF_SFX_PRIM_CHANNELS 2
#define DEF_SFX_PRIM_FREQ	  22050
#define	DEF_SFX_PRIM_BITRATE  16

// 効果音再生のクラス
class CSfxPlayer {
protected:

//	IDirectMusicPerformance8* Performance;
//	IDirectMusicAudioPath8* AudioPath;
//	IDirectSound3DBuffer8* DS3DBuffer;
	IDirectSound8* m_pDS;
	LPDIRECTSOUND3DLISTENER m_pDSListener;   // 3D listener object
	DS3DBUFFER              m_dsBufferParams;               // 3D buffer properties
	DS3DLISTENER            m_dsListenerParams;             // Listener properties

	LPDIRECTSOUND3DBUFFER   m_pDS3DBuffer;   // 3D sound buffer

	HRESULT Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener );
public:

	CSfxPlayer();
	~CSfxPlayer();

	HRESULT Initialize(HWND hwnd,DWORD dwCoopLevel);
	HRESULT SetPrimaryBufferFormat( DWORD dwPrimaryChannels,
								    DWORD dwPrimaryFreq,
							        DWORD dwPrimaryBitRate );
	HRESULT Set3DListener();



	void Set3DPosition(float x, float y, float z);
	void SetVolume(float volume);

	bool IsPlaying(CSfx* pSE);
	void Download(CSfx* pSE);
	void Play(CSfx* pSE);
	void Stop(CSfx* pSE=NULL);

};


#endif
