#ifndef _H_GAME_
#define _H_GAME_

class CDeck;
class CPlayer;
class CDealer;
class CPot;

class CGame {
private:
	// �o�C�i���Ńt�H�[���h���
	int is_fold_;
	// �o�C�i���ŃI�[���C�����
	int is_allin_;
	// �R�D
	CDeck deck_;
	// �v���C���[
	vector<CPlayer> players_;
	// �|�b�g
	CPot pot_;
	// �f�B�[���[
	CDealer dealer_;
	// �f�B�[���[�{�^��(�e)
	int dealer_btn_;
public:
	//�R���X�g���N�^
	CGame(int num_joker = 0, int bankroll = 1000, int ante = 100);
	// 1���E���h���J�n����
	void startRound();
	// ���E���h�Ɋւ��鏉����
	void initialize();
	// �h���[���E���h
	void drawRound();
	// �V���[�_�E��
	void showDown();
};


#endif //  _H_GAME_