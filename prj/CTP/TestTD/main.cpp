#include <iostream>
#include <string>
#include "../CTPTradeApi/CtpFutureTradeApi.h"

using namespace std;
using namespace kiiik;

class MyTradeSpi : public TradeSpi {
	virtual void OnDisconnect(const std::string& reson) 
	{
		cout << "����" << endl;
	}

	virtual void OnConnect() 
	{
		cout << "��½�ɹ�" << endl;
	}

	virtual void OnQryOrder(int req_id, OrderData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "OnQryOrder ���ؿ� " << err << endl;
			return;
		}
		cout << endl << "��ѯ�����ر�" << endl 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "status: " << order_data->status << endl
			<< "����: " << (order_data->direction == LONG_DIRECTION ? "��" : "��") << endl
			<< "��ƽ: " << (order_data->open_close_flag == OPEN_ORDER ? "��" : "ƽ") << endl
			<< "price: " << order_data->limit_price << endl
			<< "total_volume: " << order_data->total_volume << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	virtual void OnQryTrade(int req_id, TradeData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "OnQryTrade���ؿ�: " << err << endl;
			return;
		}
		cout << endl << "��ѯ�ɽ��ر�" << endl 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "trade_id: " << order_data->trade_id << endl
			<< "����: " << (order_data->direction == LONG_DIRECTION ? "��" : "��") << endl
			<< "��ƽ: " << (order_data->open_close_flag == OPEN_ORDER ? "��" : "ƽ") << endl
			<< "price: " << order_data->trade_price << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	virtual void OnQryAccount(int req_id, AccountData* acco_data, const std::string& err, bool is_last)
	{
		if (NULL == acco_data)
		{
			cout << "OnQryAccount���ؿ�: " << err << endl;
			return;
		}
		cout << endl << "��ѯ�ʽ�ر�" << endl 
			<< "�ʽ��˺�: " << acco_data->account_id << endl
			<< "���ʲ�: " << acco_data->asset_balance << endl
			<< "�ʽ����: " << acco_data->fund_balance << endl
			<< "�����ʽ�: " << acco_data->enable_balance << endl
			<< "��ȡ�ʽ�: " << acco_data->withdraw_balance << endl
			<< "��Ʊ��ֵ: " << acco_data->market_value << endl;
	}

	virtual void OnQryPosition(int req_id, PositionData* pos_data, const std::string& err, bool is_last)
	{
		if (NULL == pos_data)
		{
			cout << "��ֲַ��ؿ�" << err << endl;
			return;
		}
		cout << "��Ʊ����: " << pos_data->symbol.instrument << "\t"
			<< "���¼�: " << pos_data->last_price << "\t"
			<< "�ɱ���: " << pos_data->open_price << "\t"
			<< "��Ʊ���: " << pos_data->current_volume << "\t"
			<< "ӵ������: " << pos_data->open_volume << "\t"
			<< "��������: " << pos_data->enable_volume << "\t"
			<< "֤ȯ��ֵ: " << pos_data->position_cost << "\t"
			<< "����ӯ��: " << pos_data->position_profit << endl;
		if (is_last)
			cout << endl << "��ѯ�ֲֻر����" << endl ;
	}

	virtual void OnInstrumentInfo(const BaseInstrumentInfo& info, bool is_last) {
		StockInstrumentInfo *stock_info = (StockInstrumentInfo*)(&info);
		cout << stock_info->symbol.instrument << " " << stock_info->symbol.name << ": " 
			<< "��ͣ��"<< stock_info->max_rise_value << " " << "��ͣ��" << stock_info->max_down_value
			<< "״̬"<< stock_info->status << endl;
	}
};

void main() {
	TradeSpi *spi = new MyTradeSpi;
	CtpFutureTradeApi *api = new CtpFutureTradeApi;
	string err;
	//if (!api->Init("tcp://210.13.65.102:13159", spi, err)) {
	if (!api->Init("tcp://180.168.146.187:10000", spi, err)) {//ģ��
		cout << err;
		system("pause");
		return;
	}
	//if (!api->Login("7070", "901596505", "123456", err))  { //wudianʵ��
	if (!api->Login("9999", "002907", "123456", err))  {//ģ��
		cout << err;
		system("pause");
		return;
	}
	if (!api->InitPreTrade(err))  {
		cout << err;
		system("pause");
		return;
	}

	printf("login success\n");
	
	printf(" 1.֤ȯί��\n 2.�ֲֲ�ѯ\n 3.�ʽ��ѯ\n 4.ί�в�ѯ\n 5.�ɽ���ѯ\n 6.����\n 7.��Լ��Ϣ\n 0.�˳�ϵͳ\n ��������Ĳ���:");
	int chose;
	int ret;
	while(scanf("%d",&chose), 0 != chose)
	{
		//getchar();
		switch (chose)
		{
		case 1:
			{
				/*char exeid;
				char inst[10] = {0};
				int dir, open_close_flag, volum;
				double price;
				printf(" ����:\n 0)����������:2�н�3����4֣��5����\n 1)�ڻ���Լ����\n 2)��������(0���� 1����)\n ��ƽ��ʶ��0�� 1ƽ��3)�۸�(��λ Ԫ)\n 4)����(��λ ��)\n");
				scanf("%c%s%d%d%lf%d", &exeid, &inst, &dir, &open_close_flag, &price, &volum);*/
				OrderParamData param;
				param.limit_price = 11980;
				param.volume = 1;
				strcpy(param.symbol.instrument, "CF601");
				param.symbol.exchange = EXCHANGE_CZCE;
				param.direction = SHORT_DIRECTION;
				param.open_close_flag = OPEN_ORDER;
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
				long long order_id;
				cout << "input order id: " << endl;
				cin >>order_id;
				//sprintf(param.order_id, "%12d", order_id);
				param.order_id = order_id;
				//param.symbol.exchange = EXCHANGE_DCE;
				ret = api->CancelOrder(param, err);
			}
			break;
		case 7: 
			{
				ret = true == api->QryInstrument(err);					
			}
			break;
		default:
			return;
		}
		if (ret < 0)
			cout << "����ʧ�ܣ�" << err << endl;
		else
			cout << "����ɹ�" << endl;
		system("pause");
		printf(" 1.֤ȯί��\n 2.�ֲֲ�ѯ\n 3.�ʽ��ѯ\n 4.ί�в�ѯ\n 5.�ɽ���ѯ\n 6.����\n 7.��Լ��Ϣ\n 0.�˳�ϵͳ\n ��������Ĳ���:");
	} while (false);

	system("pause");
}