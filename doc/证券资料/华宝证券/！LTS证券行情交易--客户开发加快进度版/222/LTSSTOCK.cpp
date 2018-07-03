
#include "ZQLTSMD.h"
#include "ZQLTSTD.h"
#include <iomanip>
#include <float.h>
#include "iostream"	
#include "SecurityFtdcTraderApi.h"
#include "windows.h"
#include "stdio.h"
#include<fstream>


using namespace std;

//交易前置地址
char FRONT_ADDR_TD[]="tcp://211.144.195.163:44505";
//行情前置地址
char FRONT_ADDR_MD[]="tcp://211.144.195.163:44513";
// 经纪公司代码
TSecurityFtdcBrokerIDType	BROKER_ID = "2011";	
// 投资者代码
TSecurityFtdcInvestorIDType INVESTOR_ID = "020090001340";	
// 用户密码
TSecurityFtdcPasswordType  PASSWORD = "123321";	
char * InstrumentsSSE[10];
int iRequestID=0;
#pragma comment(lib,"securitymduserapi.lib")
#pragma comment(lib,"securitytraderapi.lib")

HANDLE g_hEvent;
CSecurityFtdcTraderApi *m_pTdApi;

using namespace std;

int main(int argc, char* argv[])
{
	SetConsoleTitle("华宝证券 个股期权API Demo    QQ: 16304984");
	CZQLTSTD * pTdHandler=new CZQLTSTD(m_pTdApi);
	m_pTdApi = CSecurityFtdcTraderApi::CreateFtdcTraderApi("");
	CZQLTSTD m_pTdSpi(m_pTdApi);
	m_pTdApi->RegisterSpi(&m_pTdSpi);
	m_pTdApi->SubscribePrivateTopic(SECURITY_TERT_QUICK);
	m_pTdApi->SubscribePublicTopic(SECURITY_TERT_QUICK);
	m_pTdApi->RegisterFront(FRONT_ADDR_TD);
	m_pTdApi->Init();
	Sleep(1000);
	CZQLTSMD * pMdHandler=new CZQLTSMD();
	pMdHandler->Start();
	pMdHandler->Init();

	
	HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);	
	WaitForSingleObject(g_hEvent, INFINITE);
	return 0;
}
