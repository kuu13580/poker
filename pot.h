#ifndef _H_POT_
#define _H_POT_

class CPlayer;

class CPot {
private:
	// 各プレイヤーのベット額
	vector<int> players_bet_;
	// 現在のベット額
	int current_bet_;
	// ベット総額
	int total_pot_;
public:
	int current_bet() { return current_bet_; };
	int total_pot() { return total_pot_; };
	// コンストラクタ
	CPot();
	// 初期化
	void initialize();
	// チェック・コール
	bool call(CPlayer& player);
	// ベット・レイズ
	bool raise(CPlayer& player);
	// オールイン
	bool allIn(CPlayer& player);
	// ベッティングラウンド終了処理
	void finishBetting(vector<CPlayer>& players, int is_allin);
};


#endif //  _H_POT_

