#ifndef _H_PLAYER_
#define _H_PLAYER_

class CCards;

class CPlayer {
private:
	static int m_numPlayer;
	string m_name;
	vector<Card> m_hand;
public:
	// コンストラクタ
	CPlayer();
	CPlayer(string name);
	// 手札表示
	void show();
	// ドロー
	void draw(CCards& cards);
	// n回ドロー
	void draw(CCards& cards, int n);
	// 手札ソート
	void sortHand();
	// 手札交換
	void exchangeHand(int n, int* selected, CCards& cards);
};


#endif //  _H_PLAYER_

