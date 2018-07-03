// ZQLTSTD.cpp: implementation of the CZQLTSTD class.
//
//////////////////////////////////////////////////////////////////////

#include "ZQLTSTD.h"
#include "ZQLTSMD.h"
#include <iostream>
#include <iomanip>
#include <float.h>
#include "SecurityFtdcTraderApi.h"
#include <float.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream> 

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;
//extern HANDLE g_hEvent;
extern CSecurityFtdcTraderApi *m_pTdApi;
extern char FRONT_ADDR_TD[];
extern char FRONT_ADDR_MD[];
extern TSecurityFtdcBrokerIDType	BROKER_ID;
extern TSecurityFtdcInvestorIDType INVESTOR_ID ;
extern TSecurityFtdcPasswordType  PASSWORD;	


extern int iRequestID;
extern char * InstrumentsSSE[10];
extern int iInstrumentID;


CZQLTSTD::~CZQLTSTD()
{
}

void CZQLTSTD::Init()
{
}

void CZQLTSTD::OnFrontConnected()
{
	CSecurityFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(reqUserLogin));
	
	CSecurityFtdcReqUserLoginField * reqUserLogin1=&reqUserLogin;

	strcpy(reqUserLogin1->BrokerID,BROKER_ID);
	strcpy(reqUserLogin1->UserID,INVESTOR_ID);
	strcpy(reqUserLogin1->Password,PASSWORD);
	// 发出登陆请求
	
	int iResult2 = m_pTdApi->ReqUserLogin(reqUserLogin1, 1);
	cerr << "--->>> TD登录请求: "  << iResult2 << ((iResult2 == 0) ? ", 成功" : ", 失败") << endl;


}


void CZQLTSTD::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	cout << "--->>> " << "OnRspUserLogin" << endl;
	return;

}
void CZQLTSTD::ReqQryInstrument()
{	
	CSecurityFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.ExchangeID,"SSE");
	strcpy(req.InstrumentID,"600000");
	int iResult = m_pTdApi->ReqQryInstrument(&req, ++iRequestID);
	cerr << "--->>> 查询合约操作请求: "  << iResult << ((iResult == 0) ? ", 成功" : ", 失败") << endl;
}

	///请求查询合约响应
void CZQLTSTD::OnRspQryExchange(CSecurityFtdcExchangeField *pExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if ( !IsErrorRspInfo1(pRspInfo) )
	{
		cout << "--->>> " << "OnRspQryExchange" << endl;
	}
}

void CZQLTSTD::OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;	
}

	///报单录入请求响应
void CZQLTSTD::OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	
	return;
}
//ken 撤单
void CZQLTSTD::OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	return;
}
	///请求查询报单响应
void CZQLTSTD::OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	

	return;
}
///请求查询成交响应
void CZQLTSTD::OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	

	return;
}
	///请求查询资金账户响应
void CZQLTSTD::OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

///请求查询投资者响应

void CZQLTSTD::OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo  , int nRequestID, bool bIsLast)
 {
	
	return;
}
///请求查询行情响应

void CZQLTSTD::OnRspQryDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	cout << "--->>> " << "OnRspQryDepthMarketData" << endl;
	if (bIsLast )
	{
		if(pRspInfo !=NULL)
		cout<<"查询行情回报 OnRspQryDepthMarketData"<<pRspInfo->ErrorMsg<<endl;

		if(pDepthMarketData!=NULL)
		 {
			 cout<<"查询行情回报 OnRspQryDepthMarketData  "<<" ";
			 cout<<pDepthMarketData->InstrumentID<<" ";
			 cout<<pDepthMarketData->UpdateTime<<" ";
			 cout<<"Ask1 "<<pDepthMarketData->AskPrice1<<"; ";
			 cout<<"Bid1 "<<pDepthMarketData->BidPrice1<<"; ";
			 cout<<"LastPrice "<<pDepthMarketData->LastPrice<<"; ";
			 cout<<pDepthMarketData->UpdateMillisec<<" ";
			 cout<<" "<<endl;
		}
	}
	return;
}


///请求查询投资者持仓响应
void CZQLTSTD::OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	
	return;
}

//ken 查询持仓明细回报

void CZQLTSTD::OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionDetailField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}

void CZQLTSTD::OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}

//ErrRSP&Rtn/////////////////////////////////////////////////////////////////////
///错误应答
void CZQLTSTD::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}
///报单录入错误回报
void CZQLTSTD::OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo)
{
	return;
}
///报单操作错误回报
void CZQLTSTD::OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo)
{

	return;
}
//Rtn/////////////////////////////////////////////////////////////////////
///报单通知
void CZQLTSTD::OnRtnOrder(CSecurityFtdcOrderField *pOrder)
{

	return;
}


///成交通知
void CZQLTSTD::OnRtnTrade(CSecurityFtdcTradeField *pTrade)
{

	return;

}

/////////////////////////////////////////////////////////////////////////////////////
double CZQLTSTD::getPrice(TSecurityFtdcStockPriceType price)
{
		cout<<"getPrice"<<endl;

	return sizeof(price) / sizeof(price[0]);
}
bool CZQLTSTD::IsErrorRspInfo1(CSecurityFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}
