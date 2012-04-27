#ifndef TCPH
#define TCPH

#include <winsock.h>
#include <string>
#include <vector>
using namespace std;


//==============================================================
// TCP�\�P�b�g��{�@�\
class CTCPBaseSocket {
protected:
	SOCKET Socket;
public:
	CTCPBaseSocket(SOCKET s);
	virtual ~CTCPBaseSocket();
};


//==============================================================
// TCP�\�P�b�g
class CTCPSocket : CTCPBaseSocket {
protected:
	string LineBuffer;
	vector<string> LineSeparator;
public:
	// �R���X�g���N�^
	CTCPSocket(SOCKET s=INVALID_SOCKET);

	// �ڑ�
	bool Connect(string host, int port);

	// �o�b�t�@�̑���M�F�߂�l�͑���M�����o�C�g��
	int Send(const char* buffer, int length);
	int Recv(char* buffer, int length);

	// �s�̑���M
	void SetLineSeparator(vector<string> ls);
	void SendLine(string s);
	string RecvLine();
};


//==============================================================
// TCP�T�[�o�\�P�b�g
class CTCPServerSocket : CTCPBaseSocket {
public:
	// �R���X�g���N�^
	CTCPServerSocket();

	// �|�[�g�ԍ��̐ݒ�A�ڑ��̎󂯓���
	bool Bind(int port);
	CTCPSocket* Accept();
};


#endif

