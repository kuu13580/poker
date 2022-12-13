#ifndef _H_GAME_
#define _H_GAME_

class CServer;
class CClient;
class CDeck;
class CPlayer;
class CPot;

class CGame {
protected:
	// バイナリでフォールド状態
	int is_fold_;
	// バイナリでオールイン状態
	int is_allin_;
	// 山札
	CDeck deck_;
	// プレイヤー
	vector<CPlayer> players_;
	// 各プレイヤーのベット額
	vector<int> players_bet_;
	// ディーラーボタン(親)
	int dealer_btn_;
	// 前回のベッティングラウンドまでのベット額
	int previous_bet_;
	// 現在のベット額
	int current_bet_;
	// ベット総額
	int total_pot_;
	// アンティ
	int ante_;
	// オープンベットされているか(ベットプレイヤーno)
	int is_opened_;
	// オープンベットの有効化
	bool is_final_;
public:
	vector<CPlayer>* getPlayers() { return &players_; }
	//コンストラクタ
	CGame(vector<string> name, int num_joker = 0, int bankroll = 1000, int ante = 100);
	// チェック・コール
	bool call(CPlayer& player);
	// ベット・レイズ
	bool raise(CPlayer& player, int new_bet);
	// オールイン
	bool allIn(CPlayer& player);
};

class CGameServer : public CGame {
private:
	// 通信用のクラス
	CServer network;
public:
	using CGame::CGame;
	void setNetwork(CServer& server) { network = server; }
	void drawRound();
	void startRound();
	void bettingRound();
	// ラウンドに関する初期化
	void initialize();
	// アンティ処理
	void betAnte();
	// ショーダウン
	void showDown();
};

class CGameClient : public CGame {
private:
	// 通信用のクラス
	CClient network;
public:
	using CGame::CGame;
	void setNetwork(CClient& client) { network = client; }
	void drawRound();
	void startRound();
	void bettingRound();
	// ラウンドに関する初期化
	void initialize();
	// アンティ処理
	void betAnte();
	// ショーダウン
	void showDown();
	// データセット
	void setData(vector<int> query);
};


#endif //  _H_GAME_