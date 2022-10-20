#ifndef _H_DEALER_
#define _H_DEALER_

class CDealer {
private:
	// 以下のチェックでは約無しで-1を返す
	// チェック時は数字は降順ソート必須
	vector<int> isFlush(vector<Card>& hand, vector<int> hand_numbers);
	vector<int> isStraight(vector<int> hand_numbers);
	vector<int> isPair(vector<int> hand_numbers);
public:
	// 役チェック 役の最強札を返す
	vector<int> checkHand(vector<Card> hand);
	void viewHand(vector<Card> hand);
};

#endif // _H_DEALER_