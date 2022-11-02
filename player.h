#ifndef _H_PLAYER_
#define _H_PLAYER_
class CDeck;

class CPlayer {
private:
	static int num_players_;
	string name_;
	// 手札
	vector<Card> hand_;
	// 持ち金
	int bankroll_;
public:
	vector<Card> hand() { return hand_; };
	int bankroll() { return bankroll_; };
	// コンストラクタ
	CPlayer(int bankroll) : CPlayer("プレイヤー" + to_string(num_players_ + 1), bankroll) {};
	CPlayer(string name, int bankroll);
	// 手札表示
	void show();
	// ドロー
	void draw(CDeck& cards);
	// n回ドロー
	void draw(CDeck& cards, int n);
	// 手札ソート
	void sortHand() { sort(hand_.begin(), hand_.end()); };
	// 手札交換
	void exchangeHand(int n, vector<int> selected, CDeck& cards);
	// 手札削除
	void clearHand() { hand_.clear(); };
};


#endif //  _H_PLAYER_

