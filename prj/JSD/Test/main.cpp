
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
		cout << "�յ�ί�лر�" << endl;
		cout << endl << "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "status: " << order_data->status << endl
			<< "����: " << (order_data->direction == LONG_DIRECTION ? "��" : "��") << endl
			<< "��ƽ: " << (order_data->open_close_flag == OPEN_ORDER ? "��" : "ƽ") << endl
			<< "price: " << order_data->limit_price << endl
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "total_volume: " << order_data->total_volume << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	void OnTrade(TradeData* order_data)
	{
		cout << "�յ��ɽ��ر�" << endl;
		cout  << endl << "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "trade_id: " << order_data->trade_id << endl
			<< "����: " << (order_data->direction == LONG_DIRECTION ? "��" : "��") << endl
			<< "��ƽ: " << (order_data->open_close_flag == OPEN_ORDER ? "��" : "ƽ") << endl
			<< "price: " << order_data->trade_price << endl
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	void OnCancelOrder(CancelOrderData* order_data)
	{
		if (order_data->succed)
		{
			cout << endl << "�����ɹ� order id: " << order_data->order_id <<endl;
		} 
		else
		{
			cout << endl << "����ʧ�� : " << order_data->error <<endl;
		}
	}

	void OnQryOrder(int req_id, OrderData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "��ѯ����ʧ��: " << err << endl;
			return;
		}
		cout << "\n��ѯ�����ر�\n" 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "status: " << order_data->status << endl
			<< "����: " << (order_data->direction == LONG_DIRECTION ? "��" : "��") << endl
			<< "��ƽ: " << (order_data->open_close_flag == OPEN_ORDER ? "��" : "ƽ") << endl
			<< "price: " << order_data->limit_price << endl
			<< "total_volume: " << order_data->total_volume << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
		if (is_last)
			cout<<"------------------------------��ί�н���------------------------------\n";
	}

	void OnQryTrade(int req_id, TradeData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "��ѯ�ɽ�ʧ��: " << err << endl;
			return;
		}
		cout << "\n��ѯ�ɽ��ر�\n" 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "trade_id: " << order_data->trade_id << endl
			<< "����: " << (order_data->direction == LONG_DIRECTION ? "��" : "��") << endl
			<< "price: " << order_data->trade_price << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
		if (is_last)
			cout<<"------------------------------��ɽ�����------------------------------\n";
	}

	void OnQryAccount(int req_id, AccountData* acco_data, const std::string& err, bool is_last)
	{
		if (NULL == acco_data)
		{
			cout << "��ѯ�ʽ�ʧ��: " << err << endl;
			return;
		}
		cout << "\n�ʽ�\n"
			<< "account_id: " << acco_data->account_id << endl
			<< "broker_id: " << acco_data->broker_id << endl
			<< "money_type: " << acco_data->money_type << endl
			<< "asset_balance" << acco_data->asset_balance << endl
			<< "enable_balance" << acco_data->enable_balance << endl
			<< "withdraw_balance" << acco_data->withdraw_balance << endl
			<< "market_value" << acco_data->market_value << endl
			<< "fund_balance" << acco_data->fund_balance << endl << endl;
		if (is_last)
			cout<<"------------------------------���ʽ����------------------------------\n";

	}

	void OnQryPosition(int req_id, PositionData* pos_data, const std::string& err, bool is_last)
	{
		if (NULL == pos_data)
		{
			cout << "��ѯ�ֲ�ʧ��: " << err << endl;
			return;
		}
		cout << "\n��ֲֻر�\n"
			<< "symbol: " << pos_data->symbol.instrument << endl
			<< "�����: " << pos_data->open_price << endl
			<< "�ּ�: " << pos_data->last_price << endl
			<< "��: " << pos_data->open_volume << endl
			<< "������: " << pos_data->enable_volume << endl;
		if (is_last)
			cout<<"------------------------------��ֲֽ���------------------------------\n";
	}	
};

int main() {
	MySpi *spi = new MySpi();
	JSDTradeApi* api = new JSDTradeApi();
	string err;
	if (!api->Init("116.236.247.174", 17993, "2C27D739AEAE", "211.144.202.181", spi, err))
	{
		cout << "��ʼ��ʧ�ܣ�" << err << endl;
		system("pause");
		return -1;
	}

	if (!api->Login("404", "40422772", "123456", err))
	//if (!api->Login("404", "15018001", "123456", err))
	{
		cout << "��½ʧ�ܣ�" << err << endl;
		system("pause");
		return -1;
	}

	cout << "��½�ɹ�" << endl;

	int chose = 0;
	int ret = 0;
	printf("------------------------------��ӭʹ��HS demo------------------------------\n");
	printf(" 1.֤ȯί��\n 2.�ֲֲ�ѯ\n 3.�ʽ��ѯ\n 4.ί�в�ѯ\n 5.�ɽ���ѯ\n 6.����\n 7.�ɳ�ί�в�ѯ\n 0.�˳�ϵͳ\n ��������Ĳ���:");

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
		default:printf("������Ĳ�������ȷ,��������ȷ�Ĳ���!\n");
			printf("�밴�������ʾ���˵�!:\n");
			break;
		}
		if (ret < 0)
		{
			cout << "����ʧ�ܣ�" << err << endl;
			system("pause");
			return -1;
		}
		cout << "����ɹ�" << endl;
		getchar();
		printf(" 1.֤ȯί��\n 2.�ֲֲ�ѯ\n 3.�ʽ��ѯ\n 4.ί�в�ѯ\n 5.�ɽ���ѯ\n 6.����\n 7.�ɳ�ί�в�ѯ\n 0.�˳�ϵͳ\n ��������Ĳ���:");		
	}

	system("pause");
	return 0;
}