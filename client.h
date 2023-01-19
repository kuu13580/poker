#ifndef _H_CLIENT_
#define _H_CLIENT_

class CClient : public CNetwork{
private:
	WSADATA wsa_data_;
	struct sockaddr_in server_addr_;
	SOCKET client_socket_;
public:
	// 自分のclient_no
	int self_client_no;
	// コンストラクタ
	void initClient(char* hostname);
	// デストラクタ
	~CClient();
	// 受信
	string recvData();
	vector<int> recvConvertedData();
	// 送信
	void sendData(string data);
	void sendData(vector<int>& v) { sendData(convertData(v)); }
};

#endif // _H_CLIENT_