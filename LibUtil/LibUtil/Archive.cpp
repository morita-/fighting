#include "LibUtil.h"
#include <assert.h>

// �f�X�g���N�^�i�Z�[�u�j
CArchiveSaver::~CArchiveSaver() {
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		delete[] Entry[i].Data;
	}
}

// �G���g���̒ǉ�
void CArchiveSaver::Add(string name) {
	
	// �G���g���t�@�C�����J��
	HANDLE h=CreateFile(
		name.c_str(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h!=INVALID_HANDLE_VALUE) {

		// �T�C�Y�𒲂ׂ�
		DWORD size=GetFileSize(h, NULL);
		char* data=new char[size];
		
		// �G���g���t�@�C����ǂݍ���
		DWORD nbytes;
		ReadFile(h, data, size, &nbytes, NULL);
		
		// �G���g���̓o�^
		ARCHIVE_ENTRY e;
		e.Name=name;
		e.Size=size;
		e.Data=data;
		Entry.push_back(e);
		
		// �G���g���t�@�C�������
		CloseHandle(h);

		// �w�b�_�T�C�Y�̌v�Z
		DWORD header_size=sizeof(DWORD)*2;
		for (size_t i=0, n=Entry.size(); i<n; i++) {
			header_size+=Entry[i].Name.length()+1;
			header_size+=sizeof(DWORD)*2;
		}
		
		// �I�t�Z�b�g�̌v�Z
		DWORD offset=header_size;
		for (size_t i=0, n=Entry.size(); i<n; i++) {
			Entry[i].Offset=offset;
			offset+=Entry[i].Size+1;
		}
	}
}

// �Z�[�u
bool CArchiveSaver::Save(string name) {

	// ���[�N�G���A�̊m��
	if (Entry.empty()) return false;
	DWORD work_size=Entry.back().Offset+Entry.back().Size+1;
	char* work=new char[work_size];
	char* p=work+sizeof(DWORD);

	// �G���g�����̏�������
	*(DWORD*)p=Entry.size();
	p+=sizeof(DWORD);
	
	// �G���g���̏�������
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		
		// ���O
		strcpy(p, Entry[i].Name.c_str());
		p+=Entry[i].Name.length()+1;
		
		// �T�C�Y
		*(DWORD*)p=Entry[i].Size;
		p+=sizeof(DWORD);
		
		// �I�t�Z�b�g
		*(DWORD*)p=Entry[i].Offset;
		p+=sizeof(DWORD);
	}
	
	// �f�[�^�̏�������
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		
		// �f�[�^
		memcpy(p, Entry[i].Data, Entry[i].Size);
		p+=Entry[i].Size;
		
		// �k������
		*p++='\0';
	}
	assert(p-work==work_size);

	// �`�F�b�N�T��
	DWORD sum=0;
	for (DWORD i=4, n=work_size-3; i<n; i+=4) {
		sum+=*(DWORD*)(work+i);
	}
	*(DWORD*)work=sum;

	// �X�N�����u��
	Rand.Init(Password);
	for (DWORD i=0, n=work_size-3; i<n; i+=4) {
		*(DWORD*)(work+i)^=Rand.Int32();
	}

	// �A�[�J�C�u�t�@�C�����J��
	HANDLE h=CreateFile(
		name.c_str(), GENERIC_WRITE, 0, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h==INVALID_HANDLE_VALUE) return false;
		
	// ��������
	DWORD nbytes;
	WriteFile(h, work, work_size, &nbytes, NULL);

	// �A�[�J�C�u�t�@�C�������
	CloseHandle(h);
	
	// ���[�N�G���A�̉��
	delete[] work;
	
	return true;
}

// �R���X�g���N�^�i���[�h�j
CArchiveLoader::CArchiveLoader()
:	Work(NULL), WorkSize(0) 
{}

// �f�X�g���N�^�i���[�h�j
CArchiveLoader::~CArchiveLoader() {
	if (Work) delete[] Work;
}

// ���[�h
bool CArchiveLoader::Load(string name) {
	DWORD nbytes;

	// �A�[�J�C�u�t�@�C�����J��
	HANDLE h=CreateFile(
		name.c_str(), GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h==INVALID_HANDLE_VALUE) return false;

	// �T�C�Y�𒲂ׂ�
	DWORD size=GetFileSize(h, NULL);
	char* data=new char[size];
	
	// ���[�N�G���A�̊m��
	if (WorkSize<size) {
		if (Work) delete[] Work;
		Work=new char[size];
		WorkSize=size;
	}
	
	// �A�[�J�C�u�t�@�C����ǂݍ���
	ReadFile(h, Work, size, &nbytes, NULL);
	
	// �A�[�J�C�u�t�@�C�������
	CloseHandle(h);

	// �X�N�����u������
	Rand.Init(Password);
	for (DWORD i=0, n=size-3; i<n; i+=4) {
		*(DWORD*)(Work+i)^=Rand.Int32();
	}

	// �`�F�b�N�T��
	DWORD sum=0;
	for (DWORD i=4, n=size-3; i<n; i+=4) {
		sum+=*(DWORD*)(Work+i);
	}
	if (sum!=*(DWORD*)Work) return false;

	// �G���g���̃N���A
	Entry.clear();
	
	// �G���g�����̓ǂݍ���
	char* p=Work+sizeof(DWORD);
	DWORD num_entries=*(DWORD*)p;
	p+=sizeof(DWORD);

	// �G���g���̓ǂݍ���
	for (DWORD i=0; i<num_entries; i++) {
		ARCHIVE_ENTRY e;
		
		// ���O
		e.Name=p;
		p+=e.Name.length()+1;
		
		// �T�C�Y
		e.Size=*(DWORD*)p;
		p+=sizeof(DWORD);

		// �I�t�Z�b�g
		e.Offset=*(DWORD*)p;
		p+=sizeof(DWORD);

		// �A�h���X
		e.Data=Work+e.Offset;
		
		// �G���g���̓o�^
		Entry.push_back(e);
	}
	return true;
}

// �G���g���̌���
ARCHIVE_ENTRY* CArchive::Find(string name) {
	for (size_t i=0, n=Entry.size(); i<n; i++) {
		if (name==Entry[i].Name) return &Entry[i];
	}
	return NULL;
}


