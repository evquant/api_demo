#include <iostream>
#include <string>
#include "../JZMarketDataApi/JZMarketDataApi.h"

using namespace std;
using namespace kiiik;

class MyMarketDataSpi : public MarketDataSpi {
	virtual void OnMarketPrice(BaseTick* market_price) {
		std::cout<<"代码:"<<market_price->symbol.instrument
			<<" 时间："<<market_price->date_time.Str()
			<<" 价格"<<market_price->last_price
			<<" 成交量"<<market_price->volume
			<<std::endl;
	}
};

void main() {
	MyMarketDataSpi *spi = new MyMarketDataSpi;
	JZMarketDataApi *api = new JZMarketDataApi;
	string err;
	if (!api->Init("114.141.166.21", 60002, spi, err)) {
		cout << err;
		system("pause");
		return;
	}
	if (!api->Login("", "gtja_cs", "gtja_cs", err))  {
		cout << err;
		system("pause");
		return;
	}
	cout << "login success\n";
	api->Subscribe("600000.SH", err);
	api->Subscribe("000001.SZ", err);
	system("pause");
}