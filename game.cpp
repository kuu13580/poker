#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "pot.h"
#include "game.h"


// �R���X�g���N�^
CGame::CGame(int num_joker, int bankroll, int ante) {
	deck_ = CDeck::CDeck(num_joker);
	dealer_ = CDealer::CDealer();
	pot_ = CPot::CPot(ante);
	dealer_btn_ = 0;
	// �������
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(bankroll));
	}
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// ���E���h�J�n
void CGame::startRound() {
	initialize();
	pot_.betAnte(players_, is_allin_, is_fold_);
	system("cls");
	pot_.bettingRound(players_, is_allin_, is_fold_, dealer_btn_);
	drawRound();
	pot_.bettingRound(players_, is_allin_, is_fold_, dealer_btn_);
	drawRound();
	pot_.is_final_ = true;
	pot_.bettingRound(players_, is_allin_, is_fold_, dealer_btn_);
	showDown();
	// �e���ڂ�
	dealer_btn_++;
	startRound();
}

// ���E���h�O�̏�����
void CGame::initialize() {
	// �e�N���X������
	deck_.initialize();
	pot_.initialize();
	// �v���C���[�̏�����
	for (CPlayer& player : players_) {
		player.clearHand();
		player.public_cards.clear();
		if (player.bankroll() == 0) continue;
		player.draw(deck_, NUM_HANDCARDS);
		player.sortHand();
	}
	// �e���Ԃ̏�����
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// �h���[���E���h
void CGame::drawRound() {
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) { // �t�H�[���h���Ă������΂�
			continue;
		}
		// ��ʕ\��
		system("cls");
		cout << "<<< �h���[���E���h >>>" << endl << endl;
		cout << players_.at(player_no).name() << "�̃^�[��" << endl << endl;
		players_.at(player_no).showHand();
		dealer_.viewHand(players_.at(player_no).getHand());
		cout << "���������D�̐� : ";
		int n;
		cin >> n;
		// �����ȓ���
		if (n < 0 || NUM_HANDCARDS < n) {
			player_no--;
			continue;
		}
		vector<int> selected(n);
		cout << "���������D : ";
		for (int j = 0; j < n; j++) {
			cin >> selected.at(j);
		}
		players_.at(player_no).exchangeHand(n, selected, deck_);
		cout << endl << "������:";
		players_.at(player_no).showHand();
		dealer_.viewHand(players_.at(player_no).getHand());
		cin.get();
		system("cls");
	}
}

// �V���[�_�E��
void CGame::showDown() {
	// �v���C���[�̋����̔z��擾
	vector<vector<int>> players_power;
	vector<int> fold = { -1 };
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) {
			players_power.emplace_back(fold);
			continue;
		}
		players_power.emplace_back(dealer_.checkHand(players_.at(player_no).getHand()));
	}


	// ���҂�����
	vector<int> winner = { 0 };
	for (int player_no = 1; player_no < NUM_PLAYER; player_no++) {
		vector<int> player_power = players_power.at(player_no);
		vector<int> winner_power = players_power.at(winner.at(0));
		if (winner_power.front() < player_power.front()) { // ������苭��
			winner.clear();
			winner.emplace_back(player_no);
		}
		else if (winner_power.front() == player_power.front()) { // �����ꏏ
			bool chop = true;
			for (int i = 0; i < winner_power.size(); i++) {
				if (winner_power.at(i) < player_power.at(i)) {
					chop = false;
					winner.clear();
					winner.emplace_back(player_no);
					break;
				}
				else if (winner_power.at(i) > player_power.at(i)) {
					chop = false;
					break;
				}
			}
			if (chop) { // �`���b�v�@��������
				winner.emplace_back(player_no);
			}
		}
	}

	// �܋��̌v�Z
	int prize = pot_.total_pot() / winner.size();
	for (int i : winner) {
		players_.at(i).payout(-prize);
	}

	// ���ʂ̕\��
	system("cls");
	cout << "<<< �I�� >>>" << endl;
	cout << "�|�b�g���v�F " << pot_.total_pot() << endl << endl;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		CPlayer player = players_.at(player_no);
		if ((is_fold_ & (1 << player_no)) != 0) { // �t�H�[���h
			cout << "fold ";
		}
		else if (find(winner.begin(), winner.end(), player_no) != winner.end()) { //����
			cout << " win ";
		}
		else {
			cout << "     ";
		}
		cout << "<" << player.name() << "> �c���F" << player.bankroll() << endl;
		if ((is_fold_ & (1 << player_no)) != 0) continue;
		cout << "��D�F ";
		player.showHand();
		cout << "���F " << hands[players_power.at(player_no).front()] << endl << endl;
	}
	int wait = cin.get();
	wait = cin.get();
}
