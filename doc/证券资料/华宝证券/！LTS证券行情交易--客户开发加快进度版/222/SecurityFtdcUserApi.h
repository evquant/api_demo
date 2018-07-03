/////////////////////////////////////////////////////////////////////////
///@company shanghai liber information Technology Co.,Ltd
///@file SecurityFtdcUserApi.h
///@brief 定义客户端接口
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
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected(){};
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason){};
		
	///心跳超时警告
	virtual void OnHeartBeatWarning(int nTimeLapse){};
	

	///错误应答
	virtual void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///登录请求响应
	virtual void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///登出请求响应
	virtual void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///全部登出请求响应
	virtual void OnRspLogoutAll(CSecurityFtdcLogoutAllField *pLogoutAll, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///强制用户登出请求响应
	virtual void OnRspForceUserLogout(CSecurityFtdcForceUserLogoutField *pForceUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///强制用户退出请求响应
	virtual void OnRspForceUserExit(CSecurityFtdcForceUserLogoutField *pForceUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///报单录入请求响应
	virtual void OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///报单操作请求响应
	virtual void OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///订阅行情应答
	virtual void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///根据交易所代码订阅行情应答
	virtual void OnRspSubMarketDataByExchange(CSecurityFtdcSpecificExchangeField *pSpecificExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///根据交易所代码取消订阅行情应答
	virtual void OnRspUnSubMarketDataByExchange(CSecurityFtdcSpecificExchangeField *pSpecificExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加合约响应
	virtual void OnRspInsInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改合约响应
	virtual void OnRspUpdInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改交易编码响应
	virtual void OnRspUpdTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加管理用户权限响应
	virtual void OnRspInsSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除管理用户权限响应
	virtual void OnRspDelSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改经纪公司用户口令响应
	virtual void OnRspUpdBrokerUserPassword(CSecurityFtdcBrokerUserPasswordField *pBrokerUserPassword, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加经纪公司用户权限响应
	virtual void OnRspInsBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除经纪公司用户权限响应
	virtual void OnRspDelBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改资金账户密码响应
	virtual void OnRspUpdTradingAccountPassword(CSecurityFtdcTradingAccountPasswordField *pTradingAccountPassword, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加投资者账户响应
	virtual void OnRspInsInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改投资者账户响应
	virtual void OnRspUpdInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加用户权限响应
	virtual void OnRspInsUserRight(CSecurityFtdcUserRightField *pUserRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改用户权限响应
	virtual void OnRspUpdUserRight(CSecurityFtdcUserRightField *pUserRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除用户权限响应
	virtual void OnRspDelUserRight(CSecurityFtdcUserRightField *pUserRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除禁止登录用户响应
	virtual void OnRspDelLoginForbiddenUser(CSecurityFtdcLoginForbiddenUserField *pLoginForbiddenUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加用户IP响应
	virtual void OnRspInsUserIP(CSecurityFtdcUserIPField *pUserIP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改用户IP响应
	virtual void OnRspUpdUserIP(CSecurityFtdcUserIPField *pUserIP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除用户IP响应
	virtual void OnRspDelUserIP(CSecurityFtdcUserIPField *pUserIP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加用户动态令牌参数响应
	virtual void OnRspInsBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改用户动态令牌参数响应
	virtual void OnRspUpdBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除用户动态令牌参数响应
	virtual void OnRspDelBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求增加合约交易权限响应
	virtual void OnRspInsInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求修改合约交易权限响应
	virtual void OnRspUpdInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求删除合约交易权限响应
	virtual void OnRspDelInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///资金账户口令更新请求响应
	virtual void OnRspTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求同步出入金响应
	virtual void OnRspSyncDeposit(CSecurityFtdcSyncDepositField *pSyncDeposit, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求手工同步用户动态令牌响应
	virtual void OnRspManualSyncBrokerUserOTP(CSecurityFtdcManualSyncBrokerUserOTPField *pManualSyncBrokerUserOTP, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易所响应
	virtual void OnRspQryExchange(CSecurityFtdcExchangeField *pExchange, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询产品响应
	virtual void OnRspQryProduct(CSecurityFtdcProductField *pProduct, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约响应
	virtual void OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易员响应
	virtual void OnRspQryTrader(CSecurityFtdcTraderField *pTrader, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询经纪公司响应
	virtual void OnRspQryBroker(CSecurityFtdcBrokerField *pBroker, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询经纪公司会员代码响应
	virtual void OnRspQryPartBroker(CSecurityFtdcPartBrokerField *pPartBroker, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者响应
	virtual void OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易编码响应
	virtual void OnRspQryTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询管理用户响应
	virtual void OnRspQrySuperUser(CSecurityFtdcSuperUserField *pSuperUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询管理用户功能权限响应
	virtual void OnRspQrySuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询经纪公司用户响应
	virtual void OnRspQryBrokerUser(CSecurityFtdcBrokerUserField *pBrokerUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询经纪公司用户权限响应
	virtual void OnRspQryBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询禁止登录用户响应
	virtual void OnRspQryLoginForbiddenUser(CSecurityFtdcLoginForbiddenUserField *pLoginForbiddenUser, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询行情响应
	virtual void OnRspQryDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约交易权限响应
	virtual void OnRspQryInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionDetail(CSecurityFtdcInvestorPositionDetailField *pInvestorPositionDetail, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询债券利息响应
	virtual void OnRspQryBondInterest(CSecurityFtdcBondInterestField *pBondInterest, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易员报盘机响应
	virtual void OnRspQryTraderOffer(CSecurityFtdcTraderOfferField *pTraderOffer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询行情报盘机响应
	virtual void OnRspQryMDTraderOffer(CSecurityFtdcMDTraderOfferField *pMDTraderOffer, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询前置状态响应
	virtual void OnRspQryFrontStatus(CSecurityFtdcFrontStatusField *pFrontStatus, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询用户会话响应
	virtual void OnRspQryUserSession(CSecurityFtdcUserSessionField *pUserSession, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询报单响应
	virtual void OnRspQryOrder(CSecurityFtdcOrderField *pOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查报单操作响应
	virtual void OnRspQryOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询错误报单响应
	virtual void OnRspQryErrOrder(CSecurityFtdcErrOrderField *pErrOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询错误报单操作响应
	virtual void OnRspQryErrOrderAction(CSecurityFtdcErrOrderActionField *pErrOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询成交响应
	virtual void OnRspQryTrade(CSecurityFtdcTradeField *pTrade, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询出入金流水响应
	virtual void OnRspQrySyncDeposit(CSecurityFtdcSyncDepositField *pSyncDeposit, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询联系人响应
	virtual void OnRspQryBrokerUserEvent(CSecurityFtdcBrokerUserEventField *pBrokerUserEvent, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CSecurityFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///报单通知
	virtual void OnRtnOrder(CSecurityFtdcOrderField *pOrder) {};

	///成交通知
	virtual void OnRtnTrade(CSecurityFtdcTradeField *pTrade) {};

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo) {};

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo) {};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData) {};
};

class USER_API_EXPORT CSecurityFtdcUserApi
{
public:
	///创建UserApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	static CSecurityFtdcUserApi *CreateFtdcUserApi(const char *pszFlowPath = "");
	
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release() = 0;
	
	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	virtual void Init() = 0;
	
	///等待接口线程结束运行
	virtual int Join() = 0;
	
	///获取当前交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	virtual const char *GetTradingDay() = 0;
	
	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(char *pszFrontAddress) = 0;
	
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CSecurityFtdcUserSpi *pSpi) = 0;
	
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount, char* pExchageID) = 0;

	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount, char* pExchageID) = 0;
	
	virtual int SubMarketDataByExchange(char *ppExchageID[], int nCount) = 0;

	virtual int UnSubMarketDataByExchange(char *ppExchageID[], int nCount) = 0;
	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        SECURITY_TERT_RESTART:从本交易日开始重传
	///        SECURITY_TERT_RESUME:从上次收到的续传
	///        SECURITY_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribePrivateTopic(SECURITY_TE_RESUME_TYPE nResumeType) = 0;
	
	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        SECURITY_TERT_RESTART:从本交易日开始重传
	///        SECURITY_TERT_RESUME:从上次收到的续传
	///        SECURITY_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	virtual void SubscribePublicTopic(SECURITY_TE_RESUME_TYPE nResumeType) = 0;
	
	///用户登录请求
	virtual int ReqUserLogin(CSecurityFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

	///登出请求
	virtual int ReqUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, int nRequestID) = 0;

	///全部登出请求
	virtual int ReqLogoutAll(CSecurityFtdcLogoutAllField *pLogoutAll, int nRequestID) = 0;

	///强制用户登出请求
	virtual int ReqForceUserLogout(CSecurityFtdcForceUserLogoutField *pForceUserLogout, int nRequestID) = 0;

	///强制用户退出请求
	virtual int ReqForceUserExit(CSecurityFtdcForceUserLogoutField *pForceUserLogout, int nRequestID) = 0;

	///报单录入请求
	virtual int ReqOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, int nRequestID) = 0;

	///报单操作请求
	virtual int ReqOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, int nRequestID) = 0;

	///请求增加合约
	virtual int ReqInsInstrument(CSecurityFtdcInstrumentField *pInstrument, int nRequestID) = 0;

	///请求修改合约
	virtual int ReqUpdInstrument(CSecurityFtdcInstrumentField *pInstrument, int nRequestID) = 0;

	///请求修改交易编码
	virtual int ReqUpdTradingCode(CSecurityFtdcTradingCodeField *pTradingCode, int nRequestID) = 0;

	///请求增加管理用户权限
	virtual int ReqInsSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, int nRequestID) = 0;

	///请求删除管理用户权限
	virtual int ReqDelSuperUserFunction(CSecurityFtdcSuperUserFunctionField *pSuperUserFunction, int nRequestID) = 0;

	///请求修改经纪公司用户口令
	virtual int ReqUpdBrokerUserPassword(CSecurityFtdcBrokerUserPasswordField *pBrokerUserPassword, int nRequestID) = 0;

	///请求增加经纪公司用户权限
	virtual int ReqInsBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, int nRequestID) = 0;

	///请求删除经纪公司用户权限
	virtual int ReqDelBrokerUserFunction(CSecurityFtdcBrokerUserFunctionField *pBrokerUserFunction, int nRequestID) = 0;

	///请求修改资金账户密码
	virtual int ReqUpdTradingAccountPassword(CSecurityFtdcTradingAccountPasswordField *pTradingAccountPassword, int nRequestID) = 0;

	///请求增加投资者账户
	virtual int ReqInsInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///请求修改投资者账户
	virtual int ReqUpdInvestorAccount(CSecurityFtdcInvestorAccountField *pInvestorAccount, int nRequestID) = 0;

	///请求增加用户权限
	virtual int ReqInsUserRight(CSecurityFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///请求修改用户权限
	virtual int ReqUpdUserRight(CSecurityFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///请求删除用户权限
	virtual int ReqDelUserRight(CSecurityFtdcUserRightField *pUserRight, int nRequestID) = 0;

	///请求删除禁止登录用户
	virtual int ReqDelLoginForbiddenUser(CSecurityFtdcLoginForbiddenUserField *pLoginForbiddenUser, int nRequestID) = 0;

	///请求增加用户IP
	virtual int ReqInsUserIP(CSecurityFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///请求修改用户IP
	virtual int ReqUpdUserIP(CSecurityFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///请求删除用户IP
	virtual int ReqDelUserIP(CSecurityFtdcUserIPField *pUserIP, int nRequestID) = 0;

	///请求增加用户动态令牌参数
	virtual int ReqInsBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///请求修改用户动态令牌参数
	virtual int ReqUpdBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///请求删除用户动态令牌参数
	virtual int ReqDelBrokerUserOTPParam(CSecurityFtdcBrokerUserOTPParamField *pBrokerUserOTPParam, int nRequestID) = 0;

	///请求增加合约交易权限
	virtual int ReqInsInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///请求修改合约交易权限
	virtual int ReqUpdInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///请求删除合约交易权限
	virtual int ReqDelInstrumentTradingRight(CSecurityFtdcInstrumentTradingRightField *pInstrumentTradingRight, int nRequestID) = 0;

	///用户口令更新请求
	virtual int ReqUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) = 0;

	///资金账户口令更新请求
	virtual int ReqTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) = 0;

	///请求同步出入金
	virtual int ReqSyncDeposit(CSecurityFtdcSyncDepositField *pSyncDeposit, int nRequestID) = 0;

	///请求手工同步用户动态令牌
	virtual int ReqManualSyncBrokerUserOTP(CSecurityFtdcManualSyncBrokerUserOTPField *pManualSyncBrokerUserOTP, int nRequestID) = 0;

	///请求查询交易所
	virtual int ReqQryExchange(CSecurityFtdcQryExchangeField *pQryExchange, int nRequestID) = 0;

	///请求查询产品
	virtual int ReqQryProduct(CSecurityFtdcQryProductField *pQryProduct, int nRequestID) = 0;

	///请求查询合约
	virtual int ReqQryInstrument(CSecurityFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///请求查询交易员
	virtual int ReqQryTrader(CSecurityFtdcQryTraderField *pQryTrader, int nRequestID) = 0;

	///请求查询经纪公司
	virtual int ReqQryBroker(CSecurityFtdcQryBrokerField *pQryBroker, int nRequestID) = 0;

	///请求查询经纪公司会员代码
	virtual int ReqQryPartBroker(CSecurityFtdcQryPartBrokerField *pQryPartBroker, int nRequestID) = 0;

	///请求查询投资者
	virtual int ReqQryInvestor(CSecurityFtdcQryInvestorField *pQryInvestor, int nRequestID) = 0;

	///请求查询交易编码
	virtual int ReqQryTradingCode(CSecurityFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) = 0;

	///请求查询管理用户
	virtual int ReqQrySuperUser(CSecurityFtdcQrySuperUserField *pQrySuperUser, int nRequestID) = 0;

	///请求查询管理用户功能权限
	virtual int ReqQrySuperUserFunction(CSecurityFtdcQrySuperUserFunctionField *pQrySuperUserFunction, int nRequestID) = 0;

	///请求查询经纪公司用户
	virtual int ReqQryBrokerUser(CSecurityFtdcQryBrokerUserField *pQryBrokerUser, int nRequestID) = 0;

	///请求查询经纪公司用户权限
	virtual int ReqQryBrokerUserFunction(CSecurityFtdcQryBrokerUserFunctionField *pQryBrokerUserFunction, int nRequestID) = 0;

	///请求查询资金账户
	virtual int ReqQryTradingAccount(CSecurityFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) = 0;

	///请求查询禁止登录用户
	virtual int ReqQryLoginForbiddenUser(CSecurityFtdcQryLoginForbiddenUserField *pQryLoginForbiddenUser, int nRequestID) = 0;

	///请求查询行情
	virtual int ReqQryDepthMarketData(CSecurityFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) = 0;

	///请求查询合约交易权限率
	virtual int ReqQryInstrumentTradingRight(CSecurityFtdcQryInstrumentTradingRightField *pQryInstrumentTradingRight, int nRequestID) = 0;

	///请求查询投资者持仓明细
	virtual int ReqQryInvestorPositionDetail(CSecurityFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) = 0;

	///请求查询债券利息
	virtual int ReqQryBondInterest(CSecurityFtdcQryBondInterestField *pQryBondInterest, int nRequestID) = 0;

	///请求查询交易员报盘机
	virtual int ReqQryTraderOffer(CSecurityFtdcQryTraderOfferField *pQryTraderOffer, int nRequestID) = 0;

	///请求查询行情报盘机
	virtual int ReqQryMDTraderOffer(CSecurityFtdcQryMDTraderOfferField *pQryMDTraderOffer, int nRequestID) = 0;

	///请求查询前置状态
	virtual int ReqQryFrontStatus(CSecurityFtdcQryFrontStatusField *pQryFrontStatus, int nRequestID) = 0;

	///请求查询用户会话
	virtual int ReqQryUserSession(CSecurityFtdcQryUserSessionField *pQryUserSession, int nRequestID) = 0;

	///请求查询报单
	virtual int ReqQryOrder(CSecurityFtdcQryOrderField *pQryOrder, int nRequestID) = 0;

	///请求查询报单操作
	virtual int ReqQryOrderAction(CSecurityFtdcQryOrderActionField *pQryOrderAction, int nRequestID) = 0;

	///请求查询错误报单
	virtual int ReqQryErrOrder(CSecurityFtdcQryErrOrderField *pQryErrOrder, int nRequestID) = 0;

	///请求查询错误报单操作
	virtual int ReqQryErrOrderAction(CSecurityFtdcQryErrOrderActionField *pQryErrOrderAction, int nRequestID) = 0;

	///请求查询成交
	virtual int ReqQryTrade(CSecurityFtdcQryTradeField *pQryTrade, int nRequestID) = 0;

	///请求查询投资者持仓
	virtual int ReqQryInvestorPosition(CSecurityFtdcQryInvestorPositionField *pQryInvestorPosition, int nRequestID) = 0;

	///请求查询出入金流水
	virtual int ReqQrySyncDeposit(CSecurityFtdcQrySyncDepositField *pQrySyncDeposit, int nRequestID) = 0;

	///请求查询联系人
	virtual int ReqQryBrokerUserEvent(CSecurityFtdcQryBrokerUserEventField *pQryBrokerUserEvent, int nRequestID) = 0;

	///请求查询合约手续费率
	virtual int ReqQryInstrumentCommissionRate(CSecurityFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) = 0;
protected:
	~CSecurityFtdcUserApi(){};
};
#endif
