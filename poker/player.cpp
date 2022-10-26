#include "common.h"
#include "player.h"
#include "deck.h"

int CPlayer::num_players_ = 0;

// コンストラクタ
CPlayer::CPlayer(string name, int bankroll) {
	// プレイヤーの数増加
	num_players_++;
	name_ = name;
	bankroll_ = bankroll;
	//vectorの領域確保
	hand_.reserve(NUM_HANDCARDS);
}

// 手札表示
void CPlayer::show() {
	cout << "＜" << name_ << "の手札＞" << endl;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		cout << marks[hand_.at(i).suit] << numbers[hand_.at(i).number] << " ";
	}
	cout << endl;
}

// 最初のドロー
void CPlayer::draw(CDeck& cards) {
	hand_.emplace_back(cards.draw());
}
void CPlayer::draw(CDeck& cards, int n) {
	if (n < 1) {
		cout << "ドロー回数エラー" << endl;
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		hand_.emplace_back(cards.draw());
	}
}


// 手札交換
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
