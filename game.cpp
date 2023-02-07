#include "common.h"
#include "network.h"
#include "client.h"
#include "server.h"
#include "deck.h"
#include "player.h"
#include "dealer.h"
#include "game.h"


// �R���X�g���N�^
CGame::CGame(vector<string> name, int num_joker, int bankroll, int ante)
	: deck_(CDeck(num_joker)), dealer_btn_(0), is_fold_(0b0), is_allin_(0b0), ante_(ante),
	is_opened_(-1), is_final_(false), current_bet_(0), previous_bet_(0), total_pot_(0), num_rest_player(NUM_PLAYER)
{
	// �������
	players_.reserve(NUM_PLAYER);
	for (int i = 0; i < NUM_PLAYER; i++) {
		players_.emplace_back(CPlayer(name.at(i), bankroll));
	}
}

// ���E���h�J�n
void CGameServer::startRound() {
	initialize();
	betAnte();
	cout << "�S������Ɗ���" << endl;
	// �x�b�e�B���O���E���h1
	cout << "�x�b�e�B���O���E���h1�J�n" << endl;
	bettingRound();
	cout << "�x�b�e�B���O���E���h1�I��" << endl << endl;
	//// �h���[���E���h1
	cout << "�h���[���E���h1�J�n" << endl;
	drawRound();
	cout << "�h���[���E���h1�I��" << endl << endl;
	//// �x�b�e�B���O���E���h2
	cout << "�x�b�e�B���O���E���h2�J�n" << endl;
	bettingRound();
	cout << "�x�b�e�B���O���E���h2�I��" << endl << endl;
	//// �h���[���E���h2
	cout << "�h���[���E���h2�J�n" << endl;
	drawRound();
	cout << "�h���[���E���h2�I��" << endl << endl;
	is_final_ = true;
	//// �ŏI�x�b�g
	cout << "�ŏI�x�b�e�B���O���E���h�J�n" << endl;
	bettingRound();
	cout << "�ŏI�x�b�e�B���O���E���h�I��" << endl << endl;
	//// �V���[�_�E��
	cout << "�V���[�_�E��" << endl;
	showDown();
	//// �e���ڂ�
	dealer_btn_++;
	startRound();
}
void CGameClient::startRound() {
	initialize();
	betAnte();
	system("cls");
	// �x�b�e�B���O���E���h1
	bettingRound();
	//// �h���[���E���h1
	drawRound();
	//// �x�b�e�B���O���E���h2
	bettingRound();
	//// �h���[���E���h2
	drawRound();
	is_final_ = true;
	//// �ŏI�x�b�g
	bettingRound();
	//// �V���[�_�E��
	showDown();
	startRound();
}

// ���E���h�O�̏�����
void CGameServer::initialize() {
	cout << "initialize�J�n" << endl;
	// �e�ϐ�������
	deck_.initialize();
	current_bet_ = 0;
	total_pot_ = 0;
	previous_bet_ = 0;
	players_bet_ = vector<int>(NUM_PLAYER, 0);
	// �v���C���[�̏�����
//#pragma omp parallel for
	for (int i = 0; i < NUM_PLAYER; i++) {
		CPlayer& player = players_.at(i);
		player.clearHand();
		player.public_cards = 0;
		if (player.bankroll() == 0) continue;
		player.draw(deck_, NUM_HANDCARDS);
		player.sortHand();
		// ������D�𑗐M
		vector<int> send_data = { Set_HandCards , i };
		for (int j = 0; j < NUM_HANDCARDS; j++) {
			send_data.emplace_back(network.ctoi(player.getHand().at(j)));
		}
		network.sendDataEach(network.convertData(send_data), i);
	}
	network.recvData();
	cout << "������D���M�ς�" << endl;
	// �e���Ԃ̏�����
	is_final_ = false;
	is_opened_ = -1;
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}
void CGameClient::initialize() {
	cout << "initialize�J�n" << endl;
	current_bet_ = 0;
	total_pot_ = 0;
	previous_bet_ = 0;
	players_bet_ = vector<int>(NUM_PLAYER, 0);
	// �����̃J�[�h����M���čX�V
	vector<int> recv_data = network.recvConvertedData();
	setData(recv_data);
	players_.at(network.self_client_no).sortHand();
	network.sendData(Success);
	// �e���Ԃ̏�����
	is_final_ = false;
	is_opened_ = -1;
	is_fold_ = 0b0;
	is_allin_ = 0b0;
}

