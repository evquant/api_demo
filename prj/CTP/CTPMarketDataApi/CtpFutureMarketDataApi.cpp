
#include <algorithm>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "CtpFutureMarketDataApi.h"

namespace kiiik {

/*------------------------------------------------------------------*
 |					CtpFutureMarketDataHandler模块					    |
 *------------------------------------------------------------------*/

void CtpFutureMarketDataHandler::OnRspError(CThostFtdcRspInfoField *response_infomation, int request_id, bool is_last)
{
	char request_id_str[16];
	sprintf_s(request_id_str, 16, "%d", request_id);
	api_->spi_->OnError("common", response_infomation->ErrorMsg, request_id_str);
}

void CtpFutureMarketDataHandler::OnFrontDisconnected(int reason)
{
	api_->m_succed_connect = false;
	api_->m_succed_login = false;
	reconn_ = true;

	std::string error_msg = "CTP front disconnect -> ";
	switch(reason) {
	case 0x1001:
		error_msg += "socket read error";
		break;
	case 0x1002:
		error_msg += "socket write error";
		break;
	case 0x2001:
		error_msg += "heart beat timeout";
		break;
	case 0x2002:
		error_msg += "send heart beat error";
		break;
	case 0x2003:
		error_msg += "receive error message";
		break;
	default:
		error_msg += "other error";
	}

	api_->spi_->OnDisconnect(error_msg);
}

void CtpFutureMarketDataHandler::OnHeartBeatWarning(int time_lapse)
{
	char error_msg[64];
	sprintf_s(error_msg, 64, "heart beat warning -> have not receive heart beat in %d seconds", time_lapse);
	api_->spi_->OnDisconnect(error_msg);
}

void CtpFutureMarketDataHandler::OnFrontConnected()
{
	api_->m_succed_connect = true;

	if (!reconn_) {	 //主动登录的返回
		api_->ReleaseWait();
	} 
	else if (api_->m_api_valid) {	//断线自动重连
		std::string err;
		if (!api_->ReLogin(err)) {
			char request_id_str[16];
			sprintf_s(request_id_str, 16, "%d", api_->m_request_id);
			api_->spi_->OnError("ReLogin", err, request_id_str);
		}
		else {	//登录成功，重新请求行情
			api_->spi_->OnConnect();
			if (!api_->ReReqMarketPrice(err)) {
				char request_id_str[16];
				sprintf_s(request_id_str, 16, "%d", api_->m_request_id);
				api_->spi_->OnError("ReReqMarketPrice", err, request_id_str);
			}
		}

		reconn_ = false;
	}
}

void CtpFutureMarketDataHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *rsp_user_login, 
		CThostFtdcRspInfoField *response_infomation, int request_id, bool is_last)
{
	if (IsErrorInfo(response_infomation)) {
		api_->error_msg_ = std::string("error on response ReqUserLogin ->") + response_infomation->ErrorMsg;
	} 
	else {
		api_->m_succed_login = true;
	}

	api_->ReleaseWait();
}

void CtpFutureMarketDataHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *specific_instrument, 
		CThostFtdcRspInfoField *response_infomation, int request_id, bool is_last)
{
	if (is_last && IsErrorInfo(response_infomation)) {
		char request_id_str[128];
		sprintf_s(request_id_str, 128, "%d", request_id);
		api_->spi_->OnError("SubscribeMarketData", response_infomation->ErrorMsg, request_id_str);
	}
}

void CtpFutureMarketDataHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *specific_instrument, 
		CThostFtdcRspInfoField *response_infomation, int request_id, bool is_last)
{
	if (is_last && IsErrorInfo(response_infomation)) {
		char request_id_str[128];
		sprintf_s(request_id_str, 128, "%d", request_id);
		api_->spi_->OnError("UnSubscribeMarketData", response_infomation->ErrorMsg, request_id_str);
	}
}

ExchangeIdType GetExchangeId(const char* id)
{
	if (strcmp(id, "CFFEX") == 0) { return EXCHANGE_CFFEX; }
	else if (strcmp(id, "CZCE") == 0) {  return EXCHANGE_CZCE; }
	else if (strcmp(id, "DCE") == 0) { return EXCHANGE_DCE; }
	else if (strcmp(id, "SHFE") == 0) { return EXCHANGE_SHFE; }
	else if (strcmp(id, "INE") == 0) { return EXCHANGE_INE; }
	else if (strcmp(id, "SSE") == 0) { return EXCHANGE_SSE; }
	else if (strcmp(id, "SZE") == 0) { return EXCHANGE_SZE; }
	else { return EXCHANGE_OTHER; }
}


void CtpFutureMarketDataHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *depth_market_data)
{
	/*printf("%s:%g %g %g \n", depth_market_data->InstrumentID, depth_market_data->OpenInterest, depth_market_data->BidPrice1, depth_market_data->AskPrice1);
	return;*/
	if (strlen(depth_market_data->InstrumentID) > 8) { // 期货合约名的长度一般为6位或5位
		OptionTick tick;
		tick.symbol.product = PRODUCT_OPTION;
		strcpy(tick.symbol.instrument, depth_market_data->InstrumentID);
		//期权合约所属交易所的信息 ?
		Time tick_time(depth_market_data->UpdateTime, depth_market_data->UpdateMillisec);
		Date tick_date(depth_market_data->TradingDay);
		tick.date_time = SimpleDateTime(SimpleDate(tick_date.year, tick_date.month, tick_date.day)
			, SimpleTime(tick_time.hour, tick_time.minute, tick_time.sec, tick_time.milsec));

		tick.last_price = depth_market_data->LastPrice;
		tick.volume = depth_market_data->Volume;		
		tick.amount = depth_market_data->Turnover;
		tick.pre_close = depth_market_data->PreClosePrice;
		tick.today_open = depth_market_data->OpenPrice;
		tick.today_high = depth_market_data->HighestPrice;
		tick.today_low = depth_market_data->LowestPrice;
		tick.position = depth_market_data->OpenInterest;
		tick.sell_price = depth_market_data->AskPrice1;
		tick.sell_volume = depth_market_data->AskVolume1;
		tick.buy_price = depth_market_data->BidPrice1;
		tick.buy_volume = depth_market_data->BidVolume1;
		//计算期权的风险指标
		api_->spi_->OnMarketPrice(&tick);
		return;
	}

	FutureTick tick;
	strcpy(tick.symbol.instrument, depth_market_data->InstrumentID);
	char exc_str[6] = {0};
	sscanf(tick.symbol.instrument, "%[^0-9]", exc_str);
	tick.symbol.exchange = GetExchangeId(depth_market_data->ExchangeID);
	tick.symbol.product = PRODUCT_FUTURE;

	Time tick_time(depth_market_data->UpdateTime, depth_market_data->UpdateMillisec);
	Date tick_date(depth_market_data->TradingDay);
	tick.date_time = SimpleDateTime(SimpleDate(tick_date.year, tick_date.month, tick_date.day)
		, SimpleTime(tick_time.hour, tick_time.minute, tick_time.sec, tick_time.milsec));
	//cout << tick.symbol.instrument << "  " << tick.date_time.Str() << endl;
	tick.last_price = depth_market_data->LastPrice;
	tick.volume = depth_market_data->Volume;
	tick.position = depth_market_data->OpenInterest;
	tick.sell_price = depth_market_data->AskPrice1;
	tick.sell_volume = depth_market_data->AskVolume1;
	tick.buy_price = depth_market_data->BidPrice1;
	tick.buy_volume = depth_market_data->BidVolume1;
	tick.pre_close = depth_market_data->PreClosePrice;
	tick.today_open = depth_market_data->OpenPrice;
	tick.today_high = depth_market_data->HighestPrice;
	tick.today_low = depth_market_data->LowestPrice;
	tick.amount = depth_market_data->Turnover;
	tick.pre_settlement = depth_market_data->PreSettlementPrice;

	tick.pre_open_interest = depth_market_data->PreOpenInterest;
	tick.up_limit = depth_market_data->UpperLimitPrice;
	tick.drop_limit = depth_market_data->LowerLimitPrice;

	api_->spi_->OnMarketPrice(&tick);
}

bool CtpFutureMarketDataHandler::IsErrorInfo(CThostFtdcRspInfoField *response_infomation) {
	// 如果ErrorID != 0, 说明收到了错误的响应
	return (response_infomation) && (response_infomation->ErrorID != 0);
}

/*------------------------------------------------------------------*
 |					CtpFutureMarketDataApi模快						|
 *------------------------------------------------------------------*/

CtpFutureMarketDataApi::CtpFutureMarketDataApi(void)
		:m_ctp_market_api(NULL)
		,m_ctp_market_handler(NULL)
		,m_request_id(0) 
		,m_succed_connect(false)
		,m_succed_login(false)
{
}


CtpFutureMarketDataApi::~CtpFutureMarketDataApi(void) 
{
	if (NULL != m_ctp_market_api)
	{
		m_ctp_market_api->Release();
	}
	
	if (NULL != m_ctp_market_handler) { delete m_ctp_market_handler; }
}

bool CtpFutureMarketDataApi::Init(const std::string& front_addr_str, MarketDataSpi* spi, std::string& err) 
{
	spi_ = spi;
	m_ctp_market_api = CThostFtdcMdApi::CreateFtdcMdApi();
	m_ctp_market_handler = new CtpFutureMarketDataHandler(this);

	m_ctp_market_api->RegisterSpi(m_ctp_market_handler);
	char front_addr[128];
	memcpy(front_addr, front_addr_str.c_str(), 128);
	m_ctp_market_api->RegisterFront(front_addr);
	m_ctp_market_api->RegisterFront(front_addr);
	m_ctp_market_api->RegisterFront(front_addr);
	m_ctp_market_api->Init();

	if (!TimeWait(kInitWaitTime))
	{
		m_ctp_market_api->Release();
		err = "time out for return of RegisterFront";
		return false;
	}
	if (!m_succed_connect) { 
		m_ctp_market_api->Release();
		err = error_msg_; 
		return false; 
	}

	m_api_valid = true;
	return true;
}

