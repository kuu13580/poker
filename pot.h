#ifndef _H_POT_
#define _H_POT_

class CPlayer;
class CDealer;

class CPot {
private:
	// �e�v���C���[�̃x�b�g�z
	vector<int> players_bet_;
	// �O��̃x�b�e�B���O���E���h�܂ł̃x�b�g�z
	int previous_bet_;
	// ���݂̃x�b�g�z
	int current_bet_;
	// �x�b�g���z
	int total_pot_;
	// �f�B�[���[
	CDealer dealer_;
	// �A���e�B
	int ante_;
public:
	int current_bet() { return current_bet_; };
	int total_pot() { return total_pot_; };
	// �R���X�g���N�^
	CPot() : CPot(100) {};
	CPot(int ante);
	// ������
	void initialize();
	// �A���e�B���
	void betAnte(vector<CPlayer>& players, int& is_allin, int& is_fold);
	// �`�F�b�N�E�R�[��
	bool call(CPlayer& player);
	// �x�b�g�E���C�Y
	bool raise(CPlayer& player, int new_bet);
	// �I�[���C��
	bool allIn(CPlayer& player);
	// �x�b�e�B���O���E���h
	void bettingRound(vector<CPlayer>& players, int& is_allin, int& is_fold);
};


#endif //  _H_POT_

