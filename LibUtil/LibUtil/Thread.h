#pragma once
#ifndef THREADH
#define THREADH

#include <windows.h>


//==============================================================
// �X���b�h
class CThread {
protected:
	HANDLE Thread;
	DWORD ID;
public:

	// �R���X�g���N�^
	CThread(DWORD stackSize=0);
	
	// �f�X�g���N�^
	virtual ~CThread();
	
	// �X���b�h�̋N��
	inline void Start() { ResumeThread(Thread); }

	// ID�̎擾
	inline DWORD GetID() { return ID; }
	
	// �D�揇�ʂ̎擾
	inline int GetPriority() { GetThreadPriority(Thread); }

	// �D�揇�ʂ̐ݒ�
	inline void SetPriority(int priority) { SetThreadPriority(Thread, priority); }

	// �I���̑ҋ@
	inline bool Join(DWORD millisecond=INFINITE) {
		return WaitForSingleObject(Thread, millisecond)==WAIT_OBJECT_0;
	}

	// �I���R�[�h�̎擾
	inline DWORD GetExitCode() { DWORD i; GetExitCodeThread(Thread, &i); return i; }

	// �X���b�h�̏������`����֐�
	virtual DWORD Run() { return 0; }
};


//==============================================================
// �N���e�B�J���Z�N�V����
class CCriticalSection {
protected:
	CRITICAL_SECTION CriticalSection;
public:

	// �R���X�g���N�^
	CCriticalSection() { InitializeCriticalSection(&CriticalSection); }

	// �f�X�g���N�^
	~CCriticalSection() { DeleteCriticalSection(&CriticalSection); }

	// �N���e�B�J���Z�N�V�����̊J�n
	inline void Enter() { EnterCriticalSection(&CriticalSection); }

	// �N���e�B�J���Z�N�V�����̊J�n�����݂�
	// inline BOOL TryEnter() { return TryEnterCriticalSection(&CriticalSection); }

	// �N���e�B�J���Z�N�V�����̏I��
	inline void Leave() { LeaveCriticalSection(&CriticalSection); }
};


#endif
