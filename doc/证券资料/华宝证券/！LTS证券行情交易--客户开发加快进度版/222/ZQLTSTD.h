// ZQLTSTD.h: interface for the CZQLTSTD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZQLTSTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_)
#define AFX_ZQLTSTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SecurityFtdcTraderApi.h"
#include "SecurityFtdcUserApiStruct.h"


class CZQLTSTD : public CSecurityFtdcTraderSpi  
{
public:

	CZQLTSTD(CSecurityFtdcTraderApi *m_pTdApi){}
	void Init();
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();
		///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	//void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	//void OnHeartBeatWarning(int nTimeLapse);

	///��¼������Ӧ
	void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	//void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	
	///�û��������������Ӧ
	//void OnRspUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�ʽ��˻��������������Ӧ
	//void OnRspTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ��󱨵�������Ӧ
	//void OnRspQueryMaxOrderVolume(CSecurityFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ��Լ��Ӧ
	void OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///����¼��������Ӧ
	void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	//ken ����
	void OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�����ѯ������Ӧ
	void OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�����ѯ�ɽ���Ӧ
	void OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ�ʽ��˻���Ӧ
	void OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ������Ӧ
	void OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//new
	///�����ѯ������Ӧ
	void OnRspQryDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

		///�����ѯ��������Ӧ
	void OnRspQryExchange(CSecurityFtdcExchangeField *pExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ���ױ�����Ӧ
	void OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	//new
	//ken ��ѯ�ֲ���ϸ�ر�
	void OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionDetailField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//void OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�����ѯͶ���ֲ߳���Ӧ
	void OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	
	
	///�����ѯ��Լ����������Ӧ
//	void OnRspQryInstrumentCommissionRate(CSecurityFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///����¼�����ر�
	void OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo);
	
	///������������ر�
	void OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo);

	///����֪ͨ
	void OnRtnOrder(CSecurityFtdcOrderField *pOrder);
	
	///�ɽ�֪ͨ
	void OnRtnTrade(CSecurityFtdcTradeField *pTrade);

	///��Լ����״̬֪ͨ
//	void OnRtnInstrumentStatus(CSecurityFtdcInstrumentStatusField *pInstrumentStatus);

	//CZQLTSTD();
	virtual ~CZQLTSTD();

private:
	double getPrice(TSecurityFtdcStockPriceType price);
	///�û���¼����
	void ReqUserLogin();
	///Ͷ���߽�����ȷ��
	void ReqSettlementInfoConfirm();
	///�����ѯ��Լ
	void ReqQryInstrument();
	///�����ѯ�ʽ��˻�
	void ReqQryTradingAccount();
	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo1(CSecurityFtdcRspInfoField *pRspInfo);

};
#endif // !defined(AFX_ZQLTSTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_)
