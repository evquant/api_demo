#include "configInfo.h"
#include <assert.h>
#include <QStringList>

void ConfigNode::FromStr(const char *line){
	QString tmp(line);
	QStringList content = tmp.split(",");
	
	if (content.size() == 0) return;

	pro_name = content[0].trimmed().toStdString();
	instruments.push_back(content[1].trimmed().toStdString());
	instruments.push_back(content[2].trimmed().toStdString());
	instruments.push_back(content[3].trimmed().toStdString());
	mu.code = content[4].trimmed().toStdString();
	mu.name = "";
	mu.yest_value = 0.0;
	mu.hands = content[5].trimmed().toInt();
	mu.sub_rate = content[6].trimmed().toDouble();
	mu.ask_rate = content[7].trimmed().toDouble();
}

// 策略类型: a套利 b配对 c突破 d股票池
// 证券类型: 0股票 1期货 2指数
void configInfo::Init(string path){
	
	FILE *fp;
	if((fp=fopen(path.c_str(), "r")) == NULL){
		Print("读配置文件失败");return;
	}

	while(!feof(fp))
	{
		char buffer[1024];
		fgets(buffer, 1024, fp);
		if(buffer[0]=='/' || string(buffer)==""|| string(buffer)=="\n") continue;
		ConfigNode node;
		node.FromStr(buffer);
		config_nodes_.push_back(node);
	}
	fclose(fp); 
}

configInfo::configInfo(){
	
}

set<string> configInfo::getSubInstruments(ProductType type){
	set<string> ret;
	for (vector<ConfigNode>::iterator iter = config_nodes_.begin(); iter!=config_nodes_.end(); ++iter){
		for (int i = 0; i < iter->instruments.size(); ++i){
			ret.insert(iter->instruments[i]);
		}
	}
	return ret;
}
//
//std::vector<string> configInfo::getIndexs(){
//	std::vector<string> ret;
//	for (vector<ConfigNode>::iterator iter = config_nodes_.begin(); iter!=config_nodes_.end(); ++iter){
//		for (int i = 0; i < iter->instruments.size(); ++i){
//			ret.push_back(iter->instruments[i]);
//		}
//	}
//	return ret;
//}

int configInfo::GetPos(string inst){
	if(inst.substr(7,2) == "SH"){
		return atoi(inst.substr(3,3).c_str());
	}
	else if(inst.substr(7,2) == "SZ"){
		return 1000 + atoi(inst.substr(3,3).c_str());
	}
	else if (inst.substr(7,2) == "OF") {
		return 2000 + atoi(inst.substr(3,3).c_str());
	}
	else{
		return -1;
	}
}