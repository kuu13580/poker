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
public:
	//�R���X�g���N�^
	CGame();
	// 1���E���h���J�n����
	void startRound();
	// ���E���h�Ɋւ��鏉����
	void initialize();
};


#endif //  _H_GAME_