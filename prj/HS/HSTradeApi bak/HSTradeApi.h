#pragma once

#include <map>
#include "t2sdk_interface.h"
#include "MarketDefine.h"
#include "Condition.h"

#ifdef HS_EXPORTS
#define HS_API  __declspec(dllexport)
#else 
#define HS_API  __declspec(dllimport)
#endif 

namespace kiiik {

class HS_API HSTradeApi : public CCallbackInterface
{
	enum {
		SYNCSEND = 0,
		ASYNCSEND = 1,
		USER_TOKEN_LEN = 256
	};
public:
	HSTradeApi(void);
	virtual ~HSTradeApi(void);

	bool Init(const std::string& conf_path, TradeSpi* spi, std::string& err);
	bool Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err);
	bool Logout(std::string& err);

	//沪深股票，债券(场内)，基金(场内)买卖等业务.


	//查询接口.
	int QueryAccount(std::string& err);
	int QueryPosition(std::string& err);
	int QueryOrder(std::string& err);
	int QueryTrade(std::string& err);
	//下单接口.
	int SubmitOrder(const OrderParamData& param, std::string& err);
	int CancelOrder(const OrderData& param, std::string& err);

	///券商回调函数接口
	unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv) { return 0; }
	unsigned long  FUNCTION_CALL_MODE AddRef() { return 0; }
	unsigned long  FUNCTION_CALL_MODE Release() { return 0; }
	void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection){ spi_->OnConnect(); }
	void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection){ spi_->OnConnect(); }
	void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection){}
	void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection){ spi_->OnDisconnect("断开连接"); }
	void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData) {}
	void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d) {}
	void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d) {}
	int  FUNCTION_CALL_MODE Reserved3() { return 0; }
	void FUNCTION_CALL_MODE Reserved4() {}
	void FUNCTION_CALL_MODE Reserved5() {}
	void FUNCTION_CALL_MODE Reserved6() {}
	void FUNCTION_CALL_MODE Reserved7() {}
	void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult) { }
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);

private:
	///通过这个接口收发数据，建立连接，进行发布订阅等等操作
	CConnectionInterface *conn_;
	///
	CConfigInterface* config_;
	///子账号 子策略号
	char user_token_[USER_TOKEN_LEN];

	TradeSpi* spi_;
	bool is_login_;
	std::string error_msg_;

	std::string s_broker_id_;
	int broker_id_;
	std::string user_id_;
	std::string password_;

	std::map<int, OrderParamData> order_param_cache_;
	std::map<int, OrderData> cancel_order_param_cache_;

	Mutex wait_mutex_;
	Condition wait_cond_;
	bool TimeWait(int sec)
	{
		if (sec <= 0) { return false; }

		MutexLocker lock(&wait_mutex_);
		return wait_cond_.TimedWait(&wait_mutex_, sec);
	}

	void ReleaseWait()
	{
		wait_cond_.Signal();
	}
};

}


