#ifndef JSD_TRADE_API_H
#define JSD_TRADE_API_H

#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")
typedef SOCKET HSocket; 

#include <queue>
#include <list>
#include "Thread.h"
#include "MarketDefine.h"

#ifdef JSD_EXPORTS
#define JSD_API  __declspec(dllexport)
#else 
#define JSD_API  __declspec(dllimport)
#endif 

namespace kiiik {

class JSDTradeApi;

class SpxSpi
{
public:
	virtual void OnRspense(int seq, int func_id, const std::string& rsp, bool is_last) = 0;
};

struct SpxReqData
{
	int func_id;
	bool multi_rsp;
	std::string req_str;
	SpxReqData(int id = 0, bool mul = false, const std::string& str = "") : func_id(id), multi_rsp(mul), req_str(str) {}
};

class SpxSession : public Thread
{
public:
	SpxSession(SpxSpi* spi, JSDTradeApi* api, int seq);
	~SpxSession(void);

	virtual void OnRun();

	void AppendReq(const SpxReqData& req);

	bool Connect(const std::string& ip, int port);
	bool Login(const std::string& login_cmd, std::string& err);

private:
	void DealReq(SpxReqData& req);
	bool Send(const std::string& req);
	void WaitSingleRsp();
	void WaitMultiRsp();
	bool WaitLoginRsp();

	int InitializeSocketEnvironment();
	void FreeSocketEnvironment();
	void GetAddressFrom(sockaddr_in *addr, const char *ip, int port);
	HSocket SocketOpen();
	void SocketClose(HSocket &handle);
	bool SetBufSize(HSocket &handle, bool is_recv, int size, std::string& err);

	bool GetReq(SpxReqData& req);
	void PopReq();
	int QueueSize();

private:
	int seq_;
	SpxSpi* spi_;
	JSDTradeApi* api_;
	HSocket socket_;
	static bool is_sock_env_init_;
	bool is_login_;
	std::string login_err_;
	int last_func_id_;

	std::queue<SpxReqData> requsets_;
	SpinLock requsets_lock_;
	Mutex mutex_;
	Condition cond_;
	Mutex timed_mutex_;
	Condition timed_cond_;
};



class JSD_API JSDTradeApi : public SpxSpi, public Thread
{
	enum SpxSessionSequence
	{
		SESS_RSP_TRADE = 0,		//用于获取成交回报
		SESS_RSP_ORDER = 1,		//用于获取委托回报
		SESS_SUBMIT_ORDER = 2,	//用于委托
		SESS_USER_QRY = 3,		//用于客户主动请求
		SESS_NUM = 4			//对话总个数
	};
	enum
	{
		QRY_ORDER_CYCLE = 10,			//隔 100 * 1000毫秒查一次委托
		HEART_BEAT_CYCLE = 100,			//心跳间隔
		QRY_TRADE_TIME = 600			//隔600毫秒查一次成交
	};
public:
	JSDTradeApi();
	virtual ~JSDTradeApi();

	virtual void OnRun();

	virtual void OnRspense(int seq, int func_id, const std::string& rsp, bool is_last);

	///以下为交易接口
	bool Init(const std::string& ip, int port, const std::string& sour_mac, const std::string& sour_ip, TradeSpi* spi, std::string& err);
	bool Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err);
	bool Logout(std::string& err);
	int QueryAccount(std::string& err);
	int QueryPosition(std::string& err);
	int QueryOrder(std::string& err);
	int QueryTrade(std::string& err);
	int QueryMoneyFlow(std::string& err);	//查询资金流水
	int SubmitOrder(const OrderParamData& param, std::string& err);
	int CancelOrder(const OrderData& param, std::string& err);

	//新增接口
	int QueryPlusTrade(const std::string& pos, std::string& err);	//日内增量成交查询
	//模拟环境测试异常
	int QueryHisTrade(const std::string& begin_date, const std::string& end_date, std::string& err);	//历史成交明细查询,模拟环境不支持
	//模拟环境测试异常
	int QueryHisAccountFlow(const std::string& begin_date, const std::string& end_date, std::string& err);	//查询资金流水
	int QueryAccountBalance(std::string& err);	//查询资金余额
	int QuerySpecificOrder(int order_id, std::string& err);	//查询特定委托的状态
	int QueryValidOrder(std::string& err);

	//用于维持常连接的心跳
	void HeartBeat();
	std::string GetUserId() { return user_id_; }

private:
	friend class SpxSession;
	std::string ReqHead(int func_id);
	std::string SepStr(int num);
	void StrToOrderData(OrderData& order_data, std::vector<std::string>& fields);
	OrderStatus ToOrderStatus(std::string& str);

	void DealLoginRsp(std::vector<std::string>& fields);
	void DealSubmitRsp(std::vector<std::string>& fields);
	void DealPlusTradeRsp(std::vector<std::string>& fields, bool is_last);
	void DealQueryTradeRsp(std::vector<std::string>& fields, bool is_last);
	void DealQueryHisTradeRsp(std::vector<std::string>& fields, bool is_last);
	void DealQueryOrderRsp(std::vector<std::string>& fields, bool is_last);
	void DealTimedQueryOrderRsp(std::vector<std::string>& fields);
	void DealCancelOrderRsp(std::vector<std::string>& fields);
	void DealQueryPositionRsp(std::vector<std::string>& fields, bool is_last);
	void DealQueryAccountRsp(std::vector<std::string>& fields, bool is_last);
	void DealQueryMoneyFlowRsp(std::vector<std::string>& fields, bool is_last);

	void TimeQueryOrder();
	//void TimeQueryWaitedOrder();
	void TimeQueryTrade();

	bool UpdateOrderWithTrade(const TradeData& trade_data);

private:
	SpxSession* spx_sessions_[SESS_NUM];
	TradeSpi* spi_;
	std::string sour_addr_;
	std::string broker_id_;
	std::string user_id_;
	std::string sh_stock_id_;
	std::string sz_stock_id_;
	int req_id_;
	std::string last_trade_pos_;
	int max_order_id_;
	std::map<int, OrderData> valid_orders_;
	std::map<int, OrderParamData> order_param_cache_;
	std::map<int, OrderData> calcel_order_param_cache_;
	std::list<TradeData> wait_trade_;
	bool is_order_init_;

	SpinLock valid_order_mutex_;
	SpinLock cache_mutex_;
	SpinLock trade_mutex_;
};

}

#endif	
