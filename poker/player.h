#ifndef _H_PLAYER_
#define _H_PLAYER_

class CCards;

class CPlayer {
private:
	static int num_players_;
	string name_;
public:
	// ��D
	vector<Card> hand_;
	// �R���X�g���N�^
	CPlayer();
	CPlayer(string name);
	// ��D�\��
	void show();
	// �h���[
	void draw(CCards& cards);
	// n��h���[
	void draw(CCards& cards, int n);
	// ��D�\�[�g
	void sortHand();
	// ��D����
	void exchangeHand(int n, vector<int> selected, CCards& cards);
};


#endif //  _H_PLAYER_

