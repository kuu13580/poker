#ifndef _H_GAME_
#define _H_GAME_

class CServer;
class CClient;
class CDeck;
class CPlayer;
class CPot;

class CGame {
protected:
	// �o�C�i���Ńt�H�[���h���
	int is_fold_;
	// �o�C�i���ŃI�[���C�����
	int is_allin_;
	// �R�D
	CDeck deck_;
	// �v���C���[
	vector<CPlayer> players_;
	// �e�v���C���[�̃x�b�g�z
	vector<int> players_bet_;
	// �f�B�[���[�{�^��(�e)
	int dealer_btn_;
	// �O��̃x�b�e�B���O���E���h�܂ł̃x�b�g�z
	int previous_bet_;
	// ���݂̃x�b�g�z
	int current_bet_;
	// �x�b�g���z
	int total_pot_;
	// �A���e�B
	int ante_;
	// �I�[�v���x�b�g����Ă��邩(�x�b�g�v���C���[no)
	int is_opened_;
	// �I�[�v���x�b�g�̗L����
	bool is_final_;
public:
	vector<CPlayer>* getPlayers() { return &players_; }
	//�R���X�g���N�^
	CGame(vector<string> name, int num_joker = 0, int bankroll = 1000, int ante = 100);
	// �`�F�b�N�E�R�[��
	bool call(CPlayer& player);
	// �x�b�g�E���C�Y
	bool raise(CPlayer& player, int new_bet);
	// �I�[���C��
	bool allIn(CPlayer& player);
};

class CGameServer : public CGame {
private:
	// �ʐM�p�̃N���X
	CServer network;
public:
	using CGame::CGame;
	void setNetwork(CServer& server) { network = server; }
	void drawRound();
	void startRound();
	void bettingRound();
	// ���E���h�Ɋւ��鏉����
	void initialize();
	// �A���e�B����
	void betAnte();
	// �V���[�_�E��
	void showDown();
};

class CGameClient : public CGame {
private:
	// �ʐM�p�̃N���X
	CClient network;
public:
	using CGame::CGame;
	void setNetwork(CClient& client) { network = client; }
	void drawRound();
	void startRound();
	void bettingRound();
	// ���E���h�Ɋւ��鏉����
	void initialize();
	// �A���e�B����
	void betAnte();
	// �V���[�_�E��
	void showDown();
	// �f�[�^�Z�b�g
	void setData(vector<int> query);
};


#endif //  _H_GAME_