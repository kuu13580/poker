#ifndef _H_COMMON_
#define _H_COMMON_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <bit>
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
#define NUM_PLAYER 2
#define NUM_STD_CARDS 52
#define NUM_HANDCARDS 5
#define DATA_MAX_SIZE 500

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
}TCPSocket;

typedef enum Action {
	CheckCall,
	Raise,
	Fold,
	Allin,
	Open,
}Action;

typedef enum Command {
	Wait,
	Success,
	Response,
	Request_ChangeHand,
	Request_Action,
	Set_Begin,
	Set_Fold,
	Set_Allin,
	Set_PlayersBet,
	Set_CurrentBet,
	Set_TotalPot,
	Set_Opened,
	Set_Bankroll,
	Set_HandCards,
	Set_End,
	Reset_PlayersBet,
	FinishBettingRound,
	FinishDrawRound,
	ShowDown,
	FinishShowDown,
}Command;

#endif //  _H_COMMON_