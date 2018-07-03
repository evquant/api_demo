#include "HSTradeApi.h"

namespace kiiik {

HSTradeApi::HSTradeApi(void) : conn_(NULL), config_(NULL), is_login_(false)	, broker_id_(0)
{
}


HSTradeApi::~HSTradeApi(void)
{
}

bool HSTradeApi::Init(const std::string& conf_path, TradeSpi* spi, std::string& err){
	spi_ = spi;
	config_ = NewConfig();
	if (NULL == config_)
	{
		err = "配置文件对象创建失败";
		return false;
	}
	///NewConfig()返回的接口，必须调用AddRef，否则调用Release接口就无法释放
	config_->AddRef();
	if (config_->Load(conf_path.c_str())) {
		err = "Load config file fail";
		return false;
	}

	conn_ = NewConnection(config_);
	if (NULL == conn_)
	{
		err = "连接对象创建失败";
		return false;
	}
	conn_->AddRef();
	int ret = conn_->CreateEx(this);
	if (0 != ret)
	{
		err = conn_->GetErrorMsg(ret);
		return false;
	}
	ret = conn_->Connect(5000);
	if (0 != ret)
	{
		err = std::string("连接失败：") + std::string(conn_->GetErrorMsg(ret));
		return false;
	}

	return true;
}

bool HSTradeApi::Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err){

	s_broker_id_ = broker_id;
	broker_id_ = atoi(broker_id.c_str());
	user_id_ = user_id;
	password_ = password;
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return false;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);  
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);     
	pPacker->AddField("input_content", 'C'); 
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);  
	pPacker->AddField("password", 'S', 10);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');				
	pPacker->AddStr("1");				
	pPacker->AddInt(broker_id_);			
	pPacker->AddChar('1');					
	pPacker->AddStr(user_id.c_str());			
	pPacker->AddStr("0");				
	pPacker->AddStr(password.c_str());

	/*pPacker->AddField("op_entrust_way", 'C', 1);  
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("vc_ip_address", 'S', 15); 
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("password", 'S', 10);
	

	pPacker->AddChar('0');	
	pPacker->AddStr("2C27D739AEAE");	
	pPacker->AddStr("211.144.202.181");	
	pPacker->AddStr(user_id.c_str());				
	pPacker->AddStr(password.c_str());*/

	pPacker->EndPack();
	int ret = conn_->SendBiz(331100,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return false;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	
	bool timed_ret = TimeWait(5);
	if (!timed_ret)
	{
		err = "连接超时";
		return false;
	}

	if (!is_login_)
	{
		err = error_msg_;
		return false;
	}

	return true;
}

bool HSTradeApi::Logout(std::string& err){
	return true;
}

int HSTradeApi::QueryAccount(std::string& err)
{
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);  
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);     
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');				
	pPacker->AddStr("1");				
	pPacker->AddInt(broker_id_);			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(password_.c_str());				
	pPacker->AddChar('2');					
	pPacker->AddStr(user_token_);

	pPacker->EndPack();
	int ret = conn_->SendBiz(332255,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	return ret;
}

