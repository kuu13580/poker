#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "game.h"

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
const int NUM_STD_CARDS = 52;
const int NUM_HANDCARDS = 5;

int main() {
	// 初期作業
	CGame game;
	cout << "プログラム終了" << endl;
}	
