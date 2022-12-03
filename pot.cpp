#include "common.h"
#include "dealer.h"
#include "player.h"
#include "pot.h"

// �R���X�g���N�^
CPot::CPot(int ante) {
	ante_ = ante;
	initialize();
}

// ������
void CPot::initialize() {
	dealer_ = CDealer::CDealer();
	current_bet_ = 0;
	total_pot_ = 0;
	previous_bet_ = 0;
	is_final_ = false;
	is_opened = -1;
	players_bet_ = vector<int>(NUM_PLAYER, 0);
}

void CPot::betAnte(vector<CPlayer>& players, int& is_allin, int& is_fold) {
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if (players.at(player_no).bankroll() == 0) {
			is_fold |= 1 << player_no;
		}
		else if (players.at(player_no).bankroll() < ante_) {
			allIn(players.at(player_no));
			is_allin |= 1 << player_no;
		}
		else {
			raise(players.at(player_no), ante_);
		}
	}
}

// �`�F�b�N�E�R�[��
bool CPot::call(CPlayer& player) {
	if (player.bankroll() < current_bet_) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = current_bet_;
	return true;
}

// �x�b�g�E���C�Y
bool CPot::raise(CPlayer& player, int new_bet) {
	if (player.bankroll() < new_bet || new_bet < current_bet_) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = new_bet;
	current_bet_ = new_bet;
	return true;
}

// �I�[���C��
bool CPot::allIn(CPlayer& player) {
	if (player.bankroll() <= 0) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = player.bankroll();
	current_bet_ = player.bankroll();
	return true;
}

// �x�b�e�B���O���E���h
void CPot::bettingRound(vector<CPlayer>& players, int& is_allin, int& is_fold, int dealer_btn) {
	int current_player_no = dealer_btn;
	// i�̓x�b�g�E���C�Y�����v���C���[����̂���
	for (int i = 0; i < NUM_PLAYER;) {
		CPlayer& current_player = players.at(current_player_no);
		// �t�H�[���hor�I�[���C�����Ă���
		if ((is_fold & (1 << current_player_no)) || (is_allin & (1 << current_player_no))) {
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			continue;
		}
		// ��ʕ\��
		system("cls");
		cout << "<<< �x�b�e�B���O���E���h >>>" << endl;
		cout << "�|�b�g���z    " << total_pot_ + accumulate(players_bet_.begin(), players_bet_.end(), 0) << endl;
		cout << "���݂̃x�b�g�z�F" << current_bet_ << endl << endl;
		cout << "���e�v���C���[�̏��" << endl;
		cout << right;
		for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
			cout << (player_no == current_player_no ? "��" : " ") << players.at(player_no).name() << "   �c���F" << setw(6) << players.at(player_no).bankroll() << "   �x�b�g�F" << setw(6) << players_bet_.at(player_no);
			if (is_fold & (1 << player_no)) cout << "   fold  ";
			if (is_allin & (1 << player_no)) cout << "   allin  ";
			if (players.at(player_no).public_cards.size() != 0) {
				cout << "   open  ";
				for (int selected : players.at(player_no).public_cards) {
					cout << marks[players.at(player_no).getHand().at(selected).suit] << numbers[players.at(player_no).getHand().at(selected).number] << " ";
				}
			}
			cout << endl;
		}
		cout << noshowpos;
		cout << "=======================================================" << endl;
		cout << current_player.name() << "�̃^�[��   �c�� : " << current_player.bankroll() << endl << endl;
		cout << "��D�F";
		current_player.showHand();
		dealer_.viewHand(current_player.getHand());
		// �v���C���[�s���̑I��
		if (0 <= is_opened) {
			cout << "�`�F�b�N�E�R�[��(0) �t�H�[���h(2) �I�[���C��(3) : ";
		}
		else if (is_final_) {
			cout << "�`�F�b�N�E�R�[��(0) �x�b�g�E���C�Y(1) �t�H�[���h(2) �I�[���C��(3) �I�[�v���x�b�g(4) : ";
		}
		else {
			cout << "�`�F�b�N�E�R�[��(0) �x�b�g�E���C�Y(1) �t�H�[���h(2) �I�[���C��(3) : ";
		}
		int action = 0;
		cin >> action;

		if (action == 0) { // �R�[���E�`�F�b�N
			if (!call(current_player)) continue;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
		}
		else if (action == 1 && 0 > is_opened) { // ���C�Y
			int new_bet = 0;
			cout << "�x�b�g�z : ";
			cin >> new_bet;
			if (!raise(current_player, new_bet)) continue;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
		}
		else if (action == 2) { // �t�H�[���h
			is_fold |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		else if (action == 3) { // �I�[���C��
			if (!allIn(current_player)) continue;
			is_allin |= 1 << current_player_no;
			// �I�[���C���z�ɏ���𐧌�
			for (int i = 0; i < NUM_PLAYER; i++) {
				if (current_bet_ < players_bet_.at(i)) {
					players_bet_.at(i) = current_bet_;
				}
			}
			i = 1;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		else if (action == 4 && is_final_) { // �I�[�v���x�b�g(2�{�Ńx�b�g)
			// �J�������D
			cout << "�J�������D��3���I��ł��������F";
			current_player.public_cards.resize(3);
			cin >> current_player.public_cards.at(0) >> current_player.public_cards.at(1) >> current_player.public_cards.at(2);
			int new_bet = 0;
			do {
				cout << "�x�b�g�z : ";
				cin >> new_bet;
			} while (!raise(current_player, new_bet));
			current_bet_ = new_bet * 2;
			players_bet_.at(current_player_no) = current_bet_;
			is_opened = current_player_no;
			i = 1;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
	}
	// �I������
	// �I�[�v���x�b�g�̃x�b�g�𔼊z�Ɍ��炷
	if (is_opened >= 0) players_bet_.at(is_opened) /= 2;
	// �v���C���[�̃o���N���[��������
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		players.at(player_no).payout(players_bet_.at(player_no));
	}
	total_pot_ += accumulate(players_bet_.begin(), players_bet_.end(), 0);
	//�x�b�g��������
	for (int i = 0; i < players_bet_.size(); i++) {
		players_bet_.at(i) = 0;
	}
	current_bet_ = 0;
}