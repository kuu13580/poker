#include "common.h"
#include "cards.h"
#include "player.h"
#include "dealer.h"

string marks[] = { "ス", "ハ", "ダ", "ク", "J" };
string numbers[] = { "NULL", "NULL", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
string hands[] = {
	"NoPair",
	"OnePair",
	"TwoPair",
	"ThreeOfKind",
	"Straight",
	"Flush",
	"FullHouse",
	"FourOfKind",
	"StraightFlush",
	"RoyalFlush",
};
const int NUM_PLAYER = 1;
const int NUM_CARDS = 52;
const int NUM_HANDCARDS = 5;

int main() {
	// 初期作業
	CCards cards(0);
	CPlayer player[NUM_PLAYER];
	for (int i = 0; i < NUM_PLAYER; i++) {
		player[i].draw(cards, NUM_HANDCARDS);
		player[i].sortHand();
	}

	CDealer dealer;
	int n;

	while (1) {
		player[0].show();
		dealer.viewHand(player[0].hand_);
		cout << "交換する手札の数 : ";
		cin >> n;
		if (n == 0) {
			break;
		}
		vector<int> test(n);
		cout << "交換する手札 : ";
		for (int i = 0; i < n; i++) {
			cin >> test.at(i);
		}
		player[0].exchangeHand(n, test, cards);
		cout << endl;
	}
	cout << "プログラム終了" << endl;
}	
