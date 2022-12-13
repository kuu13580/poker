#include "common.h"
#include "dealer.h"

//役チェック
vector<int> dealer::checkHand(vector<Card> hand) {
	// 数字のみの配列を作成
	vector<int> hand_numbers;
	hand_numbers.reserve(NUM_HANDCARDS);
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		hand_numbers.emplace_back(hand.at(i).number);
	};
	sort(hand_numbers.begin(), hand_numbers.end(), greater<int>());
	//JOKERの数をチェック
	int num_jokers = 0;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		if (hand_numbers.at(i) == 0) {
			num_jokers++;
		}
	}
	//ペアチェック
	int buffer_num_joker = num_jokers;
	vector<int> power_pair;
	power_pair.reserve(NUM_HANDCARDS + 1);
	power_pair = isPair(hand_numbers, num_jokers);
	//フラッシュ・ストレートチェック
	//出力用のvector準備
	vector<int> power_flush;
	vector<int> power_straight;
	power_flush.reserve(NUM_HANDCARDS + 1);
	power_straight.reserve(NUM_HANDCARDS + 1);
	//役を確認
	num_jokers = buffer_num_joker;
	power_flush = isFlush(hand, hand_numbers, num_jokers);
	num_jokers = buffer_num_joker;
	power_straight = isStraight(hand_numbers, num_jokers);
	// 出力に応じて分岐
	if (power_flush.front() > 0 && power_straight.front() > 0) { //フラッシュかつストレート
		if (power_straight.at(1) == 14 || num_jokers) {
			return { RoyalFlush };
		}
		return { StraightFlush,  power_straight.at(1) };
	}
	else if (FullHouse <= power_pair.front()) {
		return power_pair;
	}
	else if (power_flush.front() > 0) {
		return power_flush;
	}
	else if (power_straight.front() > 0) {
		return power_straight;
	}
	else {
		return power_pair;
	}

}

// フラッシュチェック　-NoPair前提
// FALSE:-1を返す
vector<int> dealer::isFlush(vector<Card>& hand, vector<int> hand_numbers, int num_jokers) {
	bool result = true;
	for (int i = 0; i < NUM_HANDCARDS - 1; i++) {
		// 隣り合うスートが異なる場合フラッシュでない
		if (hand.at(i).suit != hand.at(i + 1).suit && hand.at(i).suit != Joker && hand.at(i + 1).suit != Joker) {
			result = false;
		}
	}
	if (result) {
		vector<int> v;
		v.reserve(NUM_HANDCARDS + 1);
		v.emplace_back(Flush);
		// 数字が大きい順にキッカー
		auto itr = hand_numbers.begin();
		for (int i = 14; i > 0; i--) {
			if (itr == hand_numbers.end()) {
				break;
			}
			if (i != *itr && num_jokers > 0) {
				v.emplace_back(i);
				num_jokers--;
			}
			else if (i == *itr) {
				v.emplace_back(i);
				itr++;
			}
		}
		return v;
	}
	else {
		return { -1 };
	}
}

