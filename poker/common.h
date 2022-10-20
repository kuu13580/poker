#ifndef _H_COMMON_
#define _H_COMMON_

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

extern string marks[];
extern string numbers[];
extern string hands[];
extern const int NUM_PLAYER;
extern const int NUM_CARDS;

typedef enum
{
	Spades,
	Hearts,
	Diamonds,
	Clubs,
	Joker,
}Suit;

typedef struct Card
{
	Suit suit;
	int number;

	bool operator<(const Card& another) const {
		return suit == another.suit ? number < another.number : suit < another.suit;
	}
}Card;

typedef enum Hand {
	NoPair,
	OnePair,
	TwoPair,
	ThreeCard,
	Straight,
	Flush,
	FullHouse,
	FourCard,
	StraightFlush,
	RoyalFlush,
}Hand;

#endif //  _H_COMMON_