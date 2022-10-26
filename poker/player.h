#ifndef _H_PLAYER_
#define _H_PLAYER_

class CCards;

class CPlayer {
private:
	static int num_players_;
	string name_;
public:
	// 手札
	vector<Card> hand_;
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
	void exchangeHand(int n, vector<int> selected, CCards& cards);
};


#endif //  _H_PLAYER_

