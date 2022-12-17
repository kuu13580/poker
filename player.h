#ifndef _H_PLAYER_
#define _H_PLAYER_
class CDeck;

class CPlayer {
private:
	static int num_players_;
	string name_;
	int player_no_;
	// 手札
	vector<Card> hand_;
	// 持ち金
	int bankroll_;
public:
	vector<Card> getHand() { return hand_; };
	int bankroll() { return bankroll_; };
	string name() { return name_; };
	int player_no() { return player_no_; };
	void setName(string name) { name_ = name; };
	void setBankroll(int num) { bankroll_ = num; }
	void setHand(Card card, int pos) {
		hand_.resize(NUM_HANDCARDS);
		hand_.at(pos).number = card.number;
		hand_.at(pos).suit = card.suit;
	}
	// オープンベットの見せ札(5桁の01フラッグ)
	int public_cards;
	// コンストラクタ
	CPlayer(int bankroll) : CPlayer("プレイヤー" + to_string(num_players_ + 1), bankroll) {};
	CPlayer(string name, int bankroll);
	// 手札表示
	void showHand();
	// ドロー
	void draw(CDeck& cards);
	// n回ドロー
	void draw(CDeck& cards, int n);
	// 手札ソート
	void sortHand() { sort(hand_.begin(), hand_.end()); };
	// 手札交換
	void exchangeHand(int selected, CDeck& cards);
	// 手札削除
	void clearHand() { hand_.clear(); };
	// バンクロール処理
	void payout(int payout) { bankroll_ -= payout; };
};


#endif //  _H_PLAYER_

