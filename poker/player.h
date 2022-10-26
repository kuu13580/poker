#ifndef _H_PLAYER_
#define _H_PLAYER_

class CDeck;

class CPlayer {
private:
	static int num_players_;
	string name_;
	// 手札
	vector<Card> hand_;
public:
	vector<Card> hand() { return hand_; };
	// コンストラクタ
	CPlayer();
	CPlayer(string name);
	// 手札表示
	void show();
	// ドロー
	void draw(CDeck& cards);
	// n回ドロー
	void draw(CDeck& cards, int n);
	// 手札ソート
	void sortHand();
	// 手札交換
	void exchangeHand(int n, vector<int> selected, CDeck& cards);
};


#endif //  _H_PLAYER_

