#ifndef _H_DEALER_
#define _H_DEALER_

class CDealer {
private:
	//�O���[�o���ϐ�
	int num_joker;
	// �ȉ��̃`�F�b�N�ł͖񖳂���-1��Ԃ�
	// �`�F�b�N���͐����͍~���\�[�g�K�{
	vector<int> isFlush(vector<Card>& hand, vector<int> hand_numbers);
	vector<int> isStraight(vector<int> hand_numbers);
	vector<int> isPair(vector<int> hand_numbers);
public:
	//�R���X�g���N�^
	CDealer();
	// ���`�F�b�N ���̍ŋ��D��Ԃ�
	vector<int> checkHand(vector<Card> hand);
	// ����\������
	void viewHand(vector<Card> hand);
};

#endif // _H_DEALER_