#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "pot.h"
#include "game.h"


// コンストラクタ
CGame::CGame(int num_joker, int bankroll, int ante) {
	deck_ = CDeck::CDeck(num_joker);
	dealer_ = CDealer::CDealer();
	pot_ = CPot::CPot(ante);
	dealer_btn_ = 0;
	// 初期作業
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(bankroll));
	}
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// ラウンド開始
void CGame::startRound() {
	initialize();
	pot_.betAnte(players_, is_allin_, is_fold_);
	system("cls");
	pot_.bettingRound(players_, is_allin_, is_fold_, dealer_btn_);
	drawRound();
	pot_.bettingRound(players_, is_allin_, is_fold_, dealer_btn_);
	drawRound();
	pot_.is_final_ = true;
	pot_.bettingRound(players_, is_allin_, is_fold_, dealer_btn_);
	showDown();
	// 親を移す
	dealer_btn_++;
	startRound();
}

// ラウンド前の初期化
void CGame::initialize() {
	// 各クラス初期化
	deck_.initialize();
	pot_.initialize();
	// プレイヤーの初期化
	for (CPlayer& player : players_) {
		player.clearHand();
		player.public_cards.clear();
		if (player.bankroll() == 0) continue;
		player.draw(deck_, NUM_HANDCARDS);
		player.sortHand();
	}
	// 各種状態の初期化
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// ドローラウンド
void CGame::drawRound() {
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) { // フォールドしていたら飛ばす
			continue;
		}
		// 画面表示
		system("cls");
		cout << "<<< ドローラウンド >>>" << endl << endl;
		cout << players_.at(player_no).name() << "のターン" << endl << endl;
		players_.at(player_no).showHand();
		dealer_.viewHand(players_.at(player_no).getHand());
		cout << "交換する手札の数 : ";
		int n;
		cin >> n;
		// 無効な入力
		if (n < 0 || NUM_HANDCARDS < n) {
			player_no--;
			continue;
		}
		vector<int> selected(n);
		cout << "交換する手札 : ";
		for (int j = 0; j < n; j++) {
			cin >> selected.at(j);
		}
		players_.at(player_no).exchangeHand(n, selected, deck_);
		cout << endl << "交換後:";
		players_.at(player_no).showHand();
		dealer_.viewHand(players_.at(player_no).getHand());
		cin.get();
		system("cls");
	}
}

// ショーダウン
void CGame::showDown() {
	// プレイヤーの強さの配列取得
	vector<vector<int>> players_power;
	vector<int> fold = { -1 };
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) {
			players_power.emplace_back(fold);
			continue;
		}
		players_power.emplace_back(dealer_.checkHand(players_.at(player_no).getHand()));
	}


	// 勝者を決定
	vector<int> winner = { 0 };
	for (int player_no = 1; player_no < NUM_PLAYER; player_no++) {
		vector<int> player_power = players_power.at(player_no);
		vector<int> winner_power = players_power.at(winner.at(0));
		if (winner_power.front() < player_power.front()) { // 役がより強い
			winner.clear();
			winner.emplace_back(player_no);
		}
		else if (winner_power.front() == player_power.front()) { // 役が一緒
			bool chop = true;
			for (int i = 0; i < winner_power.size(); i++) {
				if (winner_power.at(i) < player_power.at(i)) {
					chop = false;
					winner.clear();
					winner.emplace_back(player_no);
					break;
				}
				else if (winner_power.at(i) > player_power.at(i)) {
					chop = false;
					break;
				}
			}
			if (chop) { // チョップ　引き分け
				winner.emplace_back(player_no);
			}
		}
	}

	// 賞金の計算
	int prize = pot_.total_pot() / winner.size();
	for (int i : winner) {
		players_.at(i).payout(-prize);
	}

	// 結果の表示
	system("cls");
	cout << "<<< 終了 >>>" << endl;
	cout << "ポット合計： " << pot_.total_pot() << endl << endl;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		CPlayer player = players_.at(player_no);
		if ((is_fold_ & (1 << player_no)) != 0) { // フォールド
			cout << "fold ";
		}
		else if (find(winner.begin(), winner.end(), player_no) != winner.end()) { //勝利
			cout << " win ";
		}
		else {
			cout << "     ";
		}
		cout << "<" << player.name() << "> 残金：" << player.bankroll() << endl;
		if ((is_fold_ & (1 << player_no)) != 0) continue;
		cout << "手札： ";
		player.showHand();
		cout << "役： " << hands[players_power.at(player_no).front()] << endl << endl;
	}
	int wait = cin.get();
	wait = cin.get();
}