// �A���e�B����
void CGameServer::betAnte() {
	cout << "betAnte�J�n" << endl;
	vector<int> send_data;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		CPlayer& player = players_.at(player_no);
		if (player.bankroll() == 0) {
			is_fold_ |= 1 << player_no;
		}
		else if (player.bankroll() < ante_) {
			allIn(player);
			send_data.insert(send_data.end(), { Set_PlayersBet, player_no , players_bet_.at(player_no) });
			network.addDivideInt(send_data, players_bet_.at(player_no));
			is_allin_ |= 1 << player_no;
		}
		else {
			raise(player, ante_);
		}
	}
	// �������ʂ𑗐M
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.insert(send_data.end(), { Set_PlayersBet ,player_no });
		network.addDivideInt(send_data, players_bet_.at(player_no));
	}
	send_data.insert(send_data.end(), { Set_Fold, is_fold_, Set_Allin, is_allin_, Set_CurrentBet });
	network.addDivideInt(send_data, ante_);
	network.sendData(send_data);
	network.recvData();
}
void CGameClient::betAnte() {
	cout << "betAnte�J�n" << endl;
	vector<int> recv_data = network.recvConvertedData();
	setData(recv_data);
	network.sendData( Success );
}

// ========== �x�b�e�B���O���E���h ==========
void CGameServer::bettingRound() {
	int current_player_no = dealer_btn_;
	// i�̓x�b�g�E���C�Y�����v���C���[����̂��� = NUM_PLAYER�Ńx�b�e�B���O���E���h�I��
	for (int i = 0; i < NUM_PLAYER;) {
		if (num_rest_player == 1) break;
		CPlayer& current_player = players_.at(current_player_no);
		// �t�H�[���hor�I�[���C�����Ă���
		if ((is_fold_ & (1 << current_player_no)) || (is_allin_ & (1 << current_player_no))) {
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			continue;
		}
		// ���N�G�X�g�̑��M
		vector<int> request = { Request_Action , current_player_no };
		network.sendData(request);
		cout << "  " << current_player.name() << "�̃^�[��" << endl;
		// �f�[�^�̎�M
		vector<string> recv_data_buf = network.recvData();
		vector<int> recv_data = network.convertData(recv_data_buf.at(current_player_no));
		if (recv_data.at(0) != Response || recv_data.at(1) != current_player_no) continue;
		// �f�[�^�����E�������ʑ��M
		vector<int> send_data = {};
		switch (recv_data.at(2)) {
		case CheckCall: // �R�[���E�`�F�b�N
			if (!call(current_player)) continue;
			send_data = { Set_PlayersBet, current_player_no };
			network.addDivideInt(send_data, current_bet_);
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			break;

		case Raise: // ���C�Y(�I�[�v����Ԃŕs��)
			if (0 <= is_opened_) continue;
			if (!raise(current_player, (recv_data.at(3) << 24) | (recv_data.at(4) << 16) | (recv_data.at(5) << 8) | recv_data.at(6))) continue;
			send_data = { Set_PlayersBet, current_player_no };
			network.addDivideInt(send_data, current_bet_);
			send_data.emplace_back(Set_CurrentBet);
			network.addDivideInt(send_data, current_bet_);
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
			break;

		case Fold: // �t�H�[���h
			is_fold_ |= 1 << current_player_no;
			num_rest_player--;
			send_data = { Set_Fold, is_fold_ };
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i++;
			break;

		case Allin: // �I�[���C��
			if (!allIn(current_player)) continue;
			is_allin_ |= 1 << current_player_no;
			num_rest_player--;
			// �I�[���C���z�ɏ���𐧌�
			for (int i = 0; i < NUM_PLAYER; i++) {
				if (current_bet_ < players_bet_.at(i)) {
					players_bet_.at(i) = current_bet_;
				}
			}
			send_data = { Set_Allin, is_allin_, Set_CurrentBet };
			network.addDivideInt(send_data, current_bet_);
			for (int i = 0; i < NUM_PLAYER; i++) {
				send_data.insert(send_data.end(), { Set_PlayersBet, i });
				network.addDivideInt(send_data, players_bet_.at(i));
			}
			network.sendData(send_data);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
			break;

		case Open: // �I�[�v���x�b�g(2�{�Ńx�b�g)
			if (!is_final_) break;
			if (!raise(current_player, (recv_data.at(3) << 24) | (recv_data.at(4) << 16) | (recv_data.at(5) << 8) | recv_data.at(6))) continue;
			// �J�������D
			cout << "�I�[�v���ʓr���ꂽ" << endl;
			current_player.public_cards = recv_data.at(4);
			current_bet_ = recv_data.at(3) * 2;
			players_bet_.at(current_player_no) = current_bet_;
			is_opened_ = current_player_no;
			send_data = { Set_Opened, is_opened_, current_player.public_cards };
			send_data.emplace_back(Set_CurrentBet);
			network.addDivideInt(send_data, current_bet_);
			send_data.emplace_back(Set_PlayersBet);
			send_data.emplace_back(current_player_no);
			network.addDivideInt(send_data, current_bet_);
			current_player_no = (current_player_no + 1) % NUM_PLAYER;
			i = 1;
			break;
		}
	}
	// �I������
	vector<int> send_data = { FinishBettingRound };
	// �I�[�v���x�b�g�̃x�b�g�𔼊z�Ɍ��炷
	if (is_opened_ >= 0) {
		players_bet_.at(is_opened_) /= 2;
		send_data.insert(send_data.end(), { Set_PlayersBet, is_opened_ });
		network.addDivideInt(send_data, players_bet_.at(is_opened_));
	}
	// �v���C���[�̃o���N���[��������
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.emplace_back(Set_Bankroll);
		players_.at(player_no).payout(players_bet_.at(player_no));
		send_data.emplace_back(player_no);
		network.addDivideInt(send_data, players_.at(player_no).bankroll());
	}
	total_pot_ += accumulate(players_bet_.begin(), players_bet_.end(), 0);
	send_data.emplace_back(Set_TotalPot);
	network.addDivideInt(send_data, total_pot_);
	//�x�b�g��������
	for (int i = 0; i < players_bet_.size(); i++) {
		players_bet_.at(i) = 0;
	}
	send_data.emplace_back(Reset_PlayersBet);
	current_bet_ = 0;
	send_data.insert(send_data.end(), { Set_CurrentBet, 0, 0, 0, 0 });
	network.sendData(send_data);
	network.recvData();
}

