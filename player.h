#ifndef _H_PLAYER_
#define _H_PLAYER_
class CDeck;

class CPlayer {
private:
	static int num_players_;
	string name_;
	// ��D
	vector<Card> hand_;
	// ������
	int bankroll_;
public:
	vector<Card> hand() { return hand_; };
	int bankroll() { return bankroll_; };
	// �R���X�g���N�^
	CPlayer(int bankroll) : CPlayer("�v���C���[" + to_string(num_players_ + 1), bankroll) {};
	CPlayer(string name, int bankroll);
	// ��D�\��
	void show();
	// �h���[
	void draw(CDeck& cards);
	// n��h���[
	void draw(CDeck& cards, int n);
	// ��D�\�[�g
	void sortHand() { sort(hand_.begin(), hand_.end()); };
	// ��D����
	void exchangeHand(int n, vector<int> selected, CDeck& cards);
	// ��D�폜
	void clearHand() { hand_.clear(); };
};


#endif //  _H_PLAYER_

