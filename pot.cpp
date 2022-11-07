#include "common.h"
#include "player.h"
#include "pot.h"

// �R���X�g���N�^
CPot::CPot() {
	initialize();
}

// ������
void CPot::initialize() {
	current_bet_ = 0;
	total_bet_ = 0;
	players_bet_ = vector<int>(NUM_PLAYER);
}

// �`�F�b�N�E�R�[��
bool CPot::call(CPlayer player) {
	if (player.bankroll() < current_bet_) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = current_bet_;
	return true;
}

// �x�b�g�E���C�Y
bool CPot::raise(CPlayer player) {
	// �x�b�g�z��q�˂�
	int new_bet = 0;
	cout << "�x�b�g�z : ";
	cin >> new_bet;
	if (player.bankroll() < new_bet || new_bet < current_bet_) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = new_bet;
	current_bet_ = new_bet;
	return true;
}

// �I�[���C��
bool CPot::allIn(CPlayer player) {
	if (player.bankroll() <= 0) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = player.bankroll();
	return false;
}

// �x�b�e�B���O���E���h�I������
void CPot::finishBetting() {
	total_bet_ = accumulate(players_bet_.begin(), players_bet_.end(), 0);
	for (int i = 0; i < players_bet_.size(); i++) {
		players_bet_.at(i) = 0;
	}
	current_bet_ = 0;
	cout << "���݂̑��z�F" << total_bet_ << endl;

}