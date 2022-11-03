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
}

//
void CGame::startRound() {
	initialize();
	// 参加費

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

}

// ベッティングラウンド
void CGame::bettingRound() {
	cout << "<<< ベッティングラウンド >>>" << endl;
	int action = 0;
	int current_player_no = 0;
	CPlayer current_player = players_.at(0);
	for (int i = 0; i < NUM_PLAYER;) {
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
			is_fold_ |= 1 << current_player_no;
			i++;
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
		}
		for (int k = 0; k < 10; k++) {
			cout << endl;
		}
	}

}

// ドローラウンド
void CGame::drawRound() {
	cout << "<<< ドローラウンド >>>" << endl;
	int n = 0;
	for (int i = 0; i < NUM_PLAYER; i++) {
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
		cout << endl << endl << endl << endl << endl;
	}
}

// ショーダウン
void CGame::showDown() {
	// 勝者を決定

	// 賞金の計算
}