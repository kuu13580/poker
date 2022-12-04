#ifndef _H_GAME_
#define _H_GAME_

class CDeck;
class CPlayer;
class CDealer;
class CPot;

class CGame {
private:
	// バイナリでフォールド状態
	int is_fold_;
	// バイナリでオールイン状態
	int is_allin_;
	// 山札
	CDeck deck_;
	// プレイヤー
	vector<CPlayer> players_;
	// ポット
	CPot pot_;
	// ディーラー
	CDealer dealer_;
	// ディーラーボタン(親)
	int dealer_btn_;
public:
	//コンストラクタ
	CGame(int num_joker = 0, int bankroll = 1000, int ante = 100);
	// 1ラウンドを開始する
	void startRound();
	// ラウンドに関する初期化
	void initialize();
	// ドローラウンド
	void drawRound();
	// ショーダウン
	void showDown();
};


#endif //  _H_GAME_