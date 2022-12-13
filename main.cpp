#include "common.h"
#include "network.h"
#include "server.h"
#include "client.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "game.h"

string marks[] = { "ス", "ハ", "ダ", "ク", "Joker" };
string numbers[] = { "", "NULL", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
string hands[] = {
	"ブタ",
	"ワンペア",
	"ツーペア",
	"スリーカード",
	"ストレート",
	"フラッシュ",
	"フルハウス",
	"フォーカード",
	"ストレートフラッシュ",
	"ロイヤルフラッシュ",
};

int main() {
	int n;
	cout << "サーバー起動(0)　クライアント起動(1)：";
	cin >> n;
	system("cls");
	if (n == 0) {
		cout << "サーバー起動" << endl;
		CServer server;
		server.initServer();
		// 各変数の数
		int num_joker, bankroll, ante;
		cout << "ジョーカーの数　初期バンクロール　アンティ：";
		cin >> num_joker >> bankroll >> ante;
		// 名前の受信
		vector<string> players_name = server.recvData();
		// 初期データを送信(ジョーカー数, 初期バンクロール, アンティ, 名前)
		stringstream ss;
		ss << num_joker << " " << bankroll << " " << ante;
		for (int i = 0; i < NUM_PLAYER; i++) {
			ss << " " << players_name.at(i);
		}
		server.sendData(ss.str());
		// ゲームを立ち上げる
		CGameServer game(players_name, num_joker, bankroll, ante);
		game.setNetwork(server);
		game.startRound();
	}
	else if (n == 1) {
		cout << "クライアント起動" << endl;
		CClient client;
		client.initClient();
		// クライアントナンバーを受信
		client.self_client_no = atoi(client.recvData().c_str());
		cout << "client_noを受信" << endl;
		// 名前を設定
		string name;
		cout << "名前：";
		cin >> name;
		client.sendData(name);
		//初期データを受信(ジョーカー数, 初期バンクロール, アンティ, 名前)
		int num_joker, bankroll, ante;
		stringstream ss;
		ss << client.recvData();
		cout << "ss : " << ss.str() << endl;
		ss >> num_joker >> bankroll >> ante;
		vector<string> players_name(NUM_PLAYER);
		for (int i = 0; i < NUM_PLAYER; i++) {
			ss >> players_name.at(i);
		}
		for (string name : players_name) {
			cout << name << endl;
		}
		// ゲームを立ち上げる
		CGameClient game(players_name, num_joker, bankroll, ante);
		game.setNetwork(client);
		game.startRound();
	}
	cout << "プログラム終了" << endl;
	system("pause");
	return 0;
}
