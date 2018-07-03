
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../JSDTradeApi/JSDTradeApi.h"

using namespace std;
using namespace kiiik;

class MySpi : public TradeSpi
{
public:
	void OnError(const int request_id, const std::string& error_msg)
	{
		cout << "OnError: " << error_msg << endl;
	}

	void OnOrderError(OrderData* order_data)
	{
		cout << "OnOrderError: " << order_data->status_msg << endl;
	}

	void OnDisconnect(const std::string& reson) { cout << "disconnect" << endl; }
	void OnConnected() { cout << "connect" << endl; }

	void OnOrder(OrderData* order_data) 
	{
		cout << "收到委托回报" << endl;
		cout << endl << "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "status: " << order_data->status << endl
			<< "买卖: " << (order_data->direction == LONG_DIRECTION ? "买" : "卖") << endl
			<< "开平: " << (order_data->open_close_flag == OPEN_ORDER ? "开" : "平") << endl
			<< "price: " << order_data->limit_price << endl
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "total_volume: " << order_data->total_volume << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	void OnTrade(TradeData* order_data)
	{
		cout << "收到成交回报" << endl;
		cout  << endl << "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "trade_id: " << order_data->trade_id << endl
			<< "买卖: " << (order_data->direction == LONG_DIRECTION ? "买" : "卖") << endl
			<< "开平: " << (order_data->open_close_flag == OPEN_ORDER ? "开" : "平") << endl
			<< "price: " << order_data->trade_price << endl
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	void OnCancelOrder(CancelOrderData* order_data)
	{
		if (order_data->succed)
		{
			cout << endl << "撤单成功 order id: " << order_data->order_id <<endl;
		} 
		else
		{
			cout << endl << "撤单失败 : " << order_data->error <<endl;
		}
	}

	void OnQryOrder(int req_id, OrderData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "查询订单失败: " << err << endl;
			return;
		}
		cout << "\n查询订单回报\n" 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "status: " << order_data->status << endl
			<< "买卖: " << (order_data->direction == LONG_DIRECTION ? "买" : "卖") << endl
			<< "开平: " << (order_data->open_close_flag == OPEN_ORDER ? "开" : "平") << endl
			<< "price: " << order_data->limit_price << endl
			<< "total_volume: " << order_data->total_volume << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
		if (is_last)
			cout<<"------------------------------查委托结束------------------------------\n";
	}

	void OnQryTrade(int req_id, TradeData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "查询成交失败: " << err << endl;
			return;
		}
		cout << "\n查询成交回报\n" 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "trade_id: " << order_data->trade_id << endl
			<< "买卖: " << (order_data->direction == LONG_DIRECTION ? "买" : "卖") << endl
			<< "price: " << order_data->trade_price << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
		if (is_last)
			cout<<"------------------------------查成交结束------------------------------\n";
	}

	void OnQryAccount(int req_id, AccountData* acco_data, const std::string& err, bool is_last)
	{
		if (NULL == acco_data)
		{
			cout << "查询资金失败: " << err << endl;
			return;
		}
		cout << "\n资金\n"
			<< "account_id: " << acco_data->account_id << endl
			<< "broker_id: " << acco_data->broker_id << endl
			<< "money_type: " << acco_data->money_type << endl
			<< "asset_balance" << acco_data->asset_balance << endl
			<< "enable_balance" << acco_data->enable_balance << endl
			<< "withdraw_balance" << acco_data->withdraw_balance << endl
			<< "market_value" << acco_data->market_value << endl
			<< "fund_balance" << acco_data->fund_balance << endl << endl;
		if (is_last)
			cout<<"------------------------------查资金结束------------------------------\n";

	}

	void OnQryPosition(int req_id, PositionData* pos_data, const std::string& err, bool is_last)
	{
		if (NULL == pos_data)
		{
			cout << "查询持仓失败: " << err << endl;
			return;
		}
		cout << "\n查持仓回报\n"
			<< "symbol: " << pos_data->symbol.instrument << endl
			<< "买入价: " << pos_data->open_price << endl
			<< "现价: " << pos_data->last_price << endl
			<< "量: " << pos_data->open_volume << endl
			<< "可用量: " << pos_data->enable_volume << endl;
		if (is_last)
			cout<<"------------------------------查持仓结束------------------------------\n";
	}	
};

int main() {
	MySpi *spi = new MySpi();
	JSDTradeApi* api = new JSDTradeApi();
	string err;
	if (!api->Init("116.236.247.174", 17993, "2C27D739AEAE", "211.144.202.181", spi, err))
	{
		cout << "初始化失败：" << err << endl;
		system("pause");
		return -1;
	}

	if (!api->Login("404", "40422772", "123456", err))
	//if (!api->Login("404", "15018001", "123456", err))
	{
		cout << "登陆失败：" << err << endl;
		system("pause");
		return -1;
	}

	cout << "登陆成功" << endl;

	int chose = 0;
	int ret = 0;
	printf("------------------------------欢迎使用HS demo------------------------------\n");
	printf(" 1.证券委托\n 2.持仓查询\n 3.资金查询\n 4.委托查询\n 5.成交查询\n 6.撤单\n 7.可撤委托查询\n 0.退出系统\n 请输入你的操作:");

	while(scanf("%d",&chose), 0 != chose)
	{
		getchar();
		switch (chose)
		{
		case 1: 
			{
				OrderParamData param;
				param.limit_price = 16;
				param.volume = 1000;
				strcpy(param.symbol.instrument, "600000");
				param.symbol.exchange = EXCHANGE_SSE;
				ret = api->SubmitOrder(param, err);
			}
			break;
		case 2: ret = api->QueryPosition(err);
			break;
		case 3: ret = api->QueryAccount(err);
			break;
		case 4: ret = api->QueryOrder(err);
			break;
		case 5: ret = api->QueryTrade(err);
			break;
		case 6: 
			{
				OrderData param;
				param.order_id = 11815;
				param.symbol.exchange = EXCHANGE_SZE;
				ret = api->CancelOrder(param, err);
			}
			break;
		case 7: 
			break;
		default:printf("您输入的操作不正确,请输入正确的操作!\n");
			printf("请按任意键显示主菜单!:\n");
			break;
		}
		if (ret < 0)
		{
			cout << "请求失败：" << err << endl;
			system("pause");
			return -1;
		}
		cout << "请求成功" << endl;
		getchar();
		printf(" 1.证券委托\n 2.持仓查询\n 3.资金查询\n 4.委托查询\n 5.成交查询\n 6.撤单\n 7.可撤委托查询\n 0.退出系统\n 请输入你的操作:");		
	}

	system("pause");
	return 0;
}