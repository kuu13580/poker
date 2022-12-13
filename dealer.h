#ifndef _H_DEALER_
#define _H_DEALER_

namespace dealer{
	// �ȉ��̃`�F�b�N�ł͖񖳂���-1��Ԃ�
	// �`�F�b�N���͐����͍~���\�[�g�K�{
	vector<int> isFlush(vector<Card>& hand, vector<int> hand_numbers, int num_jokers);
	vector<int> isStraight(vector<int> hand_numbers, int num_jokers);
	vector<int> isPair(vector<int> hand_numbers, int num_jokers);
	// ���`�F�b�N ���̍ŋ��D��Ԃ�
	vector<int> checkHand(vector<Card> hand);
	// ����\������
	void viewHand(vector<Card> hand);
}

#endif // _H_DEALER_