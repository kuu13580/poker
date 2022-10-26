#ifndef _H_PLAYER_
#define _H_PLAYER_

class CDeck;

class CPlayer {
private:
	static int num_players_;
	string name_;
	// ��D
	vector<Card> hand_;
public:
	vector<Card> hand() { return hand_; };
	// �R���X�g���N�^
	CPlayer();
	CPlayer(string name);
	// ��D�\��
	void show();
	// �h���[
	void draw(CDeck& cards);
	// n��h���[
	void draw(CDeck& cards, int n);
	// ��D�\�[�g
	void sortHand();
	// ��D����
	void exchangeHand(int n, vector<int> selected, CDeck& cards);
};


#endif //  _H_PLAYER_

