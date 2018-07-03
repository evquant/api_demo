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

struct ConfigNode {
	TradeType trade_type;
	vector<ProductType> product_types;
	vector<string> instruments;
	vector<string> others;
	void FromStr(const char *);
};

class configInfo : public MsgDisplayApi
{
public:
	configInfo();
	void Init(string path);

	int getNums() const {return config_nodes_.size();}
	ConfigNode getConfigNode(int i) const {return config_nodes_[i];}
	
	//������Ҫ���ĵ�����
	set<string> getSubInstruments(ProductType);

	std::vector<string> getIndexs();

	static int GetPos(string inst);

private:
	vector<ConfigNode> config_nodes_;

};

#endif CONFIGINFO_H