﻿#include "common.h"
#include "cards.h"
#include "player.h"
#include "dealer.h"

string marks[] = { "ス", "ハ", "ダ", "ク", "J" };
string numbers[] = { "NULL", "NULL", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
string hands[] = {
	"NoPair",
	"OnePair",
	"TwoPair",
	"ThreeCard",
	"Straight",
	"Flush",
	"FullHouse",
	"FourCard",
	"StraightFlush",
	"RoyalFlush",
};
const int NUM_PLAYER = 4;
const int NUM_CARDS = 52;

int main() {
	// 初期作業
	CCards cards(0);
	CPlayer player[NUM_PLAYER];
	for (int i = 0; i < NUM_PLAYER; i++) {
		player[i].draw(cards, 5);
		player[i].sortHand();
	}

	CDealer dealer;
	vector<Card> test = { {Hearts, 2},{Hearts, 3},{Hearts, 4},{Hearts, 5},{Hearts, 14} };
	dealer.viewHand(test);

}
