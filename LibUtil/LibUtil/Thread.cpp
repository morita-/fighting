#include "Thread.h"


//==============================================================
// スレッド

// スレッドの起動関数
static DWORD WINAPI ThreadProc(LPVOID thread) {
	return ((CThread*)thread)->Run();
}

// コンストラクタ
CThread::CThread(DWORD stackSize) {
	Thread=CreateThread(NULL, stackSize, ThreadProc, (LPVOID)this, CREATE_SUSPENDED, &ID);
}

// デストラクタ
CThread::~CThread() {
	CloseHandle(Thread);
}

