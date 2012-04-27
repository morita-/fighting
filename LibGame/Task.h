#ifndef TASKH
#define TASKH

class CTask;
class CTaskList;
class CTaskIter;


//==============================================================
// タスク
class CTask {
friend CTaskList;
friend CTaskIter;

	// タスクリストへのポインタ
	CTaskList* TaskList;

	// 前後のタスクへのポインタ
	CTask *Prev, *Next;

	// タスクリストを指定しないnew, deleteを無効にする
	void* operator new(size_t t) {}
	void operator delete(void* p) {}

protected:
	
	// new, deleteの処理：
	// 適切なnew, delete演算子をサブクラスで定義する
	static void* operator_new(size_t t, CTaskList* task_list);
	static void operator_delete(void* p, CTaskList* task_list);

public:
	CTask(CTaskList* task_list);
	virtual ~CTask();
};


//==============================================================
// タスクリスト
class CTaskList {
friend CTask;
friend CTaskIter;

	// アクティブタスクリスト、フリータスクリスト
	CTask *ActiveTask, *FreeTask;

	// タスクの最大サイズ、タスクの最大数
	int MaxTaskSize, MaxNumTask;
	
	// フリータスクの数
	int NumFreeTask;

public:

	// コンストラクタ
	CTaskList(int max_task_size, int max_num_task);
	
	// タスクの数
	int GetNumFreeTask() { return NumFreeTask; }
	int GetNumActiveTask() { return MaxNumTask-NumFreeTask; }

	// 全タスクの消去
	void DeleteTask();
};


//==============================================================
// タスクイテレータ
class CTaskIter {
protected:
	CTaskList* TaskList;
	CTask* Task;
public:

	// コンストラクタ
	inline CTaskIter(CTaskList* task_list)
	: TaskList(task_list), Task(task_list->ActiveTask)
	{}

	// 次の要素があるときtrue
	inline bool HasNext() {
		return Task->Next!=TaskList->ActiveTask;
	}

	// 次の要素を返す
	inline CTask* Next() {
		return Task=Task->Next;
	}

	// 直前に返した要素を削除する
	inline void Remove() {
		Task=Task->Prev;
		delete Task->Next;
	}
};


#endif

