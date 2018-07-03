
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

	virtual void OnTrade(TradeData* order_data)
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

	virtual void OnCancelOrder(CancelOrderData* order_data)
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

	virtual void OnQryOrder(int req_id, OrderData* order_data, const std::string& err, bool is_last)
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

	virtual void OnQryTrade(int req_id, TradeData* order_data, const std::string& err, bool is_last)
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

	virtual void OnQryAccount(int req_id, AccountData* acco_data, const std::string& err, bool is_last)
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

	virtual void OnQryPosition(int req_id, PositionData* pos_data, const std::string& err, bool is_last)
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

	/****************************** ������������ ******************************/
	string serverAddr  = "118.114.77.84:7785"; //UFX��������ַ
	HSTradeApi             callback1;
	CConnectionInterface* connection1 = NULL;

	//��������
	int ret = Connect(serverAddr.c_str(), &connection1, &callback1);
	if (ret != 0)
	{
		cout << "��������ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(ret) << endl;
		connection1->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "�������ӳɹ���" << endl;
	}
	/****************************** ����������� ******************************/

	//���������߳�
	ghThreadReportProcess = CreateThread(NULL,
		0,
		startHeartbeatThread,
		(LPVOID)connection1,
		0,
		NULL
		);
	if (ghThreadReportProcess == NULL)
	{
		cout << "���������߳�ʧ�ܡ�" << endl;
		getchar();
		return -1;
	}

	/****************************** ������������ ******************************/
	HSTradeApi            callback2;
	HSSubApi         subCallback;
	CSubscribeInterface*  subscribe = NULL;
	CConnectionInterface* connection2 = NULL;

	//��������
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
		cout << "�������ӳɹ���" << endl;
	}
	/****************************** ����������� ******************************/


	int iOrderID = 0;
	while (1)
	{
		cout << endl;
		cout << "1����¼ 2��֤ȯ����ί�� 3������ί�� 4������ 0���˳�" << endl;
		cout << "������ָ��ţ�";
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
				//��¼
				int handler = Login(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "��¼ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 2:
			{
				//֤ȯ����ί��
				int handler = SingleEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "ί��ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 3:
			{
				//����ί��
				int handler = BasketEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "ί��ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 4:
			{
				//����
				int handler = Withdraw(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "����ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		default:
			{
				cout << "�����ָ��Ų���ȷ��" << endl;
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

	cout << "��½�ɹ�" << endl;

	int chose = 0;
	int ret = 0;
	bool bRet = false;
	printf("------------------------------��ӭʹ��HS demo------------------------------\n");
	printf(" 1.֤ȯί��\n 2.�ֲֲ�ѯ\n 3.�ʽ��ѯ\n 4.ί�в�ѯ\n 5.�ɽ���ѯ\n 6.����\n 7.�ɳ�ί�в�ѯ\n 0.�˳�ϵͳ\n ��������Ĳ���:");

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
		default:printf("������Ĳ�������ȷ,��������ȷ�Ĳ���!\n");
			printf("�밴�������ʾ���˵�!:\n");
			break;
		}
		if (ret < 0 || !bRet)
		{
			cout << "����ʧ��" << endl;
			system("pause");
			return -1;
		}
		cout << "����ɹ�" << endl;
		getchar();
		printf(" 1.֤ȯί��\n 2.�ֲֲ�ѯ\n 3.�ʽ��ѯ\n 4.ί�в�ѯ\n 5.�ɽ���ѯ\n 6.����\n 7.�ɳ�ί�в�ѯ\n 0.�˳�ϵͳ\n ��������Ĳ���:");		
	}

	system("pause");*/
	return 0;
}