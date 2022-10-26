#ifndef _H_CARDS_
#define _H_CARDS_

class CCards {
private:
	vector<Card> m_cards;
	// �̂Ă�ꂽ�J�[�h
	vector<Card> m_burncard;
public:
	// �J�[�h�̑���
	int m_numCards;
	// �R���X�g���N�^
	CCards(int numJoker);
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
