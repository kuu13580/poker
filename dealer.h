#ifndef _H_DEALER_
#define _H_DEALER_

namespace dealer{
	// 以下のチェックでは約無しで-1を返す
	// チェック時は数字は降順ソート必須
	vector<int> isFlush(vector<Card>& hand, vector<int> hand_numbers, int num_jokers);
	vector<int> isStraight(vector<int> hand_numbers, int num_jokers);
	vector<int> isPair(vector<int> hand_numbers, int num_jokers);
	// 役チェック 役の最強札を返す
	vector<int> checkHand(vector<Card> hand);
	// 役を表示する
	void viewHand(vector<Card> hand);
}

#endif // _H_DEALER_