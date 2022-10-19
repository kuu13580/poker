#include "common.h"
#include "cards.h"
#include "player.h"

string marks[] = { "ス", "ハ", "ダ", "ク", "J" };
string numbers[] = { "NULL", "NULL", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
const int NUM_PLAYER = 4;
const int INIT_NUM_CARD = 5;
const int NUM_CARDS = 52;

int main() {
	// 初期作業
	CCards cards(0);
	cards.show();
	CPlayer player[NUM_PLAYER];
	for (int i = 0; i < NUM_PLAYER; i++) {
	player[i].draw(cards, INIT_NUM_CARD);
	player[i].sortHand();
	}
	player[0].show();
	

	cout << "終了" << endl;
}
