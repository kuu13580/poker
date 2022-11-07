#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "pot.h"
#include "game.h"


// コンストラクタ
CGame::CGame(int num_joker, int ante) {
	deck_ = CDeck::CDeck(num_joker);
	dealer_ = CDealer::CDealer();
	pot_ = CPot::CPot();
	dealer_btn_ = 0;
	// 初期作業
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(1000));
	}
	is_fold_ = 0b0;
	ante_ = ante;
	is_allin_ = 0b0;
}

//
void CGame::startRound() {
	initialize();
	// 参加費
	refleshWindow();
	bettingRound();
	drawRound();
	bettingRound();
	drawRound();
	bettingRound();
	showDown();
	// 親を移す
	dealer_btn_++;
}

// ラウンド前の初期化
void CGame::initialize() {
	// 各クラス初期化
	deck_.initialize();
	pot_.initialize();
	// プレイヤーの初期化
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.at(i).clearHand();
		players_.at(i).draw(deck_, NUM_HANDCARDS);
		players_.at(i).sortHand();
	}
	// 各種状態の初期化
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// ベッティングラウンド
void CGame::bettingRound() {
	int action = 0;
	int current_player_no = 0;
	CPlayer current_player = players_.at(0);
	for (int i = 0; i < NUM_PLAYER;) {
		cout << "<<< ベッティングラウンド >>>" << endl;
		current_player = players_.at(current_player_no);
		if ((is_fold_ & (1 << current_player_no)) != 0) { // フォールドしている
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			continue;
		}
		cout << current_player.name() << "のターン   残金：" << current_player.bankroll() << "   現在のベット額：" << pot_.current_bet() << endl;
		cout << "手札：";
		current_player.showHand();
		dealer_.viewHand(current_player.hand());
		cout << "チェック・コール(0) ベット・レイズ(1) フォールド(2) オールイン(3): ";
		cin >> action;
		if (action == 0) { // コール・チェック
			if (!pot_.call(current_player)) {
				continue;
			}
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
		}
		else if (action == 1) { // レイズ
			if (!pot_.raise(current_player)) {
				continue;
			}
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
		}
		else if (action == 2) { // フォールド
			is_fold_ |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		else if (action == 3) { // オールイン
			pot_.allIn(current_player);
			is_allin_ |= 1 << current_player_no;
			is_fold_ |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		refleshWindow();
	}

}

// ドローラウンド
void CGame::drawRound() {
	int n = 0;
	for (int i = 0; i < NUM_PLAYER; i++) {
		cout << "<<< ドローラウンド >>>" << endl;
		players_.at(i).showHand();
		dealer_.viewHand(players_.at(i).hand());
		cout << "交換する手札の数 : ";
		cin >> n;
		if (n < 0 || NUM_HANDCARDS < n) {
			i--;
			continue;
		}
		vector<int> selected(n);
		cout << "交換する手札 : ";
		for (int j = 0; j < n; j++) {
			cin >> selected.at(j);
		}
		players_.at(i).exchangeHand(n, selected, deck_);
		cout << endl << "交換後:";
		players_.at(i).showHand();
		dealer_.viewHand(players_.at(i).hand());
		refleshWindow();
	}
}

// ショーダウン
void CGame::showDown() {
	// オールインプレイヤーを戻す
	is_fold_ ^= is_allin_;
	// プレイヤーの強さの配列取得
	vector<vector<int>> players_power;
	vector<int> fold = {-1};
	for (int i = 0; i < NUM_PLAYER; i++) {
		if ((is_fold_ & (1 << i)) != 0) {
			players_power.emplace_back(fold);
			continue;
		}
		players_power.emplace_back(dealer_.checkHand(players_.at(i).hand()));
	}
	// 勝者を決定
	vector<int> winner = { 0 };
	for (int i = 1; i < NUM_PLAYER; i++) {
		if (players_power.at(winner.at(0)).at(0) < players_power.at(i).at(0)) { // 次のほうが強い
			winner.clear();
			winner.emplace_back(i);
		}
		else if (players_power.at(winner.at(0)).at(0) == players_power.at(i).at(0)) { // 役が一緒
			bool chop = true;
			for(int j = 0; j < players_power.at(winner.at(0)).size(); j++){
				if (players_power.at(winner.at(0)).at(j) < players_power.at(i).at(j)) {
					chop = false;
					winner.clear();
					winner.emplace_back(i);
					break;
				}
				else if (players_power.at(winner.at(0)).at(j) > players_power.at(i).at(j)) {
					chop = false;
					break;
				}
			}
			if (chop) { // チョップ　引き分け
				winner.emplace_back(i);
			}
		}
	}
	for (auto itr = winner.begin(); itr != winner.end(); itr++) {
		cout << " " << players_.at(*itr).name();
	}
	cout << "が勝者" << endl;
	// 賞金の計算
}