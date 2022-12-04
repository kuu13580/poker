#ifndef _H_DECK_
#define _H_DECK_

class CDeck {
private:
	vector<Card> deck_;
	// �̂Ă�ꂽ�J�[�h
	vector<Card> burncards_;
	// JOKER�̑���
	int num_jokers_;
	// �J�[�h�̌��݂̎c��
	int num_deck_cards_;
public:
	int num_deck_cards() { return num_deck_cards_; };
	// �R���X�g���N�^
	CDeck(int num_jokers = 0);
	// �\��
	void show();
	// �V���b�t��
	void shuffle();
	// 1���h���[
	Card draw();
	// �J�[�h����
	Card exchange(Card discard);
	// �̂ĎD��߂�
	void returnBurncards();
	// �J�[�h�̏�����
	void initialize();
};

#endif // _H_DECK_
