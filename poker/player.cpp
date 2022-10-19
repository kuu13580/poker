#include "common.h"
#include "player.h"
#include "cards.h"

int CPlayer::m_numPlayer = 0;

// コンストラクタ
CPlayer::CPlayer() {
	// プレイヤーの数増加
	m_numPlayer++;
	m_name = "プレイヤー" + to_string(m_numPlayer);
}
CPlayer::CPlayer(string name) {
	// プレイヤーの数増加
	m_numPlayer++;
	m_name = name;
}

// 手札表示
void CPlayer::show() {
	cout << "＜" << m_name << "の手札＞" << endl;
	for (int i = 0; i < m_hand.size(); i++) {
		cout << marks[m_hand.at(i).suit] << numbers[m_hand.at(i).number] << " ";
	}
	cout << endl;
}

// ドロー
void CPlayer::draw(CCards& cards) {
	m_hand.push_back(cards.draw());
}
void CPlayer::draw(CCards& cards, int n) {
	if (n < 1) {
		cout << "ドロー回数エラー" << endl;
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		m_hand.push_back(cards.draw());
	}
}

//手札ソート
void CPlayer::sortHand() {
	sort(m_hand.begin(), m_hand.end());
}

// 手札交換
void CPlayer::exchangeHand(int n, int* selected, CCards& cards) {
	for (int i = 0; i < n; i++) {
		m_hand.erase(m_hand.begin() + selected[i]);
		m_hand.insert(m_hand.begin() + selected[i], cards.draw());
	}
	sortHand();
}