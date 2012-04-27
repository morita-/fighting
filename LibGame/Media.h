#ifndef SHOWH
#define SHOWH

#include <DShow.h>
#include <string>
using namespace std;


//==============================================================
// ����Ɖ����̍Đ�
class CMedia {
private:

	// �O���t�r���_�i���f�B�A�\�[�X��f�R�[�_�̊Ǘ��j
	// ���f�B�A�R���g���[���i�Đ����~�j
	// ���f�B�A�C�x���g�i�Đ����̃C�x���g�����j
	// ���f�B�A�V�[�L���O�i�Đ��ʒu�̎擾��w��j
	IGraphBuilder* Graph;
	IMediaControl* Control;
	IMediaEvent* Event;
	IMediaSeeking* Seeking;
	IBasicAudio* Audio;

    void CleanUp();

public:

	// �R���X�g���N�^�C�f�X�g���N�^
	CMedia();
	~CMedia();
	
	// �Đ��C�ꎞ��~�C��~
	bool LoadFromFile(string file_name);
	void Play();
	void Pause();
	void Stop();

	// �{�����[���i0.0�`1.0�j
	float GetVolume();
	void SetVolume(float vol);

	// �o�����X�i-1.0�`1.0�j
	float GetBalance();
	void SetBalance(float bal);

	// �Đ����I���������ǂ���
	bool IsFinished();

	// �Đ����̈ʒu�C�S�̂̒����i�b�P�ʁj
	float GetPos();
	float GetLength();
	void MovePos(float offset);
	void SetPos(float pos);
};


#endif

