#ifndef _H_CLIENT_
#define _H_CLIENT_

class CClient : public CNetwork{
private:
	WSADATA wsa_data_;
	struct sockaddr_in server_addr_;
	SOCKET client_socket_;
public:
	// ������client_no
	int self_client_no;
	// �R���X�g���N�^
	void initClient(char* hostname);
	// �f�X�g���N�^
	~CClient();
	// ��M
	string recvData();
	vector<int> recvConvertedData();
	// ���M
	void sendData(string data);
	void sendData(vector<int>& v) { sendData(convertData(v)); }
};

#endif // _H_CLIENT_