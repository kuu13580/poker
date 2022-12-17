#ifndef _H_SERVER_
#define _H_SERVER_

class CServer : public CNetwork {
private:
	WSADATA wsa_data_;
	vector<TCPSocket> client_sockets_;
	SOCKET server_socket_;
public:
	// �R���X�g���N�^
	void initServer();
	// �f�X�g���N�^
	~CServer();
	// ��M
	vector<string> recvData();
	// ���M
	void sendData(string data);
	void sendData(vector<int>& v) { sendData(convertData(v)); }
	// �ʁX���M
	void sendDataEach(string data, int client_no);
	void sendDataEach(vector<int>& v, int client_no) { sendDataEach(convertData(v), client_no); }
	// �ʁX��M
	string recvDataEach(int client_no);
};

#endif // _H_SERVER_