#ifndef TASKH
#define TASKH

class CTask;
class CTaskList;
class CTaskIter;


//==============================================================
// �^�X�N
class CTask {
friend CTaskList;
friend CTaskIter;

	// �^�X�N���X�g�ւ̃|�C���^
	CTaskList* TaskList;

	// �O��̃^�X�N�ւ̃|�C���^
	CTask *Prev, *Next;

	// �^�X�N���X�g���w�肵�Ȃ�new, delete�𖳌��ɂ���
	void* operator new(size_t t) {}
	void operator delete(void* p) {}

protected:
	
	// new, delete�̏����F
	// �K�؂�new, delete���Z�q���T�u�N���X�Œ�`����
	static void* operator_new(size_t t, CTaskList* task_list);
	static void operator_delete(void* p, CTaskList* task_list);

public:
	CTask(CTaskList* task_list);
	virtual ~CTask();
};


//==============================================================
// �^�X�N���X�g
class CTaskList {
friend CTask;
friend CTaskIter;

	// �A�N�e�B�u�^�X�N���X�g�A�t���[�^�X�N���X�g
	CTask *ActiveTask, *FreeTask;

	// �^�X�N�̍ő�T�C�Y�A�^�X�N�̍ő吔
	int MaxTaskSize, MaxNumTask;
	
	// �t���[�^�X�N�̐�
	int NumFreeTask;

public:

	// �R���X�g���N�^
	CTaskList(int max_task_size, int max_num_task);
	
	// �^�X�N�̐�
	int GetNumFreeTask() { return NumFreeTask; }
	int GetNumActiveTask() { return MaxNumTask-NumFreeTask; }

	// �S�^�X�N�̏���
	void DeleteTask();
};


//==============================================================
// �^�X�N�C�e���[�^
class CTaskIter {
protected:
	CTaskList* TaskList;
	CTask* Task;
public:

	// �R���X�g���N�^
	inline CTaskIter(CTaskList* task_list)
	: TaskList(task_list), Task(task_list->ActiveTask)
	{}

	// ���̗v�f������Ƃ�true
	inline bool HasNext() {
		return Task->Next!=TaskList->ActiveTask;
	}

	// ���̗v�f��Ԃ�
	inline CTask* Next() {
		return Task=Task->Next;
	}

	// ���O�ɕԂ����v�f���폜����
	inline void Remove() {
		Task=Task->Prev;
		delete Task->Next;
	}
};


#endif

