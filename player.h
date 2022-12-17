#ifndef _H_PLAYER_
#define _H_PLAYER_
class CDeck;

class CPlayer {
private:
	static int num_players_;
	string name_;
	int player_no_;
	// ��D
	vector<Card> hand_;
	// ������
	int bankroll_;
public:
	vector<Card> getHand() { return hand_; };
	int bankroll() { return bankroll_; };
	string name() { return name_; };
	int player_no() { return player_no_; };
	void setName(string name) { name_ = name; };
	void setBankroll(int num) { bankroll_ = num; }
	void setHand(Card card, int pos) {
		hand_.resize(NUM_HANDCARDS);
		hand_.at(pos).number = card.number;
		hand_.at(pos).suit = card.suit;
	}
	// �I�[�v���x�b�g�̌����D(5����01�t���b�O)
	int public_cards;
	// �R���X�g���N�^
	CPlayer(int bankroll) : CPlayer("�v���C���[" + to_string(num_players_ + 1), bankroll) {};
	CPlayer(string name, int bankroll);
	// ��D�\��
	void showHand();
	// �h���[
	void draw(CDeck& cards);
	// n��h���[
	void draw(CDeck& cards, int n);
	// ��D�\�[�g
	void sortHand() { sort(hand_.begin(), hand_.end()); };
	// ��D����
	void exchangeHand(int selected, CDeck& cards);
	// ��D�폜
	void clearHand() { hand_.clear(); };
	// �o���N���[������
	void payout(int payout) { bankroll_ -= payout; };
};


#endif //  _H_PLAYER_

