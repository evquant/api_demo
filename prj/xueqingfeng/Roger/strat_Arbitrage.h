#pragma once
#include "StrategyBase.h"

// ��������
class strat_Arbitrage : public StrategyBase
{
public:
	strat_Arbitrage(void);
	virtual ~strat_Arbitrage(void);

	virtual void OnTick(const Tick& tick);

private:
	// ���� ��ô�㣿  �ֱ�Ҫ��ʾ��Щ������ ���ڴ˶���
	double
};

