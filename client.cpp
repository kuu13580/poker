#include "common.h"
#include "network.h"
#include "client.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

void CClient::initClient() {
	self_client_no = 0;
	// ソケット作成
	client_socket_ = makeSocket();
	// サーバーソケットの設定
	server_addr_.sin_family = AF_INET;
	server_addr_.sin_port = htons(25565);
	char ip[20] = "127.0.0.1";
	inet_pton(server_addr_.sin_family, ip, &server_addr_.sin_addr.S_un.S_addr);
	// サーバーに接続
	connect(client_socket_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));
}

CClient::~CClient() {
	WSACleanup();
}

// サーバーからデータ受信
string CClient::recvData() {
	char buf[DATA_MAX_SIZE];
	memset(buf, 0, sizeof(buf));
	if (recv(client_socket_, buf, sizeof(buf), 0) == -1) {
		cout << "recv ERROR :" << WSAGetLastError() << endl;
		system("pause");
		exit(0);
	}
	/*cout << "recv : " << buf << endl;
	cout << endl;*/
	return buf;
}
vector<int> CClient::recvConvertedData() {
	return convertData(recvData());
}


// サーバーにデータ送信
void CClient::sendData(string data) {
	if (send(client_socket_, data.c_str(), data.length(), 0) == -1) {
		cout << "send ERROR :" << WSAGetLastError() << endl;
		system("pause");
		exit(0);
	};
	//cout << "send : " << data << endl;
}