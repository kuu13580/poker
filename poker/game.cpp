#include "common.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "game.h"


// �R���X�g���N�^
CGame::CGame() {
	CDeck new_deck(0);
	deck_ = new_deck;
	// �������
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

// ���E���h�O�̏�����
void CGame::initialize() {
	// �R�D�̏�����
	deck_.initialize();
	// �v���C���[�̏�����
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.at(i).clearHand();
	}
}