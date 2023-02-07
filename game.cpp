#include "common.h"
#include "network.h"
#include "client.h"
#include "server.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "game.h"


// コンストラクタ
CGame::CGame(vector<string> name, int num_joker, int bankroll, int ante)
	: deck_(CDeck(num_joker)), dealer_btn_(0), is_fold_(0b0), is_allin_(0b0), ante_(ante),
	is_opened_(-1), is_final_(false), current_bet_(0), previous_bet_(0), total_pot_(0), num_rest_player(NUM_PLAYER)
{
	// 初期作業
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(name.at(i), bankroll));
	}
}

// ラウンド開始
void CGameServer::startRound() {
	initialize();
	betAnte();
	cout << "全初期作業完了" << endl;
	// ベッティングラウンド1
	cout << "ベッティングラウンド1開始" << endl;
	bettingRound();
	cout << "ベッティングラウンド1終了" << endl << endl;
	//// ドローラウンド1
	cout << "ドローラウンド1開始" << endl;
	drawRound();
	cout << "ドローラウンド1終了" << endl << endl;
	//// ベッティングラウンド2
	cout << "ベッティングラウンド2開始" << endl;
	bettingRound();
	cout << "ベッティングラウンド2終了" << endl << endl;
	//// ドローラウンド2
	cout << "ドローラウンド2開始" << endl;
	drawRound();
	cout << "ドローラウンド2終了" << endl << endl;
	is_final_ = true;
	//// 最終ベット
	cout << "最終ベッティングラウンド開始" << endl;
	bettingRound();
	cout << "最終ベッティングラウンド終了" << endl << endl;
	//// ショーダウン
	cout << "ショーダウン" << endl;
	showDown();
	//// 親を移す
	dealer_btn_++;
	startRound();
}
void CGameClient::startRound() {
	initialize();
	betAnte();
	system("cls");
	// ベッティングラウンド1
	bettingRound();
	//// ドローラウンド1
	drawRound();
	//// ベッティングラウンド2
	bettingRound();
	//// ドローラウンド2
	drawRound();
	is_final_ = true;
	//// 最終ベット
	bettingRound();
	//// ショーダウン
	showDown();
	startRound();
}

// ラウンド前の初期化
void CGameServer::initialize() {
	cout << "initialize開始" << endl;
	// 各変数初期化
	deck_.initialize();
	current_bet_ = 0;
	total_pot_ = 0;
	previous_bet_ = 0;
	players_bet_ = vector<int>(NUM_PLAYER, 0);
	// プレイヤーの初期化
//#pragma omp parallel for
	for (int i = 0; i < NUM_PLAYER; i++) {
		CPlayer& player = players_.at(i);
		player.clearHand();
		player.public_cards = 0;
		if (player.bankroll() == 0) continue;
		player.draw(deck_, NUM_HANDCARDS);
		player.sortHand();
		// 初期手札を送信
		vector<int> send_data = { Set_HandCards , i };
		for (int j = 0; j < NUM_HANDCARDS; j++) {
			send_data.emplace_back(network.ctoi(player.getHand().at(j)));
		}
		network.sendDataEach(network.convertData(send_data), i);
	}
	network.recvData();
	cout << "初期手札送信済み" << endl;
	// 各種状態の初期化
	is_final_ = false;
	is_opened_ = -1;
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}
void CGameClient::initialize() {
	cout << "initialize開始" << endl;
	current_bet_ = 0;
	total_pot_ = 0;
	previous_bet_ = 0;
	players_bet_ = vector<int>(NUM_PLAYER, 0);
	// 自分のカードを受信して更新
	vector<int> recv_data = network.recvConvertedData();
	setData(recv_data);
	players_.at(network.self_client_no).sortHand();
	network.sendData(Success);
	// 各種状態の初期化
	is_final_ = false;
	is_opened_ = -1;
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// アンティ処理
void CGameServer::betAnte() {
	cout << "betAnte開始" << endl;
	vector<int> send_data;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		CPlayer& player = players_.at(player_no);
		if (player.bankroll() == 0) {
			is_fold_ |= 1 << player_no;
		}
		else if (player.bankroll() < ante_) {
			allIn(player);
			send_data.insert(send_data.end(), { Set_PlayersBet, player_no , players_bet_.at(player_no) });
			network.addDivideInt(send_data, players_bet_.at(player_no));
			is_allin_ |= 1 << player_no;
		}
		else {
			raise(player, ante_);
		}
	}
	// 処理結果を送信
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.insert(send_data.end(), { Set_PlayersBet ,player_no });
		network.addDivideInt(send_data, players_bet_.at(player_no));
	}
	send_data.insert(send_data.end(), { Set_Fold, is_fold_, Set_Allin, is_allin_, Set_CurrentBet });
	network.addDivideInt(send_data, ante_);
	network.sendData(send_data);
	network.recvData();
}
void CGameClient::betAnte() {
	cout << "betAnte開始" << endl;
	vector<int> recv_data = network.recvConvertedData();
	setData(recv_data);
	network.sendData( Success );
}

