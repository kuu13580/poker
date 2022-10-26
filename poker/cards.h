#ifndef _H_CARDS_
#define _H_CARDS_

class CCards {
private:
	vector<Card> cards_;
	// 捨てられたカード
	vector<Card> burncards_;
	// カードの総数
	int num_cards_;
public:
	int num_cards() { return num_cards_; };
	// コンストラクタ
	CCards(int num_jokers_);
	// 表示
	void show();
	// シャッフル
	void shuffle();
	// 1枚ドロー
	Card draw();
	// カード交換
	Card exchange(Card discard);
	// 捨て札を戻す
	void returnCards();
};

#endif // _H_CARDS_
