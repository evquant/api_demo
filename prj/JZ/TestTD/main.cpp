#include <iostream>
#include <string>
#include "../JZTradeApi/JZTradeApi.h"

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

ExchangeIdType GetMarketByStockCode(const std::string &strStockCode) {
	if(strStockCode.empty()) return EXCHANGE_OTHER;

	if (strStockCode.back() == 'H') return EXCHANGE_SSE;
	else if (strStockCode.back() == 'Z') return EXCHANGE_SZE;
	else return EXCHANGE_OTHER;		
}

void main() {
	TradeSpi *spi = new MyTradeSpi;
	JZTradeApi *api = new JZTradeApi;
	string err;
	if (!api->Init("114.141.166.21", 21000, "KCBP01", "KCXP00", "888888", "req_zb", "ans_zb", spi, err)) {
		cout << err;
		system("pause");
		return;
	}
	if (!api->Login("2102", "7133", "990990", "110010014533", err))  {
		cout << err;
		system("pause");
		return;
	}
	
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
				char inst[10] = {0};
				int dir, volum;
				double price;
				printf(" ����:\n 1)��λ��Ʊ����(��֤��ָ������ĩβ��SH)\n 2)��������(0���� 1����)\n 3)�۸�(��λ Ԫ)\n 4)����(��λ ��)\n");
				scanf("%s%d%lf%d", &inst, &dir, &price, &volum);
				OrderParamData param;
				param.limit_price = price;
				param.volume = volum;
				strcpy(param.symbol.instrument, std::string(inst).substr(0,6).c_str());
				param.symbol.exchange = GetMarketByStockCode(inst);
				param.direction = dir==0? LONG_DIRECTION:SHORT_DIRECTION;
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