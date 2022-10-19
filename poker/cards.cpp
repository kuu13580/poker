#include "common.h"
#include "cards.h"

// コンストラクタ
CCards::CCards(int numJoker) : m_numCards(0){
	m_numCards = NUM_CARDS + numJoker;

	// カードの初期化
	for (int i = 0; i < 4; i++){
		for (int j = 2; j <= 14; j++){
			m_cards.push_back({ Suit(i) ,j });
		}
	}
	for (int i = 1; i <= numJoker; i++){
		m_cards.push_back({ Joker ,0 });
	}
	shuffle();
}

// 内容表示
void CCards::show() {
	for (int i = 0; i < m_cards.size(); i++) {
		cout << marks[m_cards.at(i).suit] << numbers[m_cards.at(i).number] << " ";
	}
	cout << endl;
}

// シャッフル
void CCards::shuffle() {
	random_shuffle( m_cards.begin(), m_cards.end());
}

//1枚ドロー
Card CCards::draw() {
	if (m_cards.empty()) {
		cout << "カードドローエラー" << endl;
		exit(0);
	}
	Card top = m_cards.back();
	m_cards.pop_back();
	return top;
}
