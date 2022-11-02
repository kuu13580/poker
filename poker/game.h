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
	// ディーラーボタン(親)
	int dealer_btn_;
	// new
public:
	//コンストラクタ
	CGame();
	// 1ラウンドを開始する
	void startRound();
	// ラウンドに関する初期化
	void initialize();
	// ベッティングラウンド
	void bettingRound();
	// ドローラウンド
	void drawRound();
};


#endif //  _H_GAME_