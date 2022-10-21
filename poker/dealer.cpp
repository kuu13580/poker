#include "common.h"
#include "dealer.h"

//役チェック
vector<int> CDealer::checkHand(vector<Card> hand) {
	// 数字のみの配列を作成
	vector<int> hand_numbers;
	for (int i = 0; i < hand.size(); i++) {
		hand_numbers.push_back(hand.at(i).number);
	}
	sort(hand_numbers.begin(), hand_numbers.end(), greater<int>());
	//ペアチェック
	vector<int> power_pair;
	power_pair = isPair(hand_numbers);
	//フラッシュ・ストレートチェック
	if (power_pair.at(0) == NoPair) {
		vector<int> power_flush;
		vector<int> power_straight;
		power_flush = isFlush(hand, hand_numbers);
		power_straight = isStraight(hand_numbers);

		if (power_flush.at(0) > 0 && power_straight.at(0) > 0) {
			if (power_straight.at(1) == 14) {
				return { RoyalFlush };
			}
			return { StraightFlush,  power_straight.at(1) };
		}
		else if (power_flush.at(0) > 0) {
			return power_flush;
		}
		else {
			return power_straight;
		}
	}
	return power_pair;
}

// フラッシュチェック
// FALSE:-1を返す
vector<int> CDealer::isFlush(vector<Card>& hand, vector<int> hand_numbers) {
	bool result = true;
	for (int i = 0; i < hand.size() - 1; i++) {
		// 隣り合うスートが異なる場合フラッシュでない
		if (hand.at(i).suit != hand.at(i + 1).suit) {
			result = false;
		}
	}
	if (result) {
		vector<int> v;
		v.push_back(Flush);
		// 数字が大きい順にキーカード
		for (int i = 0; i < 5; i++) {
			v.push_back(hand_numbers.at(i));
		}
		return v;
	}
	else {
		return { -1 };
	}
}

//ストレートチェック
// FALSE:-1を返す
vector<int> CDealer::isStraight(vector<int> hand_numbers) {
	bool result = true;
	for (int i = 0; i < 4; i++) {
		// 数字が連続でない場合ストレートではない
		if (hand_numbers.at(i) - 1 != hand_numbers.at(i + 1)) {
			result = false;
		}
	}
	if (result) {
		return { Straight, hand_numbers.front() };
	}
	else {
		vector<int> weak_straight = { 14,5,4,3,2 };
		if (hand_numbers == weak_straight) {
			return { Straight, 5 };
		}
		return { -1 };
	}
}

//ペアチェック
vector<int> CDealer::isPair(vector<int> hand_numbers) {
	// 比較成功回数を調べる
	int count = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < i; j++) {
			if (hand_numbers.at(i) == hand_numbers.at(j)) {
				count++;
			}
		}
	}

	//役によって分岐してハイカード処理
	vector<int> power_pair;
	if (count == 1) {
		power_pair.push_back(OnePair);
		vector<int> key_numbers;
		copy(hand_numbers.begin(), hand_numbers.end(), back_inserter(key_numbers));
		for (int i = 0; i < key_numbers.size() - 1; i++) {
			if (key_numbers.at(i) == key_numbers.at(i + 1)) {
				key_numbers.erase(key_numbers.begin() + i);
			}
		}
		power_pair.insert(power_pair.end(), key_numbers.begin(), key_numbers.end());
	}
	else if (count == 2) {
		power_pair.push_back(TwoPair);
		vector<int> key_numbers;
		copy(hand_numbers.begin(), hand_numbers.end(), back_inserter(key_numbers));
		for (int i = 0; i < key_numbers.size() - 1; i++) {
			if (key_numbers.at(i) == key_numbers.at(i + 1)) {
				key_numbers.erase(key_numbers.begin() + i);
			}
		}
		power_pair.insert(power_pair.end(), key_numbers.begin(), key_numbers.end());
	}
	else if (count == 3) {
		power_pair.push_back(ThreeCard);
		power_pair.push_back(hand_numbers.at(2));
		if (hand_numbers.at(2) == hand_numbers.at(3)) {
			power_pair.push_back(hand_numbers.at(0));
		}
		else {
			power_pair.push_back(hand_numbers.at(3));
		}
		if (hand_numbers.at(1) == hand_numbers.at(2)) {
			power_pair.push_back(hand_numbers.at(4));
		}
		else {
			power_pair.push_back(hand_numbers.at(1));
		}
	}
	else if (count == 4) {
		power_pair.push_back(FullHouse);
		power_pair.push_back(hand_numbers.at(2));
		if (hand_numbers.at(2) == hand_numbers.front()) {
			power_pair.push_back(hand_numbers.back());
		}
		else {
			power_pair.push_back(hand_numbers.front());
		}
	}
	else if (count == 6) { // フォーカード
		power_pair.push_back(FourCard);
		power_pair.push_back(hand_numbers.at(2));
		if (hand_numbers.at(2) == hand_numbers.front()) {
			power_pair.push_back(hand_numbers.back());
		}
		else {
			power_pair.push_back(hand_numbers.front());
		}
	}
	else { // ノーペア
		power_pair.push_back(NoPair);
		power_pair.insert(power_pair.end(), hand_numbers.begin(), hand_numbers.end());
	}
	return power_pair;
}

void CDealer::viewHand(vector<Card> hand) {
	vector<int> result = checkHand(hand);
	cout << "役 : " << hands[result.at(0)] << endl << "配列 : [";
	for (int i = 0; i < result.size(); i++) {
		cout << " " << result.at(i) << ",";
	}
	cout << "]" << endl;
}