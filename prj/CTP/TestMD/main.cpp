#include <iostream>
#include <string>
#include "../CTPMarketDataApi/CtpFutureMarketDataApi.h"

using namespace std;
using namespace kiiik;

class MyMarketDataSpi : public MarketDataSpi {
	virtual void OnMarketPrice(BaseTick* market_price) {
		std::cout<<"代码:"<<market_price->symbol.instrument
			<<" 最新价"<<market_price->last_price
			<<" B1:"<<((FutureTick*)market_price)->buy_price
			<<" V1:"<<((FutureTick*)market_price)->buy_volume
			<<" A1:"<<((FutureTick*)market_price)->sell_price
			<<" A1:"<<((FutureTick*)market_price)->sell_volume
			<<std::endl;
	}
};

void main() {
	MyMarketDataSpi *spi = new MyMarketDataSpi;
	CtpFutureMarketDataApi *api = new CtpFutureMarketDataApi;
	string err;
	if (!api->Init("tcp://222.66.97.241:41213", spi, err)) {
		cout << err;
		system("pause");
		return;
	}
	if (!api->Login("7070", "", "", err))  {
		cout << err;
		system("pause");
		return;
	}
	cout << "login success\n";
	api->Subscribe("IC1512", err);
	api->Subscribe("IF1512", err);
	system("pause");
}