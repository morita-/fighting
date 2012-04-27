#include "LibUtil.h"


//==============================================================
// Winsock�̏������ƏI��

// �g�p���̃\�P�b�g��
static int SocketCount=0;

// Winsock�̏�����
static void InitializeWinsock() {
	if (SocketCount==0) {
		WSADATA data;
		WSAStartup(MAKEWORD(2,0), &data);
	}
	SocketCount++;
}

// Winsock�̏I��
static void FinalizeWinsock() {
	SocketCount--;
	if (SocketCount==0) {
		WSACleanup();
	}
}


//==============================================================
// �\�P�b�g��{�@�\

// �R���X�g���N�^
CTCPBaseSocket::CTCPBaseSocket(SOCKET s)
:	Socket(s)
{
	InitializeWinsock();
	if (Socket==INVALID_SOCKET) {
		Socket=socket(AF_INET, SOCK_STREAM, 0);
	}
}

// �f�X�g���N�^
CTCPBaseSocket::~CTCPBaseSocket() {
	closesocket(Socket);
	FinalizeWinsock();
}


//==============================================================
// TCP�\�P�b�g

// �R���X�g���N�^
CTCPSocket::CTCPSocket(SOCKET s)
:	CTCPBaseSocket(s)
{
	LineSeparator.push_back("\r\n");
	LineSeparator.push_back("\r");
	LineSeparator.push_back("\n");
}

// �ڑ��F
// ��������true��Ԃ�
bool CTCPSocket::Connect(string host, int port) {

	// �z�X�g�����A�h���X�ɕϊ�
	LPHOSTENT hostent=gethostbyname(host.c_str());
	if (!hostent ||
		hostent->h_addrtype!=AF_INET ||
		hostent->h_length!=4 ||
		!hostent->h_addr_list[0]) return false;

	// �A�h���X�ƃ|�[�g�̓o�^
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_addr.s_addr=*(unsigned long*)(hostent->h_addr_list[0]);
	sa.sin_port=htons(port);

	// �ڑ��̎��s
	return connect(Socket, (sockaddr*)&sa, sizeof(sa))==0;
}

// �o�b�t�@�̑��M
int CTCPSocket::Send(const char* buffer, int length) {
	return send(Socket, buffer, length, 0);
}

// �o�b�t�@�̎�M
int CTCPSocket::Recv(char* buffer, int length) {
	return recv(Socket, buffer, length, 0);
}

// �s�Z�p���[�^�̐ݒ�
void CTCPSocket::SetLineSeparator(vector<string> ls) {
	LineSeparator.clear();
	for (size_t i=0, n=ls.size(); i<n; i++) {
		LineSeparator.push_back(ls[i]);
	}
}

// �s�̑��M
void CTCPSocket::SendLine(string s) {
	if (!LineSeparator.empty()) s+=LineSeparator[0];
	Send(s.c_str(), s.length());
}

// �s�̎�M
string CTCPSocket::RecvLine() {
	char buffer[1000];
	int length=0;
	do {
		buffer[length]='\0';
		LineBuffer+=buffer;
		for (size_t i=0, n=LineSeparator.size(); i<n; i++) {
			int pos;
			if ((pos=Find(LineBuffer, LineSeparator[i]))>=0) {
				string s=LineBuffer.substr(0, pos);
				LineBuffer=LineBuffer.substr(pos+LineSeparator[i].length());
				return s;
			}
		}
	} while ((length=Recv(buffer, sizeof(buffer)-1))>0);
	return "";
}


//==============================================================
// TCP�T�[�o�\�P�b�g

// �R���X�g���N�^
CTCPServerSocket::CTCPServerSocket()
:	CTCPBaseSocket(INVALID_SOCKET)
{}

// �|�[�g�ԍ��̐ݒ�F
// ��������true��Ԃ�
bool CTCPServerSocket::Bind(int port) {
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_port=htons(port);
	return bind(Socket, (sockaddr*)&sa, sizeof(sa))==0;
}

// �ڑ��̎󂯓���F
// ����ɐڑ��ł����Ƃ��ɂ�CTCPSocket�̐V�����C���X�^���X��Ԃ�
CTCPSocket* CTCPServerSocket::Accept() {

	// �ڑ��҂�
	if (listen(Socket, SOMAXCONN)!=0) return NULL;

	// �ڑ��̎󂯓���
	SOCKET socket=accept(Socket, NULL, NULL);
	if (socket==INVALID_SOCKET) return NULL;

	// CTCPSocket�̐���
	return new CTCPSocket(socket);
}