int HSTradeApi::QueryPosition(std::string& err)
{
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');					
	pPacker->AddStr("1");					
	pPacker->AddInt(broker_id_);				
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(password_.c_str());				
	pPacker->AddChar('2');					
	pPacker->AddStr(user_token_);

	pPacker->EndPack();
	int ret = conn_->SendBiz(333104,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	return ret;
}

int HSTradeApi::QueryOrder(std::string& err)
{
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');					
	pPacker->AddStr("1");					
	pPacker->AddInt(broker_id_);				
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(password_.c_str());				
	pPacker->AddChar('2');					
	pPacker->AddStr(user_token_);

	pPacker->EndPack();
	int ret = conn_->SendBiz(333101,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	return ret;
}

int HSTradeApi::QueryTrade(std::string& err)
{
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("request_num", 'I', 8);
	pPacker->AddField("report_no", 'I', 8);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');					
	pPacker->AddStr("1");					
	pPacker->AddInt(broker_id_);				
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(password_.c_str());				
	pPacker->AddChar('2');					
	pPacker->AddStr(user_token_);
	pPacker->AddInt(200);
	pPacker->AddInt(11824);

	pPacker->EndPack();
	int ret = conn_->SendBiz(333102,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	return ret;
}

int HSTradeApi::SubmitOrder(const OrderParamData& param, std::string& err)
{
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);

	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'F', 18, 3);
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("entrust_prop", 'S', 3);	

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');					
	pPacker->AddStr("1");					
	pPacker->AddInt(broker_id_);				
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(password_.c_str());				
	pPacker->AddChar('2');					
	pPacker->AddStr(user_token_);

	if (param.symbol.exchange == EXCHANGE_SSE) {
		pPacker->AddStr("1");
	} else if (param.symbol.exchange == EXCHANGE_SZE) {
		pPacker->AddStr("2");
	}
	else {
		err = "交易所参数错误";
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}
	pPacker->AddStr(param.symbol.instrument);				
	pPacker->AddDouble(param.volume);				
	pPacker->AddDouble(param.limit_price);
	//TODO:查以下两个字段的字典
	if (param.direction == LONG_DIRECTION) {
		pPacker->AddStr("1");
	}
	else {
		pPacker->AddStr("2");
	}					
	pPacker->AddStr("0");

	pPacker->EndPack();
	int ret = conn_->SendBiz(333002,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	order_param_cache_[ret] = param;
	return ret;
}

int HSTradeApi::CancelOrder(const OrderData& param, std::string& err)
{
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		err = "取打包器失败";
		return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("entrust_no", 'I', 8);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('5');					
	pPacker->AddStr("1");					
	pPacker->AddInt(broker_id_);				
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(user_id_.c_str());			
	pPacker->AddStr(password_.c_str());				
	pPacker->AddChar('2');	
	pPacker->AddStr(user_token_);
	if (param.symbol.exchange == EXCHANGE_SSE) {
		pPacker->AddStr("1");
	} 
	else if (param.symbol.exchange == EXCHANGE_SZE) {
		pPacker->AddStr("2");
	}
	else {
		err = "交易所参数错误";
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}
	pPacker->AddInt(param.order_id);

	pPacker->EndPack();
	int ret = conn_->SendBiz(333017,pPacker, ASYNCSEND, 0);
	if (0 > ret)
	{
		err = conn_->GetErrorMsg(ret);
		pPacker->FreeMem(pPacker->GetPackBuf());
		pPacker->Release();
		return -1;
	}

	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	cancel_order_param_cache_[ret] = param;
	return ret;
}

static ExchangeIdType GetExchangeId(const std::string & exc_str) {
	if (exc_str == "1") {
		return EXCHANGE_SSE;
	} 
	else if (exc_str == "2") {
		return EXCHANGE_SZE;
	}
	else {
		return EXCHANGE_OTHER;
	}
}

static OrderStatus ToOrderStatus(char status_char) {
	switch (status_char)
	{
	case '1':
		return ORDER_STATUS_WAIT_SUBMIT;
	case '2':
		return ORDER_STATUS_BEEN_SUBMIT;
	case '3':
		return ORDER_STATUS_BEEN_SUBMIT_WAIT_CANCEL;
	case '4':
		return ORDER_STATUS_PART_TRADE_WAIT_CANCEL;
	case '5':
	case '6':
		return ORDER_STATUS_BEEN_CANCEL;
	case '7':
		return ORDER_STATUS_PART_TRADE;
	case '8':
		return ORDER_STATUS_ALL_TRADE;
	default:
		return ORDER_STATUS_INVALID;
	}
}

void HSTradeApi::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	std::string err_msg = lpRetData->errorInfo != nullptr ? lpRetData->errorInfo : "";	

	IF2UnPacker *pUnPacker = (IF2UnPacker *)lpUnpackerOrStr;

	switch (lpRetData->functionID)
	{
	case 331100: // 客户登录
		{
			if(lpRetData->errorNo != 0 || lpRetData->returnCode != 0)
			{
				error_msg_ = lpRetData->errorInfo ? lpRetData->errorInfo : "登陆失败";
			}
			else
			{
				const char *user_token = pUnPacker->GetStr("user_token");
				if(user_token) 
				{
					is_login_ = true;
					strcpy(user_token_, user_token);
				}
				else
				{
					error_msg_ = "获取令牌失败";
				}
			}
			ReleaseWait();
		}
		break;
	case 332255: // 客户资金精确查询
		{
			if(lpRetData->errorNo != 0 || lpRetData->returnCode != 0)
			{
				spi_->OnQryAccount(hSend, NULL, err_msg.c_str(), true);				
				return;
			}

			AccountData account_data;
			strcpy(account_data.account_id, user_id_.c_str());
			strcpy(account_data.broker_id, s_broker_id_.c_str());
			account_data.money_type = MONEY_RMB;
			for (int i = 0; i < pUnPacker->GetDatasetCount(); ++i)
			{
				pUnPacker->SetCurrentDatasetByIndex(i);
				for (int j=0; j < pUnPacker->GetRowCount(); ++j)
				{					
					account_data.asset_balance = pUnPacker->GetDouble("asset_balance");
					account_data.enable_balance = pUnPacker->GetDouble("enable_balance");
					account_data.withdraw_balance = pUnPacker->GetDouble("fetch_balance");
					account_data.frozen_balance = pUnPacker->GetDouble("frozen_balance");
					account_data.market_value = pUnPacker->GetDouble("market_value");
					account_data.fund_balance = pUnPacker->GetDouble("fund_balance");

					spi_->OnQryAccount(hSend, &account_data, "", j+1 == pUnPacker->GetRowCount());
					pUnPacker->Next();
				}
			}
		}
		break;
	case 333104: // 查持仓
		{
			if(lpRetData->errorNo != 0 || lpRetData->returnCode != 0)
			{
				spi_->OnQryPosition(hSend, NULL, err_msg.c_str(), true);								
				return;
			}

			for (int i = 0; i < pUnPacker->GetDatasetCount(); ++i)
			{
				pUnPacker->SetCurrentDatasetByIndex(i);
				for (int j=0; j < pUnPacker->GetRowCount(); ++j)
				{
					PositionData pos_data;
					strcpy(pos_data.symbol.instrument, pUnPacker->GetStr("stock_code"));
					pos_data.symbol.product = PRODUCT_STOCK;
					pos_data.symbol.exchange = GetExchangeId(pUnPacker->GetStr("exchange_type"));
					if (pos_data.symbol.exchange == EXCHANGE_OTHER) continue;

					pos_data.last_price = pUnPacker->GetDouble("last_price");
					pos_data.open_price = pUnPacker->GetDouble("cost_price");
					pos_data.position_profit = pUnPacker->GetDouble("income_balance");
					pos_data.open_volume = (int)100 * pUnPacker->GetDouble("current_amount");					
					pos_data.position_cost = pUnPacker->GetDouble("market_value");
					pos_data.current_volume = (int)100 * pUnPacker->GetDouble("current_amount");
					pos_data.enable_volume = (int)100 * pUnPacker->GetDouble("enable_amount");

					spi_->OnQryPosition(hSend, &pos_data, "", j+1 == pUnPacker->GetRowCount());
					pUnPacker->Next();
				}
			}
		}
		break;
	case 333100: // 可撤单委托查询
	case 333101: // 证券委托查询
		{
			if(lpRetData->errorNo != 0 || lpRetData->returnCode != 0)
			{
				spi_->OnQryOrder(hSend, NULL, err_msg.c_str(), true);												
				return;
			}

			for (int i = 0; i < pUnPacker->GetDatasetCount(); ++i)
			{
				pUnPacker->SetCurrentDatasetByIndex(i);
				for (int j=0; j < pUnPacker->GetRowCount(); ++j)
				{
					OrderData order_data;
					strcpy(order_data.symbol.instrument, pUnPacker->GetStr("stock_code"));
					order_data.symbol.product = PRODUCT_STOCK;					
					order_data.symbol.exchange = GetExchangeId(pUnPacker->GetStr("exchange_type"));
					if (order_data.symbol.exchange == EXCHANGE_OTHER) continue;

					order_data.order_id = pUnPacker->GetInt("entrust_no");
					order_data.local_order_id = hSend;
					order_data.status = ToOrderStatus(pUnPacker->GetChar("entrust_status"));

					int date = pUnPacker->GetInt("entrust_date");
					order_data.submit_time.date = Date(date/10000, date%10000/100, date%100);
					int time = pUnPacker->GetInt("entrust_time");
					order_data.submit_time.time = Time(time/10000, time%10000/100, time%100, 0);
					order_data.update_time.date = order_data.submit_time.date;
					time = pUnPacker->GetInt("report_time");
					order_data.update_time.time = Time(time/10000, time%10000/100, time%100, 0);

					if ('1' == pUnPacker->GetChar("entrust_bs"))
						order_data.direction = LONG_DIRECTION;
					else
						order_data.direction = SHORT_DIRECTION;

					order_data.order_price_flag = LIMIT_PRICE_ORDER;					
					order_data.limit_price = pUnPacker->GetDouble("entrust_price");
					order_data.total_volume = (int)100 * pUnPacker->GetDouble("entrust_amount");
					order_data.trade_volume = (int)100 * pUnPacker->GetDouble("business_amount");

					spi_->OnQryOrder(hSend, &order_data, "", j+1 == pUnPacker->GetRowCount());
					pUnPacker->Next();
				}
			}
		}
		break;
	case 333102: // 查成交
		{
			if(lpRetData->errorNo != 0 || lpRetData->returnCode != 0)
			{
				spi_->OnQryTrade(hSend, NULL, err_msg.c_str(), true);																
				return;
			}

			for (int i = 0; i < pUnPacker->GetDatasetCount(); ++i)
			{
				pUnPacker->SetCurrentDatasetByIndex(i);
				for (int j=0; j < pUnPacker->GetRowCount(); ++j)
				{
					TradeData trade_data;
					strcpy(trade_data.symbol.instrument, pUnPacker->GetStr("stock_code"));
					trade_data.symbol.product = PRODUCT_STOCK;
					std::string exc_str = pUnPacker->GetStr("exchange_type");
					trade_data.symbol.exchange = GetExchangeId(pUnPacker->GetStr("exchange_type"));
					if (trade_data.symbol.exchange == EXCHANGE_OTHER) continue;

					trade_data.order_id = pUnPacker->GetInt("entrust_no");
					trade_data.local_order_id = hSend;
					trade_data.trade_id = pUnPacker->GetInt("business_no");

					trade_data.trade_time.date = DateTime(NULL).date;
					int time = pUnPacker->GetInt("business_time");
					trade_data.trade_time.time = Time(time/10000, time%10000/100, time%100, 0);

					if ('1' == pUnPacker->GetChar("entrust_bs"))
						trade_data.direction = LONG_DIRECTION;
					else
						trade_data.direction = SHORT_DIRECTION;

					trade_data.trade_price = (int)100 * pUnPacker->GetDouble("business_price");
					trade_data.trade_volume = (int)100 * pUnPacker->GetDouble("business_amount");
					//算手续费

					spi_->OnQryTrade(hSend, &trade_data, "", j+1 == pUnPacker->GetRowCount());
					pUnPacker->Next();
				}
			}
		}
		break;
	case 333002: // 普通委托回报
		{
			std::map<int, OrderParamData>::iterator iter = order_param_cache_.find(hSend);
			if (iter == order_param_cache_.end())
			{
				spi_->OnOrderError(nullptr);																				
				return;
			}

			OrderData order_data;
			order_data.symbol = iter->second.symbol;	
			strcpy(order_data.user_tag, iter->second.user_tag);
			order_data.submit_time.date = DateTime(nullptr).date;			
			order_data.update_time = DateTime(nullptr);
			order_data.direction = iter->second.direction;			
			order_data.order_price_flag = LIMIT_PRICE_ORDER;
			order_data.limit_price = iter->second.limit_price;
			order_data.total_volume = iter->second.volume;
			order_data.trade_volume = 0;			

			if (lpRetData->errorNo != 0 || lpRetData->returnCode != 0) {
				order_data.status = ORDER_STATUS_ERROR;
				strncpy(order_data.status_msg, err_msg.c_str(), sizeof(StatusMsgType));
				spi_->OnOrderError(&order_data);				
			}
			else {
				order_data.status = ORDER_STATUS_BEEN_SUBMIT;
				for (int i = 0; i < pUnPacker->GetDatasetCount(); ++i)
				{
					pUnPacker->SetCurrentDatasetByIndex(i);
					for (int j=0; j < pUnPacker->GetRowCount(); ++j)
					{					
						order_data.order_id = pUnPacker->GetInt("entrust_no");
						int time = pUnPacker->GetInt("entrust_time");
						order_data.submit_time.time = Time(time/10000, time%10000/100, time%100, 0);					
						spi_->OnOrder(&order_data);
						pUnPacker->Next();	
					}
				}
			}			

			order_param_cache_.erase(iter);	
		}
		break;
	case 333017: // 撤单回报
		{
			std::map<int, OrderData>::iterator iter = cancel_order_param_cache_.find(hSend);
			if (iter == cancel_order_param_cache_.end())
			{
				spi_->OnOrderError(nullptr);
				return;
			}

			OrderData cancel_order_data = iter->second;

			if (lpRetData->errorNo != 0 || lpRetData->returnCode != 0) {
				cancel_order_data.status = CANCEL_ORDER_STATUS_ERROR;
				strncpy(cancel_order_data.status_msg, err_msg.c_str(), sizeof(StatusMsgType));
				spi_->OnOrderError(&cancel_order_data);						
			}
			else {
				cancel_order_data.status = ORDER_STATUS_PART_TRADE_WAIT_CANCEL;
				for (int i = 0; i < pUnPacker->GetDatasetCount(); ++i)
				{					
					pUnPacker->SetCurrentDatasetByIndex(i);
					for (int j=0; j < pUnPacker->GetRowCount(); ++j)
					{
						if (0==i && 0==j) cancel_order_data.status = ORDER_STATUS_BEEN_SUBMIT_WAIT_CANCEL;
						cancel_order_data.order_id = pUnPacker->GetInt("entrust_no");
						spi_->OnOrder(&cancel_order_data);
						pUnPacker->Next();
					}
				}
			}			

			cancel_order_param_cache_.erase(iter);
		}
		break;
	default:
		{
			if (lpRetData->errorNo != 0 || lpRetData->returnCode != 0)
			{
				char err_str[1024] = {0};
				sprintf(err_str, "功能号: %d, 返回错误号: %d, 返回码: %d, 错误信息: %s!\r\n",
					lpRetData->functionID, lpRetData->errorNo, lpRetData->returnCode, err_msg.c_str());
				spi_->OnError(hSend, err_str);				
			}
		}
		break;
	}
}

}