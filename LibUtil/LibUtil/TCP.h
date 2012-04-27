#ifndef TCPH
#define TCPH

#include <winsock.h>
#include <string>
#include <vector>
using namespace std;


//==============================================================
// TCPソケット基本機能
class CTCPBaseSocket {
protected:
	SOCKET Socket;
public:
	CTCPBaseSocket(SOCKET s);
	virtual ~CTCPBaseSocket();
};


//==============================================================
// TCPソケット
class CTCPSocket : CTCPBaseSocket {
protected:
	string LineBuffer;
	vector<string> LineSeparator;
public:
	// コンストラクタ
	CTCPSocket(SOCKET s=INVALID_SOCKET);

	// 接続
	bool Connect(string host, int port);

	// バッファの送受信：戻り値は送受信したバイト数
	int Send(const char* buffer, int length);
	int Recv(char* buffer, int length);

	// 行の送受信
	void SetLineSeparator(vector<string> ls);
	void SendLine(string s);
	string RecvLine();
};


//==============================================================
// TCPサーバソケット
class CTCPServerSocket : CTCPBaseSocket {
public:
	// コンストラクタ
	CTCPServerSocket();

	// ポート番号の設定、接続の受け入れ
	bool Bind(int port);
	CTCPSocket* Accept();
};


#endif

