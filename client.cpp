#include "common.h"
#include "network.h"
#include "client.h"

void CClient::initClient(char* hostname) {
	self_client_no = 0;
	// ソケット作成
	client_socket_ = makeSocket();
	// サーバーソケットの設定
	struct addrinfo hints, * res;
	struct in_addr addr;
	int error;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	if ((error = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
		cout << "ERROR: ホスト名解決できませんでした" << endl;
		system("pause");
		exit(0);
	}
	addr.S_un = ((struct sockaddr_in*)(res->ai_addr))->sin_addr.S_un;
	char ip[20];
	inet_ntop(AF_INET, &addr, ip, sizeof(ip));
	server_addr_.sin_family = AF_INET;
	server_addr_.sin_port = htons(25565);
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
	cout << "send : " << data << endl;
}