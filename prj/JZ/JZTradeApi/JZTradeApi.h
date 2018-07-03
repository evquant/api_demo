#ifndef GTA_STOCK_MARKET_TRADE_API_H
#define GTA_STOCK_MARKET_TRADE_API_H

#include <map>
#include "kcbpcli.h"
#include "MarketDefine.h"

#ifdef JZ_TD_EXPORTS
#define JZ_TD_API  __declspec(dllexport)
#else 
#define JZ_TD_API  __declspec(dllimport)
#endif 

namespace kiiik {

enum GtaQueryEnum {
	kQueryAccount,
	kQueryOrder,
	kQueryTrade,
	kQueryPosition,
	kQryInstrument,
	kQueryOther
};

class JZ_TD_API JZTradeApi {
public:
	JZTradeApi(void);
	virtual ~JZTradeApi(void);

	bool Init(const std::string& ip, int port, 
		const std::string& szServerName,
		const std::string& szUserName,
		const std::string& szPassword,
		const std::string& szSendQName,
		const std::string& szReceiveQName,
		TradeSpi* spi, std::string& err);
	bool Login(const std::string& broker_id, 
		const std::string& user_id, const std::string& password, 
		const std::string& szLoginNetaddr,
		std::string& err);
	bool Logout(std::string& err);
	bool InitPreTrade(std::string& err) { return true; }

	int QueryAccount(std::string& err);
	int QueryPosition(std::string& err);
	int QueryOrder(std::string& err);
	int QueryTrade(std::string& err);
	bool QryInstrument(std::string& err);

	// 用[410336]实现 批量委托: 只提供用于普通股票、基金、和债券的买卖业务，不可用于新股申购等非交易或其他特殊交易业务。允许单次委托笔数上限为50笔
	int SubmitOrder(const OrderParamData& param, std::string& err);
	// 用[410411]实现 委托买卖业务: 
	// 可做深圳的转股回售，但是BSFLAG必须送G或H，前台需要单独菜单】【也可以做交易所基金的申购和赎回，买卖类别送3或4
	// 可做深圳的权证行权，但是BSFLAG必须送7，前台需要单独菜单
	int SubmitOrder(const OrderParamData *param, int num, std::string& err);
	// 用委托序号或委托批号撤单
	int CancelOrder(const OrderData& param, std::string& err);

private:
	KCBPCLIHANDLE m_hKCBPHandle;

	bool m_succed_connect;	/**< 连接成功 */
	bool m_succed_login;	/**< 登陆成功 */
	std::string ip_;
	int port_;
	std::string m_user_id;
	std::string m_password;
	typedef struct
	{
		char szCustid[30];					//客户代码
		char szLoginOrgid[12];				//机构代码
		char szTradePassword[40];			//加密的交易密码
		char szLoginNetaddr[40];			//网卡地址或电话
		char szOperateOrgid[12];			//操作机构
		char szOperway[4];					//操作方式
		char szServerid[4];					//服务器序号
	}CustInfo;
	CustInfo m_cust_info;

	std::map<ExchangeIdType, std::string> exe_secuid_; // 交易所 => 股东代码
	std::string bankcode_; // 外部银行
	bool m_iCreditflag; // 是否是信用账户

	TradeSpi *spi_;

private:
	int Stock_ADoTrade(const char *szFuncID, const char *szParameter, std::string &err);
	int query(tagCallCtrl &stControl, std::string &szRetMsg);

	void OnQueryAccount(std::string& szRetMsg);
	void OnQueryPosition(std::string& szRetMsg);
	void OnQueryOrder(std::string& szRetMsg);
	void OnQueryTrade(std::string& szRetMsg);
	void OnQueryCancelAbledOrder(std::string& szRetMsg);
	void OnQryInstrument(std::string& szRetMsg);

	GtaQueryEnum GetQryType(const char *szFuncID) {
		if (!strcmp(szFuncID, "410502")) return kQueryAccount;
		else if (!strcmp(szFuncID, "410510")) return kQueryOrder;
		else if (!strcmp(szFuncID, "410512")) return kQueryTrade;
		else if (!strcmp(szFuncID, "410503")) return kQueryPosition;
		else if (!strcmp(szFuncID, "410203")) return kQryInstrument;
		else return kQueryOther;
	}
};

}

#endif
