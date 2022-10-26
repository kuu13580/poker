#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "game.h"


// コンストラクタ
CGame::CGame() {
	CDeck new_deck(0);
	deck_ = new_deck;
	// 初期作業
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(100));
	}
}

//
void CGame::startRound() {
	deck_.shuffle();
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.at(i).draw(deck_, NUM_HANDCARDS);
		players_.at(i).sortHand();
	}
}

// ラウンド前の初期化
void CGame::initialize() {
	// 山札の初期化
	deck_.initialize();
	// プレイヤーの初期化
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.at(i).clearHand();
	}
}