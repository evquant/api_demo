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
	// ������½����
	
	int iResult2 = m_pTdApi->ReqUserLogin(reqUserLogin1, 1);
	cerr << "--->>> TD��¼����: "  << iResult2 << ((iResult2 == 0) ? ", �ɹ�" : ", ʧ��") << endl;


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
	cerr << "--->>> ��ѯ��Լ��������: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
}

	///�����ѯ��Լ��Ӧ
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

	///����¼��������Ӧ
void CZQLTSTD::OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	
	return;
}
//ken ����
void CZQLTSTD::OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	return;
}
	///�����ѯ������Ӧ
void CZQLTSTD::OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	

	return;
}
///�����ѯ�ɽ���Ӧ
void CZQLTSTD::OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	

	return;
}
	///�����ѯ�ʽ��˻���Ӧ
void CZQLTSTD::OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

///�����ѯͶ������Ӧ

void CZQLTSTD::OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo  , int nRequestID, bool bIsLast)
 {
	
	return;
}
///�����ѯ������Ӧ

void CZQLTSTD::OnRspQryDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	cout << "--->>> " << "OnRspQryDepthMarketData" << endl;
	if (bIsLast )
	{
		if(pRspInfo !=NULL)
		cout<<"��ѯ����ر� OnRspQryDepthMarketData"<<pRspInfo->ErrorMsg<<endl;

		if(pDepthMarketData!=NULL)
		 {
			 cout<<"��ѯ����ر� OnRspQryDepthMarketData  "<<" ";
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


///�����ѯͶ���ֲ߳���Ӧ
void CZQLTSTD::OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	
	return;
}

//ken ��ѯ�ֲ���ϸ�ر�

void CZQLTSTD::OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionDetailField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}

void CZQLTSTD::OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}

//ErrRSP&Rtn/////////////////////////////////////////////////////////////////////
///����Ӧ��
void CZQLTSTD::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}
///����¼�����ر�
void CZQLTSTD::OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo)
{
	return;
}
///������������ر�
void CZQLTSTD::OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo)
{

	return;
}
//Rtn/////////////////////////////////////////////////////////////////////
///����֪ͨ
void CZQLTSTD::OnRtnOrder(CSecurityFtdcOrderField *pOrder)
{

	return;
}


///�ɽ�֪ͨ
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
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}
