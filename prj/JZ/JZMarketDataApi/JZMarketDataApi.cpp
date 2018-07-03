#include <fstream>
#include "JZMarketDataApi.h"
#include "matchpushdefine.h"


namespace kiiik {


JZMarketDataApi::JZMarketDataApi() : num_syms_(0) {

}

JZMarketDataApi::~JZMarketDataApi() {

}

void initDateTime(SimpleDateTime &date_time, T_I32 nDate, T_I32 nTime) {
	date_time.date.year = short(nDate/10000);
	date_time.date.month = char((nDate%10000)/100);
	date_time.date.day = char(nDate%100);
	date_time.time.hour = char(nTime/10000000);
	date_time.time.minute = char((nTime%10000000)/100000);
	date_time.time.sec = char((nTime%100000)/1000);
	date_time.time.mil_sec =short(nTime%1000);
}

struct MyStockTick : public StockTick {
	MyStockTick(T_I32 nDate, T_I32 nMarketId, const char* sCode, PSH_StockMarketDataL2 pTick) {
		symbol.product = PRODUCT_STOCK;
		symbol.exchange = EXCHANGE_SSE;
		strcpy(symbol.instrument, sCode);
		initDateTime(date_time, nDate, pTick->nTime);
		last_price = pTick->uMatch/10000.0;
		volume = pTick->iVolume;
		amount = pTick->iTurnover;
		pre_close = pTick->uPreClose/10000.0;
		today_open = pTick->uOpen/10000.0;
		today_high = pTick->uHigh/10000.0;
		today_low = pTick->uLow/10000.0;

		for (int i = 0; i < PRICE_LEVEL_NUM; ++i) {
			buy_price[i] = pTick->uBidPrice[i] / 10000.0;
			sell_price[i] = pTick->uAskPrice[i] / 10000.0;
			buy_volume[i] = pTick->uBidVol[i];
			sell_volume[i] = pTick->uAskVol[i];
		}
	}
	MyStockTick(T_I32 nDate, T_I32 nMarketId, const char* sCode, PSZ_StockMarketDataL2 pTick) {
		symbol.product = PRODUCT_STOCK;
		symbol.exchange = EXCHANGE_SZE;
		strcpy(symbol.instrument, sCode);
		initDateTime(date_time, nDate, pTick->nTime);
		last_price = pTick->uMatch/10000.0;
		volume = pTick->iVolume;
		amount = pTick->iTurnover;
		pre_close = pTick->uPreClose/10000.0;
		today_open = pTick->uOpen/10000.0;
		today_high = pTick->uHigh/10000.0;
		today_low = pTick->uLow/10000.0;

		for (int i = 0; i < PRICE_LEVEL_NUM; ++i) {
			buy_price[i] = pTick->uBidPrice[i] / 10000.0;
			sell_price[i] = pTick->uAskPrice[i] / 10000.0;
			buy_volume[i] = pTick->uBidVol[i];
			sell_volume[i] = pTick->uAskVol[i];
		}
	}
};

struct MyIndexTick : public IndexTick {
	MyIndexTick(T_I32 nDate, T_I32 nMarketId, const char* sCode, PSH_StockIndex pTick) {
		symbol.product = PRODUCT_INDEX;
		symbol.exchange = EXCHANGE_SSE;
		strcpy(symbol.instrument, sCode);
		initDateTime(date_time, nDate, pTick->nTime);
		last_price = pTick->nLastIndex/10000.0;
		volume = pTick->iTotalVolume;
		amount = pTick->iTurnover;
		pre_close = pTick->nPreCloseIndex/10000.0;
		today_open = pTick->nOpenIndex/10000.0;
		today_high = pTick->nHighIndex/10000.0;
		today_low = pTick->nLowIndex/10000.0;

		advance_num = decline_num = 0;//国君无此数据
	}
	MyIndexTick(T_I32 nDate, T_I32 nMarketId, const char* sCode, PSZ_StockIndex pTick) {
		symbol.product = PRODUCT_INDEX;
		symbol.exchange = EXCHANGE_SZE;
		strcpy(symbol.instrument, sCode);
		initDateTime(date_time, nDate, pTick->nTime);
		last_price = pTick->nLastIndex/10000.0;
		volume = pTick->iTotalVolume;
		amount = pTick->iTurnover;
		pre_close = pTick->nPreCloseIndex/10000.0;
		today_open = pTick->nOpenIndex/10000.0;
		today_high = pTick->nHighIndex/10000.0;
		today_low = pTick->nLowIndex/10000.0;

		advance_num = decline_num = 0;//国君无此数据
	}
};

struct MyOptionTick : public OptionTick {
	MyOptionTick(T_I32 nDate, T_I32 nMarketId, const char* sCode, PSHOP_MarketData pTick) {
		symbol.product = PRODUCT_OPTION;
		symbol.exchange = EXCHANGE_SSE;
		strcpy(symbol.instrument, sCode);
		initDateTime(date_time, nDate, pTick->nDataTimestamp);
		last_price = pTick->iLastPx/10000.0;
		volume = pTick->iTotalVolumeTrade;
		amount = pTick->iTotalValueTrade; // 不要除，直接精确到元了，注释还要改一下
		pre_close = pTick->iPreSettlPrice/10000.0;
		today_open = pTick->iOpenPx/10000.0;
		today_high = pTick->iHighPx/10000.0;
		today_low = pTick->iLowPx/10000.0;
		position = pTick->iTotalLongPosition;
		buy_price = pTick->iBidPx[0]/10000.0;
		sell_price = pTick->iOfferPx[0]/10000.0;
		buy_volume = (VolumeType)pTick->iBidSize[0];
		sell_volume = (VolumeType)pTick->iOfferSize[0];
	}
	MyOptionTick(T_I32 nDate, T_I32 nMarketId, const char* sCode, PSZOP_MARKETDATA pTick) {
		symbol.product = PRODUCT_OPTION;
		symbol.exchange = EXCHANGE_SZE;
		strcpy(symbol.instrument, sCode);
		initDateTime(date_time, nDate, pTick->nTime);
		last_price = pTick->i64LastPrice/10000.0;
		volume = pTick->i64TotalVolumeTrade;
		amount = pTick->i64TotalValueTrade; // 不要除，直接精确到元了，注释还要改一下
		pre_close = pTick->i64PrevClosePx/10000.0;
		today_open = pTick->i64OpenPrice/10000.0;
		today_high = pTick->i64HighPrice/10000.0;
		today_low = pTick->i64LowPrice/10000.0;
		position = pTick->i64ContractPosition;
		buy_price = pTick->i64BidPrice[0]/10000.0;
		sell_price = pTick->i64OfferPrice[0]/10000.0;
		buy_volume = (VolumeType)pTick->i64BidQty[0];
		sell_volume = (VolumeType)pTick->i64OfferQty[0];
	}
};
//**********************************************************************************
//数据到达回调函数
//**********************************************************************************

// 数据到达回调，pUserParam为调用着自己的参数，即TDR_Create中的pParamData参数
void  __stdcall OnReceiveData(
	void* pUserParam, //[in]用户自定义参数,由用户调用TDR_Create时传入
	T_I32 nDate,      //[in]日期       
	T_I32 nMarketId,  //[in]市场代码，参见tdr.h中对市场进行的宏定义
	const char* sCode,//[in]证券代码
	const char* sName,//[in]证券名称
	T_I32 nServiceId, //[in]服务数据ID，比如行情、逐笔成交等，参见tdr.h文件定义
	void* pData,       //[in]数据内容
	T_I32 nLen
	)
{
	MarketDataSpi *spi = (MarketDataSpi*)pUserParam;

	switch(nMarketId)
	{
	case SH:
		{
			if (ID_SH_MARKETDATA == nServiceId) {
				PSH_StockMarketDataL2 pMarket=(PSH_StockMarketDataL2)pData;
				MyStockTick tick(nDate, nMarketId, sCode, pMarket);
				spi->OnMarketPrice(&tick);
			}
			else if (ID_SH_INDEXDATA == nServiceId) {
				PSH_StockIndex pMarket=(PSH_StockIndex)pData;
				MyIndexTick tick(nDate, nMarketId, sCode, pMarket);
				spi->OnMarketPrice(&tick);
			}
		}
		break;
	case SHOP:
		{
			PSHOP_MarketData pMarket = (PSHOP_MarketData)pData;
			MyOptionTick tick(nDate, nMarketId, sCode, pMarket);
			spi->OnMarketPrice(&tick);
		}
		break;
	case SZ:
		{
			if (ID_SZ_MARKETDATA == nServiceId) {
				PSZ_StockMarketDataL2 pMarket = (PSZ_StockMarketDataL2)pData;
				MyStockTick tick(nDate, nMarketId, sCode, pMarket);
				spi->OnMarketPrice(&tick);
			}
			else if (ID_SZ_INDEXDATA == nServiceId) {
				PSZ_StockIndex pMarket=(PSZ_StockIndex)pData;
				MyIndexTick tick(nDate, nMarketId, sCode, pMarket);
				spi->OnMarketPrice(&tick);
			}
		}
		break;
	case SZOP:
		{
			PSZOP_MARKETDATA pMarket = (PSZOP_MARKETDATA)pData;
			MyOptionTick tick(nDate, nMarketId, sCode, pMarket);
			spi->OnMarketPrice(&tick);
		}
		break;
	default:
		break;
	}
}

void  __stdcall OnErrorMsg(void* pUserParam,T_I32 nError,T_I32 nErrSource,T_U32 uData)
{
	MarketDataSpi *spi = (MarketDataSpi*)pUserParam;
	if((SE_OK == nError)&&(nErrSource == ERRMSGSRC_LOGIN)){
		spi->OnConnect();
	}
	else if (nError>=SE_NOCONNECT && nError<=SE_OP_PASSWORD) {
		spi->OnError("","login error", "");
	}
}

bool JZMarketDataApi::Init(const std::string& ip, int port, MarketDataSpi* spi, std::string& err){
	ip_ = ip;
	port_ = port;
	spi_ = spi;

	return true;
}

bool JZMarketDataApi::Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err) {

	user_id_ = user_id;
	password_ = password;

	hTDR_.push_back(TDR_Create(NULL,(ONRECEIVEDATA)OnReceiveData,spi_,(ONERRORMSG)OnErrorMsg,spi_,true,true));

	int nret = TDR_ConnectByDynamic(hTDR_.back(), ip_.c_str(), port_, SIP_SVR_WAN,WAN_TC,
		user_id.c_str(), password.c_str(), UI_LOGIN_NORMAL, 5);
	if (nret) {
		err = "TDR_ConnectByDynamic 连接股票行情失败!";
		return false;
	}
	m_succed_connect = true;
	m_succed_login = true;
	return true;
}