void CGameClient::bettingRound() {
	cout << "bettingRound�J�n" << endl;
	// ���E���h���̓��[�v(�I���M����̂�break)
	while (1) {
		// �f�[�^����M
		vector<int> data = network.recvConvertedData();
		// ��M�f�[�^���X�V�N�G��
		if (Set_Begin < data.at(0) && data.at(0) < Set_End) {
			setData(data);
			continue;
		}
		// ��M�f�[�^���I���N�G��
		if (data.at(0) == FinishBettingRound) {
			setData(data);
			network.sendData( Success );
			break;
		}
		// �ȉ���M�f�[�^�����N�G�X�g�N�G��
		if (data.at(0) != Request_Action) {
			cout << "Betting Round Data Format ERROR" << endl;
			system("pause");
			exit(0);
		}
		int current_player_no = data.at(1);
		CPlayer& current_player = players_.at(current_player_no);
		system("cls");
		cout << "<<< �x�b�e�B���O���E���h >>>" << endl;
		cout << "�|�b�g���z    " << total_pot_ + accumulate(players_bet_.begin(), players_bet_.end(), 0) << endl;
		cout << "���݂̃x�b�g�z�F" << current_bet_ << endl << endl;
		cout << "���e�v���C���[�̏��" << endl;
		cout << right;
		for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
			cout << (player_no == current_player_no ? "�� " : "�� ") << left << setw(12) << players_.at(player_no).name() << "   �c���F" << right << setw(6) << players_.at(player_no).bankroll() << "   �x�b�g�F" << setw(6) << players_bet_.at(player_no);
			if (is_fold_ & (1 << player_no)) cout << "   fold  ";
			if (is_allin_ & (1 << player_no)) cout << "   allin  ";
			if (players_.at(player_no).public_cards != 0) {
				cout << "   open  ";
				for (int i = 0; i < NUM_HANDCARDS; i++) {
					if (players_.at(player_no).public_cards & (1 << i)) {
						cout << marks[players_.at(player_no).getHand().at(i).suit] << numbers[players_.at(player_no).getHand().at(i).number] << " ";
					}
				}
			}
			cout << endl;
		}
		cout << noshowpos;
		cout << "=======================================================" << endl;
		cout << current_player.name() << "�̃^�[��" << endl << endl;
		// �ȉ������̃^�[��
		// �I�������A�N�V�����������f�[�^�ɂ����đÓ��ł���΃f�[�^���M
		if (current_player_no != network.self_client_no) {
			network.sendData( Wait );
			continue;
		}
		cout << "��D�F";
		current_player.showHand();
		dealer::viewHand(current_player.getHand());
		bool is_send = false;
		while (!is_send) {
			// ���͑ҋ@
			cout << "�`�F�b�N�E�R�[��(0)";
			if (0 > is_opened_) cout << " �x�b�g�E���C�Y(1)";
			cout << " �t�H�[���h(2) �I�[���C��(3)";
			if (is_final_) cout << " �I�[�v���x�b�g(4)";
			cout << " : ";
			int action = 0;
			int new_bet = 0;
			cin >> action;
			vector<int> data = { Response , current_player_no };
			switch (action) {
			case CheckCall: // �R�[���E�`�F�b�N
				if (current_player.bankroll() < current_bet_) continue;
				// �f�[�^���M(���X�|���X)
				data.emplace_back(CheckCall);
				network.sendData(data);
				is_send = true;
				break;

			case Raise: // ���C�Y(�I�[�v����Ԃŕs��)
				if (0 <= is_opened_) break;
				cout << "�x�b�g�z : ";
				cin >> new_bet;
				if (current_player.bankroll() < new_bet || new_bet < current_bet_) continue;
				// �f�[�^���M(���X�|���X)
				data.emplace_back(Raise);
				network.addDivideInt(data, new_bet);
				network.sendData(data);
				is_send = true;
				break;

			case Fold: // �t�H�[���h
				// �f�[�^���M(���X�|���X)
				data.emplace_back(Fold);
				network.sendData(data);
				is_send = true;
				break;

			case Allin: // �I�[���C��
				if (current_player.bankroll() <= 0) {
					system("pause");
					exit(0);
				}
				// �f�[�^���M(���X�|���X)
				data.emplace_back(Allin);
				network.sendData(data);
				is_send = true;
				break;

			case Open: // �I�[�v���x�b�g(2�{�Ńx�b�g)
				if (!is_final_) break;
				// �J�������D
				int selected = 0;
				int buf = 0;
				for (int j = 0; j < 3; j++) {
					do {
						cout << j + 1 << "�ڂ̌��J��D : ";
						cin >> buf;
					} while (buf < 1 || NUM_HANDCARDS < buf || (selected & (1 << buf)));
					selected |= 1 << (buf - 1);
				}
				do {
					cout << "�x�b�g�z : ";
					cin >> new_bet;
				} while (current_player.bankroll() < new_bet || new_bet * 2 < current_bet_);
				// �f�[�^���M(���X�|���X)
				data.emplace_back(Open);
				network.addDivideInt(data, new_bet);
				data.emplace_back(selected);
				network.sendData(data);
				is_send = true;
				break;
			}
		};
	}
}

