#include <assert.h>
#include <sstream>
#include "JZTradeApi.h"
#include "kdstrapi.hpp"

using namespace std;

namespace kiiik {

static char szLoginType[6][2] = {"Z","B","C","K","X","N"}; // Z表示以资金账号登陆

ExchangeIdType GetExchangeId(const char* id) {
	if (strcmp(id, "1") == 0) { return EXCHANGE_SSE; }
	else if (strcmp(id, "2") == 0) { return EXCHANGE_SZE; }
	else { return EXCHANGE_OTHER; }
}

void JZTradeApi::OnQueryAccount(std::string& szRetMsg) {
	char szRowBuf[RESULTSROWMAX] = {'\0'};
	int nRowNum = Stock_GetRowNum(szRetMsg.c_str());
	for(int row = 0; row < nRowNum; row++){ //get all rows
		Stock_GetSeparateValue(szRetMsg.c_str(), RESULTROWSEP, row, szRowBuf, sizeof(szRowBuf)-1);
		if(szRowBuf[0] == '\0') break; //the last
		AccountData account_data;
		account_data.money_type = MONEY_RMB;
		// get cell value
		char szCellBuf[256] = {'\0'};
		int nCellNum = Stock_GetCellNum(szRowBuf);
		for(int cell = 0; cell < nCellNum; cell++){
			Stock_GetSeparateValue(szRowBuf, RESULTCELLSEP, cell, szCellBuf, sizeof(szCellBuf)-1);
			if(szCellBuf[0] == '\0') break; //the last

			char szColName[256] = {'\0'};
			char szColVal[256] = {'\0'};
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 0, szColName, sizeof(szColName)-1);
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 1, szColVal, sizeof(szColVal)-1);

			if (0 == strcmp(szColName, "fundid"))
				strcpy(account_data.account_id, szColVal);
			else if (0 == strcmp(szColName, "orgid"))
				strcpy(account_data.broker_id, szColVal);
			else if (0 == strcmp(szColName, "marketvalue"))
				account_data.asset_balance = atof(szColVal);
			else if (0 == strcmp(szColName, "fundavl"))
				account_data.enable_balance = atof(szColVal);
			else if (0 == strcmp(szColName, "fund"))
				account_data.withdraw_balance = atof(szColVal);
			else if (0 == strcmp(szColName, "stkvalue"))
				account_data.market_value = atof(szColVal);
			else if (0 == strcmp(szColName, "fundbal"))
				account_data.fund_balance = atof(szColVal);			
		}
		account_data.frozen_balance = account_data.fund_balance - account_data.enable_balance;
		spi_->OnQryAccount(0, &account_data, "", row == nRowNum-1 );
	} 
}

void JZTradeApi::OnQueryPosition(std::string& szRetMsg) {
	char szRowBuf[RESULTSROWMAX] = {'\0'};
	int nRowNum = Stock_GetRowNum(szRetMsg.c_str());
	for(int row = 0; row < nRowNum; row++){ //get all rows
		Stock_GetSeparateValue(szRetMsg.c_str(), RESULTROWSEP, row, szRowBuf, sizeof(szRowBuf)-1);
		if(szRowBuf[0] == '\0') break; //the last

		PositionData position_data;
		position_data.symbol.product = PRODUCT_STOCK;
		// get cell value
		char szCellBuf[256] = {'\0'};
		int nCellNum = Stock_GetCellNum(szRowBuf);
		for(int cell = 0; cell < nCellNum; cell++){
			Stock_GetSeparateValue(szRowBuf, RESULTCELLSEP, cell, szCellBuf, sizeof(szCellBuf)-1);
			if(szCellBuf[0] == '\0') break; //the last

			char szColName[256] = {'\0'};
			char szColVal[256] = {'\0'};
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 0, szColName, sizeof(szColName)-1);
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 1, szColVal, sizeof(szColVal)-1);

			if (0 == strcmp(szColName, "stkcode"))
				strcpy(position_data.symbol.instrument, szColVal);
			else if (0 == strcmp(szColName, "market"))
				position_data.symbol.exchange = GetExchangeId(szColVal); 
			else if (0 == strcmp(szColName, "lastprice"))
				position_data.last_price = atof(szColVal);
			else if (0 == strcmp(szColName, "costprice"))
				position_data.open_price = atof(szColVal);
			else if (0 == strcmp(szColName, "income"))
				position_data.position_profit = atof(szColVal);
			else if (0 == strcmp(szColName, "stkqty"))
				position_data.open_volume = atoi(szColVal);
			else if (0 == strcmp(szColName, "mktval"))
				position_data.position_cost = atof(szColVal);
			else if (0 == strcmp(szColName, "stkbal"))
				position_data.current_volume = atoi(szColVal);			
			else if (0 == strcmp(szColName, "stkavl"))
				position_data.enable_volume = atoi(szColVal);
		}
		spi_->OnQryPosition(0, &position_data, "", row == nRowNum-1);
	} 
}

