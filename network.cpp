#include "common.h"
#include "network.h"

SOCKET CNetwork::makeSocket() {
	WSADATA wsa_data;
	int err = WSAStartup(MAKEWORD(2, 0), &wsa_data);
	if (err != 0) {
		switch (err) {
		case WSASYSNOTREADY:
			cout << "�l�b�g���[�N�ւ̐ڑ�������" << endl;
			break;
		case WSAEINPROGRESS:
			cout << "�u���b�L���O���s��" << endl;
			break;
		case WSAVERNOTSUPPORTED:
			cout << "winsock�̃o�[�W�������T�|�[�g����Ă��܂���" << endl;
			break;
		case WSAEPROCLIM:
			cout << "winsock�̍ő又�����𒴂��܂���" << endl;
			break;
		case WSAEFAULT:
			cout << "���������L���ȃ|�C���^�ł͂���܂���" << endl;
			break;
		}
		system("pause");
		exit(0);
	}
	// �\�P�b�g�쐬
	return socket(AF_INET, SOCK_STREAM, 0);
}


// �f�[�^�ϊ�
vector<int> CNetwork::convertData(string data) {
	// �f�[�^�G���[
	if (data.at(0) == '\0') {
		cout << "�f�[�^�ϊ��G���[" << endl;
		system("pause");
		exit(0);
	}
	vector<int> result;
	char buf[3];
	buf[2] = '\0';
	for (int i = 0; data.begin() + i + 1 < data.end(); i += 2) {
		buf[0] = data.at(i);
		buf[1] = data.at(i + 1);
		if (buf[0] == 'F' && buf[1] == 'F') {
			result.emplace_back(-1);
			continue;
		}
		result.emplace_back(strtol(buf, NULL, 16));
	}
	cout << "recv : ";
	for (int i : result) {
		cout << i << " ";
	}
	cout << endl;
	return result;
}
string CNetwork::convertData(vector<int>& data) {
	// �f�[�^�G���[
	if (data.empty()) {
		cout << "�f�[�^�ϊ��G���[" << endl;
		system("pause");
		exit(0);
	}
	// �������ЂƂЂƂ���(16�i��)
	cout << "send : ";
	string output;
	for (int i : data) {
		cout << i << " ";
		stringstream ss;
		if (i == -1) {
			ss << "ff";
		}
		else {
			ss << setw(2) << setfill('0') << hex << i;
		}
		output += ss.str();
	}
	cout << endl;
	return output;
}

Card CNetwork::itoc(int number) {
	Card buf;
	buf.suit = Suit((number - 1) / 14);
	buf.number = number - 14 * buf.suit;
	return buf;
}
int CNetwork::ctoi(Card card) {
	return (int)card.suit * 14 + (int)card.number;
}

void CNetwork::addDivideInt(vector<int>& v, int number) {
	vector<int> result;
	uint32_t mask = 0b11111111 << 24;
	for (int i = 0; i < 4; i++) {
		result.emplace_back(((number)&mask) >> 24);
		number <<= 8;
	}
	v.insert(v.end(), result.begin(), result.end());
}