// ========== �h���[���E���h ==========
void CGameServer::drawRound() {
#pragma omp parallel for
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) { // �t�H�[���h���Ă������΂�
			continue;
		}
		vector<int> send_data = { Request_ChangeHand };
		// ���N�G�X�g���M
		network.sendDataEach(send_data, player_no);
		// ��D�����f�[�^��M
		vector<int> recv_data = network.convertData(network.recvDataEach(player_no));
		// ��D����
		players_.at(player_no).exchangeHand(recv_data.at(2), deck_);
		// ������̎�D���M
		send_data = { Set_HandCards };
		for (int j = 0; j < NUM_HANDCARDS; j++) {
			send_data.emplace_back(network.ctoi(players_.at(player_no).getHand().at(j)));
		}
		network.sendDataEach(send_data, player_no);
	}
	// �I������
	network.recvData();
}

void CGameClient::drawRound() {
	// ��D�������N�G�X�g��M
	if (network.recvConvertedData().at(0) != Request_ChangeHand) {
		cout << "�h���[���E���h�J�n�G���[" << endl;
		system("pause");
		exit(0);
	}
	// ��ʕ\��
	system("cls");
	cout << "<<< �h���[���E���h >>>" << endl << endl;
	CPlayer& player = players_.at(network.self_client_no);
	player.showHand();
	dealer::viewHand(player.getHand());
	int n = 0;
	int selected = 0;
	do {
		cout << "���������D�̐� : ";
		cin >> n;
	} while (n < 0 || NUM_HANDCARDS < n);
	int buf = 0;
	for (int j = 0; j < n; j++) {
		do {
			cout << j + 1 << "�ڂ̌�����D : ";
			cin >> buf;
		} while (buf < 1 || NUM_HANDCARDS < buf || (selected & (1 << buf)));
		selected |= 1 << (buf - 1);
	}
	// selected�𑗐M
	vector<int> send_data = { Response, network.self_client_no, selected };
	network.sendData(send_data);
	// ������̎�D����M
	vector<int> recv_data = network.recvConvertedData();
	for (int i = 0; i < NUM_HANDCARDS; i++) {
		player.setHand(network.itoc(recv_data.at(1 + i)), i);
	}
	cout << endl << "������:";
	player.showHand();
	dealer::viewHand(player.getHand());
	system("pause");
	system("cls");
	// �h���[���E���h�I�����N�G�X�g���M
	network.sendData(FinishDrawRound);
}

