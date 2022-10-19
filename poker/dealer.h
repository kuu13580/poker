#ifndef _H_DEALER_
#define _H_DEALER_

class CDealer {
private:
	// 以下のチェックでは約無しで-1を返す
	int isFlush(vector<Card>& hand);
	int isStraight(vector<int> hand);
public:
	// コンストラクタ
	// 役チェック 役の最強札を返す
	Hand checkHand(vector<Card> hand);
};

#endif // _H_DEALER_