bool CtpFutureMarketDataApi::Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err) 
{
	if ("" == broker_id /*|| "" == user_id || "" == password*/) 
	{
		err = "the arguemnt is empty";
		return false;
	}

	m_broker_id = broker_id;
	m_user_id = user_id;
	m_password = password;

	if (!m_succed_connect) 
	{
		err = "has not connected to CTP front address";
		return false;
	}

	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), broker_id.c_str());
	strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), user_id.c_str());
	strcpy_s(req.Password, sizeof(TThostFtdcPasswordType), password.c_str());
	int result = m_ctp_market_api->ReqUserLogin(&req, ++m_request_id);
	if (0 != result) 
	{
		err = "fail to ReqUserLogin";
		return false;
	}

	if (!TimeWait(kLoginWaitTime)) 
	{
		err = "time out for return of ReqUserLogin";
		return false;
	}

	if (!m_succed_login) { 
		err = error_msg_;
		return false; 
	}

	return true;
}

bool CtpFutureMarketDataApi::Logout(std::string& err)
{
	m_symbols.clear();
	CThostFtdcUserLogoutField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), m_broker_id.c_str());
	strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), m_user_id.c_str());
	int result = m_ctp_market_api->ReqUserLogout(&req, ++m_request_id);
	if (0 != result) {
		err = "fail to ReqUserLogout";
		return false;
	}

	m_succed_login = false;
	m_succed_connect = false;
	m_ctp_market_api->Release();
	m_api_valid = false;
	return true;
}

bool CtpFutureMarketDataApi::Subscribe(const std::string& symbol, std::string& err) 
{
	 std::vector<std::string> inst_vec;
	 Split(symbol, ",", &inst_vec);
	 int len = inst_vec.size();
	 char** insts  = new char*[len];
	 for (int i=0; i < len; ++i)
	 {
		 insts[i] = new char[inst_vec[i].size() + 1];
		 strcpy(insts[i], inst_vec[i].c_str());
	 }

	if (m_ctp_market_api == NULL) { err = "Not Init !"; return false; }
	int result = m_ctp_market_api->SubscribeMarketData(insts, len);
	if (0 != result) 
	{
		for (int i=0; i < len; ++i)
		{
			delete [] insts[i];
		}
		delete [] insts;

		err = "fail to SubscribeMarketData";
		return false;
	}

	for (int i=0; i < len; ++i)
	{
		m_symbols.push_back(inst_vec[i]);
		delete [] insts[i];
	}
	delete [] insts;
	return true;
}

bool CtpFutureMarketDataApi::UnSubscribe(const std::string& symbol, std::string& err) 
{
	std::list<std::string>::iterator ite;
	ite = std::find(m_symbols.begin(), m_symbols.end(), symbol);
	if (ite == m_symbols.end()) { return false; }	//没订阅过，直接返回

	char* symbols[1] = {const_cast<char*>(symbol.c_str())};
	if (m_ctp_market_api == NULL) { err = "Not Init !"; return false; }
	int result = m_ctp_market_api->UnSubscribeMarketData(symbols, 1);
	if (0 != result) 
	{
		err = "fail to UnSubscribeMarketData";
		return false;
	}

	m_symbols.erase(ite);
	return true;
}



void CtpFutureMarketDataApi::Trim(std::string &s) 
{
	if (s.empty()) 
	{
		return;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}

void CtpFutureMarketDataApi::Split(const std::string& s, const std::string delim,std::vector<std::string>* ret)
{
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		std::string part_str = s.substr(last,index-last);
		Trim(part_str);
		if (!part_str.empty() && std::find(m_symbols.end(), m_symbols.end(), part_str) == m_symbols.end()) 
		{ 
			ret->push_back(part_str); 
		}
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index - last > 0)  
	{  
		std::string part_str = s.substr(last,index-last);
		Trim(part_str);
		if (!part_str.empty() && std::find(m_symbols.end(), m_symbols.end(), part_str) == m_symbols.end()) 
		{ 
			ret->push_back(part_str); 
		}
	}  
}


bool CtpFutureMarketDataApi::ReLogin(std::string& err) 
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), m_broker_id.c_str());
	strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), m_user_id.c_str());
	strcpy_s(req.Password, sizeof(TThostFtdcPasswordType), m_password.c_str());
	int result = m_ctp_market_api->ReqUserLogin(&req, ++m_request_id);
	if (0 != result) {
		err = "fail to ReqUserLogin";
		return false;
	}

	return true;
}

bool CtpFutureMarketDataApi::ReReqMarketPrice(std::string& err) 
{
	char* symbols[100];
	int num = 0;
	std::list<std::string>::const_iterator kite;
	for(kite = m_symbols.begin(); kite != m_symbols.end() && num < 100; ++kite) {
		symbols[num] = const_cast<char*>((*kite).c_str());
		++num;
	}

	int result = m_ctp_market_api->SubscribeMarketData(symbols, num);
	if (0 != result) {
		err = "fail to SubscribeMarketData";
		return false;
	}

	return true;
}

}