// ========== �V���[�_�E�� ==========
void CGameServer::showDown() {
	// �v���C���[�̋����̔z��擾
	vector<vector<int>> players_power(NUM_PLAYER, { -1 });
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		if ((is_fold_ & (1 << player_no)) != 0) continue;
		players_power.emplace_back(dealer::checkHand(players_.at(player_no).getHand()));
	}

	// ���҂�����
	int winners = 0;
	int winner = 0;
	for (int player_no = 1; player_no < NUM_PLAYER; player_no++) {
		vector<int> player_power = players_power.at(player_no);
		vector<int> winner_power = players_power.at(winner);
		if (winner_power.front() < player_power.front()) { // ������苭��
			winners = 1 << player_no;
		}
		else if (winner_power.front() == player_power.front()) { // �����ꏏ
			bool chop = true;
			for (int i = 0; i < winner_power.size(); i++) {
				if (winner_power.at(i) < player_power.at(i)) {
					chop = false;
					winners = 1 << player_no;
					break;
				}
				else if (winner_power.at(i) > player_power.at(i)) {
					chop = false;
					break;
				}
			}
			if (chop) { // �`���b�v�@��������
				winners |= 1 << player_no;
			}
		}
	}

	// �܋��̌v�Z
	unsigned count = (winners & 0x55555555) + ((winners >> 1) & 0x55555555);
	count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
	count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
	count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
	count = (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
	int prize = total_pot_ / count;
	for (int i = 0; i < NUM_PLAYER; i++) {
		if (winners & (1 << i)) players_.at(i).payout(-prize);
	}

	// ���M
	vector<int> send_data;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.emplace_back(Set_HandCards);
		send_data.emplace_back(player_no);
		for (int j = 0; j < NUM_HANDCARDS; j++) {
			send_data.emplace_back(network.ctoi(players_.at(player_no).getHand().at(j)));
		}
	}
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.emplace_back(Set_Bankroll);
		send_data.emplace_back(player_no);
		network.addDivideInt(send_data, players_.at(player_no).bankroll());
	}
	network.sendData(send_data);
	network.recvData();
	send_data = { ShowDown };
	// players_power��0xf0�ŋ�؂�
	send_data.emplace_back(winners);
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		send_data.insert(send_data.end(), players_power.at(player_no).begin(), players_power.at(player_no).end());
		send_data.emplace_back(0xf0);
	}
	network.sendData(send_data);

	// �I����M
	network.recvData();
}