// ========== ベッティングラウンド ==========
void CGameServer::bettingRound() {
	int current_player_no = dealer_btn_;
	// iはベット・レイズしたプレイヤーからのずれ = NUM_PLAYERでベッティングラウンド終了
	for (int i = 0; i < NUM_PLAYER;) {
		if (num_rest_player == 1) break;
		CPlayer& current_player = players_.at(current_player_no);
		// フォールドorオールインしている
		if ((is_fold_ & (1 << current_player_no)) || (is_allin_ & (1 << current_player_no))) {
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			continue;
		}
		// リクエストの送信
		vector<int> request = { Request_Action , current_player_no };
		network.sendData(request);
		cout << "  " << current_player.name() << "のターン" << endl;
		// データの受信
		vector<string> recv_data_buf = network.recvData();
		vector<int> recv_data = network.convertData(recv_data_buf.at(current_player_no));
		if (recv_data.at(0) != Response || recv_data.at(1) != current_player_no) continue;
		// データ処理・処理結果送信
		vector<int> send_data = {};
		switch (recv_data.at(2)) {
		case CheckCall: // コール・チェック
			if (!call(current_player)) continue;
			send_data = { Set_PlayersBet, current_player_no };
			network.addDivideInt(send_data, current_bet_);
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			break;

		case Raise: // レイズ(オープン状態で不可)
			if (0 <= is_opened_) continue;
			if (!raise(current_player, (recv_data.at(3) << 24) | (recv_data.at(4) << 16) | (recv_data.at(5) << 8) | recv_data.at(6))) continue;
			send_data = { Set_PlayersBet, current_player_no };
			network.addDivideInt(send_data, current_bet_);
			send_data.emplace_back(Set_CurrentBet);
			network.addDivideInt(send_data, current_bet_);
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
			break;

		case Fold: // フォールド
			is_fold_ |= 1 << current_player_no;
			num_rest_player--;
			send_data = { Set_Fold, is_fold_ };
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			break;

		case Allin: // オールイン
			if (!allIn(current_player)) continue;
			is_allin_ |= 1 << current_player_no;
			num_rest_player--;
			// オールイン額に上限を制限
			for (int i = 0; i < NUM_PLAYER; i++) {
				if (current_bet_ < players_bet_.at(i)) {
					players_bet_.at(i) = current_bet_;
				}
			}
			send_data = { Set_Allin, is_allin_, Set_CurrentBet };
			network.addDivideInt(send_data, current_bet_);
			for (int i = 0; i < NUM_PLAYER; i++) {
				send_data.insert(send_data.end(), { Set_PlayersBet, i });
				network.addDivideInt(send_data, players_bet_.at(i));
			}
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
			break;

		case Open: // オープンベット(2倍でベット)
			if (!is_final_) break;
			if (!raise(current_player, (recv_data.at(3) << 24) | (recv_data.at(4) << 16) | (recv_data.at(5) << 8) | recv_data.at(6))) continue;
			// 開示する手札
			cout << "オープン別途された" << endl;
			current_player.public_cards = recv_data.at(4);
			current_bet_ = recv_data.at(3) * 2;
			players_bet_.at(current_player_no) = current_bet_;
			is_opened_ = current_player_no;
			send_data = { Set_Opened, is_opened_, current_player.public_cards };
			send_data.emplace_back(Set_CurrentBet);
			network.addDivideInt(send_data, current_bet_);
			send_data.emplace_back(Set_PlayersBet);
			send_data.emplace_back(current_player_no);
			network.addDivideInt(send_data, current_bet_);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
			break;
		}
	}
	// 終了処理
	vector<int> send_data = { FinishBettingRound };
	// オープンベットのベットを半額に減らす
	if (is_opened_ >= 0) {
		players_bet_.at(is_opened_) /= 2;
		send_data.insert(send_data.end(), { Set_PlayersBet, is_opened_ });
		network.addDivideInt(send_data, players_bet_.at(is_opened_));
	}
	// プレイヤーのバンクロールを処理
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.emplace_back(Set_Bankroll);
		players_.at(player_no).payout(players_bet_.at(player_no));
		send_data.emplace_back(player_no);
		network.addDivideInt(send_data, players_.at(player_no).bankroll());
	}
	total_pot_ += accumulate(players_bet_.begin(), players_bet_.end(), 0);
	send_data.emplace_back(Set_TotalPot);
	network.addDivideInt(send_data, total_pot_);
	//ベットを初期化
	for (int i = 0; i < players_bet_.size(); i++) {
		players_bet_.at(i) = 0;
	}
	send_data.emplace_back(Reset_PlayersBet);
	current_bet_ = 0;
	send_data.insert(send_data.end(), { Set_CurrentBet, 0, 0, 0, 0 });
	network.sendData(send_data);
	network.recvData();
}

