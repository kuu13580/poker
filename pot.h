#ifndef _H_POT_
#define _H_POT_

class CPlayer;

class CPot {
private:
	// �e�v���C���[�̃x�b�g�z
	vector<int> players_bet_;
	// ���݂̃x�b�g�z
	int current_bet_;
	// �x�b�g���z
	int total_pot_;
public:
	int current_bet() { return current_bet_; };
	int total_pot() { return total_pot_; };
	// �R���X�g���N�^
	CPot();
	// ������
	void initialize();
	// �`�F�b�N�E�R�[��
	bool call(CPlayer& player);
	// �x�b�g�E���C�Y
	bool raise(CPlayer& player);
	// �I�[���C��
	bool allIn(CPlayer& player);
	// �x�b�e�B���O���E���h�I������
	void finishBetting(vector<CPlayer>& players, int is_allin);
};


#endif //  _H_POT_

