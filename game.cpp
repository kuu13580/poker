#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "pot.h"
#include "game.h"


// �R���X�g���N�^
CGame::CGame(int num_joker, int ante) {
	deck_ = CDeck::CDeck(num_joker);
	dealer_ = CDealer::CDealer();
	pot_ = CPot::CPot();
	dealer_btn_ = 0;
	// �������
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(1000));
	}
	is_fold_ = 0b0;
	ante_ = ante;
	is_allin_ = 0b0;
}

//
void CGame::startRound() {
	initialize();
	// �Q����
	refleshWindow();
	bettingRound();
	drawRound();
	bettingRound();
	drawRound();
	bettingRound();
	showDown();
	// �e���ڂ�
	dealer_btn_++;
}

// ���E���h�O�̏�����
void CGame::initialize() {
	// �e�N���X������
	deck_.initialize();
	pot_.initialize();
	// �v���C���[�̏�����
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.at(i).clearHand();
		players_.at(i).draw(deck_, NUM_HANDCARDS);
		players_.at(i).sortHand();
	}
	// �e���Ԃ̏�����
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// �x�b�e�B���O���E���h
void CGame::bettingRound() {
	int action = 0;
	int current_player_no = 0;
	CPlayer current_player = players_.at(0);
	for (int i = 0; i < NUM_PLAYER;) {
		cout << "<<< �x�b�e�B���O���E���h >>>" << endl;
		current_player = players_.at(current_player_no);
		if ((is_fold_ & (1 << current_player_no)) != 0) { // �t�H�[���h���Ă���
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			continue;
		}
		cout << current_player.name() << "�̃^�[��   �c���F" << current_player.bankroll() << "   ���݂̃x�b�g�z�F" << pot_.current_bet() << endl;
		cout << "��D�F";
		current_player.showHand();
		dealer_.viewHand(current_player.hand());
		cout << "�`�F�b�N�E�R�[��(0) �x�b�g�E���C�Y(1) �t�H�[���h(2) �I�[���C��(3): ";
		cin >> action;
		if (action == 0) { // �R�[���E�`�F�b�N
			if (!pot_.call(current_player)) {
				continue;
			}
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
		}
		else if (action == 1) { // ���C�Y
			if (!pot_.raise(current_player)) {
				continue;
			}
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
		}
		else if (action == 2) { // �t�H�[���h
			is_fold_ |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		else if (action == 3) { // �I�[���C��
			pot_.allIn(current_player);
			is_allin_ |= 1 << current_player_no;
			is_fold_ |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		refleshWindow();
	}

}

// �h���[���E���h
void CGame::drawRound() {
	int n = 0;
	for (int i = 0; i < NUM_PLAYER; i++) {
		cout << "<<< �h���[���E���h >>>" << endl;
		players_.at(i).showHand();
		dealer_.viewHand(players_.at(i).hand());
		cout << "���������D�̐� : ";
		cin >> n;
		if (n < 0 || NUM_HANDCARDS < n) {
			i--;
			continue;
		}
		vector<int> selected(n);
		cout << "���������D : ";
		for (int j = 0; j < n; j++) {
			cin >> selected.at(j);
		}
		players_.at(i).exchangeHand(n, selected, deck_);
		cout << endl << "������:";
		players_.at(i).showHand();
		dealer_.viewHand(players_.at(i).hand());
		refleshWindow();
	}
}

// �V���[�_�E��
void CGame::showDown() {
	// �I�[���C���v���C���[��߂�
	is_fold_ ^= is_allin_;
	// �v���C���[�̋����̔z��擾
	vector<vector<int>> players_power;
	vector<int> fold = {-1};
	for (int i = 0; i < NUM_PLAYER; i++) {
		if ((is_fold_ & (1 << i)) != 0) {
			players_power.emplace_back(fold);
			continue;
		}
		players_power.emplace_back(dealer_.checkHand(players_.at(i).hand()));
	}
	// ���҂�����
	vector<int> winner = { 0 };
	for (int i = 1; i < NUM_PLAYER; i++) {
		if (players_power.at(winner.at(0)).at(0) < players_power.at(i).at(0)) { // ���̂ق�������
			winner.clear();
			winner.emplace_back(i);
		}
		else if (players_power.at(winner.at(0)).at(0) == players_power.at(i).at(0)) { // �����ꏏ
			bool chop = true;
			for(int j = 0; j < players_power.at(winner.at(0)).size(); j++){
				if (players_power.at(winner.at(0)).at(j) < players_power.at(i).at(j)) {
					chop = false;
					winner.clear();
					winner.emplace_back(i);
					break;
				}
				else if (players_power.at(winner.at(0)).at(j) > players_power.at(i).at(j)) {
					chop = false;
					break;
				}
			}
			if (chop) { // �`���b�v�@��������
				winner.emplace_back(i);
			}
		}
	}
	for (auto itr = winner.begin(); itr != winner.end(); itr++) {
		cout << " " << players_.at(*itr).name();
	}
	cout << "������" << endl;
	// �܋��̌v�Z
}