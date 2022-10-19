#ifndef _H_CARDS_
#define _H_CARDS_

class CCards {
private:
	int m_numCards;
	vector<Card> m_cards;
public:
	// コンストラクタ
	CCards(int numJoker);
	// 表示
	void show();
	// シャッフル
	void shuffle();
	// 1枚ドロー
	Card draw();
};

#endif // _H_CARDS_
