#include "common.h"
#include "deck.h"

// コンストラクタ
CDeck::CDeck(int num_jokers)
	: num_deck_cards_(NUM_STD_CARDS + num_jokers), num_jokers_(num_jokers) {
	// vectorの領域確保
	deck_.reserve(num_deck_cards_);
	int buffer = num_deck_cards_ - NUM_HANDCARDS * NUM_PLAYER;
	burncards_.reserve(buffer);

	// カードの初期化
	initialize();
}

// 内容表示
void CDeck::show() {
	cout << "<山札>" << endl;
	for (int i = 0; i < deck_.size(); i++) {
		cout << marks[deck_.at(i).suit] << numbers[deck_.at(i).number] << " ";
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
void CDeck::shuffle() {
	random_device get_rand_dev;
	mt19937 get_rand_mt(get_rand_dev());
	std::shuffle(deck_.begin(), deck_.end(), get_rand_mt);
}

// 1枚ドロー
Card CDeck::draw() {
	if (deck_.empty()) {
		cout << "ERROR : カードドローエラー" << endl;
		system("pause");
		exit(0);
	}
	num_deck_cards_--;
	Card top = deck_.back();
	deck_.pop_back();
	return top;
}

// カード交換
Card CDeck::exchange(Card discard) {
	if (num_deck_cards_ <= 0) {
		cout << "ERROR : カード交換できませんでした" << endl;
		system("pause");
		exit(0);
	}
	burncards_.emplace_back(discard);
	return draw();
}

// 捨て札を戻す
void CDeck::returnBurncards() {
	num_deck_cards_ += (int)burncards_.size();
	copy(burncards_.begin(), burncards_.end(), back_inserter(deck_));
	burncards_.clear();
	shuffle();
}

// カードの初期化
void CDeck::initialize() {
	deck_.clear();
	burncards_.clear();
	num_deck_cards_ = NUM_STD_CARDS + num_jokers_;
	// カードを追加
	Card n_card;
	for (int i = 0; i < 4; i++) {
		for (int j = 2; j <= 14; j++) {
			n_card = { Suit(i) ,j };
			deck_.emplace_back(n_card);
		}
	}
	n_card = { Joker ,0 };
	for (int i = 1; i <= num_jokers_; i++) {
		deck_.emplace_back(n_card);
	}
	shuffle();
}