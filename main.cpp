#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "pot.h"
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
const int NUM_PLAYER = 4;
const int NUM_STD_CARDS = 52;
const int NUM_HANDCARDS = 5;

void refleshWindow() {
	for (int i = 0; i < 50; i++) {
		cout << endl;
	}
}

int main() {
	// 初期作業
	int num_joker;
	cout << "ジョーカーの数：";
	cin >> num_joker;
	CGame game(num_joker, 100);
	game.startRound();
	cout << "プログラム終了" << endl;
}
