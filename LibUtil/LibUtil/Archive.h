#ifndef ARCHIVEH
#define ARCHIVEH

#include "Rand.h"
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

// アーカイブされたファイルのエントリ
struct ARCHIVE_ENTRY {
	string Name;
	DWORD Size, Offset;
	char* Data;
};

// 共通部分
class CArchive {
protected:
	DWORD Version;
	string Password;
	vector<ARCHIVE_ENTRY> Entry;
	CRand Rand;
public:
	CArchive() {}
	void SetPassword(string password) { Password=password; }
	size_t Count() { return Entry.size(); }
	ARCHIVE_ENTRY* Find(string name);
	ARCHIVE_ENTRY* Get(size_t index) { return &Entry[index]; }
};

// アーカイブの作成
class CArchiveSaver : public CArchive {
public:
	CArchiveSaver() {}
	~CArchiveSaver();
	void Add(string name);
	bool Save(string name);
};

// アーカイブのロード
class CArchiveLoader : public CArchive {
protected:
	char* Work;
	DWORD WorkSize;
public:
	CArchiveLoader();
	~CArchiveLoader();
	bool Load(string name);
};

#endif
