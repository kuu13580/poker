#ifndef _H_DECK_
#define _H_DECK_

class CDeck {
private:
	vector<Card> deck_;
	// 捨てられたカード
	vector<Card> burncards_;
	// カードの総数
	int num_deck_cards_;
public:
	int num_deck_cards() { return num_deck_cards_; };
	// コンストラクタ
	CDeck(int num_jokers_);
	// 表示
	void show();
	// シャッフル
	void shuffle();
	// 1枚ドロー
	Card draw();
	// カード交換
	Card exchange(Card discard);
	// 捨て札を戻す
	void returnBurncards();
};

#endif // _H_DECK_
