#ifndef _H_DEALER_
#define _H_DEALER_

class CDealer {
private:
	// �ȉ��̃`�F�b�N�ł͖񖳂���-1��Ԃ�
	int isFlush(vector<Card>& hand);
	int isStraight(vector<int> hand);
public:
	// �R���X�g���N�^
	// ���`�F�b�N ���̍ŋ��D��Ԃ�
	Hand checkHand(vector<Card> hand);
};

#endif // _H_DEALER_