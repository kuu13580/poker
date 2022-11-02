#ifndef _H_GAME_
#define _H_GAME_

class CDeck;
class CPlayer;
class CDealer;

class CGame {
private:
	// �R�D
	CDeck deck_;
	// �v���C���[
	vector<CPlayer> players_;
	// �f�B�[���[
	CDealer dealer_;
	// �f�B�[���[�{�^��(�e)
	int dealer_btn_;
	// new
public:
	//�R���X�g���N�^
	CGame();
	// 1���E���h���J�n����
	void startRound();
	// ���E���h�Ɋւ��鏉����
	void initialize();
	// �x�b�e�B���O���E���h
	void bettingRound();
	// �h���[���E���h
	void drawRound();
};


#endif //  _H_GAME_