// ZQLTSMD.h: interface for the CZQLTSMD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZQLTSMD_H__E1C8C421_0DB0_4129_AC7E_7DF1BB1C6DFC__INCLUDED_)
#define AFX_ZQLTSMD_H__E1C8C421_0DB0_4129_AC7E_7DF1BB1C6DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SecurityFtdcMdApi.h"
#include "SecurityFtdcTraderApi.h"




#ifdef WINDOWS
static long GetAccurateTime(char *pCurrentTime)
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	long nCurrentTime = t.wHour*3600000 + t.wMinute*60000 + t.wSecond*1000 + t.wMilliseconds;
	if (pCurrentTime)
	{
		sprintf(pCurrentTime, "%02d:%02d:%02d:%03d", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	}

	return nCurrentTime;
}
#else
#endif



class CZQLTSMD : public CSecurityFtdcMdSpi  
{
public:
	void OnFrontConnected();
	void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnFrontDisconnected(int nReason);
	void Start();
	void Init();
	void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData);
	CZQLTSMD();
	virtual ~CZQLTSMD();

private:
	CSecurityFtdcMdApi * m_pMdApi;

};

#endif // !defined(AFX_ZQLTSMD_H__E1C8C421_0DB0_4129_AC7E_7DF1BB1C6DFC__INCLUDED_)
