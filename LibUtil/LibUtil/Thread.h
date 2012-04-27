#pragma once
#ifndef THREADH
#define THREADH

#include <windows.h>


//==============================================================
// スレッド
class CThread {
protected:
	HANDLE Thread;
	DWORD ID;
public:

	// コンストラクタ
	CThread(DWORD stackSize=0);
	
	// デストラクタ
	virtual ~CThread();
	
	// スレッドの起動
	inline void Start() { ResumeThread(Thread); }

	// IDの取得
	inline DWORD GetID() { return ID; }
	
	// 優先順位の取得
	inline int GetPriority() { GetThreadPriority(Thread); }

	// 優先順位の設定
	inline void SetPriority(int priority) { SetThreadPriority(Thread, priority); }

	// 終了の待機
	inline bool Join(DWORD millisecond=INFINITE) {
		return WaitForSingleObject(Thread, millisecond)==WAIT_OBJECT_0;
	}

	// 終了コードの取得
	inline DWORD GetExitCode() { DWORD i; GetExitCodeThread(Thread, &i); return i; }

	// スレッドの処理を定義する関数
	virtual DWORD Run() { return 0; }
};


//==============================================================
// クリティカルセクション
class CCriticalSection {
protected:
	CRITICAL_SECTION CriticalSection;
public:

	// コンストラクタ
	CCriticalSection() { InitializeCriticalSection(&CriticalSection); }

	// デストラクタ
	~CCriticalSection() { DeleteCriticalSection(&CriticalSection); }

	// クリティカルセクションの開始
	inline void Enter() { EnterCriticalSection(&CriticalSection); }

	// クリティカルセクションの開始を試みる
	// inline BOOL TryEnter() { return TryEnterCriticalSection(&CriticalSection); }

	// クリティカルセクションの終了
	inline void Leave() { LeaveCriticalSection(&CriticalSection); }
};


#endif
