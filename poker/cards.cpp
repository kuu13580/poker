#include "common.h"
#include "cards.h"

// コンストラクタ
CCards::CCards(int num_jokers_) : num_cards_(0) {
	num_cards_ = NUM_CARDS + num_jokers_;
	// カードの初期化
	Card n_card;
	for (int i = 0; i < 4; i++) {
		for (int j = 2; j <= 14; j++) {
			n_card = { Suit(i) ,j };
			cards_.emplace_back(n_card);
		}
	}
	n_card = { Joker ,0 };
	for (int i = 1; i <= num_jokers_; i++) {
		cards_.emplace_back(n_card);
	}
	shuffle();
}

// 内容表示
void CCards::show() {
	cout << "<山札>" << endl;
	for (int i = 0; i < cards_.size(); i++) {
		cout << marks[cards_.at(i).suit] << numbers[cards_.at(i).number] << " ";
	}
	cout << endl;
	// デバッグ用
	cout << "<捨て札>" << endl;
	for (int i = 0; i < burncards_.size(); i++) {
		cout << marks[burncards_.at(i).suit] << numbers[burncards_.at(i).number] << " ";
	}
	cout << endl;
}

// シャッフル
void CCards::shuffle() {
	random_device get_rand_dev;
	mt19937 get_rand_mt(get_rand_dev());
	std::shuffle(cards_.begin(), cards_.end(), get_rand_mt);
}

// 1枚ドロー
Card CCards::draw() {
	if (cards_.empty()) {
		cout << "ERROR : カードドローエラー" << endl;
		exit(0);
	}
	num_cards_--;
	Card top = cards_.back();
	cards_.pop_back();
	return top;
}

// カード交換
Card CCards::exchange(Card discard) {
	if (num_cards_ < 0) {
		cout << "ERROR : カード交換できませんでした" << endl;
		exit(0);
	}
	burncards_.emplace_back(discard);
	return draw();
}

// 捨て札を戻す
void CCards::returnCards() {
	if (!cards_.empty()) {
		cout << "ERROR : 捨て札を戻せませんでした" << endl;
		exit(0);
	}
	copy(burncards_.begin(), burncards_.end(), back_inserter(cards_));
	burncards_.clear();
	shuffle();
}