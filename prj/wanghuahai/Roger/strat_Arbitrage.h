#pragma once
#include "StrategyBase.h"

// 套利策略
class strat_Arbitrage : public StrategyBase
{
public:
	strat_Arbitrage(void);
	virtual ~strat_Arbitrage(void);

	virtual void OnTick(const Tick& tick);

private:
	// 基差 怎么算？  分别要显示哪些变量的 ？在此定义
	double
};

