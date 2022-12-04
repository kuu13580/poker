#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "pot.h"
#include "game.h"
#include "server.h"
#include "client.h"

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
const int NUM_PLAYER = 2;
const int NUM_STD_CARDS = 52;
const int NUM_HANDCARDS = 5;

int main() {
	// 初期作業
	/*int num_joker, bankroll, ante;
	cout << "ジョーカーの数：";
	cin >> num_joker; 
	cout << "初期持ち金：";
	cin >> bankroll;
	cout << "アンティ(参加費)：";
	cin >> ante;
	CGame game(num_joker, bankroll, ante);
	game.startRound();*/
	int n;
	cin >> n;
	if (n == 0) {
		cout << "サーバー起動" << endl;
		CServer server;
		while (1) {
			int n;
			cout << "データの受信先";
			cin >> n;
			server.send_data();
			server.listen_data(n);
		}
	}
	else if (n == 1) {
		cout << "クライアント起動" << endl;
		CClient client;
		while (1) {
			client.listen_data();
			client.send_data();
		}
	}
	cout << "プログラム終了" << endl;
	system("pause");
	return 0;
}
