#include "common.h"
#include "dealer.h"

//�R���X�g���N�^
CDealer::CDealer() : num_jokers_(0) {

}

//���`�F�b�N
vector<int> CDealer::checkHand(vector<Card> hand) {
	// �����݂̂̔z����쐬
	vector<int> hand_numbers;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		hand_numbers.emplace_back(hand.at(i).number);
	}
	sort(hand_numbers.begin(), hand_numbers.end(), greater<int>());
	//JOKER�̐�
	num_jokers_ = 0;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		if (hand_numbers.at(i) == 0) {
			num_jokers_++;
		}
	}
	//�y�A�`�F�b�N
	int buffer_num_joker = num_jokers_;
	vector<int> power_pair;
	power_pair = isPair(hand_numbers);
	//�t���b�V���E�X�g���[�g�`�F�b�N
	if (power_pair.at(0) == NoPair) {
		vector<int> power_flush;
		vector<int> power_straight;
		num_jokers_ = buffer_num_joker;
		power_flush = isFlush(hand, hand_numbers);
		num_jokers_ = buffer_num_joker;
		power_straight = isStraight(hand_numbers);

		if (power_flush.at(0) > 0 && power_straight.at(0) > 0) { //�t���b�V�����X�g���[�g
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

// �t���b�V���`�F�b�N
// FALSE:-1��Ԃ�
vector<int> CDealer::isFlush(vector<Card>& hand, vector<int> hand_numbers) {
	bool result = true;
	for (int i = 0; i < NUM_HANDCARDS - 1; i++) {
		// �ׂ荇���X�[�g���قȂ�ꍇ�t���b�V���łȂ�
		if (hand.at(i).suit != hand.at(i + 1).suit && hand.at(i).suit != Joker && hand.at(i + 1).suit != Joker) {
			result = false;
		}
	}
	if (result) {
		vector<int> v;
		v.emplace_back(Flush);
		// �������傫�����ɃL�b�J�[
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

//�X�g���[�g�`�F�b�N
// FALSE:-1��Ԃ�
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
	if (difference <= 4) { //�X�g���[�g�m��
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

//�y�A�`�F�b�N
vector<int> CDealer::isPair(vector<int> hand_numbers) {
	//���v���P�ȋC������
	sort(hand_numbers.begin(), hand_numbers.end(), greater<int>());
	// ��r�����񐔂𒲂ׂ�
	int count = 0;
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		for (int j = 0; j < i; j++) {
			if (hand_numbers.at(i) == hand_numbers.at(j) && hand_numbers.at(i) != 0) {
				count++;
			}
		}
	}

	//���ɂ���ĕ��򂵂ăn�C�J�[�h����
	vector<int> power_pair; //�Ԃ��z��
	if (count == 1) { // �����y�A
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
		//JOKER�����݂���ꍇ
		if (num_jokers_ > 0) {
			cout << num_jokers_;
			num_jokers_--;
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers);
		}
	}
	else if (count == 2) { // �c�[�y�A
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
		//JOKER�����݂���ꍇ
		if (num_jokers_ > 0) {
			num_jokers_--;
			hand_numbers.back() = power_pair.at(1);
			return isPair(hand_numbers);
		}
	}
	else if (count == 3) { // �X���[�J�[�h
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
	else if (count == 4) { // �t���n�E�X
		power_pair.emplace_back(FullHouse);
		power_pair.emplace_back(hand_numbers.at(2));
		if (hand_numbers.at(2) == hand_numbers.front()) {
			power_pair.emplace_back(hand_numbers.back());
		}
		else {
			power_pair.emplace_back(hand_numbers.front());
		}
		//�t���n�E�X��Joker�͑��݂����Ȃ�
	}
	else if (count == 6) { // �t�H�[�J�[�h
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
	else { // �m�[�y�A
		power_pair.emplace_back(NoPair);
		power_pair.insert(power_pair.end(), hand_numbers.begin(), hand_numbers.end());
	}
	return power_pair;
}

// ���̕\��(�f�o�b�O�p)
void CDealer::viewHand(vector<Card> hand) {
	vector<int> result = checkHand(hand);
	cout << "�� : " << hands[result.at(0)] << endl;
	// �z��̕\��
	/*cout << "�z�� : [";
	for (int i = 0; i < result.size(); i++) {
		cout << " " << result.at(i) << ",";
	}
	cout << "]" << endl;*/
}