void CGameClient::bettingRound() {
	cout << "bettingRound開始" << endl;
	// ラウンド中はループ(終了信号受領でbreak)
	while (1) {
		// データを受信
		vector<int> data = network.recvConvertedData();
		// 受信データが更新クエリ
		if (Set_Begin < data.at(0) && data.at(0) < Set_End) {
			setData(data);
			continue;
		}
		// 受信データが終了クエリ
		if (data.at(0) == FinishBettingRound) {
			setData(data);
			network.sendData( Success );
			break;
		}
		// 以下受信データがリクエストクエリ
		if (data.at(0) != Request_Action) {
			cout << "Betting Round Data Format ERROR" << endl;
			system("pause");
			exit(0);
		}
		int current_player_no = data.at(1);
		CPlayer& current_player = players_.at(current_player_no);
		system("cls");
		cout << "<<< ベッティングラウンド >>>" << endl;
		cout << "ポット総額    " << total_pot_ + accumulate(players_bet_.begin(), players_bet_.end(), 0) << endl;
		cout << "現在のベット額：" << current_bet_ << endl << endl;
		cout << "＜各プレイヤーの情報＞" << endl;
		cout << right;
		for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
			cout << (player_no == current_player_no ? "● " : "○ ") << left << setw(12) << players_.at(player_no).name() << "   残金：" << right << setw(6) << players_.at(player_no).bankroll() << "   ベット：" << setw(6) << players_bet_.at(player_no);
			if (is_fold_ & (1 << player_no)) cout << "   fold  ";
			if (is_allin_ & (1 << player_no)) cout << "   allin  ";
			if (players_.at(player_no).public_cards != 0) {
				cout << "   open  ";
				for (int i = 0; i < NUM_HANDCARDS; i++) {
					if (players_.at(player_no).public_cards & (1 << i)) {
						cout << marks[players_.at(player_no).getHand().at(i).suit] << numbers[players_.at(player_no).getHand().at(i).number] << " ";
					}
				}
			}
			cout << endl;
		}
		cout << noshowpos;
		cout << "=======================================================" << endl;
		cout << current_player.name() << "のターン" << endl << endl;
		// 以下自分のターン
		// 選択したアクションが内部データにおいて妥当であればデータ送信
		if (current_player_no != network.self_client_no) {
			network.sendData( Wait );
			continue;
		}
		cout << "手札：";
		current_player.showHand();
		dealer::viewHand(current_player.getHand());
		bool is_send = false;
		while (!is_send) {
			// 入力待機
			cout << "チェック・コール(0)";
			if (0 > is_opened_) cout << " ベット・レイズ(1)";
			cout << " フォールド(2) オールイン(3)";
			if (is_final_) cout << " オープンベット(4)";
			cout << " : ";
			int action = 0;
			int new_bet = 0;
			cin >> action;
			vector<int> data = { Response , current_player_no };
			switch (action) {
			case CheckCall: // コール・チェック
				if (current_player.bankroll() < current_bet_) continue;
				// データ送信(レスポンス)
				data.emplace_back(CheckCall);
				network.sendData(data);
				is_send = true;
				break;

			case Raise: // レイズ(オープン状態で不可)
				if (0 <= is_opened_) break;
				cout << "ベット額 : ";
				cin >> new_bet;
				if (current_player.bankroll() < new_bet || new_bet < current_bet_) continue;
				// データ送信(レスポンス)
				data.emplace_back(Raise);
				network.addDivideInt(data, new_bet);
				network.sendData(data);
				is_send = true;
				break;

			case Fold: // フォールド
				// データ送信(レスポンス)
				data.emplace_back(Fold);
				network.sendData(data);
				is_send = true;
				break;

			case Allin: // オールイン
				if (current_player.bankroll() <= 0) {
					system("pause");
					exit(0);
				}
				// データ送信(レスポンス)
				data.emplace_back(Allin);
				network.sendData(data);
				is_send = true;
				break;

			case Open: // オープンベット(2倍でベット)
				if (!is_final_) break;
				// 開示する手札
				int selected = 0;
				int buf = 0;
				for (int j = 0; j < 3; j++) {
					do {
						cout << j + 1 << "つ目の公開手札 : ";
						cin >> buf;
					} while (buf < 1 || NUM_HANDCARDS < buf || (selected & (1 << buf)));
					selected |= 1 << (buf - 1);
				}
				do {
					cout << "ベット額 : ";
					cin >> new_bet;
				} while (current_player.bankroll() < new_bet || new_bet * 2 < current_bet_);
				// データ送信(レスポンス)
				data.emplace_back(Open);
				network.addDivideInt(data, new_bet);
				data.emplace_back(selected);
				network.sendData(data);
				is_send = true;
				break;
			}
		};
	}
}

