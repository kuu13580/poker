#include "common.h"
#include "player.h"
#include "pot.h"

// コンストラクタ
CPot::CPot() {
	initialize();
}

// 初期化
void CPot::initialize() {
	current_bet_ = 0;
	total_pot_ = 0;
	players_bet_ = vector<int>(NUM_PLAYER);
}

// チェック・コール
bool CPot::call(CPlayer& player) {
	if (player.bankroll() < current_bet_) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = current_bet_;
	// 総額計算
	total_pot_ = accumulate(players_bet_.begin(), players_bet_.end(), 0);
	return true;
}

// ベット・レイズ
bool CPot::raise(CPlayer& player) {
	// ベット額を尋ねる
	int new_bet = 0;
	cout << "ベット額 : ";
	cin >> new_bet;
	if (player.bankroll() < new_bet || new_bet < current_bet_) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = new_bet;
	current_bet_ = new_bet;
	// 総額計算
	total_pot_ = accumulate(players_bet_.begin(), players_bet_.end(), 0);
	return true;
}

// オールイン
bool CPot::allIn(CPlayer& player) {
	if (player.bankroll() <= 0) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = player.bankroll();
	// 総額計算
	total_pot_ = accumulate(players_bet_.begin(), players_bet_.end(), 0);
	return false;
}

// ベッティングラウンド終了処理
void CPot::finishBetting(vector<CPlayer>& players, int is_allin) {
    // プレイヤーのバンクロールを処理
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		players.at(player_no).payout(players_bet_.at(player_no));
	}
	//ベットを初期化
	for (int i = 0; i < players_bet_.size(); i++) {
		players_bet_.at(i) = 0;
	}
	current_bet_ = 0;
	cout << "現在の総額：" << total_pot_ << endl;

}