#ifndef _H_GAME_
#define _H_GAME_

class CDeck;
class CPlayer;
class CDealer;

class CGame {
private:
	// 山札
	CDeck deck_;
	// プレイヤー
	vector<CPlayer> players_;
	// ディーラー
	CDealer dealer_;
public:
	//コンストラクタ
	CGame();
	// 1ラウンドを開始する
	void startRound();
	// ラウンドに関する初期化
	void initialize();
};


#endif //  _H_GAME_