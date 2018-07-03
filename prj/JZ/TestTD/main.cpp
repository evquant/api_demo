#include <iostream>
#include <string>
#include "../JZTradeApi/JZTradeApi.h"

using namespace std;
using namespace kiiik;

class MyTradeSpi : public TradeSpi {
	virtual void OnDisconnect(const std::string& reson) 
	{
		cout << "断线" << endl;
	}

	virtual void OnConnect() 
	{
		cout << "登陆成功" << endl;
	}

	virtual void OnQryOrder(int req_id, OrderData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "OnQryOrder 返回空 " << err << endl;
			return;
		}
		cout << endl << "查询订单回报" << endl 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "status: " << order_data->status << endl
			<< "买卖: " << (order_data->direction == LONG_DIRECTION ? "买" : "卖") << endl
			<< "开平: " << (order_data->open_close_flag == OPEN_ORDER ? "开" : "平") << endl
			<< "price: " << order_data->limit_price << endl
			<< "total_volume: " << order_data->total_volume << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	virtual void OnQryTrade(int req_id, TradeData* order_data, const std::string& err, bool is_last)
	{
		if (NULL == order_data)
		{
			cout << "OnQryTrade返回空: " << err << endl;
			return;
		}
		cout << endl << "查询成交回报" << endl 
			<< "symbol: " << order_data->symbol.instrument << endl
			<< "order_id: " << order_data->order_id << endl
			<< "user_tag: " << order_data->user_tag << endl
			<< "trade_id: " << order_data->trade_id << endl
			<< "买卖: " << (order_data->direction == LONG_DIRECTION ? "买" : "卖") << endl
			<< "开平: " << (order_data->open_close_flag == OPEN_ORDER ? "开" : "平") << endl
			<< "price: " << order_data->trade_price << endl
			<< "trade_volume: " << order_data->trade_volume << endl;
	}

	virtual void OnQryAccount(int req_id, AccountData* acco_data, const std::string& err, bool is_last)
	{
		if (NULL == acco_data)
		{
			cout << "OnQryAccount返回空: " << err << endl;
			return;
		}
		cout << endl << "查询资金回报" << endl 
			<< "资金账号: " << acco_data->account_id << endl
			<< "总资产: " << acco_data->asset_balance << endl
			<< "资金余额: " << acco_data->fund_balance << endl
			<< "可用资金: " << acco_data->enable_balance << endl
			<< "可取资金: " << acco_data->withdraw_balance << endl
			<< "股票市值: " << acco_data->market_value << endl;
	}

	virtual void OnQryPosition(int req_id, PositionData* pos_data, const std::string& err, bool is_last)
	{
		if (NULL == pos_data)
		{
			cout << "查持仓返回空" << err << endl;
			return;
		}
		cout << "股票代码: " << pos_data->symbol.instrument << "\t"
			<< "最新价: " << pos_data->last_price << "\t"
			<< "成本价: " << pos_data->open_price << "\t"
			<< "股票余额: " << pos_data->current_volume << "\t"
			<< "拥股数量: " << pos_data->open_volume << "\t"
			<< "可用数量: " << pos_data->enable_volume << "\t"
			<< "证券市值: " << pos_data->position_cost << "\t"
			<< "浮动盈亏: " << pos_data->position_profit << endl;
		if (is_last)
			cout << endl << "查询持仓回报完毕" << endl ;
	}

	virtual void OnInstrumentInfo(const BaseInstrumentInfo& info, bool is_last) {
		StockInstrumentInfo *stock_info = (StockInstrumentInfo*)(&info);
		cout << stock_info->symbol.instrument << " " << stock_info->symbol.name << ": " 
			<< "涨停价"<< stock_info->max_rise_value << " " << "跌停价" << stock_info->max_down_value
			<< "状态"<< stock_info->status << endl;
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
	
	printf(" 1.证券委托\n 2.持仓查询\n 3.资金查询\n 4.委托查询\n 5.成交查询\n 6.撤单\n 7.合约信息\n 0.退出系统\n 请输入你的操作:");
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
				printf(" 输入:\n 1)六位股票代码(上证的指数请在末尾加SH)\n 2)买卖方向(0：买 1：卖)\n 3)价格(单位 元)\n 4)数量(单位 股)\n");
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
			cout << "请求失败：" << err << endl;
		else
			cout << "请求成功" << endl;
		system("pause");
		printf(" 1.证券委托\n 2.持仓查询\n 3.资金查询\n 4.委托查询\n 5.成交查询\n 6.撤单\n 7.合约信息\n 0.退出系统\n 请输入你的操作:");
	} while (false);

	system("pause");
}