#ifndef _H_CARDS_
#define _H_CARDS_

class CCards {
private:
	vector<Card> cards_;
	// �̂Ă�ꂽ�J�[�h
	vector<Card> burncards_;
	// �J�[�h�̑���
	int num_cards_;
public:
	int num_cards() { return num_cards_; };
	// �R���X�g���N�^
	CCards(int num_jokers_);
	// �\��
	void show();
	// �V���b�t��
	void shuffle();
	// 1���h���[
	Card draw();
	// �J�[�h����
	Card exchange(Card discard);
	// �̂ĎD��߂�
	void returnCards();
};

#endif // _H_CARDS_
