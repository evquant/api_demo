#pragma once
#include <string>
#include <vector>
#include <map>
#include "QueueBuffer.h"
#include "RealTimeTick.h"
#include "MsgDisplay.h"
#include "configInfo.h"
using namespace std;

// 作为策略的基类，给各个策略推送数据用
class WindDataSpi{
public:
	virtual void OnTick(const Tick& tick) = 0;
};

class WindApi : public QueueBuffer<Tick, 500>, public MsgDisplayApi
{
public:
	WindApi();
	virtual ~WindApi(void);

	int loginWind();
	MuJiJin getHisData(string sym){
		return hist_datas_[sym];
	}
	

	void addStrategy(WindDataSpi *s) { strategys_.push_back(s); }

private:
	virtual bool Comsume(const Tick& );
	

	CWAPIWrapperCpp windApi;
	vector<WindDataSpi*> strategys_;

	//"168001.OF" => 
	map<string, MuJiJin> hist_datas_;
	// 
	void getHisMinDatas(ConfigNode &);
};