OrderStatus ToOrderStatus(const char status_char) {
	OrderStatus status = ORDER_STATUS_INVALID;
	switch (status_char) {
	case '0': // 未报
	case 'A': // 待报
	case 'C': // 大宗未报
	case 'H': // 固收未报
		status = ORDER_STATUS_WAIT_SUBMIT;
		break;
	case '1': // 正报
	case '2': // 已报
	case 'B': // 大宗正报
	case 'D': // 大宗已报
	case 'I': // 固收正报
	case 'J': // 固收已报
		status = ORDER_STATUS_BEEN_SUBMIT;
		break;
	case '3': // 已报待撤
		status = ORDER_STATUS_BEEN_SUBMIT_WAIT_CANCEL;
		break;
	case '4': // 部成待撤
	case 'L': // 固收待撤
		status = ORDER_STATUS_PART_TRADE_WAIT_CANCEL;
		break;
	case '5': // 部撤
	case '6': // 已撤
	case 'E': // 大宗已撤
	case 'M': // 固收已撤
		status = ORDER_STATUS_BEEN_CANCEL;
		break;
	case '7': // 部成
		status = ORDER_STATUS_PART_TRADE;
		break;
	case '8': // 已成
	case 'K': // 固收已成
		status = ORDER_STATUS_ALL_TRADE;
		break;		
	case '9': // 废单
	case 'F': // 大宗作废
	case 'N': // 固收废单
		status = ORDER_STATUS_INVALID;
		break;
	case 'b':
		status = ORDER_STATUS_NOT_TOUCHED;
		break;
	case 'c':
		status = ORDER_STATUS_TOUCHED;
		break;
	default:
		status = ORDER_STATUS_INVALID;
	}

	return status;
}

void JZTradeApi::OnQueryOrder(std::string& szRetMsg) {
	char szRowBuf[RESULTSROWMAX] = {'\0'};
	int nRowNum = Stock_GetRowNum(szRetMsg.c_str());
	for(int row = 0; row < nRowNum; row++){ //get all rows
		Stock_GetSeparateValue(szRetMsg.c_str(), RESULTROWSEP, row, szRowBuf, sizeof(szRowBuf)-1);
		if(szRowBuf[0] == '\0') break; //the last

		OrderData order_data;
		// get cell value
		char szCellBuf[256] = {'\0'};
		int nCellNum = Stock_GetCellNum(szRowBuf);
		for(int cell = 0; cell < nCellNum; cell++){
			Stock_GetSeparateValue(szRowBuf, RESULTCELLSEP, cell, szCellBuf, sizeof(szCellBuf)-1);
			if(szCellBuf[0] == '\0') break; //the last

			char szColName[256] = {'\0'};
			char szColVal[256] = {'\0'};
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 0, szColName, sizeof(szColName)-1);
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 1, szColVal, sizeof(szColVal)-1);

			if (0 == strcmp(szColName, "market"))
				order_data.symbol.exchange = GetExchangeId(szColVal);
			else if (0 == strcmp(szColName, "stkcode"))
				strcpy(order_data.symbol.instrument, szColVal);
			else if (0 == strcmp(szColName, "ordersno"))
				order_data.order_id = atoi(szColVal);
			else if (0 == strcmp(szColName, "orderstatus"))
				order_data.status = ToOrderStatus(szColVal[0]);
			else if (0 == strcmp(szColName, "orderdate")) {
				Date date(szColVal);
				order_data.submit_time.date = Date(date.year, date.month, date.day);
				order_data.update_time.date = order_data.submit_time.date;
			}
			else if (0 == strcmp(szColName, "opertime")) {
				int hour, minute, sec, milsec;
				sscanf(szColVal, "%2d%2d%2d%2d", &hour, &minute, &sec, &milsec);
				order_data.submit_time.time = Time(hour, minute, sec, milsec*10);
				order_data.update_time.time = order_data.submit_time.time;
			}
			else if (0 == strcmp(szColName, "bsflag")) {
				if ('B' == szColVal[0]) {
					order_data.direction = LONG_DIRECTION;
					order_data.open_close_flag = OPEN_ORDER;
				}
				else if ('S' == szColVal[0]) {
					order_data.direction = SHORT_DIRECTION;	
					order_data.open_close_flag = CLOSE_ORDER;
				}
				else
					assert(false);
			}
			else if (0 == strcmp(szColName, "orderprice"))
				order_data.limit_price = atof(szColVal);
			else if (0 == strcmp(szColName, "orderqty"))
				order_data.total_volume = atoi(szColVal);
			else if (0 == strcmp(szColName, "matchqty"))
				order_data.trade_volume = atoi(szColVal);
			else if (0 == strcmp(szColName, "matchamt"))
				order_data.trade_volume = atof(szColVal);

			order_data.symbol.product = PRODUCT_STOCK;
		}
		spi_->OnQryOrder(0, &order_data, "", row == nRowNum-1 );
	} 
}

