#ifndef _H_SERVER_
#define _H_SERVER_

class CServer : public CNetwork {
private:
	WSADATA wsa_data_;
	vector<TCPSocket> client_sockets_;
	SOCKET server_socket_;
public:
	// コンストラクタ
	void initServer();
	// デストラクタ
	~CServer();
	// 受信
	vector<string> recvData();
	// 送信
	void sendData(string data);
	void sendData(vector<int>& v) { sendData(convertData(v)); }
	// 別々送信
	void sendDataEach(string data, int client_no);
	void sendDataEach(vector<int>& v, int client_no) { sendDataEach(convertData(v), client_no); }
	// 別々受信
	string recvDataEach(int client_no);
};

#endif // _H_SERVER_