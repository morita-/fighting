#include "LibUtil.h"


//==============================================================
// Winsockの初期化と終了

// 使用中のソケット数
static int SocketCount=0;

// Winsockの初期化
static void InitializeWinsock() {
	if (SocketCount==0) {
		WSADATA data;
		WSAStartup(MAKEWORD(2,0), &data);
	}
	SocketCount++;
}

// Winsockの終了
static void FinalizeWinsock() {
	SocketCount--;
	if (SocketCount==0) {
		WSACleanup();
	}
}


//==============================================================
// ソケット基本機能

// コンストラクタ
CTCPBaseSocket::CTCPBaseSocket(SOCKET s)
:	Socket(s)
{
	InitializeWinsock();
	if (Socket==INVALID_SOCKET) {
		Socket=socket(AF_INET, SOCK_STREAM, 0);
	}
}

// デストラクタ
CTCPBaseSocket::~CTCPBaseSocket() {
	closesocket(Socket);
	FinalizeWinsock();
}


//==============================================================
// TCPソケット

// コンストラクタ
CTCPSocket::CTCPSocket(SOCKET s)
:	CTCPBaseSocket(s)
{
	LineSeparator.push_back("\r\n");
	LineSeparator.push_back("\r");
	LineSeparator.push_back("\n");
}

// 接続：
// 成功時にtrueを返す
bool CTCPSocket::Connect(string host, int port) {

	// ホスト名をアドレスに変換
	LPHOSTENT hostent=gethostbyname(host.c_str());
	if (!hostent ||
		hostent->h_addrtype!=AF_INET ||
		hostent->h_length!=4 ||
		!hostent->h_addr_list[0]) return false;

	// アドレスとポートの登録
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_addr.s_addr=*(unsigned long*)(hostent->h_addr_list[0]);
	sa.sin_port=htons(port);

	// 接続の実行
	return connect(Socket, (sockaddr*)&sa, sizeof(sa))==0;
}

// バッファの送信
int CTCPSocket::Send(const char* buffer, int length) {
	return send(Socket, buffer, length, 0);
}

// バッファの受信
int CTCPSocket::Recv(char* buffer, int length) {
	return recv(Socket, buffer, length, 0);
}

// 行セパレータの設定
void CTCPSocket::SetLineSeparator(vector<string> ls) {
	LineSeparator.clear();
	for (size_t i=0, n=ls.size(); i<n; i++) {
		LineSeparator.push_back(ls[i]);
	}
}

// 行の送信
void CTCPSocket::SendLine(string s) {
	if (!LineSeparator.empty()) s+=LineSeparator[0];
	Send(s.c_str(), s.length());
}

// 行の受信
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
// TCPサーバソケット

// コンストラクタ
CTCPServerSocket::CTCPServerSocket()
:	CTCPBaseSocket(INVALID_SOCKET)
{}

// ポート番号の設定：
// 成功時にtrueを返す
bool CTCPServerSocket::Bind(int port) {
	sockaddr_in sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_port=htons(port);
	return bind(Socket, (sockaddr*)&sa, sizeof(sa))==0;
}

// 接続の受け入れ：
// 正常に接続できたときにはCTCPSocketの新しいインスタンスを返す
CTCPSocket* CTCPServerSocket::Accept() {

	// 接続待ち
	if (listen(Socket, SOMAXCONN)!=0) return NULL;

	// 接続の受け入れ
	SOCKET socket=accept(Socket, NULL, NULL);
	if (socket==INVALID_SOCKET) return NULL;

	// CTCPSocketの生成
	return new CTCPSocket(socket);
}


