#include "common.h"
#include "player.h"
#include "deck.h"

int CPlayer::num_players_ = 0;

// コンストラクタ
CPlayer::CPlayer(string name, int bankroll)
	:name_(name), player_no_(num_players_), bankroll_(bankroll), public_cards(0){
	// プレイヤーの数増加
	num_players_++;
	//vectorの領域確保
	hand_.reserve(NUM_HANDCARDS);
}

// 情報表示
void CPlayer::showHand() {
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
		system("pause");
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		hand_.emplace_back(cards.draw());
	}
}


// 手札交換
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
