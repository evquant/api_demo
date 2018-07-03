// ZQLTSMD.cpp: implementation of the CZQLTSMD class.
//
//////////////////////////////////////////////////////////////////////

#include "ZQLTSMD.h"
#include "ZQLTSTD.h"
#include "SecurityFtdcTraderApi.h"
#include <iostream>
#include <iomanip>
#include <float.h>
#include <time.h>
#include <stdio.h>
#include "windows.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;


extern char FRONT_ADDR_TD[];
extern char FRONT_ADDR_MD[];
extern TSecurityFtdcBrokerIDType	BROKER_ID;
extern TSecurityFtdcInvestorIDType INVESTOR_ID ;
extern TSecurityFtdcPasswordType  PASSWORD;	
extern char * InstrumentsSSE[10];
extern int iRequestID;


CZQLTSMD::CZQLTSMD()
{
}

void CZQLTSMD::Start()
{

}

CZQLTSMD::~CZQLTSMD()
{
	m_pMdApi->Release();

}

void CZQLTSMD::Init()
{
	m_pMdApi=CSecurityFtdcMdApi::CreateFtdcMdApi("");// 产生一个CSecurityFtdcMdApi实例
	m_pMdApi->RegisterSpi(this);

	m_pMdApi->RegisterFront(FRONT_ADDR_MD);
	m_pMdApi->Init();
}

void CZQLTSMD::OnFrontConnected()
{
	CSecurityFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID,BROKER_ID);
	strcpy(reqUserLogin.UserID,INVESTOR_ID);
	strcpy(reqUserLogin.Password,PASSWORD);
	m_pMdApi->ReqUserLogin(&reqUserLogin,1);
}
void CZQLTSMD::OnFrontDisconnected(int nReason)
{
	printf("I am in OnFrontDisconnected!\n");
}
void CZQLTSMD::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("I am in OnRspUserLogin!\n");
	//订阅行情
	InstrumentsSSE[0]="600000";
	InstrumentsSSE[1]="600005";
	InstrumentsSSE[2]="000300";
	InstrumentsSSE[3]="600104";
	InstrumentsSSE[4]="510050";
	InstrumentsSSE[5]="510180";

	m_pMdApi->SubscribeMarketData(InstrumentsSSE,10,"SSE");
}
void CZQLTSMD::OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData) 
{
	char pCurrentTime[80];
	SYSTEMTIME t;
	GetLocalTime(&t);
	sprintf(pCurrentTime, "%02d:%02d:%02d", t.wHour, t.wMinute, t.wSecond);
	cout<<pDepthMarketData->InstrumentID<<",";
	cout<<"最新价:"<<pDepthMarketData->LastPrice<<",";
	cout<<pCurrentTime<<" "<<endl;
	std::cout<<pCurrentTime<<" ";
	std::cout<<pDepthMarketData->ExchangeID<<" ";
	std::cout<<pDepthMarketData->InstrumentID<<" 买 ";
	std::cout<<pDepthMarketData->BidPrice1<<" ";
	std::cout<<pDepthMarketData->BidVolume1/100<<" ";
	std::cout<<pDepthMarketData->BidPrice2<<" ";
	std::cout<<pDepthMarketData->BidVolume2/100<<" ";
	std::cout<<pDepthMarketData->BidPrice3<<" ";
	std::cout<<pDepthMarketData->BidVolume3/100<<" ";
	std::cout<<pDepthMarketData->BidPrice4<<" ";
	std::cout<<pDepthMarketData->BidVolume4/100<<" ";
	std::cout<<pDepthMarketData->BidPrice5<<" ";
	std::cout<<pDepthMarketData->BidVolume5/100<<" | ";
	std::cout<<std::endl;
	std::cout<<pCurrentTime<<" ";	
	std::cout<<pDepthMarketData->ExchangeID<<" ";
	std::cout<<pDepthMarketData->InstrumentID<<" 卖 ";
	std::cout<<pDepthMarketData->AskPrice1<<" ";
	std::cout<<pDepthMarketData->AskVolume1/100<<" ";
	std::cout<<pDepthMarketData->AskPrice2<<" ";
	std::cout<<pDepthMarketData->AskVolume2/100<<" ";
	std::cout<<pDepthMarketData->AskPrice3<<" ";
	std::cout<<pDepthMarketData->AskVolume3/100<<" ";
	std::cout<<pDepthMarketData->AskPrice4<<" ";
	std::cout<<pDepthMarketData->AskVolume4/100<<" ";
	std::cout<<pDepthMarketData->AskPrice5<<" ";
	std::cout<<pDepthMarketData->AskVolume5/100<<" ";
	std::cout<<std::endl;
	
}
