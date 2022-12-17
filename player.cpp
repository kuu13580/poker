#include "common.h"
#include "player.h"
#include "deck.h"

int CPlayer::num_players_ = 0;

// �R���X�g���N�^
CPlayer::CPlayer(string name, int bankroll)
	:name_(name), player_no_(num_players_), bankroll_(bankroll), public_cards(0){
	// �v���C���[�̐�����
	num_players_++;
	//vector�̗̈�m��
	hand_.reserve(NUM_HANDCARDS);
}

// ���\��
void CPlayer::showHand() {
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
		system("pause");
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		hand_.emplace_back(cards.draw());
	}
}


// ��D����
void CPlayer::exchangeHand(int selected, CDeck& cards) {
	vector<int> arr_selected;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		if (selected & (1 << i)) arr_selected.emplace_back(i);
	}
	if (cards.num_deck_cards() < arr_selected.size()) {
		cards.returnBurncards();
	}
	Card new_card;
	for (int i = 0; i < arr_selected.size(); i++) {
		new_card = cards.exchange(hand_.at(arr_selected[i]));
		hand_.erase(hand_.begin() + arr_selected[i]);
		hand_.insert(hand_.begin() + arr_selected[i], new_card);
	}
	sortHand();
}
