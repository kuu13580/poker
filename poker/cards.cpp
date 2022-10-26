#include "common.h"
#include "cards.h"

// コンストラクタ
CCards::CCards(int numJoker) : m_numCards(0) {
	m_numCards = NUM_CARDS + numJoker;

	// カードの初期化
	for (int i = 0; i < 4; i++) {
		for (int j = 2; j <= 14; j++) {
			m_cards.push_back({ Suit(i) ,j });
		}
	}
	for (int i = 1; i <= numJoker; i++) {
		m_cards.push_back({ Joker ,0 });
	}
	shuffle();
}

// 内容表示
void CCards::show() {
	cout << "<山札>" << endl;
	for (int i = 0; i < m_cards.size(); i++) {
		cout << marks[m_cards.at(i).suit] << numbers[m_cards.at(i).number] << " ";
	}
	cout << endl;
	// デバッグ用
	cout << "<捨て札>" << endl;
	for (int i = 0; i < m_burncard.size(); i++) {
		cout << marks[m_burncard.at(i).suit] << numbers[m_burncard.at(i).number] << " ";
	}
	cout << endl;
}

// シャッフル
void CCards::shuffle() {
	random_device get_rand_dev;
	mt19937 get_rand_mt(get_rand_dev());
	std::shuffle(m_cards.begin(), m_cards.end(), get_rand_mt);
}

// 1枚ドロー
Card CCards::draw() {
	if (m_cards.empty()) {
		cout << "ERROR : カードドローエラー" << endl;
		exit(0);
	}
	m_numCards--;
	Card top = m_cards.back();
	m_cards.pop_back();
	return top;
}

// カード交換
Card CCards::exchange(Card discard) {
	if (m_numCards < 0) {
		cout << "ERROR : カード交換できませんでした" << endl;
		exit(0);
	}
	m_burncard.push_back(discard);
	return draw();
}

// 捨て札を戻す
void CCards::returnCards() {
	if (!m_cards.empty()) {
		cout << "ERROR : 捨て札を戻せませんでした" << endl;
		exit(0);
	}
	copy(m_burncard.begin(), m_burncard.end(), back_inserter(m_cards));
	m_burncard.clear();
	shuffle();
}