// ========== ドローラウンド ==========
void CGameServer::drawRound() {
#pragma omp parallel for
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) { // フォールドしていたら飛ばす
			continue;
		}
		vector<int> send_data = { Request_ChangeHand };
		// リクエスト送信
		network.sendDataEach(send_data, player_no);
		// 手札交換データ受信
		vector<int> recv_data = network.convertData(network.recvDataEach(player_no));
		// 手札交換
		players_.at(player_no).exchangeHand(recv_data.at(2), deck_);
		// 交換後の手札送信
		send_data = { Set_HandCards };
		for (int j = 0; j < NUM_HANDCARDS; j++) {
			send_data.emplace_back(network.ctoi(players_.at(player_no).getHand().at(j)));
		}
		network.sendDataEach(send_data, player_no);
	}
	// 終了処理
	network.recvData();
}

void CGameClient::drawRound() {
	// 手札交換リクエスト受信
	if (network.recvConvertedData().at(0) != Request_ChangeHand) {
		cout << "ドローラウンド開始エラー" << endl;
		system("pause");
		exit(0);
	}
	// 画面表示
	system("cls");
	cout << "<<< ドローラウンド >>>" << endl << endl;
	CPlayer& player = players_.at(network.self_client_no);
	player.showHand();
	dealer::viewHand(player.getHand());
	int n = 0;
	int selected = 0;
	do {
		cout << "交換する手札の数 : ";
		cin >> n;
	} while (n < 0 || NUM_HANDCARDS < n);
	int buf = 0;
	for (int j = 0; j < n; j++) {
		do {
			cout << j + 1 << "つ目の交換手札 : ";
			cin >> buf;
		} while (buf < 1 || NUM_HANDCARDS < buf || (selected & (1 << buf)));
		selected |= 1 << (buf - 1);
	}
	// selectedを送信
	vector<int> send_data = { Response, network.self_client_no, selected };
	network.sendData(send_data);
	// 交換後の手札を受信
	vector<int> recv_data = network.recvConvertedData();
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		player.setHand(network.itoc(recv_data.at(1 + i)), i);
	}
	cout << endl << "交換後:";
	player.showHand();
	dealer::viewHand(player.getHand());
	system("pause");
	system("cls");
	// ドローラウンド終了リクエスト送信
	network.sendData(FinishDrawRound);
}

// ========== ショーダウン ==========
void CGameServer::showDown() {
	// プレイヤーの強さの配列取得
	vector<vector<int>> players_power(NUM_PLAYER, { -1 });
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) continue;
		players_power.emplace_back(dealer::checkHand(players_.at(player_no).getHand()));
	}

	// 勝者を決定
	int winners = 0;
	int winner = 0;
	for (int player_no = 1; player_no < NUM_PLAYER; player_no++) {
		vector<int> player_power = players_power.at(player_no);
		vector<int> winner_power = players_power.at(winner);
		if (winner_power.front() < player_power.front()) { // 役がより強い
			winners = 1 << player_no;
		}
		else if (winner_power.front() == player_power.front()) { // 役が一緒
			bool chop = true;
			for (int i = 0; i < winner_power.size(); i++) {
				if (winner_power.at(i) < player_power.at(i)) {
					chop = false;
					winners = 1 << player_no;
					break;
				}
				else if (winner_power.at(i) > player_power.at(i)) {
					chop = false;
					break;
				}
			}
			if (chop) { // チョップ　引き分け
				winners |= 1 << player_no;
			}
		}
	}

	// 賞金の計算
	unsigned count = (winners & 0x55555555) + ((winners >> 1) & 0x55555555);
	count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
	count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
	count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
	count = (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
	int prize = total_pot_ / count;
	for (int i = 0; i < NUM_PLAYER; i++) {
		if (winners & (1 << i)) players_.at(i).payout(-prize);
	}

	// 送信
	vector<int> send_data;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.emplace_back(Set_HandCards);
		send_data.emplace_back(player_no);
		for (int j = 0; j < NUM_HANDCARDS; j++) {
			send_data.emplace_back(network.ctoi(players_.at(player_no).getHand().at(j)));
		}
	}
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.emplace_back(Set_Bankroll);
		send_data.emplace_back(player_no);
		network.addDivideInt(send_data, players_.at(player_no).bankroll());
	}
	network.sendData(send_data);
	network.recvData();
	send_data = { ShowDown };
	// players_powerは0xf0で区切る
	send_data.emplace_back(winners);
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.insert(send_data.end(), players_power.at(player_no).begin(), players_power.at(player_no).end());
		send_data.emplace_back(0xf0);
	}
	network.sendData(send_data);

	// 終了受信
	network.recvData();
}

