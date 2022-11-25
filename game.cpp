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
	pot_ = CPot::CPot(ante);
	dealer_btn_ = 0;
	// �������
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(1000));
	}
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

//
void CGame::startRound() {
	initialize();
	pot_.betAnte(players_, is_allin_, is_fold_);
	system("cls");
	pot_.bettingRound(players_, is_allin_, is_fold_);
	drawRound();
	pot_.bettingRound(players_, is_allin_, is_fold_);
	drawRound();
	pot_.bettingRound(players_, is_allin_, is_fold_);
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
		player.draw(deck_, NUM_HANDCARDS);
		player.sortHand();
	}
	// �e���Ԃ̏�����
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// �h���[���E���h
void CGame::drawRound() {
	int n = 0;
	for (int i = 0; i < NUM_PLAYER; i++) {
		if ((is_fold_ & (1 << i)) != 0) { // �t�H�[���h���Ă������΂�
			continue;
		}
		// ��ʕ\��
		system("cls");
		cout << "<<< �h���[���E���h >>>" << endl << endl;
		cout << players_.at(i).name() << "�̃^�[��" << endl;
		players_.at(i).showHand();
		dealer_.viewHand(players_.at(i).getHand());
		cout << "���������D�̐� : ";
		cin >> n;
		// �����ȓ���
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
		dealer_.viewHand(players_.at(i).getHand());
		cin.get();
		system("cls");
	}
}

// �V���[�_�E��
void CGame::showDown() {
	// �v���C���[�̋����̔z��擾
	vector<vector<int>> players_power;
	vector<int> fold = { -1 };
	for (int i = 0; i < NUM_PLAYER; i++) {
		if ((is_fold_ & (1 << i)) != 0) {
			players_power.emplace_back(fold);
			continue;
		}
		players_power.emplace_back(dealer_.checkHand(players_.at(i).getHand()));
	}


	// ���҂�����
	vector<int> winner = { 0 };
	for (int i = 1; i < NUM_PLAYER; i++) {
		if (players_power.at(winner.at(0)).front() < players_power.at(i).front()) { // ���̂ق�������
			winner.clear();
			winner.emplace_back(i);
		}
		else if (players_power.at(winner.at(0)).front() == players_power.at(i).front()) { // �����ꏏ
			bool chop = true;
			for (int j = 0; j < players_power.at(winner.at(0)).size(); j++) {
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

	// �܋��̌v�Z
	int prize = pot_.total_pot() / winner.size();
	for (int i : winner) {
		players_.at(i).payout(-prize);
	}

	// ���ʂ̕\��
	system("cls");
	cout << "<<< �I�� >>>" << endl;
	cout << "�|�b�g���v�F " << pot_.total_pot() << endl << endl;
	for (int i = 0; i < NUM_PLAYER; i++) {
		CPlayer player = players_.at(i);
		if ((is_fold_ & (1 << i)) != 0) { // �t�H�[���h
			cout << "fold ";
		}
		else if(find(winner.begin(), winner.end(), i) != winner.end()){ //����
			cout << " win ";
		}
		else {
			cout << "     ";
		}
		cout << "<" << player.name() << "> �c���F" << player.bankroll() << endl;
		cout << "��D�F ";
		player.showHand();
		cout << "���F " << hands[players_power.at(i).front()] << endl << endl;
	}
	int wait = cin.get();
	wait = cin.get();
}
