#include "common.h"
#include "network.h"
#include "server.h"

void CServer::initServer() {
	// ソケット作成
	server_socket_ = makeSocket();
	struct sockaddr_in addr;
	if (server_socket_ == INVALID_SOCKET) {
		cout << "socket ERROR : " << WSAGetLastError() << endl;
		exit(0);
	}
	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(25565);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(server_socket_, (struct sockaddr*)&addr, sizeof(addr));
	// 接続
	client_sockets_.resize(NUM_PLAYER);
	if (listen(server_socket_, 5) != 0) {
		cout << "listen ERROR : " << WSAGetLastError() << endl;
		system("pause");
		exit(0);
	}
	string tmp_name[NUM_PLAYER];
#pragma omp parallel for
	for (int client_no = 0; client_no < NUM_PLAYER; client_no++) {
		int len = sizeof(client_sockets_.at(client_no).addr);
		client_sockets_.at(client_no).socket = accept(server_socket_, (struct sockaddr*)&client_sockets_.at(client_no).addr, &len);
		stringstream ss;
		ss << client_no;
		send(client_sockets_.at(client_no).socket, ss.str().c_str(), sizeof(ss.str().c_str()), 0);
		//cout << client_no << "の接続確認" << endl;
	}
	cout << "すべてのクライアントの接続完了" << endl;
}

CServer::~CServer() {
	for (int i = 0; i < NUM_PLAYER; i++) {
		closesocket(client_sockets_.at(i).socket);
	}
	WSACleanup();
}

vector<string> CServer::recvData() {
	// クライアントからデータ受信
	vector<string> data(NUM_PLAYER);
#pragma omp parallel for
	for (int client_no = 0; client_no < NUM_PLAYER; client_no++) {
		char buf[DATA_MAX_SIZE];
		memset(buf, 0, sizeof(buf));
		recv(client_sockets_.at(client_no).socket, buf, sizeof(buf), 0);
		data.at(client_no) = buf;
		stringstream debug;
		debug << "  recv from " << client_no << " : " << buf << endl;
		cout << debug.str();
	}
	return data;
}

string CServer::recvDataEach(int client_no) {
	char buf[DATA_MAX_SIZE];
	memset(buf, 0, sizeof(buf));
	recv(client_sockets_.at(client_no).socket, buf, sizeof(buf), 0);
	stringstream debug;
	debug << "  recv from " << client_no << " : " << buf << endl;
	cout << debug.str();
	return buf;
}

void CServer::sendData(string data) {
	// すべてのクライアントにデータ送信
	cout << "  send：" << data << endl;
#pragma omp parallel for
	for (int i = 0; i < NUM_PLAYER; i++) {
		send(client_sockets_.at(i).socket, data.c_str(), data.length(), 0);
	}
}

void CServer::sendDataEach(string data, int client_no) {
	stringstream debug;
	debug << "  send to " << client_no << " : " << data << endl;
	cout << debug.str();
	send(client_sockets_.at(client_no).socket, data.c_str(), data.length(), 0);
}
