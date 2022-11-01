#include "common.h"
#include "player.h"
#include "deck.h"

int CPlayer::num_players_ = 0;

// �R���X�g���N�^
CPlayer::CPlayer(string name, int bankroll) {
	// �v���C���[�̐�����
	num_players_++;
	name_ = name;
	bankroll_ = bankroll;
	//vector�̗̈�m��
	hand_.reserve(NUM_HANDCARDS);
}

// ��D�\��
void CPlayer::show() {
	cout << "��" << name_ << "�̎�D��" << endl;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		cout << marks[hand_.at(i).suit] << numbers[hand_.at(i).number] << " ";
	}
	cout << endl;
}

// �ŏ��̃h���[
void CPlayer::draw(CDeck& cards) {
	hand_.emplace_back(cards.draw());
}
void CPlayer::draw(CDeck& cards, int n) {
	if (n < 1) {
		cout << "�h���[�񐔃G���[" << endl;
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		hand_.emplace_back(cards.draw());
	}
}


// ��D����
void CPlayer::exchangeHand(int n, vector<int> selected, CDeck& cards) {
	if (cards.num_deck_cards() < n) {
		cards.returnBurncards();
	}
	Card new_card;
	for (int i = 0; i < n; i++) {
		new_card = cards.exchange(hand_.at(selected[i]));
		hand_.erase(hand_.begin() + selected[i]);
		hand_.insert(hand_.begin() + selected[i], new_card);
	}
	sortHand();
}