void JZTradeApi::OnQueryTrade(std::string& szRetMsg) {
	char szRowBuf[RESULTSROWMAX] = {'\0'};
	int nRowNum = Stock_GetRowNum(szRetMsg.c_str());
	for(int row = 0; row < nRowNum; row++){ //get all rows
		Stock_GetSeparateValue(szRetMsg.c_str(), RESULTROWSEP, row, szRowBuf, sizeof(szRowBuf)-1);
		if(szRowBuf[0] == '\0') break; //the last

		TradeData trade_data;
		trade_data.symbol.product = PRODUCT_STOCK;
		// get cell value
		char szCellBuf[256] = {'\0'};
		int nCellNum = Stock_GetCellNum(szRowBuf);
		for(int cell = 0; cell < nCellNum; cell++){
			Stock_GetSeparateValue(szRowBuf, RESULTCELLSEP, cell, szCellBuf, sizeof(szCellBuf)-1);
			if(szCellBuf[0] == '\0') break; //the last

			char szColName[256] = {'\0'};
			char szColVal[256] = {'\0'};
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 0, szColName, sizeof(szColName)-1);
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 1, szColVal, sizeof(szColVal)-1);

			if (0 == strcmp(szColName, "market"))
				trade_data.symbol.exchange = GetExchangeId(szColVal);
			else if (0 == strcmp(szColName, "stkcode"))
				strcpy(trade_data.symbol.instrument, szColVal);
			else if (0 == strcmp(szColName, "ordersno"))
				trade_data.order_id = atoi(szColVal);	
			else if (0 == strcmp(szColName, "matchcode"))
				trade_data.trade_id = atoi(szColVal);	
			else if (0 == strcmp(szColName, "trddate")) {
				Date date(szColVal);
				trade_data.trade_time.date = Date(date.year, date.month, date.day);
			}
			else if (0 == strcmp(szColName, "matchtime")) {
				int hour, minute, sec, milsec;
				sscanf(szColVal, "%2d%2d%2d%2d", &hour, &minute, &sec, &milsec);
				trade_data.trade_time.time = Time(hour, minute, sec, milsec*10);
			}
			else if (0 == strcmp(szColName, "bsflag")) {
				if ('B' == szColVal[0]) {
					trade_data.direction = LONG_DIRECTION;
					trade_data.open_close_flag = OPEN_ORDER;
				}
				else if ('S' == szColVal[0]) {
					trade_data.direction = SHORT_DIRECTION;	
					trade_data.open_close_flag = CLOSE_ORDER;
				}
				else
					assert(false);
			}
			else if (0 == strcmp(szColName, "matchprice"))
				trade_data.trade_price = atof(szColVal);
			else if (0 == strcmp(szColName, "matchqty"))
				trade_data.trade_volume = atoi(szColVal);
		}
		spi_->OnQryTrade(0, &trade_data, "", row == nRowNum-1);
	} 
}

void JZTradeApi::OnQueryCancelAbledOrder(std::string& szRetMsg) {

}

