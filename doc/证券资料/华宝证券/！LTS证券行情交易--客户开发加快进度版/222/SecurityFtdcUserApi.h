/////////////////////////////////////////////////////////////////////////
///@company shanghai liber information Technology Co.,Ltd
///@file SecurityFtdcUserApi.h
///@brief ����ͻ��˽ӿ�
/////////////////////////////////////////////////////////////////////////

#if !defined(SECURITY_FTDCUSERAPI_H)
#define SECURITY_FTDCUSERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SecurityFtdcUserApiStruct.h"

#if defined(USERAPI_IS_LIB) && defined(WIN32)
#ifdef LIB_USER_API_EXPORT
#define USER_API_EXPORT __declspec(dllexport)
#else
#define USER_API_EXPORT __declspec(dllimport)
#endif
#else
#define USER_API_EXPORT 
#endif

class CSecurityFtdcUserSpi
{
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected(){};
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason){};
		
	///������ʱ����
	virtual void OnHeartBeatWarning(int nTimeLapse){};
	

	///����Ӧ��
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��¼������Ӧ
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȫ���ǳ�������Ӧ
	virtual void OnRspLogoutAll(CSecurityFtdcLogoutAllField *pLogoutAll, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ǿ���û��ǳ�������Ӧ
	virtual void OnRspForceUserLogout(CSecurityFtdcForceUserLogoutField *pForceUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ǿ���û��˳�������Ӧ
	virtual void OnRspForceUserExit(CSecurityFtdcForceUserLogoutField *pForceUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������������Ӧ
	virtual void OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ӧ��
	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ݽ��������붩������Ӧ��
	virtual void OnRspSubMarketDataByExchange(CSecurityFtdcSpecificExchangeField *pSpecificExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ݽ���������ȡ����������Ӧ��
	virtual void OnRspUnSubMarketDataByExchange(CSecurityFtdcSpecificExchangeField *pSpecificExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ��Ӧ
	virtual void OnRspInsInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ��Ӧ
	virtual void OnRspUpdInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸Ľ��ױ�����Ӧ
	virtual void OnRspUpdTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������ӹ����û�Ȩ����Ӧ
	virtual void OnRspInsSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�������û�Ȩ����Ӧ
	virtual void OnRspDelSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ľ��͹�˾�û�������Ӧ
	virtual void OnRspUpdBrokerUserPassword(CSecurityFtdcBrokerUserPasswordField *pBrokerUserPassword, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӿ��͹�˾�û�Ȩ����Ӧ
	virtual void OnRspInsBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ�����͹�˾�û�Ȩ����Ӧ
	virtual void OnRspDelBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��ʽ��˻�������Ӧ
	virtual void OnRspUpdTradingAccountPassword(CSecurityFtdcTradingAccountPasswordField *pTradingAccountPassword, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��������Ͷ�����˻���Ӧ
	virtual void OnRspInsInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸�Ͷ�����˻���Ӧ
	virtual void OnRspUpdInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û�Ȩ����Ӧ
	virtual void OnRspInsUserRight(CSecurityFtdcUserRightField *pUserRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û�Ȩ����Ӧ
	virtual void OnRspUpdUserRight(CSecurityFtdcUserRightField *pUserRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û�Ȩ����Ӧ
	virtual void OnRspDelUserRight(CSecurityFtdcUserRightField *pUserRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����ֹ��¼�û���Ӧ
	virtual void OnRspDelLoginForbiddenUser(CSecurityFtdcLoginForbiddenUserField *pLoginForbiddenUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û�IP��Ӧ
	virtual void OnRspInsUserIP(CSecurityFtdcUserIPField *pUserIP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û�IP��Ӧ
	virtual void OnRspUpdUserIP(CSecurityFtdcUserIPField *pUserIP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û�IP��Ӧ
	virtual void OnRspDelUserIP(CSecurityFtdcUserIPField *pUserIP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���������û���̬���Ʋ�����Ӧ
	virtual void OnRspInsBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸��û���̬���Ʋ�����Ӧ
	virtual void OnRspUpdBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ���û���̬���Ʋ�����Ӧ
	virtual void OnRspDelBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ӻ�Լ����Ȩ����Ӧ
	virtual void OnRspInsInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����޸ĺ�Լ����Ȩ����Ӧ
	virtual void OnRspUpdInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ɾ����Լ����Ȩ����Ӧ
	virtual void OnRspDelInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�û��������������Ӧ
	virtual void OnRspUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ʽ��˻��������������Ӧ
	virtual void OnRspTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ͬ���������Ӧ
	virtual void OnRspSyncDeposit(CSecurityFtdcSyncDepositField *pSyncDeposit, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ֹ�ͬ���û���̬������Ӧ
	virtual void OnRspManualSyncBrokerUserOTP(CSecurityFtdcManualSyncBrokerUserOTPField *pManualSyncBrokerUserOTP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��������Ӧ
	virtual void OnRspQryExchange(CSecurityFtdcExchangeField *pExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Ʒ��Ӧ
	virtual void OnRspQryProduct(CSecurityFtdcProductField *pProduct, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����Ա��Ӧ
	virtual void OnRspQryTrader(CSecurityFtdcTraderField *pTrader, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾��Ӧ
	virtual void OnRspQryBroker(CSecurityFtdcBrokerField *pBroker, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾��Ա������Ӧ
	virtual void OnRspQryPartBroker(CSecurityFtdcPartBrokerField *pPartBroker, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ������Ӧ
	virtual void OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���ױ�����Ӧ
	virtual void OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�����û���Ӧ
	virtual void OnRspQrySuperUser(CSecurityFtdcSuperUserField *pSuperUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�����û�����Ȩ����Ӧ
	virtual void OnRspQrySuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾�û���Ӧ
	virtual void OnRspQryBrokerUser(CSecurityFtdcBrokerUserField *pBrokerUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���͹�˾�û�Ȩ����Ӧ
	virtual void OnRspQryBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��ֹ��¼�û���Ӧ
	virtual void OnRspQryLoginForbiddenUser(CSecurityFtdcLoginForbiddenUserField *pLoginForbiddenUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������Ӧ
	virtual void OnRspQryDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ����Ȩ����Ӧ
	virtual void OnRspQryInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	virtual void OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionDetailField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯծȯ��Ϣ��Ӧ
	virtual void OnRspQryBondInterest(CSecurityFtdcBondInterestField *pBondInterest, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ����Ա���̻���Ӧ
	virtual void OnRspQryTraderOffer(CSecurityFtdcTraderOfferField *pTraderOffer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���鱨�̻���Ӧ
	virtual void OnRspQryMDTraderOffer(CSecurityFtdcMDTraderOfferField *pMDTraderOffer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯǰ��״̬��Ӧ
	virtual void OnRspQryFrontStatus(CSecurityFtdcFrontStatusField *pFrontStatus, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�û��Ự��Ӧ
	virtual void OnRspQryUserSession(CSecurityFtdcUserSessionField *pUserSession, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����鱨��������Ӧ
	virtual void OnRspQryOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���󱨵���Ӧ
	virtual void OnRspQryErrOrder(CSecurityFtdcErrOrderField *pErrOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ���󱨵�������Ӧ
	virtual void OnRspQryErrOrderAction(CSecurityFtdcErrOrderActionField *pErrOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ�������ˮ��Ӧ
	virtual void OnRspQrySyncDeposit(CSecurityFtdcSyncDepositField *pSyncDeposit, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��ϵ����Ӧ
	virtual void OnRspQryBrokerUserEvent(CSecurityFtdcBrokerUserEventField *pBrokerUserEvent, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ѯ��Լ����������Ӧ
	virtual void OnRspQryInstrumentCommissionRate(CSecurityFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����֪ͨ
	virtual void OnRtnOrder(CSecurityFtdcOrderField *pOrder) {};

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CSecurityFtdcTradeField *pTrade) {};

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo) {};

	///������������ر�
	virtual void OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo) {};

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData) {};
};

class USER_API_EXPORT CSecurityFtdcUserApi
{
public:
	///����UserApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	static CSecurityFtdcUserApi *CreateFtdcUserApi(const char *pszFlowPath = "");
	
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release() = 0;
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init() = 0;
	
	///�ȴ��ӿ��߳̽�������
	virtual int Join() = 0;
	
	///��ȡ��ǰ������
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay() = 0;
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress) = 0;
	
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CSecurityFtdcUserSpi *pSpi) = 0;
	
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount, char* pExchageID) = 0;

	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount, char* pExchageID) = 0;
	
	virtual int SubMarketDataByExchange(char *ppExchageID[], int nCount) = 0;

	virtual int UnSubMarketDataByExchange(char *ppExchageID[], int nCount) = 0;
	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        SECURITY_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        SECURITY_TERT_RESUME:���ϴ��յ�������
	///        SECURITY_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(SECURITY_TE_RESUME_TYPE nResumeType) = 0;
	
	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        SECURITY_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        SECURITY_TERT_RESUME:���ϴ��յ�������
	///        SECURITY_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(SECURITY_TE_RESUME_TYPE nResumeType) = 0;
	
	///�û���¼����
	virtual int ReqUserLogin(CSecurityFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

	///�ǳ�����
	virtual int ReqUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, int nRequestID) = 0;

	///ȫ���ǳ�����
	virtual int ReqLogoutAll(CSecurityFtdcLogoutAllField *pLogoutAll, int nRequestID) = 0;

	///ǿ���û��ǳ�����
	virtual int ReqForceUserLogout(CSecurityFtdcForceUserLogoutField *pForceUserLogout, int nRequestID) = 0;

	///ǿ���û��˳�����
	virtual int ReqForceUserExit(CSecurityFtdcForceUserLogoutField *pForceUserLogout, int nRequestID) = 0;

	///����¼������
	virtual int ReqOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, int nRequestID) = 0;

	///������������
	virtual int ReqOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, int nRequestID) = 0;

	///�������Ӻ�Լ
	virtual int ReqInsInstrument(CSecurityFtdcInstrumentField *pInstrument, int nRequestID) = 0;

	///�����޸ĺ�Լ
	virtual int ReqUpdInstrument(CSecurityFtdcInstrumentField *pInstrument, int nRequestID) = 0;

	///�����޸Ľ��ױ���
	virtual int ReqUpdTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, int nRequestID) = 0;

	///�������ӹ����û�Ȩ��
	virtual int ReqInsSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, int nRequestID) = 0;

	///����ɾ�������û�Ȩ��
	virtual int ReqDelSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, int nRequestID) = 0;

	///�����޸ľ��͹�˾�û�����
	virtual int ReqUpdBrokerUserPassword(CSecurityFtdcBrokerUserPasswordField *pBrokerUserPassword, int nRequestID) = 0;

	///�������Ӿ��͹�˾�û�Ȩ��
	virtual int ReqInsBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, int nRequestID) = 0;

	///����ɾ�����͹�˾�û�Ȩ��
	virtual int ReqDelBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, int nRequestID) = 0;

	///�����޸��ʽ��˻�����
	virtual int ReqUpdTradingAccountPassword(CSecurityFtdcTradingAccountPasswordField *pTradingAccountPassword, int nRequestID) = 0;

	///��������Ͷ�����˻�
	virtual int ReqInsInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///�����޸�Ͷ�����˻�
	virtual int ReqUpdInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///���������û�Ȩ��
	virtual int ReqInsUserRight(CSecurityFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///�����޸��û�Ȩ��
	virtual int ReqUpdUserRight(CSecurityFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///����ɾ���û�Ȩ��
	virtual int ReqDelUserRight(CSecurityFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///����ɾ����ֹ��¼�û�
	virtual int ReqDelLoginForbiddenUser(CSecurityFtdcLoginForbiddenUserField *pLoginForbiddenUser, int nRequestID) = 0;

	///���������û�IP
	virtual int ReqInsUserIP(CSecurityFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///�����޸��û�IP
	virtual int ReqUpdUserIP(CSecurityFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///����ɾ���û�IP
	virtual int ReqDelUserIP(CSecurityFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///���������û���̬���Ʋ���
	virtual int ReqInsBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///�����޸��û���̬���Ʋ���
	virtual int ReqUpdBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///����ɾ���û���̬���Ʋ���
	virtual int ReqDelBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///�������Ӻ�Լ����Ȩ��
	virtual int ReqInsInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///�����޸ĺ�Լ����Ȩ��
	virtual int ReqUpdInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///����ɾ����Լ����Ȩ��
	virtual int ReqDelInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///�û������������
	virtual int ReqUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) = 0;

	///�ʽ��˻������������
	virtual int ReqTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) = 0;

	///����ͬ�������
	virtual int ReqSyncDeposit(CSecurityFtdcSyncDepositField *pSyncDeposit, int nRequestID) = 0;

	///�����ֹ�ͬ���û���̬����
	virtual int ReqManualSyncBrokerUserOTP(CSecurityFtdcManualSyncBrokerUserOTPField *pManualSyncBrokerUserOTP, int nRequestID) = 0;

	///�����ѯ������
	virtual int ReqQryExchange(CSecurityFtdcQryExchangeField *pQryExchange, int nRequestID) = 0;

	///�����ѯ��Ʒ
	virtual int ReqQryProduct(CSecurityFtdcQryProductField *pQryProduct, int nRequestID) = 0;

	///�����ѯ��Լ
	virtual int ReqQryInstrument(CSecurityFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///�����ѯ����Ա
	virtual int ReqQryTrader(CSecurityFtdcQryTraderField *pQryTrader, int nRequestID) = 0;

	///�����ѯ���͹�˾
	virtual int ReqQryBroker(CSecurityFtdcQryBrokerField *pQryBroker, int nRequestID) = 0;

	///�����ѯ���͹�˾��Ա����
	virtual int ReqQryPartBroker(CSecurityFtdcQryPartBrokerField *pQryPartBroker, int nRequestID) = 0;

	///�����ѯͶ����
	virtual int ReqQryInvestor(CSecurityFtdcQryInvestorField *pQryInvestor, int nRequestID) = 0;

	///�����ѯ���ױ���
	virtual int ReqQryTradingCode(CSecurityFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) = 0;

	///�����ѯ�����û�
	virtual int ReqQrySuperUser(CSecurityFtdcQrySuperUserField *pQrySuperUser, int nRequestID) = 0;

	///�����ѯ�����û�����Ȩ��
	virtual int ReqQrySuperUserFunction(CSecurityFtdcQrySuperUserFunctionField *pQrySuperUserFunction, int nRequestID) = 0;

	///�����ѯ���͹�˾�û�
	virtual int ReqQryBrokerUser(CSecurityFtdcQryBrokerUserField *pQryBrokerUser, int nRequestID) = 0;

	///�����ѯ���͹�˾�û�Ȩ��
	virtual int ReqQryBrokerUserFunction(CSecurityFtdcQryBrokerUserFunctionField *pQryBrokerUserFunction, int nRequestID) = 0;

	///�����ѯ�ʽ��˻�
	virtual int ReqQryTradingAccount(CSecurityFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) = 0;

	///�����ѯ��ֹ��¼�û�
	virtual int ReqQryLoginForbiddenUser(CSecurityFtdcQryLoginForbiddenUserField *pQryLoginForbiddenUser, int nRequestID) = 0;

	///�����ѯ����
	virtual int ReqQryDepthMarketData(CSecurityFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) = 0;

	///�����ѯ��Լ����Ȩ����
	virtual int ReqQryInstrumentTradingRight(CSecurityFtdcQryInstrumentTradingRightField *pQryInstrumentTradingRight, int nRequestID) = 0;

	///�����ѯͶ���ֲ߳���ϸ
	virtual int ReqQryInvestorPositionDetail(CSecurityFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) = 0;

	///�����ѯծȯ��Ϣ
	virtual int ReqQryBondInterest(CSecurityFtdcQryBondInterestField *pQryBondInterest, int nRequestID) = 0;

	///�����ѯ����Ա���̻�
	virtual int ReqQryTraderOffer(CSecurityFtdcQryTraderOfferField *pQryTraderOffer, int nRequestID) = 0;

	///�����ѯ���鱨�̻�
	virtual int ReqQryMDTraderOffer(CSecurityFtdcQryMDTraderOfferField *pQryMDTraderOffer, int nRequestID) = 0;

	///�����ѯǰ��״̬
	virtual int ReqQryFrontStatus(CSecurityFtdcQryFrontStatusField *pQryFrontStatus, int nRequestID) = 0;

	///�����ѯ�û��Ự
	virtual int ReqQryUserSession(CSecurityFtdcQryUserSessionField *pQryUserSession, int nRequestID) = 0;

	///�����ѯ����
	virtual int ReqQryOrder(CSecurityFtdcQryOrderField *pQryOrder, int nRequestID) = 0;

	///�����ѯ��������
	virtual int ReqQryOrderAction(CSecurityFtdcQryOrderActionField *pQryOrderAction, int nRequestID) = 0;

	///�����ѯ���󱨵�
	virtual int ReqQryErrOrder(CSecurityFtdcQryErrOrderField *pQryErrOrder, int nRequestID) = 0;

	///�����ѯ���󱨵�����
	virtual int ReqQryErrOrderAction(CSecurityFtdcQryErrOrderActionField *pQryErrOrderAction, int nRequestID) = 0;

	///�����ѯ�ɽ�
	virtual int ReqQryTrade(CSecurityFtdcQryTradeField *pQryTrade, int nRequestID) = 0;

	///�����ѯͶ���ֲ߳�
	virtual int ReqQryInvestorPosition(CSecurityFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID) = 0;

	///�����ѯ�������ˮ
	virtual int ReqQrySyncDeposit(CSecurityFtdcQrySyncDepositField *pQrySyncDeposit, int nRequestID) = 0;

	///�����ѯ��ϵ��
	virtual int ReqQryBrokerUserEvent(CSecurityFtdcQryBrokerUserEventField *pQryBrokerUserEvent, int nRequestID) = 0;

	///�����ѯ��Լ��������
	virtual int ReqQryInstrumentCommissionRate(CSecurityFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) = 0;
protected:
	~CSecurityFtdcUserApi(){};
};
#endif
