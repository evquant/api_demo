#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include <vector>
#include <string>
#include <fstream>
#include <set>
#include "RealTimeTick.h"
#include "MsgDisplay.h"
using namespace std;
/*
�����ļ��ĸ�ʽ��
�������ͣ���Լ���ͣ���Լ���룬��Լ���ͣ���Լ���� ...
......

�м��У��ʹ�������ui����
*/

enum TradeType {//��������
	E_ARBITRAGE,
	E_PAIR_TRADE,
	E_BREAK_THROUGH,
	E_STOCK_POOL,
	E_HUAHAI, //������ �ּ���������
};

struct MuJiJin {
	std::string code;
	std::string name;
	double yest_value; //���վ�ֵ
	int hands; //�µ���
	double sub_rate;//�깺����
	double ask_rate;
};

struct ConfigNode {
	string pro_name; // "IH" "IF"
	vector<string> instruments;
	MuJiJin mu;
	void FromStr(const char *);
};

class configInfo : public MsgDisplayApi
{
public:
	configInfo();
	void Init(string path);

	int getNums() const {return config_nodes_.size();}
	ConfigNode getConfigNode(int i) const {return config_nodes_[i];}
	
	//������Ҫ���ĵĻ������ A B ����
	set<string> getSubInstruments(ProductType);

	

	static int GetPos(string inst);

private:
	vector<ConfigNode> config_nodes_;

};

#endif CONFIGINFO_H