void CGameClient::showDown() {
	// 結果の受信
	setData(network.recvConvertedData());
	network.sendData( Success );
	vector<int> recv_data = network.recvConvertedData();
	if (recv_data.at(0) != ShowDown) {
		cout << "ショーダウンデータ受信エラー" << endl;
		system("pause");
		exit(0);
	}
	int winners = recv_data.at(1);
	vector<vector<int>> players_power;
	vector<int> v;
	for (int i = 2, length = recv_data.size(); i < length; i++) {
		if (recv_data.at(i) == 0xf0) {
			players_power.insert(players_power.end(), v.begin(), v.end());
			v.clear();
			continue;
		}
		v.emplace_back(recv_data.at(i));
	}
	system("cls");
	cout << "<<< 終了 >>>" << endl;
	cout << "ポット合計： " << total_pot_ << endl << endl;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		CPlayer& player = players_.at(player_no);
		if ((is_fold_ & (1 << player_no)) != 0) { // フォールド
			cout << "fold ";
		}
		else if (winners & (1 << player_no)) { //勝利
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
	system("pause");
	// ショーダウン終了リクエスト送信
	vector<int> request = { FinishShowDown };
	network.sendData(request);
}

// チェック・コール
bool CGame::call(CPlayer& player) {
	if (player.bankroll() < current_bet_) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = current_bet_;
	return true;
}

// ベット・レイズ
bool CGame::raise(CPlayer& player, int new_bet) {
	if (player.bankroll() < new_bet || new_bet < current_bet_) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = new_bet;
	current_bet_ = new_bet;
	return true;
}

// オールイン
bool CGame::allIn(CPlayer& player) {
	if (player.bankroll() <= 0) {
		cout << "無効です" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = player.bankroll();
	current_bet_ = player.bankroll();
	return true;
}

// データセット関数
void CGameClient::setData(vector<int> query) {
	int i = 0;
	while (query.begin() + i != query.end()) {
		switch (query.at(i)) {
		case Set_PlayersBet:
			players_bet_.at(query.at(i + 1)) = (query.at(i + 2) << 24) | (query.at(i + 3) << 16) | (query.at(i + 4) << 8) | query.at(i + 5);
			i += 6;
			break;
		case Set_CurrentBet:
			current_bet_ = (query.at(i + 1) << 24) | (query.at(i + 2) << 16) | (query.at(i + 3) << 8) | query.at(i + 4);
			i += 5;
			break;
		case Set_Fold:
			is_fold_ = query.at(i + 1);
			i += 2;
			break;
		case Set_Allin:
			is_allin_ = query.at(i + 1);
			i += 2;
			break;
		case Set_Opened:
			is_opened_ = query.at(i + 1);
			players_.at(is_opened_).public_cards = query.at(i + 2);
			i += 3;
			break;
		case Set_Bankroll:
			players_.at(query.at(i + 1)).setBankroll((query.at(i + 2) << 24) | (query.at(i + 3) << 16) | (query.at(i + 4) << 8) | query.at(i + 5));
			i += 6;
			break;
		case Set_TotalPot:
			total_pot_ = (query.at(i + 1) << 24) | (query.at(i + 2) << 16) | (query.at(i + 3) << 8) | query.at(i + 4);
			i += 5;
			break;
		case Reset_PlayersBet:
			for (int j = 0; j < players_bet_.size(); j++) {
				players_bet_.at(j) = 0;
			}
			i += 1;
			break;
		case Set_HandCards:
			for (int j = 0; j < NUM_HANDCARDS; j++) {
				players_.at(query.at(i + 1)).setHand(network.itoc(query.at(i + 2 + j)), j);
			}
		default:
			i++;
			break;
		}
	}
}