void CGameClient::showDown() {
	// ���ʂ̎�M
	setData(network.recvConvertedData());
	network.sendData( Success );
	vector<int> recv_data = network.recvConvertedData();
	if (recv_data.at(0) != ShowDown) {
		cout << "�V���[�_�E���f�[�^��M�G���[" << endl;
		system("pause");
		exit(0);
	}
	int winners = recv_data.at(1);
	vector<vector<int>> players_power;
	vector<int> v;
	for (int i = 2, length = recv_data.size(); i < length; i++) {
		if (recv_data.at(i) == 0xf0) {
			players_power.insert(players_power.end(), v.begin(), v.end());
			v.clear();
			continue;
		}
		v.emplace_back(recv_data.at(i));
	}
	system("cls");
	cout << "<<< �I�� >>>" << endl;
	cout << "�|�b�g���v�F " << total_pot_ << endl << endl;
	for (int player_no = 0; player_no < NUM_PLAYER; player_no++) {
		CPlayer& player = players_.at(player_no);
		if ((is_fold_ & (1 << player_no)) != 0) { // �t�H�[���h
			cout << "fold ";
		}
		else if (winners & (1 << player_no)) { //����
			cout << " win ";
		}
		else {
			cout << "     ";
		}
		cout << "<" << player.name() << "> �c���F" << player.bankroll() << endl;
		if ((is_fold_ & (1 << player_no)) != 0) continue;
		cout << "��D�F ";
		player.showHand();
		cout << "���F " << hands[players_power.at(player_no).front()] << endl << endl;
	}
	system("pause");
	// �V���[�_�E���I�����N�G�X�g���M
	vector<int> request = { FinishShowDown };
	network.sendData(request);
}

// �`�F�b�N�E�R�[��
bool CGame::call(CPlayer& player) {
	if (player.bankroll() < current_bet_) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = current_bet_;
	return true;
}

// �x�b�g�E���C�Y
bool CGame::raise(CPlayer& player, int new_bet) {
	if (player.bankroll() < new_bet || new_bet < current_bet_) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = new_bet;
	current_bet_ = new_bet;
	return true;
}

// �I�[���C��
bool CGame::allIn(CPlayer& player) {
	if (player.bankroll() <= 0) {
		cout << "�����ł�" << endl;
		return false;
	}
	players_bet_.at(player.player_no()) = player.bankroll();
	current_bet_ = player.bankroll();
	return true;
}

// �f�[�^�Z�b�g�֐�
void CGameClient::setData(vector<int> query) {
	int i = 0;
	while (query.begin() + i != query.end()) {
		switch (query.at(i)) {
		case Set_PlayersBet:
			players_bet_.at(query.at(i + 1)) = (query.at(i + 2) << 24) | (query.at(i + 3) << 16) | (query.at(i + 4) << 8) | query.at(i + 5);
			i += 6;
			break;
		case Set_CurrentBet:
			current_bet_ = (query.at(i + 1) << 24) | (query.at(i + 2) << 16) | (query.at(i + 3) << 8) | query.at(i + 4);
			i += 5;
			break;
		case Set_Fold:
			is_fold_ = query.at(i + 1);
			i += 2;
			break;
		case Set_Allin:
			is_allin_ = query.at(i + 1);
			i += 2;
			break;
		case Set_Opened:
			is_opened_ = query.at(i + 1);
			players_.at(is_opened_).public_cards = query.at(i + 2);
			i += 3;
			break;
		case Set_Bankroll:
			players_.at(query.at(i + 1)).setBankroll((query.at(i + 2) << 24) | (query.at(i + 3) << 16) | (query.at(i + 4) << 8) | query.at(i + 5));
			i += 6;
			break;
		case Set_TotalPot:
			total_pot_ = (query.at(i + 1) << 24) | (query.at(i + 2) << 16) | (query.at(i + 3) << 8) | query.at(i + 4);
			i += 5;
			break;
		case Reset_PlayersBet:
			for (int j = 0; j < players_bet_.size(); j++) {
				players_bet_.at(j) = 0;
			}
			i += 1;
			break;
		case Set_HandCards:
			for (int j = 0; j < NUM_HANDCARDS; j++) {
				players_.at(query.at(i + 1)).setHand(network.itoc(query.at(i + 2 + j)), j);
			}
		default:
			i++;
			break;
		}
	}
}