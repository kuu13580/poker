#ifndef _H_CARDS_
#define _H_CARDS_

class CCards {
private:
	vector<Card> m_cards;
	// 捨てられたカード
	vector<Card> m_burncard;
public:
	// カードの総数
	int m_numCards;
	// コンストラクタ
	CCards(int numJoker);
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