//ストレートチェック
// FALSE:-1を返す
vector<int> dealer::isStraight(vector<int> hand_numbers, int num_jokers) {
	int max = hand_numbers.front();
	int min = 0;
	for (auto itr = hand_numbers.end() - 1; itr != hand_numbers.begin(); itr--) {
		if (*itr != 0) {
			min = *itr;
			break;
		}
	}
	// カードの数の振れ幅
	int difference = max - min;
	if (difference <= 4) { //ストレート確定
		int hicard = hand_numbers.front() + (4 - difference);
		if (hicard > 14) {
			hicard = 14;
		}
		return { Straight, hicard };
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
vector<int> dealer::isPair(vector<int> hand_numbers, int num_jokers) {
	// 並べ替え
	sort(hand_numbers.begin(), hand_numbers.end(), greater<int>());
	// 比較成功回数を調べる
	int count = 0;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		for (int j = 0; j < i; j++) {
			if (hand_numbers.at(i) == hand_numbers.at(j) && hand_numbers.at(i) != 0) {
				count++;
			}
		}
	}

	//役によって分岐してハイカード処理
	vector<int> power_pair; //返す配列
	power_pair.reserve(NUM_HANDCARDS + 1);
	if (count == 1) { // ワンペア
		power_pair.emplace_back(OnePair);
		for (auto itr = hand_numbers.begin(); itr < hand_numbers.end() - 1; itr++) {
			if (*(itr) == *(itr + 1) && *(itr) != 0) {
				power_pair.insert(power_pair.begin() + 1, *itr);
				itr++;
			}
			else {
				power_pair.emplace_back(*itr);
			}
		}
		if (*(hand_numbers.end() - 1) != *(hand_numbers.end() - 2)) {
			power_pair.emplace_back(hand_numbers.back());
		}
		//JOKERが存在する場合
		if (num_jokers > 0) {
			cout << num_jokers;
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers, num_jokers - 1);
		}
	}
	else if (count == 2) { // ツーペア
		power_pair.emplace_back(TwoPair);
		int count = 0;
		for (auto itr = hand_numbers.begin(); itr < hand_numbers.end() - 1; itr++) {
			if (*(itr) == *(itr + 1) && *(itr) != 0) {
				count++;
				power_pair.insert(power_pair.begin() + count, *itr);
				itr++;
			}
			else {
				power_pair.emplace_back(*itr);
			}
		}
		if (*(hand_numbers.end() - 1) != *(hand_numbers.end() - 2)) {
			power_pair.emplace_back(hand_numbers.back());
		}
		//JOKERが存在する場合
		if (num_jokers > 0) {
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers, num_jokers - 1);
		}
	}
	else if (count == 3) { // スリーカード
		power_pair.emplace_back(ThreeOfKind);
		power_pair.emplace_back(hand_numbers.at(2));
		if (hand_numbers.at(2) == hand_numbers.at(3)) {
			power_pair.emplace_back(hand_numbers.at(0));
		}
		else {
			power_pair.emplace_back(hand_numbers.at(3));
		}
		if (hand_numbers.at(1) == hand_numbers.at(2)) {
			power_pair.emplace_back(hand_numbers.at(4));
		}
		else {
			power_pair.emplace_back(hand_numbers.at(1));
		}
		if (num_jokers > 0) {
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers, num_jokers - 1);
		}
	}
	else if (count == 4) { // フルハウス
		power_pair.emplace_back(FullHouse);
		power_pair.emplace_back(hand_numbers.at(2));
		if (hand_numbers.at(2) == hand_numbers.front()) {
			power_pair.emplace_back(hand_numbers.back());
		}
		else {
			power_pair.emplace_back(hand_numbers.front());
		}
		//フルハウスにJokerは存在し得ない
	}
	else if (count == 6) { // フォーカード
		power_pair.emplace_back(FourOfKind);
		power_pair.emplace_back(hand_numbers.at(2));
		if (num_jokers > 0) {
			if (hand_numbers.at(2) == 14) {
				hand_numbers.back() = 13;
			}
			else {
				hand_numbers.back() = 14;
			}
			return isPair(hand_numbers, num_jokers - 1);
		}
		if (hand_numbers.at(2) == hand_numbers.front()) {
			power_pair.emplace_back(hand_numbers.back());
		}
		else {
			power_pair.emplace_back(hand_numbers.front());
		}
	}
	else { // ノーペア
		power_pair.emplace_back(NoPair);
		power_pair.insert(power_pair.end(), hand_numbers.begin(), hand_numbers.end());
		//JOKERが存在する場合
		if (num_jokers > 0) {
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers, num_jokers - 1);
		}
	}
	return power_pair;
}

// 役の表示(デバッグ用)
void dealer::viewHand(vector<Card> hand) {
	vector<int> result = checkHand(hand);
	result.reserve(NUM_HANDCARDS + 1);
	cout << "役 : " << hands[result.front()] << endl;
	// 配列の表示
	/*cout << "配列 : [";
	for (int i = 0; i < result.size(); i++) {
		cout << " " << result.at(i) << ",";
	}
	cout << "]" << endl;*/
}