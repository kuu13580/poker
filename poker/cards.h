#ifndef _H_CARDS_
#define _H_CARDS_

class CCards {
private:
	int m_numCards;
	vector<Card> m_cards;
public:
	// �R���X�g���N�^
	CCards(int numJoker);
	// �\��
	void show();
	// �V���b�t��
	void shuffle();
	// 1���h���[
	Card draw();
};

#endif // _H_CARDS_
