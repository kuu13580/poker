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
	// アンティ(参加費)
	int ante_;
public:
	//コンストラクタ
	CGame() : CGame(0, 100) {};
	CGame(int num_joker, int ante);
	// 1ラウンドを開始する
	void startRound();
	// ラウンドに関する初期化
	void initialize();
	// ベッティングラウンド
	void bettingRound();
	// ドローラウンド
	void drawRound();
	// ショーダウン
	void showDown();
};


#endif //  _H_GAME_