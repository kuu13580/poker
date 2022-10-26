#include "common.h"
#include "dealer.h"

//コンストラクタ
CDealer::CDealer() : num_jokers_(0) {

}

//役チェック
vector<int> CDealer::checkHand(vector<Card> hand) {
	// 数字のみの配列を作成
	vector<int> hand_numbers;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		hand_numbers.emplace_back(hand.at(i).number);
	}
	sort(hand_numbers.begin(), hand_numbers.end(), greater<int>());
	//JOKERの数
	num_jokers_ = 0;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		if (hand_numbers.at(i) == 0) {
			num_jokers_++;
		}
	}
	//ペアチェック
	int buffer_num_joker = num_jokers_;
	vector<int> power_pair;
	power_pair = isPair(hand_numbers);
	//フラッシュ・ストレートチェック
	if (power_pair.at(0) == NoPair) {
		vector<int> power_flush;
		vector<int> power_straight;
		num_jokers_ = buffer_num_joker;
		power_flush = isFlush(hand, hand_numbers);
		num_jokers_ = buffer_num_joker;
		power_straight = isStraight(hand_numbers);

		if (power_flush.at(0) > 0 && power_straight.at(0) > 0) { //フラッシュかつストレート
			if (power_straight.at(1) == 14) {
				return { RoyalFlush };
			}
			return { StraightFlush,  power_straight.at(1) };
		}
		else if (power_flush.at(0) > 0) {
			return power_flush;
		}
		else if (power_straight.at(0) > 0) {
			return power_straight;
		}
	}
	return power_pair;
}

// フラッシュチェック
// FALSE:-1を返す
vector<int> CDealer::isFlush(vector<Card>& hand, vector<int> hand_numbers) {
	bool result = true;
	for (int i = 0; i < NUM_HANDCARDS - 1; i++) {
		// 隣り合うスートが異なる場合フラッシュでない
		if (hand.at(i).suit != hand.at(i + 1).suit && hand.at(i).suit != Joker && hand.at(i + 1).suit != Joker) {
			result = false;
		}
	}
	if (result) {
		vector<int> v;
		v.emplace_back(Flush);
		// 数字が大きい順にキッカー
		auto itr = hand_numbers.begin();
		for (int i = 14; i > 0; i--) {
			if (itr == hand_numbers.end()) {
				break;
			}
			if (i != *itr && num_jokers_ > 0) {
				v.emplace_back(i);
				num_jokers_--;
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
vector<int> CDealer::isStraight(vector<int> hand_numbers) {
	int max = hand_numbers.front();
	int min = 0;
	for (auto itr = hand_numbers.end() - 1; itr != hand_numbers.begin(); itr--) {
		if (*itr != 0) {
			min = *itr;
			break;
		}
	}
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
vector<int> CDealer::isPair(vector<int> hand_numbers) {
	//↓要改善な気がする
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
		if (num_jokers_ > 0) {
			cout << num_jokers_;
			num_jokers_--;
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers);
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
		if (num_jokers_ > 0) {
			num_jokers_--;
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers);
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
		if (num_jokers_ > 0) {
			num_jokers_--;
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers);
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
		if (num_jokers_ > 0) {
			num_jokers_--;
			if (hand_numbers.at(2) == 14) {
				hand_numbers.back() = 13;
			}
			else {
				hand_numbers.back() = 14;
			}
			return isPair(hand_numbers);
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
	}
	return power_pair;
}

// 役の表示(デバッグ用)
void CDealer::viewHand(vector<Card> hand) {
	vector<int> result = checkHand(hand);
	cout << "役 : " << hands[result.at(0)] << endl;
	// 配列の表示
	/*cout << "配列 : [";
	for (int i = 0; i < result.size(); i++) {
		cout << " " << result.at(i) << ",";
	}
	cout << "]" << endl;*/
}