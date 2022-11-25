#ifndef _H_POT_
#define _H_POT_

class CPlayer;
class CDealer;

class CPot {
private:
	// 各プレイヤーのベット額
	vector<int> players_bet_;
	// 前回のベッティングラウンドまでのベット額
	int previous_bet_;
	// 現在のベット額
	int current_bet_;
	// ベット総額
	int total_pot_;
	// ディーラー
	CDealer dealer_;
	// アンティ
	int ante_;
public:
	int current_bet() { return current_bet_; };
	int total_pot() { return total_pot_; };
	// コンストラクタ
	CPot() : CPot(100) {};
	CPot(int ante);
	// 初期化
	void initialize();
	// アンティ回収
	void betAnte(vector<CPlayer>& players, int& is_allin, int& is_fold);
	// チェック・コール
	bool call(CPlayer& player);
	// ベット・レイズ
	bool raise(CPlayer& player, int new_bet);
	// オールイン
	bool allIn(CPlayer& player);
	// ベッティングラウンド
	void bettingRound(vector<CPlayer>& players, int& is_allin, int& is_fold);
};


#endif //  _H_POT_

