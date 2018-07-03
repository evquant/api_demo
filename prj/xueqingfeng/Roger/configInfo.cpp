#include "configInfo.h"
#include <assert.h>
#include <QStringList>

void ConfigNode::FromStr(const char *line){
	QString tmp(line);
	QStringList content = tmp.split(",");
	
	if (content.size() > 0)
		trade_type = (TradeType)(content[0].toStdString().c_str()[0] - 'a');

	if (trade_type == E_PAIR_TRADE) {
		product_types.push_back((ProductType)content[1].toInt());
		instruments.push_back(content[2].toStdString());
		product_types.push_back((ProductType)content[3].toInt());
		instruments.push_back(content[4].toStdString());
		for(int i=5; i<content.size(); ++i)
			others.push_back(content[i].toStdString());
	}

}

// 策略类型: a套利 b配对 c突破 d股票池
// 证券类型: 0股票 1期货 2指数
void configInfo::Init(string path){
	std::ifstream cinStream(path);
	if(!cinStream.good()){
		Print("读配置文件"+path+"失败");
		return;
	}

	while(!cinStream.eof())
	{
		char buffer[1024];
		cinStream.getline(buffer, sizeof(buffer));
		if(buffer[0]=='/' || string(buffer)=="") continue;
		ConfigNode node;
		node.FromStr(buffer);
		config_nodes_.push_back(node);
	}
	cinStream.close();
}

configInfo::configInfo(){
	
}

set<string> configInfo::getSubInstruments(ProductType type){
	set<string> ret;
	for (vector<ConfigNode>::iterator iter = config_nodes_.begin(); iter!=config_nodes_.end(); ++iter){
		for (int i = 0; i < iter->instruments.size() && i < iter->product_types.size(); ++i){
			if (iter->product_types[i] == type)
				ret.insert(iter->instruments[i]);
		}
	}
	return ret;
}

std::vector<string> configInfo::getIndexs(){
	std::vector<string> ret;
	for (vector<ConfigNode>::iterator iter = config_nodes_.begin(); iter!=config_nodes_.end(); ++iter){
		if(iter->trade_type == E_HUAHAI){
			for (int i = 0; i < iter->instruments.size() && i < iter->product_types.size(); ++i){
				ret.push_back(iter->instruments[i]);
			}
		}
	}
	return ret;
}

int configInfo::GetPos(string inst){
	if(inst.substr(7,2) == "SH"){
		return atoi(inst.substr(3,3).c_str());
	}
	else if(inst.substr(7,2) == "SZ"){
		return 1000 + atoi(inst.substr(3,3).c_str());
	}
	else{
		return -1;
	}
}