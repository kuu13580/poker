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
	for (int i = 0; i < m_hand.size(); i++) {
		cout << marks[m_hand.at(i).suit] << numbers[m_hand.at(i).number] << " ";
	}
	cout << endl;
}

// �h���[
void CPlayer::draw(CCards& cards) {
	m_hand.push_back(cards.draw());
}
void CPlayer::draw(CCards& cards, int n) {
	if (n < 1) {
		cout << "�h���[�񐔃G���[" << endl;
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		m_hand.push_back(cards.draw());
	}
}

//��D�\�[�g
void CPlayer::sortHand() {
	sort(m_hand.begin(), m_hand.end());
}

// ��D����
void CPlayer::exchangeHand(int n, int* selected, CCards& cards) {
	for (int i = 0; i < n; i++) {
		m_hand.erase(m_hand.begin() + selected[i]);
		m_hand.insert(m_hand.begin() + selected[i], cards.draw());
	}
	sortHand();
}