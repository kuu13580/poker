#ifndef _H_NETWORK_
#define _H_NETWORK_

class CNetwork {
protected:
public:
	SOCKET makeSocket();
	vector<int> convertData(string data);
	string convertData(vector<int>& data);
	Card itoc(int number);
	int ctoi(Card card);
	void addDivideInt(vector<int>& v, int number);
};


#endif