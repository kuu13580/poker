#ifndef _H_COMMON_
#define _H_COMMON_

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#include <functional>
#include <random>
#include <numeric>
#include <stdlib.h>
#include <ios>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


using namespace std;

extern string marks[];
extern string numbers[];
extern string hands[];
extern const int NUM_PLAYER;
extern const int NUM_STD_CARDS;
extern const int NUM_HANDCARDS;


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
	ThreeOfKind,
	Straight,
	Flush,
	FullHouse,
	FourOfKind,
	StraightFlush,
	RoyalFlush,
}Hand;

typedef struct TCPSocket {
	SOCKET socket;
	struct sockaddr_in addr;
};

#endif //  _H_COMMON_