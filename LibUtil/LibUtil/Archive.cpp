#include "LibUtil.h"
#include <assert.h>

// デストラクタ（セーブ）
CArchiveSaver::~CArchiveSaver() {
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		delete[] Entry[i].Data;
	}
}

// エントリの追加
void CArchiveSaver::Add(string name) {
	
	// エントリファイルを開く
	HANDLE h=CreateFile(
		name.c_str(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h!=INVALID_HANDLE_VALUE) {

		// サイズを調べる
		DWORD size=GetFileSize(h, NULL);
		char* data=new char[size];
		
		// エントリファイルを読み込む
		DWORD nbytes;
		ReadFile(h, data, size, &nbytes, NULL);
		
		// エントリの登録
		ARCHIVE_ENTRY e;
		e.Name=name;
		e.Size=size;
		e.Data=data;
		Entry.push_back(e);
		
		// エントリファイルを閉じる
		CloseHandle(h);

		// ヘッダサイズの計算
		DWORD header_size=sizeof(DWORD)*2;
		for (size_t i=0, n=Entry.size(); i<n; i++) {
			header_size+=Entry[i].Name.length()+1;
			header_size+=sizeof(DWORD)*2;
		}
		
		// オフセットの計算
		DWORD offset=header_size;
		for (size_t i=0, n=Entry.size(); i<n; i++) {
			Entry[i].Offset=offset;
			offset+=Entry[i].Size+1;
		}
	}
}

// セーブ
bool CArchiveSaver::Save(string name) {

	// ワークエリアの確保
	if (Entry.empty()) return false;
	DWORD work_size=Entry.back().Offset+Entry.back().Size+1;
	char* work=new char[work_size];
	char* p=work+sizeof(DWORD);

	// エントリ数の書き込み
	*(DWORD*)p=Entry.size();
	p+=sizeof(DWORD);
	
	// エントリの書き込み
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		
		// 名前
		strcpy(p, Entry[i].Name.c_str());
		p+=Entry[i].Name.length()+1;
		
		// サイズ
		*(DWORD*)p=Entry[i].Size;
		p+=sizeof(DWORD);
		
		// オフセット
		*(DWORD*)p=Entry[i].Offset;
		p+=sizeof(DWORD);
	}
	
	// データの書き込み
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		
		// データ
		memcpy(p, Entry[i].Data, Entry[i].Size);
		p+=Entry[i].Size;
		
		// ヌル文字
		*p++='\0';
	}
	assert(p-work==work_size);

	// チェックサム
	DWORD sum=0;
	for (DWORD i=4, n=work_size-3; i<n; i+=4) {
		sum+=*(DWORD*)(work+i);
	}
	*(DWORD*)work=sum;

	// スクランブル
	Rand.Init(Password);
	for (DWORD i=0, n=work_size-3; i<n; i+=4) {
		*(DWORD*)(work+i)^=Rand.Int32();
	}

	// アーカイブファイルを開く
	HANDLE h=CreateFile(
		name.c_str(), GENERIC_WRITE, 0, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h==INVALID_HANDLE_VALUE) return false;
		
	// 書き込み
	DWORD nbytes;
	WriteFile(h, work, work_size, &nbytes, NULL);

	// アーカイブファイルを閉じる
	CloseHandle(h);
	
	// ワークエリアの解放
	delete[] work;
	
	return true;
}

// コンストラクタ（ロード）
CArchiveLoader::CArchiveLoader()
:	Work(NULL), WorkSize(0) 
{}

// デストラクタ（ロード）
CArchiveLoader::~CArchiveLoader() {
	if (Work) delete[] Work;
}

// ロード
bool CArchiveLoader::Load(string name) {
	DWORD nbytes;

	// アーカイブファイルを開く
	HANDLE h=CreateFile(
		name.c_str(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h==INVALID_HANDLE_VALUE) return false;

	// サイズを調べる
	DWORD size=GetFileSize(h, NULL);
	char* data=new char[size];
	
	// ワークエリアの確保
	if (WorkSize<size) {
		if (Work) delete[] Work;
		Work=new char[size];
		WorkSize=size;
	}
	
	// アーカイブファイルを読み込む
	ReadFile(h, Work, size, &nbytes, NULL);
	
	// アーカイブファイルを閉じる
	CloseHandle(h);

	// スクランブル解除
	Rand.Init(Password);
	for (DWORD i=0, n=size-3; i<n; i+=4) {
		*(DWORD*)(Work+i)^=Rand.Int32();
	}

	// チェックサム
	DWORD sum=0;
	for (DWORD i=4, n=size-3; i<n; i+=4) {
		sum+=*(DWORD*)(Work+i);
	}
	if (sum!=*(DWORD*)Work) return false;

	// エントリのクリア
	Entry.clear();
	
	// エントリ数の読み込み
	char* p=Work+sizeof(DWORD);
	DWORD num_entries=*(DWORD*)p;
	p+=sizeof(DWORD);

	// エントリの読み込み
	for (DWORD i=0; i<num_entries; i++) {
		ARCHIVE_ENTRY e;
		
		// 名前
		e.Name=p;
		p+=e.Name.length()+1;
		
		// サイズ
		e.Size=*(DWORD*)p;
		p+=sizeof(DWORD);

		// オフセット
		e.Offset=*(DWORD*)p;
		p+=sizeof(DWORD);

		// アドレス
		e.Data=Work+e.Offset;
		
		// エントリの登録
		Entry.push_back(e);
	}
	return true;
}

// エントリの検索
ARCHIVE_ENTRY* CArchive::Find(string name) {
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		if (name==Entry[i].Name) return &Entry[i];
	}
	return NULL;
}


