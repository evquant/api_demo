#pragma once

#include <set>
#include <string>
#include "RealTimeTick.h"

using namespace std;
#include "MsgDisplay.h"
#include "WindApi.h"


class StrategyBase : public WindDataSpi, public MsgDisplayApi
{
public:
	StrategyBase(void);
	virtual ~StrategyBase(void);

	//virtual void OnTick(const Tick& tick);
private:
	// 本策略所关注的合约
	std::set<string> instruments_;
};

