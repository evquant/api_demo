
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../HSTradeApi/HSTradeApi.h"
#include "../HSTradeApi/HSHeader.h"
#include "../HSTradeApi/HSSubApi.h"

using namespace kiiik;

static DWORD WINAPI startHeartbeatThread (LPVOID lpParam)
{
	gbIsRun = true;
	runHeartbeat((CConnectionInterface*)lpParam);
	return 0;
}

class MySpi : public TradeSpi
{
public:
	virtual void OnError(const int request_id, const std::string& error_msg)
	{
		cout << "OnError: " << error_msg << endl;
	}

	virtual void OnOrderError(OrderData* order_data)
	{
		cout << "OnOrderError: " << order_data->status_msg << endl;
	}

	virtual void OnDisconnect(const std::string& reson) { cout << "disconnect" << endl; }
	virtual void OnConnected() { cout << "connect" << endl; }

	virtual void OnOrder(OrderData* order_data) 
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

	virtual void OnTrade(TradeData* order_data)
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

	virtual void OnCancelOrder(CancelOrderData* order_data)
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

	virtual void OnQryOrder(int req_id, OrderData* order_data, const std::string& err, bool is_last)
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

	virtual void OnQryTrade(int req_id, TradeData* order_data, const std::string& err, bool is_last)
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

	virtual void OnQryAccount(int req_id, AccountData* acco_data, const std::string& err, bool is_last)
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

	virtual void OnQryPosition(int req_id, PositionData* pos_data, const std::string& err, bool is_last)
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

	/****************************** 创建功能连接 ******************************/
	string serverAddr  = "118.114.77.84:7785"; //UFX服务器地址
	HSTradeApi             callback1;
	CConnectionInterface* connection1 = NULL;

	//功能连接
	int ret = Connect(serverAddr.c_str(), &connection1, &callback1);
	if (ret != 0)
	{
		cout << "功能连接失败。错误信息为：" << connection1->GetErrorMsg(ret) << endl;
		connection1->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "功能连接成功。" << endl;
	}
	/****************************** 创建连接完成 ******************************/

	//创建心跳线程
	ghThreadReportProcess = CreateThread(NULL,
		0,
		startHeartbeatThread,
		(LPVOID)connection1,
		0,
		NULL
		);
	if (ghThreadReportProcess == NULL)
	{
		cout << "创建心跳线程失败。" << endl;
		getchar();
		return -1;
	}

	/****************************** 创建订阅连接 ******************************/
	HSTradeApi            callback2;
	HSSubApi         subCallback;
	CSubscribeInterface*  subscribe = NULL;
	CConnectionInterface* connection2 = NULL;

	//订阅连接
	ret = SubConnect(&connection2, &callback2, &subscribe, &subCallback);
	if (ret != 0)
	{
		if (subscribe != NULL)
		{
			subscribe->Release();
		}
		connection2->Release();
		connection1->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "订阅连接成功。" << endl;
	}
	/****************************** 创建连接完成 ******************************/


	int iOrderID = 0;
	while (1)
	{
		cout << endl;
		cout << "1：登录 2：证券单笔委托 3：篮子委托 4：撤单 0：退出" << endl;
		cout << "请输入指令号：";
		scanf("%d",&iOrderID);
		switch(iOrderID)
		{
		case 0:
			{
				gbIsRun = false;
				Sleep(1000);
				subscribe->Release();
				connection2->Release();
				connection1->Release();
				return 0;
			}
		case 1:
			{
				//登录
				int handler = Login(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "登录失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 2:
			{
				//证券单笔委托
				int handler = SingleEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "委托失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 3:
			{
				//篮子委托
				int handler = BasketEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "委托失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 4:
			{
				//撤单
				int handler = Withdraw(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "撤单失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		default:
			{
				cout << "输入的指令号不正确！" << endl;
				//continue;
			}
		}
	}

	map<int,CSubscribeParamInterface*>::iterator it = gAllSubscribeParam.begin();
	for (; it != gAllSubscribeParam.end(); it++)
	{
		if ((*it).second != NULL)
		{
			(*it).second->Release();
			(*it).second = NULL;
		}
	}
	gAllSubscribeParam.clear();
	subscribe->Release();
	connection2->Release();
	connection1->Release();
	getchar();    

	/*
	MySpi *spi = new MySpi();
	HSTradeApi* api = new HSTradeApi(spi);
	string err;
	if (!api->Connect())
	{
		system("pause");
		return -1;
	}

	if (!api->Login())
	{
		system("pause");
		return -1;
	}

	cout << "登陆成功" << endl;

	int chose = 0;
	int ret = 0;
	bool bRet = false;
	printf("------------------------------欢迎使用HS demo------------------------------\n");
	printf(" 1.证券委托\n 2.持仓查询\n 3.资金查询\n 4.委托查询\n 5.成交查询\n 6.撤单\n 7.可撤委托查询\n 0.退出系统\n 请输入你的操作:");

	while(scanf("%d",&chose), 0 != chose)
	{
		getchar();
		switch (chose)
		{
		case 1: 
			{				
				ret = api->Entrust("1", "801354", "3", 10.10, 100);
			}
			break;
		case 2: bRet = api->QueryPosition(E_STOCK);
			break;
		case 3: bRet = api->QueryAccount(E_STOCK);
			break;
		case 4: bRet = api->QueryEntrusts(E_STOCK);
			break;
		case 5: bRet = api->QueryDeals(E_STOCK);
			break;
		case 6: 
			{
				ret = api->CancelEntrust();
			}
			break;
		case 7: 
			break;
		default:printf("您输入的操作不正确,请输入正确的操作!\n");
			printf("请按任意键显示主菜单!:\n");
			break;
		}
		if (ret < 0 || !bRet)
		{
			cout << "请求失败" << endl;
			system("pause");
			return -1;
		}
		cout << "请求成功" << endl;
		getchar();
		printf(" 1.证券委托\n 2.持仓查询\n 3.资金查询\n 4.委托查询\n 5.成交查询\n 6.撤单\n 7.可撤委托查询\n 0.退出系统\n 请输入你的操作:");		
	}

	system("pause");*/
	return 0;
}