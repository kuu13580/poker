#include "common.h"
#include "player.h"
#include "cards.h"

int CPlayer::m_numPlayer = 0;

// �R���X�g���N�^
CPlayer::CPlayer() {
	// �v���C���[�̐�����
	m_numPlayer++;
	m_name = "�v���C���[" + to_string(m_numPlayer);
}
CPlayer::CPlayer(string name) {
	// �v���C���[�̐�����
	m_numPlayer++;
	m_name = name;
}

// ��D�\��
void CPlayer::show() {
	cout << "��" << m_name << "�̎�D��" << endl;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		cout << marks[m_hand.at(i).suit] << numbers[m_hand.at(i).number] << " ";
	}
	cout << endl;
}

// �ŏ��̃h���[
void CPlayer::draw(CCards& cards) {
	m_hand.emplace_back(cards.draw());
}
void CPlayer::draw(CCards& cards, int n) {
	if (n < 1) {
		cout << "�h���[�񐔃G���[" << endl;
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		m_hand.emplace_back(cards.draw());
	}
}

//��D�\�[�g
void CPlayer::sortHand() {
	sort(m_hand.begin(), m_hand.end());
}

// ��D����
void CPlayer::exchangeHand(int n, vector<int> selected, CCards& cards) {
	if (cards.m_numCards < n) {
		cards.returnCards();
	}
	Card new_card;
	for (int i = 0; i < n; i++) {
		new_card = cards.exchange(m_hand.at(selected[i]));
		m_hand.erase(m_hand.begin() + selected[i]);
		m_hand.insert(m_hand.begin() + selected[i], new_card);
	}
	sortHand();
}