void JZTradeApi::OnQryInstrument(std::string& szRetMsg) {
	char szRowBuf[RESULTSROWMAX] = {'\0'};
	int nRowNum = Stock_GetRowNum(szRetMsg.c_str());
	for(int row = 0; row < nRowNum; row++) { //get all rows
		Stock_GetSeparateValue(szRetMsg.c_str(), RESULTROWSEP, row, szRowBuf, sizeof(szRowBuf)-1);
		if(szRowBuf[0] == '\0') break; //the last

		// get cell value
		char szCellBuf[256] = {'\0'};
		int nCellNum = Stock_GetCellNum(szRowBuf);
		StockInstrumentInfo stock_info;
		for(int cell = 0; cell < nCellNum; cell++){
			Stock_GetSeparateValue(szRowBuf, RESULTCELLSEP, cell, szCellBuf, sizeof(szCellBuf)-1);
			if(szCellBuf[0] == '\0') break; //the last

			char szColName[256] = {'\0'};
			char szColVal[256] = {'\0'};
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 0, szColName, sizeof(szColName)-1);
			Stock_GetSeparateValue(szCellBuf, RESULTNAMEVALUESEP, 1, szColVal, sizeof(szColVal)-1);

			stock_info.symbol.product = PRODUCT_STOCK;
			if (0 == strcmp(szColName, "market")) {
				if (0 == strcmp(szColVal, "1"))
					stock_info.symbol.exchange = EXCHANGE_SSE;
				else if (0 == strcmp(szColVal, "2"))
					stock_info.symbol.exchange = EXCHANGE_SZE;
				else {
					stock_info.symbol.exchange = EXCHANGE_OTHER;
					break;
				}
			}
			else if (0 == strcmp(szColName, "stkcode"))
				strcpy(stock_info.symbol.instrument, szColVal);
			else if (0 == strcmp(szColName, "stkname"))
				strcpy(stock_info.symbol.name, szColVal);		
			else if (!strcmp(szColName, "priceunit"))
				stock_info.price_tick = atoi(szColVal) / 1000.0;
			else if (!strcmp(szColName, "maxrisevalue"))
				stock_info.max_rise_value = atof(szColVal);
			else if (!strcmp(szColName, "maxdownvalue"))
				stock_info.max_down_value = atof(szColVal);
			else if (!strcmp(szColName, "maxqty"))
				stock_info.max_qty = atoi(szColVal);
			else if (!strcmp(szColName, "minqty"))
				stock_info.min_qty = atoi(szColVal);
			else if (!strcmp(szColName, "buyunit"))
				stock_info.buy_unit = atoi(szColVal);
			else if (!strcmp(szColName, "saleunit"))
				stock_info.sell_unit = atoi(szColVal);
			else if ((!strcmp(szColName, "stklevel"))) {
				if (szColVal[0]=='N')
					stock_info.status = STK_STATUS_N;
				else if (szColVal[0]=='Z')
					stock_info.status = STK_STATUS_Z;
				else
					stock_info.status = STK_STATUS_UNKNOWN;
			}
		}
		if (stock_info.symbol.exchange == EXCHANGE_SSE && stock_info.symbol.instrument[0] != '5' && stock_info.symbol.instrument[0] != '6')
			continue;
		if (stock_info.symbol.exchange == EXCHANGE_SZE && stock_info.symbol.instrument[0] != '0' && stock_info.symbol.instrument[0] != '3')
			continue;
		
		if (row == nRowNum-1)
			spi_->OnInstrumentInfo(stock_info, true);
		else if (stock_info.symbol.exchange != EXCHANGE_OTHER)
			spi_->OnInstrumentInfo(stock_info, false);
	} 
}

JZTradeApi::JZTradeApi(void) : m_iCreditflag(false)
{
	/*新建KCBP实例*/
	KCBPCLI_Init( &m_hKCBPHandle );		
}

JZTradeApi::~JZTradeApi(void)
{
	KCBPCLI_Exit(m_hKCBPHandle);
}

bool JZTradeApi::Init(const std::string& ip, int port, 
	const std::string& szServerName,
	const std::string& szUserName,
	const std::string& szPassword,
	const std::string& szSendQName,
	const std::string& szReceiveQName,
	TradeSpi* spi, std::string& err) {

	ip_ = ip;
	port_ = port;
	spi_ = spi;	
	
	tagKCBPConnectOptionEx stKCBPConnection; memset(&stKCBPConnection, 0 , sizeof(stKCBPConnection));
	strcpy(stKCBPConnection.szServerName, szServerName.c_str());
	stKCBPConnection.nProtocal = 0; // TCP
	strcpy(stKCBPConnection.szAddress, ip_.c_str());
	stKCBPConnection.nPort = port_;
	strcpy(stKCBPConnection.szSendQName, szSendQName.c_str());
	strcpy(stKCBPConnection.szReceiveQName, szReceiveQName.c_str());

	/*连接KCBP服务器*/
	if( KCBPCLI_SetOptions( m_hKCBPHandle, KCBP_OPTION_CONNECT_EX, &stKCBPConnection, sizeof(stKCBPConnection) ) )
	{
		err = ( "KCBPCLI_SetConnectOption error" );
		KCBPCLI_Exit( m_hKCBPHandle );
		return false;
	}

	if( KCBPCLI_SetCliTimeOut( m_hKCBPHandle, 60 ) )
	{
		err = ( "KCBPCLI_SetCliTimeOut Error" );
		KCBPCLI_Exit( m_hKCBPHandle );
		return false;
	}
	
	if (KCBPCLI_SQLConnect( m_hKCBPHandle, stKCBPConnection.szServerName, (char*)szUserName.c_str(), (char*)szPassword.c_str()))
	{
		err = ( "KCBPCLI_SQLConnect Error" );
		KCBPCLI_Exit( m_hKCBPHandle );
		return false;
	}

	m_succed_connect = true; return true;
}



