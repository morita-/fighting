#include "Thread.h"


//==============================================================
// �X���b�h

// �X���b�h�̋N���֐�
static DWORD WINAPI ThreadProc(LPVOID thread) {
	return ((CThread*)thread)->Run();
}

// �R���X�g���N�^
CThread::CThread(DWORD stackSize) {
	Thread=CreateThread(NULL, stackSize, ThreadProc, (LPVOID)this, CREATE_SUSPENDED, &ID);
}

// �f�X�g���N�^
CThread::~CThread() {
	CloseHandle(Thread);
}