bool JZMarketDataApi::Logout(std::string& err) {
	m_succed_connect = false;
	m_succed_login = false;
	for (std::vector<T_HANDLE>::const_iterator it = hTDR_.begin(); it != hTDR_.end(); ++it) {
		TDR_DisConnect(*it);
		TDR_Destroy(*it);
	}
	
	return true;
}

bool JZMarketDataApi::Subscribe(const std::string& symbol, std::string& err) {
	if (!m_succed_login){
		err = "logout";
		return false;
	}
	
	/************************************************************************/
		/* 三、订阅行情
		 订阅行情可按个股订阅、组合订阅、市场订阅，具体订阅函数参见tdrapi.h文件
		 订阅模式有两种
		 1、最新订阅。RSS_MODE_NEW 服务器只推送最新数据包。
		 2、增量订阅。RSS_MODE_INC 服务器推送订阅的所有数据包。
		 订阅行情后可以回调函数里面获取行情*/
	/************************************************************************/
	std::string strMarket = GetMarketByStockCode(symbol);
	int service = GetServiceIdByStockCode(symbol);	

	int ret = TDR_SubscribeByCode(hTDR_.back(), strMarket.c_str(), symbol.substr(0,6).c_str(), RSS_MODE_INC, service);
	if (0 != ret) {
		err = "Subscribe "+symbol+" fail\n";
		return false;
	}
	return true;
}


}