bool JZTradeApi::Login(const std::string& broker_id, 
	const std::string& user_id, const std::string& password, 
	const std::string& szLoginNetaddr,std::string& err) {

	memset(&m_cust_info, 0, sizeof(CustInfo));
	m_user_id = user_id;
	m_password = password;
	
	strcpy(m_cust_info.szCustid, "-1");
	strcpy(m_cust_info.szLoginOrgid, broker_id.c_str());
	// encode the password
	char szEncodedPassword[256];
	char szEncodedPasswordSrc[256];
	if (Stock_EncodePassword(szEncodedPasswordSrc, 256, password.c_str()) != 0){ //似乎只能加密8位
		err = "Encode password failure!";
		return false;
	}
	// base64 encode
	unsigned long dwLen = strlen(szEncodedPasswordSrc);
	base64(szEncodedPasswordSrc, szEncodedPassword, &dwLen);
	szEncodedPassword[256-1] = '\0';
	strcpy(m_cust_info.szTradePassword, szEncodedPassword);
	strcpy(m_cust_info.szLoginNetaddr, szLoginNetaddr.c_str());
	strcpy(m_cust_info.szOperateOrgid, broker_id.c_str());
	strcpy(m_cust_info.szOperway, "7");
	strcpy(m_cust_info.szServerid, "");
	
	// generate parameter
	char szParameter[10*1024];	//parameter
	_snprintf(szParameter, sizeof(szParameter), "inputtype:%s,inputid:%s,bankpwd:,inputinfo:", "Z", user_id.c_str());
	
	std::string szRetMsg;
	// do login
	if (Stock_ADoTrade("410305", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return false;
	}

	// login successful
	// get custid
	Stock_GetCellValue((char*)szRetMsg.c_str(), 0, "custid", m_cust_info.szCustid, 30); //save cust id

	// get server id
	std::string szRetMsgServerID;

	_snprintf(szParameter, sizeof(szParameter), "inputtype:Z,inputid:%s", user_id.c_str());
	if (Stock_ADoTrade("410312", szParameter, szRetMsgServerID) != 0){ //failed
		err = szRetMsgServerID;
		return false;
	}else{ //successful
		Stock_GetCellValue((char*)szRetMsgServerID.c_str(), 0, "serverid", m_cust_info.szServerid, 4); //save server id
	}

	int nRowNum = Stock_GetRowNum(szRetMsg.c_str());
	for(int i=0; i<nRowNum; i++){
		char szMarket[4] = {0};		//市场代码 
		char szSecuid[30] = {0};	//股东代码
		Stock_GetCellValue(szRetMsg.c_str(), i, "market", szMarket, sizeof(szMarket)); //get market
		Stock_GetCellValue(szRetMsg.c_str(), i, "secuid", szSecuid, sizeof(szSecuid)); //get secuid
		exe_secuid_[GetExchangeId(szMarket)] = szSecuid;		
		if (i == 0) {
			char szBankcode[10] = {0};	//外部银行
			char szCreditflag[3] = {0};	//资金帐号属性 0 普通 1 信用
			Stock_GetCellValue(szRetMsg.c_str(), i, "bankcode", szBankcode, sizeof(szBankcode)); //get bankcode
			bankcode_ = szBankcode;
			Stock_GetCellValue(szRetMsg.c_str(), i, "creditflag", szCreditflag, sizeof(szCreditflag)); //get secuid
			if (!strcmp(szCreditflag, "1")){ //信用
				m_iCreditflag = true;
			}
		}		
	}
	spi_->OnConnect();
	m_succed_login = true;
	return true;
}

bool JZTradeApi::Logout(std::string& err) {
	// 关闭本地socket，强制断开连接
	if (KCBPCLI_DisConnectForce(m_hKCBPHandle)) {
		err = "KCBPCLI_DisConnectForce error";
		spi_->OnDisconnect(err.c_str());
		return false;
	}

	spi_->OnDisconnect("disconnect success");
	return true;
}

int JZTradeApi::QueryAccount(std::string& err) {
	// generate parameter
	char szParameter[10*1024];	//parameter
	_snprintf(szParameter, sizeof(szParameter), "fundid:,moneytype:0");
	
	// do trade
	std::string szRetMsg;
	if (Stock_ADoTrade("410502", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return -1;
	}
	return 0;
}

int JZTradeApi::QueryPosition(std::string& err) {
	std::string szRetMsg;
	// generate parameter
	char szParameter[10*1024];	//parameter
	_snprintf(szParameter, sizeof(szParameter), "market:,fundid:,secuid:,stkcode:,qryflag:1,count:10000,poststr:");

	// do trade
	if (Stock_ADoTrade("410503", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return -1;
	}
	return 0;
}

int JZTradeApi::QueryOrder(std::string& err) {
	std::string szRetMsg;
	// generate parameter
	char szParameter[10*1024];	//parameter
	_snprintf(szParameter, sizeof(szParameter), "market:,fundid:,secuid:,stkcode:,ordersno:,bankcode:,qryflag:1,count:1000,poststr:");

	// do trade
	if (Stock_ADoTrade("410510", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return -1;
	}
	return 0;
}

int JZTradeApi::QueryTrade(std::string& err) {	
	// generate parameter
	char szParameter[10*1024];	//parameter
	_snprintf(szParameter, sizeof(szParameter), "fundid:,market:,secuid:,stkcode:,ordersno:,bankcode:,qryflag:1,count:200,poststr:");

	// do trade
	std::string szRetMsg;
	if (Stock_ADoTrade("410512", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return -1;
	}
	return 0;
}

bool JZTradeApi::QryInstrument(std::string& err) {
	std::string szRetMsg; //返回结果
	int ret;

	// generate parameter
	char szParameter[10*1024];	//parameter
	_snprintf(szParameter, sizeof(szParameter), "market:,stkcode:");

	ret = Stock_ADoTrade("410203", szParameter, szRetMsg);
	if (ret != 0) {
		spi_->OnError(0, szRetMsg);
		return false;
	}
	return true;
}

int JZTradeApi::SubmitOrder(const OrderParamData *param, int num, std::string& err)
{
	if (nullptr == param || num < 1) {
		err = "nullptr == param || num < 1"; return -1;
	}

	string szRetMsg, szMarket, szDetail;
	int ordergroup;
	ExchangeIdType exchange;
	OrderDirection direction;
	for (int i = 0; i < num; ++i) {
		if (0 == i) {
			if ((exchange=param[i].symbol.exchange) == EXCHANGE_SSE) szMarket = "1";
			else if ((exchange=param[i].symbol.exchange) == EXCHANGE_SZE) szMarket = "2";
			else assert(false);
			ordergroup = param[i].order_group;
			direction = param[i].direction;
		}
		if (exchange != param[i].symbol.exchange || ordergroup != param[i].order_group || direction != param[i].direction) {
			err = "注意：只能用同一个委托批号ordergroup对同一市场（上证或深证）的股票进行同时买或同时卖"; return -1;
		}

		szDetail += param[i].symbol.instrument; szDetail += "|";
		char limit_price[10] = {0}; sprintf(limit_price, "%.3f", param[i].limit_price);
		szDetail += limit_price; szDetail += "|";
		char volumn[10] = {0}; sprintf(volumn, "%d", param[i].volume);
		szDetail += volumn; szDetail += ";";
	}
	

	// generate parameter
	char szParameter[10*1024];			//parameter
	_snprintf(szParameter, sizeof(szParameter), "market:%s,secuid:%s,fundid:%s,bsflag:%s,ordergroup:%d,bankcode:,remark:,num:1,detail:%s", 
		szMarket.c_str(), exe_secuid_[exchange].c_str(), m_user_id.c_str(), LONG_DIRECTION==direction?"B":"S", 0, szDetail.c_str());

	// do trade
	if (Stock_ADoTrade("410336", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return -1;
	}
	return 0;
}

int JZTradeApi::SubmitOrder(const OrderParamData& param, std::string& err) {
	string szRetMsg, szMarket, szDetail;
	if (param.symbol.exchange == EXCHANGE_SSE) szMarket = "1";
	else if (param.symbol.exchange == EXCHANGE_SZE) szMarket = "2";
	else assert(false);

	// generate parameter
	char szParameter[10*1024];			//parameter
	_snprintf(szParameter, sizeof(szParameter), "market:%s,secuid:%s,fundid:%s,stkcode:%s,bsflag:%s,price:%.3f,qty:%d,ordergroup:%d,bankcode:%s,remark:,bankpwd:", 
		szMarket.c_str(), exe_secuid_[param.symbol.exchange].c_str(), m_user_id.c_str(), param.symbol.instrument, 
		LONG_DIRECTION==param.direction?"B":"S", param.limit_price, param.volume, param.order_group, bankcode_.c_str());

	// do trade
	if (Stock_ADoTrade("410411", szParameter, szRetMsg) != 0){ //failed
		err = szRetMsg;
		return -1;
	}

	return 0;
}

int JZTradeApi::CancelOrder(const OrderData& param, std::string& err) {
	string szRetMsg;

	char orderdate[10];
	sprintf(orderdate, "%02d%02d%02d", param.submit_time.date.year, param.submit_time.date.month, param.submit_time.date.day);
	// generate parameter
	char szParameter[10*1024];			//parameter
	if (param.order_group == -1) {
		_snprintf(szParameter, sizeof(szParameter), "orderdate:%s,fundid:%s,ordersno:%lld,bankpwd:", 
			orderdate, m_user_id.c_str(), param.order_id, param.order_id);

		// do trade
		if (Stock_ADoTrade("410413", szParameter, szRetMsg) != 0){ //failed
			err = szRetMsg;
			return -1;
		}
	}
	else {
		_snprintf(szParameter, sizeof(szParameter), "orderdate:%s,fundid:%s,ordergroup:%d", 
			orderdate, m_user_id.c_str(), param.order_id, param.order_group);

		// do trade
		if (Stock_ADoTrade("410414", szParameter, szRetMsg) != 0){ //failed
			err = szRetMsg;
			return -1;
		}
	}
	
	return 0;
}

int JZTradeApi::Stock_ADoTrade(const char *szFuncID, const char *szParameter, std::string &err)
{
	if (!m_succed_connect) {
		err = "not have login!"; return -1;
	}
	char szFixedParameter[3*1024];
	_snprintf(szFixedParameter, sizeof(szFixedParameter), "funcid:%s,custid:%s,custorgid:%s,trdpwd:%s,netaddr:%s,orgid:%s%s,operway:%s,ext:0",
		szFuncID, m_cust_info.szCustid, m_cust_info.szLoginOrgid, m_cust_info.szTradePassword, 
		m_cust_info.szLoginNetaddr, m_cust_info.szOperateOrgid, m_cust_info.szServerid, m_cust_info.szOperway);

	string szFullParameter = string(szFixedParameter)+","+szParameter;

	char szTmpbuf[3*1024] = {0};
	char szTmpbufVal[3*1024] = {0}; //1024
	char szTmpRowbuf[10*1024] = {0}; //add by hyd 2012/11/2
	int  nReturnCode;

	char szColName[256] = {0}; //64
	char szColVal[3*1024]; //512
	int  i, j;

	//初始化传入参数缓冲区
	nReturnCode = KCBPCLI_BeginWrite(m_hKCBPHandle);
	if( nReturnCode !=0 ) {
		err = "KCBPCLI_BeginWrite Error!";
		return -1;
	}

	for (i = 0; !getsubstr(szTmpbuf, sizeof(szTmpbuf)-1, (char*)(szFullParameter.c_str()), i, ','); i++) {
		getsubstr(szColName, sizeof(szColName)-1, szTmpbuf, 0, ':');

		// add by hyd 20110406 - 密码加密后或其他字段可能有冒号，如001111
		memset(szColVal, 0x00, sizeof(szColVal) );
		for(j = 1; !getsubstr(szTmpbufVal, sizeof(szTmpbufVal)-1, szTmpbuf, j, ':'); j++) {
			if(j > 1) strcat(szColVal, ":");
			strcat(szColVal, szTmpbufVal);
		}			

		// add by hyd 2014/04/25
		if(!strcmp(szColName, "trdpwd")){
			unsigned long dwLen;
			char buf[3072];
			unbase64(szColVal, buf, &dwLen);
			strcpy(szColVal, buf);
		}

		if((!strcmp(szFuncID, "410336") || !strcmp(szFuncID, "410338") || !strcmp(szFuncID, "410440"))
			&&  !strcmp(szColName, "detail")){ //add by hyd 2014/07/17
				std::string strTemp = szColVal;
				for (int i = 0; i < strTemp.size(); ++i) {
					if (strTemp[i] == '|') strTemp[i] = ',';
				}
				strcpy(szColVal, strTemp.c_str());
		}

		if(strcmp(szColName, "orgid")==0){ //!=0 mod by hyd 2012/12/17				
			if( KCBPCLI_SetSystemParam(m_hKCBPHandle, KCBP_PARAM_RESERVED, szColVal) ){   //必须设置营业部代码
				err = "KCBPCLI_SetSystemParam error";
				continue;
			}

			if(strlen(szColVal) > 4){ //is full orgid
				char szShortOrgid[5];
				for(int i=0; i<4; i++){
					szShortOrgid[i] = szColVal[i];
				}
				szShortOrgid[5-1] = '\0';

				KCBPCLI_SetValue(m_hKCBPHandle, szColName, szShortOrgid);
				continue;
			}
		}

		KCBPCLI_SetValue(m_hKCBPHandle, szColName, szColVal);
	}

	//客户端向服务器提交请求
	tagCallCtrl	stControl; memset(&stControl, 0, sizeof(stControl));
	stControl.nExpiry = 5; // 秒
	nReturnCode = KCBPCLI_ACallProgramAndCommit(m_hKCBPHandle, (char*)szFuncID, &stControl);
	if( nReturnCode != 0 ) {
		if( nReturnCode == 2003 || nReturnCode == 2004 || nReturnCode == 2055 || nReturnCode == 2054 ) {
			KCBPCLI_DisConnect(m_hKCBPHandle);
			nReturnCode = KCBPCLI_ConnectServer(m_hKCBPHandle, KCBPSERVERNAME, (char*)m_user_id.c_str(), (char*)m_password.c_str());
			if (nReturnCode != 0)
				err = "reconnect server error";
			else
				err = "reconnect server success and please reLogin";
		}
		else
			err = "KCBPCLI_ACallProgramAndCommit Program Failure";
		
		return -1;
	}

	if (GetQryType(szFuncID) != kQueryOther) {		
		strcpy(stControl.szId, szFuncID);
		std::string szRetMsg;
		if (0 == query(stControl, szRetMsg)) {
			switch(GetQryType(szFuncID)) {
			case kQryInstrument:
				OnQryInstrument(szRetMsg);
				break;
			case kQueryAccount:
				OnQueryAccount(szRetMsg);
				break;
			case kQueryOrder:
				OnQueryOrder(szRetMsg);
				break;
			case kQueryTrade:
				OnQueryTrade(szRetMsg);
				break;
			case kQueryPosition:
				OnQueryPosition(szRetMsg);
				break;
			default:
				break;
			}
		}
	}

	if (!strcmp(szFuncID, "410305")) { // 登录
		return query(stControl, err);
	}
	return 0;
}

int JZTradeApi::query(tagCallCtrl &stControl, std::string &szRetMsg) {
	char szFuncID[10]; strncpy(szFuncID, stControl.szId, sizeof(szFuncID)); 
	if (KCBPCLI_GetReply(m_hKCBPHandle, &stControl)) {
		szRetMsg = "KCBPCLI_GetReply error"; return -1;
	}

	int nErrCode = 0;
	char szErrMsg[256] = {0};
	char szTmpbuf[3*1024] = {0};
	int  nCol;
	char szTmpRowbuf[10*1024] = {0};
	KCBPCLI_GetErrorCode(m_hKCBPHandle, &nErrCode);
	if( nErrCode != 0 ) {
		KCBPCLI_GetErrorMsg(m_hKCBPHandle, szErrMsg );
		szRetMsg = szErrMsg;
		return -1;
	}

	nErrCode = KCBPCLI_RsOpen(m_hKCBPHandle);
	if( nErrCode != 0 && nErrCode != 100 ) {
		szRetMsg = "KCBPCLI_RsOpen error";
		return -1;
	}

	KCBPCLI_SQLNumResultCols(m_hKCBPHandle, &nCol);
	KCBPCLI_SQLFetch(m_hKCBPHandle);

	// get trade type - add by hyd 2014/06/26 
	int iTradeType = 0;
	if(strlen(szFuncID) > 3){
		if(szFuncID[0]=='l'||szFuncID[0]=='L'){
			iTradeType = 1;
		}
	}

	char szFieldCode[20]="CODE", szFieldLevel[20]="LEVEL", szFieldMsg[20]="MSG";
	if(iTradeType == 1){
		strcpy(szFieldCode, "MSG_CODE");
		strcpy(szFieldLevel, "MSG_LEVEL");
		strcpy(szFieldMsg, "MSG_TEXT");
	}


	if( KCBPCLI_RsGetColByName(m_hKCBPHandle, szFieldCode, szTmpbuf ) ) {
		szRetMsg = "Get CODE Fail";
		return -1;
	}

	if( strcmp(szTmpbuf, "0") != 0 ) {
		KCBPCLI_RsGetColByName(m_hKCBPHandle, szFieldMsg, szTmpbuf );
		szRetMsg = szTmpbuf;
		return -1;
	}

	// get data
	int nRows = 0;
	while( KCBPCLI_SQLMoreResults(m_hKCBPHandle) == 0 ) {	// update by mahui 2013/11/25 将if 改成 while 支持返回多个结果集	
		KCBPCLI_SQLNumResultCols(m_hKCBPHandle, &nCol);
		while( !KCBPCLI_RsFetchRow(m_hKCBPHandle) ) {

			// clear old value
			memset(szTmpRowbuf, 0x00, sizeof(szTmpRowbuf));

			for(int i = 1; i <= nCol; i++ ) {
				KCBPCLI_RsGetColName(m_hKCBPHandle, i, szTmpbuf, sizeof(szTmpbuf)-1 );
				alltrim(szTmpbuf); //trim //replace by hyd 2010/06/17
				strcat(szTmpRowbuf, szTmpbuf); //replaced by hyd 2010/06/18

				KCBPCLI_RsGetColByName(m_hKCBPHandle, szTmpbuf, szTmpbuf);
				strcat(szTmpRowbuf, RESULTNAMEVALUESEP); //replaced by hyd 2010/06/18
				strcat(szTmpRowbuf, szTmpbuf);
				strcat(szTmpRowbuf, RESULTCELLSEP);
			}
			strcat(szTmpRowbuf, RESULTROWSEP); //replaced by hyd 2010/06/18

			// add row
			szRetMsg += szTmpRowbuf;
			nRows++;
		}
	}

	KCBPCLI_SQLCloseCursor(m_hKCBPHandle);
	return 0;
}

}
