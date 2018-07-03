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
	vector<HisData> &getHisMin(string sym){
		return hist_datas_[sym];
	}
	

	void addStrategy(WindDataSpi *s) { strategys_.push_back(s); }

private:
	virtual bool Comsume(const Tick& );
	

	CWAPIWrapperCpp windApi;
	vector<WindDataSpi*> strategys_;

	map<string, vector<HisData> > hist_datas_;
	// 取过去7天的分钟历史数据
	vector<HisData> getHisMinDatas(/*ProductType, */string/*合约名*/);
};

