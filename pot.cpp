#include "common.h"
#include "dealer.h"
#include "player.h"
#include "pot.h"

// コンストラクタ
CPot::CPot(int ante) {
	ante_ = ante;
	initialize();
}

// 初期化
void CPot::initialize() {
	dealer_ = CDealer::CDealer();
	current_bet_ = 0;
	total_pot_ = 0;
	previous_bet_ = 0;
	is_final_ = false;
	is_opened = -1;
	players_bet_ = vector<int>(NUM_PLAYER, 0);
}

void CPot::betAnte(vector<CPlayer>& players, int& is_allin, int& is_fold) {
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if (players.at(player_no).bankroll() == 0) {
			is_fold |= 1 << player_no;
		}
		else if (players.at(player_no).bankroll() < ante_) {
			allIn(players.at(player_no));
			is_allin |= 1 << player_no;
		}
		else {
			raise(players.at(player_no), ante_);
		}
	}
}

// チェック・コール
bool CPot::call(CPlayer& player) {
	if (player.bankroll() < current_bet_) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = current_bet_;
	return true;
}

// ベット・レイズ
bool CPot::raise(CPlayer& player, int new_bet) {
	if (player.bankroll() < new_bet || new_bet < current_bet_) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = new_bet;
	current_bet_ = new_bet;
	return true;
}

// オールイン
bool CPot::allIn(CPlayer& player) {
	if (player.bankroll() <= 0) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = player.bankroll();
	current_bet_ = player.bankroll();
	return true;
}

// ベッティングラウンド
void CPot::bettingRound(vector<CPlayer>& players, int& is_allin, int& is_fold, int dealer_btn) {
	int current_player_no = dealer_btn;
	// iはベット・レイズしたプレイヤーからのずれ
	for (int i = 0; i < NUM_PLAYER;) {
		CPlayer& current_player = players.at(current_player_no);
		// フォールドorオールインしている
		if ((is_fold & (1 << current_player_no)) || (is_allin & (1 << current_player_no))) {
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			continue;
		}
		// 画面表示
		system("cls");
		cout << "<<< ベッティングラウンド >>>" << endl;
		cout << "ポット総額    " << total_pot_ + accumulate(players_bet_.begin(), players_bet_.end(), 0) << endl;
		cout << "現在のベット額：" << current_bet_ << endl << endl;
		cout << "＜各プレイヤーの情報＞" << endl;
		cout << right;
		for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
			cout << (player_no == current_player_no ? "●" : " ") << players.at(player_no).name() << "   残金：" << setw(6) << players.at(player_no).bankroll() << "   ベット：" << setw(6) << players_bet_.at(player_no);
			if (is_fold & (1 << player_no)) cout << "   fold  ";
			if (is_allin & (1 << player_no)) cout << "   allin  ";
			if (players.at(player_no).public_cards.size() != 0) {
				cout << "   open  ";
				for (int selected : players.at(player_no).public_cards) {
					cout << marks[players.at(player_no).getHand().at(selected).suit] << numbers[players.at(player_no).getHand().at(selected).number] << " ";
				}
			}
			cout << endl;
		}
		cout << noshowpos;
		cout << "=======================================================" << endl;
		cout << current_player.name() << "のターン   残金 : " << current_player.bankroll() << endl << endl;
		cout << "手札：";
		current_player.showHand();
		dealer_.viewHand(current_player.getHand());
		// プレイヤー行動の選択
		if (0 <= is_opened) {
			cout << "チェック・コール(0) フォールド(2) オールイン(3) : ";
		}
		else if (is_final_) {
			cout << "チェック・コール(0) ベット・レイズ(1) フォールド(2) オールイン(3) オープンベット(4) : ";
		}
		else {
			cout << "チェック・コール(0) ベット・レイズ(1) フォールド(2) オールイン(3) : ";
		}
		int action = 0;
		cin >> action;

		if (action == 0) { // コール・チェック
			if (!call(current_player)) continue;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
		}
		else if (action == 1 && 0 > is_opened) { // レイズ
			int new_bet = 0;
			cout << "ベット額 : ";
			cin >> new_bet;
			if (!raise(current_player, new_bet)) continue;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
		}
		else if (action == 2) { // フォールド
			is_fold |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		else if (action == 3) { // オールイン
			if (!allIn(current_player)) continue;
			is_allin |= 1 << current_player_no;
			// オールイン額に上限を制限
			for (int i = 0; i < NUM_PLAYER; i++) {
				if (current_bet_ < players_bet_.at(i)) {
					players_bet_.at(i) = current_bet_;
				}
			}
			i = 1;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		else if (action == 4 && is_final_) { // オープンベット(2倍でベット)
			// 開示する手札
			cout << "開示する手札を3枚選んでください：";
			current_player.public_cards.resize(3);
			cin >> current_player.public_cards.at(0) >> current_player.public_cards.at(1) >> current_player.public_cards.at(2);
			int new_bet = 0;
			do {
				cout << "ベット額 : ";
				cin >> new_bet;
			} while (!raise(current_player, new_bet));
			current_bet_ = new_bet * 2;
			players_bet_.at(current_player_no) = current_bet_;
			is_opened = current_player_no;
			i = 1;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
	}
	// 終了処理
	// オープンベットのベットを半額に減らす
	if (is_opened >= 0) players_bet_.at(is_opened) /= 2;
	// プレイヤーのバンクロールを処理
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		players.at(player_no).payout(players_bet_.at(player_no));
	}
	total_pot_ += accumulate(players_bet_.begin(), players_bet_.end(), 0);
	//ベットを初期化
	for (int i = 0; i < players_bet_.size(); i++) {
		players_bet_.at(i) = 0;
	}
	current_bet_ = 0;
}