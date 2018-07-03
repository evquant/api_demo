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
配置文件的格式：
策略类型，合约类型，合约代码，合约类型，合约代码 ...
......

有几行，就创建几个ui界面
*/

enum TradeType {//策略类型
	E_ARBITRAGE,
	E_PAIR_TRADE,
	E_BREAK_THROUGH,
	E_STOCK_POOL,
	E_HUAHAI, //王华海 分级基金套利
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
	
	//返回需要订阅的内容
	set<string> getSubInstruments(ProductType);

	std::vector<string> getIndexs();

	static int GetPos(string inst);

private:
	vector<ConfigNode> config_nodes_;

};

#endif CONFIGINFO_H