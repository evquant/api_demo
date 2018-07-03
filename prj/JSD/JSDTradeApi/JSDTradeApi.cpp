

#include "JSDTradeApi.h"

using namespace std;

#define MAX_LINE 10240

namespace kiiik {

static int split(const string& str, vector<string>& ret_, string sep = ",")
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

bool SpxSession::is_sock_env_init_ = false;

SpxSession::SpxSession(SpxSpi* spi, JSDTradeApi* api, int seq)
	: spi_(spi), api_(api), seq_(seq), socket_(INVALID_SOCKET), is_login_(false), last_func_id_(0)
{
	InitializeSocketEnvironment();
}

SpxSession::~SpxSession()
{
	FreeSocketEnvironment();
	SocketClose(socket_);
}

void SpxSession::GetAddressFrom(sockaddr_in *addr, const char *ip, int port)  
{  
    memset(addr, 0, sizeof(sockaddr_in));  
    addr->sin_family = AF_INET;            /*地址类型为AF_INET*/  
    if(ip)  
    {  
        addr->sin_addr.s_addr = inet_addr(ip);  
    }  
    else  
    {  
        /*网络地址为INADDR_ANY，这个宏表示本地的任意IP地址，因为服务器可能有多个网卡，每个网卡也可能绑定多个IP地址， 
        这样设置可以在所有的IP地址上监听，直到与某个客户端建立了连接时才确定下来到底用哪个IP地址*/  
        addr->sin_addr.s_addr = htonl(INADDR_ANY);  
    }  
    addr->sin_port = htons(port);   /*端口号*/  
}

int SpxSession::InitializeSocketEnvironment()  
{  
	if (is_sock_env_init_) { return 0; }
#if defined(WIN32)  
	WSADATA  Ws;  
	//Init Windows Socket  
	if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )  
	{  
		return -1;  
	}  
#endif 
	is_sock_env_init_ = true;
	return 0;  
}  

void SpxSession::FreeSocketEnvironment()  
{  
	if (!is_sock_env_init_) { return; }
#if defined(WIN32)  
	WSACleanup();  
#endif  
	is_sock_env_init_ = false;
}  

HSocket SpxSession::SocketOpen()  
{  
	int protocol = 0;  
	HSocket hs;  
#if defined(WIN32)  
	protocol=IPPROTO_TCP;  
#endif  
	hs = socket(AF_INET, SOCK_STREAM, protocol);  
	return hs;  
}  

void SpxSession::SocketClose(HSocket &handle)  
{  
#if defined(WIN32)  
		closesocket(handle);  
#else
		close(handle);  
#endif  
} 

bool SpxSession::SetBufSize(HSocket &handle, bool is_recv, int size, std::string& err)
{
	int opt_name = SO_RCVBUF;
	if (!is_recv) {
		opt_name = SO_SNDBUF;
	}

	int ret = 0;
#if defined(WIN32)  
	ret = setsockopt(handle, SOL_SOCKET, opt_name, (char*) &size, sizeof(size));
	if (ret == SOCKET_ERROR)
	{      
		int err_code = WSAGetLastError();
		std::stringstream ss;
		ss << "err code: " << err_code;
		err = ss.str();
		return false;
	}
#else
	ret = setsockopt(handle, SOL_SOCKET, opt_name, (const void*) &size, (socklen_t)sizeof(size));
	if (-1 == ret)
	{
		err = "error on setsockopt";
		return false;
	}
#endif

	int new_size = 0;
	int size_len = sizeof(new_size);
#if defined(WIN32)
	ret = getsockopt(handle, SOL_SOCKET, opt_name, (char*) &new_size, &size_len);
	if (ret == SOCKET_ERROR) { 
		err = "error on getsockopt";
		return false;
	}
#else
	ret = getsockopt(handle, SOL_SOCKET, opt_name, (const void*) &new_size, (socklen_t*)&size_len);
	if (ret == -1) { 
		err = "error on getsockopt";
		return false;
	}
#endif

	if (new_size != size)
	{
		err = "set error";
		return false;
	}

	return true;
}

bool SpxSession::Connect(const std::string& ip, int port)
{
	struct sockaddr_in server;
	GetAddressFrom(&server, ip.c_str(), port);
	socket_ = SocketOpen();
	if (connect(socket_,(struct sockaddr *)&server,sizeof(server)) != 0)
	{
		return false;
	}
	Start();
	return true;
}

bool SpxSession::Login(const std::string& login_cmd, std::string& err)
{
	last_func_id_ = 61;
	if (!Send(login_cmd))
	{
		err = "socket error";
		return false;
	}
	cond_.Signal();
	if (!timed_cond_.TimedWait(&timed_mutex_, 5))
	{
		err = "timeout for response";
		return false;
	}
	if (!is_login_)
	{
		err = login_err_;
		return false;
	}
	return true;
}

bool SpxSession::Send(const std::string& req)
{
	while(send(socket_, req.c_str(), req.size(), 0) <= 0)
	{
#if defined(WIN32) 
		int err_code = WSAGetLastError();
		printf("err code: %d\n", err_code);
		if (err_code == 10035)
		{
			Thread::Sleep(100);
		}
		else
		{
			return false;
		}
#else
		return false;
#endif
	}
	return true;
}

void SpxSession::WaitSingleRsp()
{
	char buf[MAX_LINE] = {0};

	int n;
	std::string rsp_str = "";
	//防止一次没收全,检验最后一个字符严格是 |
	do 
	{
		memset(buf, 0, MAX_LINE);
		n = recv(socket_, buf, MAX_LINE, 0);
		rsp_str += buf;
	} while (/*buf[n - 1] != '|'*/0);
	spi_->OnRspense(seq_, last_func_id_, rsp_str, true);
}

bool SpxSession::WaitLoginRsp()
{
	char buf[MAX_LINE] = {0};

	int n;
	string rsp_str = "";
	//防止一次没收全,检验最后一个字符严格是 |
	do 
	{
		memset(buf, 0, MAX_LINE);
		n = recv(socket_, buf, MAX_LINE, 0);
		rsp_str += buf;
	} while (/*buf[n-1] != '|'*/0);


	vector<string> fields;
	split(rsp_str, fields, "|");
	if (fields.size() < 5)
	{
		login_err_ = "invalid return";
		timed_cond_.Signal();
		return false;
	}
	if (fields[3] == "N")
	{
		login_err_ = fields[4];
		timed_cond_.Signal();
		return false;
	}

	spi_->OnRspense(seq_, last_func_id_, rsp_str, true);

	is_login_ = true;
	timed_cond_.Signal();
	return true;
}

void SpxSession::WaitMultiRsp()
{
	char buf[MAX_LINE] = {0};
	int n = recv(socket_, buf, MAX_LINE, 0);
	string rsp_str = buf;
	vector<string> fields;
	split(rsp_str, fields, "|");
	if (fields.size() < 5) { return; }
	if (fields[3] == "N")
	{
		spi_->OnRspense(seq_, last_func_id_, rsp_str, true);
	} 
	else
	{
		int num = atoi(fields[4].c_str());
		//std::string req_str = api_->ReqHead2(num) + "RJWT|";
		//n = recv(socket_, buf, MAX_LINE, 0);

		for (int i=0; i < num; ++i)
		{
			std::string req_str = api_->ReqHead(0) + "RJWT|";
			if (!Send(req_str)) { 
				DateTime now(NULL);
				cout << now.Str() << " : sock write error while send spx request" << endl;
				return;
			}
			memset(buf, 0, MAX_LINE);
			n = recv(socket_, buf, MAX_LINE, 0);
			spi_->OnRspense(seq_, last_func_id_, buf, i == num - 1);
		}
	}
}

void SpxSession::AppendReq(const SpxReqData& req)
{
	Locker locker(&requsets_lock_);
	if (req.func_id == 22 || seq_ == 0 || seq_ == 1)
	{
		if (requsets_.size() > 0) { return; }
		//TODO:加入时间判断
	}

	requsets_.push(req);
	cond_.Signal();
}

int SpxSession::QueueSize()
{
	Locker locker(&requsets_lock_);
	return requsets_.size();
}

bool SpxSession::GetReq(SpxReqData& req)
{
	Locker locker(&requsets_lock_);
	if (0 == requsets_.size()) { return false; }
	req = requsets_.front();
	return true;
}

void SpxSession::PopReq()
{
	Locker locker(&requsets_lock_);
	requsets_.pop();
}


void SpxSession::DealReq(SpxReqData& req)
{
	last_func_id_ = req.func_id;

	if (!Send(req.req_str)) { 
		DateTime now(NULL);
		cout << now.Str() << " : sock write error while send spx request" << endl;
		return;
	}

	if (req.multi_rsp)
	{
		WaitMultiRsp();
	}
	else
	{
		WaitSingleRsp();
	}
}

void SpxSession::OnRun()
{
	mutex_.Lock();
	cond_.Wait(&mutex_);
	mutex_.Unlock();
	WaitLoginRsp();

	while (is_login_ && IsRuning())
	{
		while (QueueSize() == 0)
		{
			mutex_.Lock();
			cond_.Wait(&mutex_);
			mutex_.Unlock();
		}

		SpxReqData req;
		while (GetReq(req))
		{
			DealReq(req);
			PopReq();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

JSDTradeApi::JSDTradeApi() : req_id_(1), sh_stock_id_(""), sz_stock_id_(""), last_trade_pos_(""), is_order_init_(false)
{
	for (int i=0; i < SESS_NUM; ++i)
	{
		spx_sessions_[i] = new SpxSession(this, this, i);
	}

	DateTime date_time(NULL);
	char dest[20];
	sprintf(dest, "%4d-%2d-%2d", date_time.date.year, date_time.date.month, date_time.date.day);
	last_trade_pos_ = dest;
}

JSDTradeApi::~JSDTradeApi()
{
	for (int i=0; i < SESS_NUM; ++i)
	{
		delete spx_sessions_[i];
	}
}

bool JSDTradeApi::Init(const std::string& spx_ip, int spx_port, const std::string& sour_mac, const std::string& sour_ip, TradeSpi* spi, std::string& err)
{
	spi_ = spi;
	
	if ("" == spx_ip || 0 >= spx_port || "" == sour_mac || "" == sour_ip)
	{
		err = "invlid config";
		return false;
	}

	sour_addr_ = sour_mac + "," + sour_ip;
	for (int i=0; i < SESS_NUM; ++i)
	{
		if (!spx_sessions_[i]->Connect(spx_ip, spx_port))
		{
			err = "spx session connect error";
			return false;
		}
	}

	return true;
}

bool JSDTradeApi::Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err)
{
	int fund_id = 61;
	broker_id_ = broker_id;
	user_id_ = user_id;
	stringstream ss;
	ss << ReqHead(fund_id) << "0||" << user_id << "|" << password << "|RJWT||1|";
	for (int i=0; i < SESS_NUM; ++i)
	{
		if (!spx_sessions_[i]->Login(ss.str(), err))
		{
			return false;
		}
	}

	Start();
	return true;
}

void JSDTradeApi::OnRun()
{
	std::string err;

	QueryOrder(err);//TODO:为了准确获取最大的order_id,需要改为查询所有委托
	//test
	//is_order_init_ = true;

	while (!is_order_init_)
	{
		Thread::Sleep(2000); 
	}

	cout << "自动线程自动更新成交和委托信息" << endl;
	int num = 0;
	int heart_num = 0;
	while (IsRuning())
	{
		TimeQueryTrade();

		if (num == QRY_ORDER_CYCLE)
		{
			num = 0;
			TimeQueryOrder();	//test
		}

		if (heart_num == HEART_BEAT_CYCLE)
		{
			heart_num = 0;
			HeartBeat();
		}

		Thread::Sleep(QRY_TRADE_TIME);
		num++;
		heart_num++;
	}
}

void JSDTradeApi::HeartBeat()
{
	int fund_id = 22;
	stringstream ss;
	ss << ReqHead(fund_id) << "RJWT|";
	for (int i=0; i < SESS_NUM; ++i)
	{
		spx_sessions_[i]->AppendReq(SpxReqData(fund_id, false, ss.str()));
	}
}

void JSDTradeApi::TimeQueryTrade()
{
	std::string err;
	QueryPlusTrade(last_trade_pos_, err);
}

void JSDTradeApi::TimeQueryOrder()
{
	DateTime date_time(NULL);
	char date_str[20];
	sprintf(date_str, "%4d%2d%2d", date_time.date.year, date_time.date.month, date_time.date.day);

	int fund_id = 14;
	stringstream ss;
	//ss << ReqHead(fund_id)  << "||" << user_id_ << SepStr(8)  << "RJWT||2"<< SepStr(5) << "0||";
	ss << ReqHead(fund_id)  << "||" << user_id_ << "|" << date_str << "|" << date_str<< SepStr(6)  << "RJWT||1"<< SepStr(5) << "0||";		
	spx_sessions_[SESS_RSP_ORDER]->AppendReq(SpxReqData(fund_id, true, ss.str()));
}

//void JSDTradeApi::TimeQueryWaitedOrder()
//{
//	
//	std::string err;
//	Locker locker(&valid_order_mutex_);
//	for (std::map<int, OrderData>::iterator iter = valid_orders_.begin(); iter != valid_orders_.end(); ++iter)
//	{
//		if (iter->second.status == ORDER_STATUS_WAIT_SUBMIT || iter->second.status == ORDER_STATUS_WAIT_CANCEL)
//		{
//			cout << "查询待报委托: " << iter->first << endl;
//			QuerySpecificOrder(iter->first, err);
//		}
//	}
//}

void JSDTradeApi::OnRspense(int seq, int func_id, const std::string& rsp, bool is_last)
{
	vector<string> fields;
	split(rsp, fields, "|");
	if (fields.size() < 5)  { return; }
	if (fields[3] == "N") // 出错了
	{
		int order_no = atoi(fields[2].c_str());
		switch (func_id)
		{
		case 3: // 委托
			{
				Locker locker(&cache_mutex_);				
				std::map<int, OrderParamData>::iterator iter = order_param_cache_.find(order_no);
				if (iter != order_param_cache_.end())
				{
					OrderData order_data;
					order_data.symbol = iter->second.symbol;
					order_data.order_id = iter->second.entrust_id;
					order_data.local_order_id = iter->first;
					strcpy(order_data.user_tag, iter->second.user_tag);
					order_data.status = ORDER_STATUS_ERROR;					
					memcpy(order_data.status_msg, fields[4].c_str(), ERROR_MSG_LEN);
					spi_->OnOrderError(&order_data);
					order_param_cache_.erase(iter);
				}
				else
				{
					cout << "委托失败，request id: "  << order_no << endl;
				}
			}
			break;
		case 122:	//没有增加的成交记录也会返回错误信息，过滤
			break;
		case 4: // 撤单
			{
				Locker locker(&cache_mutex_);
				std::string err_msg = std::string("撤单失败:") + fields[4];
				spi_->OnError(order_no, err_msg);
				Locker locker2(&valid_order_mutex_);
				std::map<int, OrderData>::iterator iter = calcel_order_param_cache_.find(order_no);
				if (iter != calcel_order_param_cache_.end())
				{
					std::map<int, OrderData>::iterator order_iter  = valid_orders_.find(iter->second.order_id);
					if (order_iter != valid_orders_.end())
					{
						strncpy(order_iter->second.status_msg, err_msg.c_str(), sizeof(StatusMsgType));
						spi_->OnOrder(&order_iter->second);
						strcpy(order_iter->second.status_msg, "");
					}
				}
				calcel_order_param_cache_.erase(order_no);
				//APP_LOG(LOG_LEVEL_ERROR) << err_msg;
			}
			break;
		default:
			cout << "func_id=" << func_id << " request error: " << fields[4] << endl;
		}

		return;
	}

	switch (func_id)
	{
	case 61:
		DealLoginRsp(fields);
		break;
	case 3:
		DealSubmitRsp(fields);
		break;
	case 62:
		DealQueryHisTradeRsp(fields, is_last);
		break;
	case 122:
		if (seq == SESS_USER_QRY) {
			DealQueryTradeRsp(fields, is_last);
		} 
		else {
			DealPlusTradeRsp(fields, is_last);
		}
		break;
	case 14:
		if (seq == SESS_USER_QRY) {
			DealQueryOrderRsp(fields, is_last);
		} 
		else {
			DealTimedQueryOrderRsp(fields);
		}
		break;
	case 4:
		DealCancelOrderRsp(fields);
		break;
	case 10:
		DealQueryPositionRsp(fields, is_last);
		break;
		//case 13:
		//	DealQueryTradeRsp(fields, is_last);
		//	break;
	case 36:
		DealQueryAccountRsp(fields, is_last);
		break;
	case 11:
		DealQueryMoneyFlowRsp(fields, is_last);
		break;
	case 22:
		//cout << "心跳回报" << endl;
		break;
	default:
		cout << func_id << " : " << rsp << endl;
	}
}

void JSDTradeApi::DealLoginRsp(std::vector<std::string>& fields)
{
	if (fields.size() > 16)
	{
		vector<string> account_records;
		split(fields[16], account_records, ";");
		for (int i=0; i < account_records.size(); ++i)
		{
			vector<string> account_fields;
			split(account_records[i], account_fields, ",");
			if (account_fields.size() > 2)
			{
				if (account_fields[1] == "沪A")
				{
					sh_stock_id_ = account_fields[2];
					cout << "沪A股东账号: " << sh_stock_id_ << endl;
				} 
				else if (account_fields[1] == "深A")
				{
					sz_stock_id_ = account_fields[2];
					cout << "深A股东账号: " << sz_stock_id_ << endl;
				}
			}
		}
	}
}

void JSDTradeApi::DealQueryAccountRsp(std::vector<std::string>& fields, bool is_last)
{
	AccountData account;
	strcpy(account.account_id, user_id_.c_str());
	strcpy(account.broker_id, broker_id_.c_str());
	if (fields[14] == "人民币")
	{
		account.money_type = MONEY_RMB;
	} 
	else if (fields[14] == "美元")
	{
		account.money_type = MONEY_DOLLAR;
	} 
	else
	{
		account.money_type = MONEY_HK;
	}
	account.asset_balance = atof(fields[7].c_str());
	account.enable_balance = atof(fields[5].c_str());
	account.withdraw_balance = atof(fields[29].c_str());
	account.market_value = atof(fields[6].c_str());
	account.fund_balance = atof(fields[4].c_str());

	spi_->OnQryAccount(atoi(fields[2].c_str()), &account, "", is_last);
}

void JSDTradeApi::DealQueryMoneyFlowRsp(std::vector<std::string>& fields, bool is_last)
{
	MoneyFlowData account;
	strcpy(account.account_id, user_id_.c_str());
	strcpy(account.broker_id, broker_id_.c_str());
	strcpy(account.flow_id, fields[12].c_str());
	std::string aaa = fields[15];
	account.money_type = '0' + atoi(fields[15].c_str());
	strcpy(account.business_abstract, fields[3].c_str());
	strcpy(account.abstract_type, fields[13].c_str());
	account.change_value = atof(fields[4].c_str());
	account.reserve_value = atof(fields[5].c_str());
	if (fields[7] == "")
	{
		account.symbol = NAN_SYMBOL;
	}
	else
	{
		strcpy(account.symbol.instrument, fields[7].c_str());
		if (atoi(fields[18].c_str()) == 1) { account.symbol.exchange = EXCHANGE_SSE; }
		else { account.symbol.exchange = EXCHANGE_SZE; }
		if (fields[8] == "买") { account.direction = LONG_DIRECTION; }
		else { account.direction = SHORT_DIRECTION; }
	}
	account.trade_price = atof(fields[10].c_str());
	account.trade_volume = atoi(fields[9].c_str());
	account.trade_value = atof(fields[20].c_str());
	account.commission = atof(fields[22].c_str());
	account.transfer_fee = atof(fields[23].c_str());
	account.stamp_tax = atof(fields[24].c_str());
	account.other_fee = atof(fields[25].c_str());
	account.trade_time.date = atoi(fields[16].c_str());
	account.trade_time.time = atoi(fields[19].c_str()) % 1000000;

	//spi_->OnQryMoneyFlow(fields[2].toInt(), &account, is_last);
}

void JSDTradeApi::DealSubmitRsp(std::vector<std::string>& fields)
{
	int order_id = atoi(fields[4].c_str());
	Locker locker1(&valid_order_mutex_);
	Locker locker2(&cache_mutex_);
	std::map<int, OrderData>::iterator order_iter = valid_orders_.find(order_id);
	std::map<int, OrderParamData>::iterator iter = order_param_cache_.find(atoi(fields[2].c_str()));

	if (order_iter == valid_orders_.end() && iter != order_param_cache_.end())
	{
		OrderData order_data;
		order_data.symbol = iter->second.symbol;
		order_data.order_id = order_id;
		order_data.status = ORDER_STATUS_BEEN_SUBMIT;
		DateTime now(NULL);
		order_data.submit_time.date = now.date.year * 10000 + now.date.month * 100 + now.date.day;
		order_data.submit_time.time = now.time.hour * 10000 + now.time.minute * 100 + now.time.sec;
		order_data.update_time = order_data.submit_time;
		order_data.direction = iter->second.direction;
		if (order_data.direction == SHORT_DIRECTION) { order_data.open_close_flag = CLOSE_ORDER; }
		order_data.open_close_flag = iter->second.open_close_flag;
		order_data.order_price_flag = iter->second.order_price_flag;
		order_data.limit_price = iter->second.limit_price;
		order_data.total_volume = iter->second.volume;
		strcpy(order_data.user_tag, iter->second.user_tag);

		bool is_exist = false;
		Locker locker3(&trade_mutex_);
		for (std::list<TradeData>::iterator trade_iter = wait_trade_.begin(); trade_iter != wait_trade_.end(); )
		{
			if (trade_iter->order_id == order_id)
			{
				std::list<TradeData>::iterator cur_iter = trade_iter;
				trade_iter++;
				is_exist = true;
				order_data.trade_volume += cur_iter->trade_volume;
				if (order_data.trade_volume > order_data.total_volume) { order_data.trade_volume = order_data.total_volume; }
				order_data.status = ORDER_STATUS_PART_TRADE;
				if (order_data.trade_volume == order_data.total_volume)
				{
					order_data.status = ORDER_STATUS_ALL_TRADE;
				}
				spi_->OnOrder(&order_data);
				spi_->OnTrade(&(*cur_iter));
				wait_trade_.erase(cur_iter);
			}
			else
			{
				++trade_iter;
			}
		}

		if (order_data.status != ORDER_STATUS_INVALID && order_data.status != ORDER_STATUS_ALL_TRADE
			&& order_data.status != ORDER_STATUS_BEEN_CANCEL) 
		{
			valid_orders_[order_data.order_id] = order_data;
		}

		if (!is_exist) { spi_->OnOrder(&order_data); }
	}

	if (iter != order_param_cache_.end())
	{
		order_param_cache_.erase(iter);
	}
}

void JSDTradeApi::DealCancelOrderRsp(std::vector<std::string>& fields)
{
	Locker locker1(&valid_order_mutex_);
	Locker locker2(&cache_mutex_);
	std::map<int, OrderData>::iterator param_iter = calcel_order_param_cache_.find(atoi(fields[2].c_str()));

	if (param_iter != calcel_order_param_cache_.end())
	{
		int order_id = param_iter->second.order_id;

		std::map<int, OrderData>::iterator iter2 = valid_orders_.find(order_id);
		if (iter2 != valid_orders_.end())
		{
			iter2->second.status = ORDER_STATUS_BEEN_CANCEL;
			spi_->OnOrder(&iter2->second);
			valid_orders_.erase(iter2);
		}

		calcel_order_param_cache_.erase(param_iter);
	}
}

void JSDTradeApi::DealPlusTradeRsp(std::vector<std::string>& fields, bool is_last)
{
	if (fields[11] != "0") { return; }
	if (fields[9] == "2")	//撤单
	{
		//撤单逻辑
		cout << "成交回报：撤单" << endl;
		return;
	}

	TradeData trade_data;
	strcpy(trade_data.symbol.instrument, fields[13].c_str());
	if (atoi(fields[5].c_str()) == 1) {
		trade_data.symbol.exchange = EXCHANGE_SSE;
	}
	else {
		trade_data.symbol.exchange = EXCHANGE_SZE;
	}
	trade_data.order_id = atoi(fields[18].c_str());
	trade_data.trade_id = atoi(fields[23].c_str());
	trade_data.trade_time.date = atoi(fields[3].c_str());
	string time_str = fields[20].substr(0,6);
	trade_data.trade_time.time = atoi(time_str.c_str());
	if (fields[7] == "1") {
		trade_data.direction = LONG_DIRECTION;
		trade_data.open_close_flag = OPEN_ORDER;
	} 
	else {
		trade_data.direction = SHORT_DIRECTION;
		trade_data.open_close_flag = CLOSE_ORDER;
	}
	trade_data.trade_price = atof(fields[16].c_str());
	trade_data.trade_volume = atoi(fields[15].c_str());
	//trade_data.trade_value = fields[17].c_str());
	if (UpdateOrderWithTrade(trade_data))
	{
		spi_->OnTrade(&trade_data);
	}

	last_trade_pos_ = fields[21];
}

void JSDTradeApi::DealQueryTradeRsp(std::vector<std::string>& fields, bool is_last)
{
	if (fields[11] != "0") { return; }
	if (fields[9] == "2")	//撤单
	{
		//撤单逻辑
		cout << "成交回报：撤单" << endl;
		return;
	}

	TradeData trade_data;
	strcpy(trade_data.symbol.instrument, fields[13].c_str());
	if (atoi(fields[5].c_str()) == 1) {
		trade_data.symbol.exchange = EXCHANGE_SSE;
	}
	else {
		trade_data.symbol.exchange = EXCHANGE_SZE;
	}
	trade_data.order_id = atoi(fields[18].c_str());
	trade_data.trade_id = atoi(fields[23].c_str());
	trade_data.trade_time.date = atoi(fields[3].c_str());
	string time_str = fields[20].substr(0,6);
	trade_data.trade_time.time = atoi(time_str.c_str());
	if (fields[7] == "1") {
		trade_data.direction = LONG_DIRECTION;
		trade_data.open_close_flag = OPEN_ORDER;
	} 
	else {
		trade_data.direction = SHORT_DIRECTION;
		trade_data.open_close_flag = CLOSE_ORDER;
	}
	trade_data.trade_price = atof(fields[16].c_str());
	trade_data.trade_volume = atoi(fields[15].c_str());
	spi_->OnQryTrade(atoi(fields[2].c_str()), &trade_data, "", is_last);
}

void JSDTradeApi::DealQueryHisTradeRsp(std::vector<std::string>& fields, bool is_last)
{
	TradeData trade_data;
	strcpy(trade_data.symbol.instrument, fields[6].c_str());
	if (fields[6].substr(0, 2) == "60") {
		trade_data.symbol.exchange = EXCHANGE_SSE;
	}
	else {
		trade_data.symbol.exchange = EXCHANGE_SZE;
	}
	trade_data.order_id = atoi(fields[19].c_str());
	trade_data.trade_id = atoi(fields[20].c_str());
	trade_data.trade_time.date = atoi(fields[3].c_str());
	string time_str = fields[18].substr(0,6);
	trade_data.trade_time.time = atoi(time_str.c_str());
	if (fields[21] == "1") {
		trade_data.direction = LONG_DIRECTION;
		trade_data.open_close_flag = OPEN_ORDER;
	} 
	else {
		trade_data.direction = SHORT_DIRECTION;
		trade_data.open_close_flag = CLOSE_ORDER;
	}
	trade_data.trade_price = atof(fields[9].c_str());
	trade_data.trade_volume = atoi(fields[8].c_str());	
	spi_->OnQryTrade(atoi(fields[2].c_str()), &trade_data, "", is_last);
}

bool JSDTradeApi::UpdateOrderWithTrade(const TradeData& trade_data)
{
	Locker locker(&valid_order_mutex_);
	std::map<int, OrderData>::iterator iter = valid_orders_.find(trade_data.order_id);
	if (iter == valid_orders_.end()) //成交回报先于委托回报返回,无法知道委托量，继续查询
	{ 
		Locker locker2(&trade_mutex_);
		wait_trade_.push_back(trade_data);
		cout << "rsp trade not in order list: " << trade_data.trade_id << endl;
		return false;
	}

	iter->second.trade_volume += trade_data.trade_volume;
	if (iter->second.trade_volume > iter->second.total_volume) { iter->second.trade_volume = iter->second.total_volume; }
	if (iter->second.trade_volume == iter->second.total_volume) {
		iter->second.status = ORDER_STATUS_ALL_TRADE;
	}
	else {
		iter->second.status = ORDER_STATUS_PART_TRADE;
	}
	spi_->OnOrder(&iter->second);
	if (iter->second.status == ORDER_STATUS_ALL_TRADE) { 
		valid_orders_.erase(iter); 
	}

	return true;
}


void JSDTradeApi::DealQueryOrderRsp(std::vector<std::string>& fields, bool is_last)
{
	OrderData order_data;
	StrToOrderData(order_data, fields);
	if (is_order_init_)
	{
		spi_->OnQryOrder(atoi(fields[2].c_str()), &order_data, "", is_last);
	}
	else //启动时初始化委托数据
	{
		static int num = 0;
		num++;
		

		if (order_data.order_id > max_order_id_) { max_order_id_ = order_data.order_id; }
		if (order_data.status != ORDER_STATUS_INVALID && order_data.status != ORDER_STATUS_ALL_TRADE
			&& order_data.status != ORDER_STATUS_BEEN_CANCEL) 
		{
			valid_orders_[order_data.order_id] = order_data;
		}

		if (is_last) { 
			is_order_init_ = true; 
		}
	}
}

void JSDTradeApi::DealTimedQueryOrderRsp(std::vector<std::string>& fields)
{
	int order_id = atoi(fields[9].c_str());
	OrderStatus status = ToOrderStatus(fields[20]);
	Locker locker(&valid_order_mutex_);
	std::map<int, OrderData>::iterator iter = valid_orders_.find(order_id);
	if (iter != valid_orders_.end() && iter->second.status != status)
	{
		OrderData order_data;
		StrToOrderData(order_data, fields);
		spi_->OnOrder(&order_data);
	}

	if (status != ORDER_STATUS_INVALID && status != ORDER_STATUS_ALL_TRADE && status != ORDER_STATUS_BEEN_CANCEL)
	{
		//OrderData order_data;
		//StrToOrderData(order_data, fields);
		//valid_orders_[order_data.order_id] = order_data;
	}
	else if (iter != valid_orders_.end())
	{
		valid_orders_.erase(iter);
	}
}

void JSDTradeApi::DealQueryPositionRsp(std::vector<std::string>& fields, bool is_last)
{
	PositionData pos_data;
	strcpy(pos_data.symbol.instrument, fields[5].c_str());
	if (atoi(fields[3].c_str()) == 1) {
		pos_data.symbol.exchange = EXCHANGE_SSE;
	}
	else {
		pos_data.symbol.exchange = EXCHANGE_SZE;
	}
	pos_data.direction = LONG_DIRECTION;
	pos_data.open_price = atof(fields[20].c_str());
	pos_data.open_volume = atoi(fields[7].c_str());
	pos_data.enable_volume = atoi(fields[8].c_str());
	pos_data.yestd_volume = atoi(fields[13].c_str());
	pos_data.last_price = atof(fields[12].c_str());
	pos_data.position_cost = atof(fields[14].c_str());
	pos_data.position_profit = atof(fields[15].c_str());
	spi_->OnQryPosition(atoi(fields[2].c_str()), &pos_data, "", is_last);
}

std::string JSDTradeApi::ReqHead(int func_id)
{
	stringstream ss;
	ss << "R|" << sour_addr_ << "|" << ++req_id_ << "|" << func_id << "|" << broker_id_ << "|";
	return ss.str();
}

std::string JSDTradeApi::SepStr(int num)
{
	stringstream ss;
	for (int i=0; i < num; ++i)
	{
		ss << "|";
	}
	return ss.str();
}

void JSDTradeApi::StrToOrderData(OrderData& order_data, std::vector<std::string>& fields)
{
	strcpy(order_data.symbol.instrument, fields[5].c_str());
	if (atoi(fields[18].c_str()) == 1) {
		order_data.symbol.exchange = EXCHANGE_SSE;
	}
	else {
		order_data.symbol.exchange = EXCHANGE_SZE;
	}
	order_data.order_id = atoi(fields[9].c_str());
	order_data.status = ToOrderStatus(fields[20]);
	order_data.submit_time.date = atoi(fields[15].c_str());
	vector<std::string> time_list;
	split(fields[14], time_list, ":");
	if (time_list.size() == 3)
	{
		order_data.submit_time.time = atoi(time_list[0].c_str()) * 10000 + atoi(time_list[1].c_str()) * 100 + atoi(time_list[2].c_str());
	}
	DateTime now(NULL);
	order_data.update_time.date = now.date;
	order_data.update_time.time = now.time;
	if (fields[19] == "1") {
		order_data.direction = LONG_DIRECTION;
		order_data.open_close_flag = OPEN_ORDER;
	} 
	else {
		order_data.direction = SHORT_DIRECTION;
		order_data.open_close_flag = CLOSE_ORDER;
	}
	order_data.limit_price = atof(fields[8].c_str());
	order_data.total_volume = atoi(fields[7].c_str());
	order_data.trade_volume = atoi(fields[10].c_str());
	strcpy(order_data.status_msg, fields[16].c_str());
	//strcpy(order_data.entrust_way, fields[32].c_str());
}

OrderStatus JSDTradeApi::ToOrderStatus(std::string& str)
{
	char c = str[0];
	switch (c)
	{
	case '1':
		return ORDER_STATUS_WAIT_SUBMIT; 
	case '2':
		return ORDER_STATUS_BEEN_SUBMIT; 
	case '3':
		return ORDER_STATUS_BEEN_SUBMIT_WAIT_CANCEL;
	case '4':
		return ORDER_STATUS_PART_TRADE_WAIT_CANCEL; 
	case '5':
	case '6':
	case 'A':
		return ORDER_STATUS_BEEN_CANCEL; 
	case '7':
		return ORDER_STATUS_PART_TRADE; 
	case '8':
		return ORDER_STATUS_ALL_TRADE;
	default:
		return  ORDER_STATUS_INVALID;
	}
}

bool JSDTradeApi::Logout(std::string& err)
{
	return true;
}

int JSDTradeApi::QueryAccount(std::string& err)
{
	int fund_id = 36;
	stringstream ss;
	ss << ReqHead(fund_id) << "" << "||" << user_id_ << SepStr(3);	
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));
	return req_id_;
}

int JSDTradeApi::QueryMoneyFlow(std::string& err)
{
	int fund_id = 11;
	stringstream ss;
	ss << ReqHead(fund_id) << "||" << user_id_ << "|1" << SepStr(5) << "RJWT|";
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));
	return req_id_;
}

int JSDTradeApi::QueryHisAccountFlow(const std::string& begin_date, const std::string& end_date, std::string& err)
{
	int fund_id = 11;
	stringstream ss;
	ss << ReqHead(fund_id) << "||" << user_id_ << "|1|" << begin_date << "|" << end_date << SepStr(3) << "RJWT|";
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));
	return req_id_;
}

int JSDTradeApi::QueryAccountBalance(std::string& err)
{
	int fund_id = 9;
	stringstream ss;
	ss << ReqHead(fund_id) << "1||" << user_id_ << "|" << "RJWT|1|";
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, false, ss.str()));
	return req_id_;
}

int JSDTradeApi::QueryPosition(std::string& err)
{
	int fund_id = 10;
	stringstream ss;
	int max_num = 1000;
	//int market_type = 1;
	//int max_num = 5;
	//std::string imstrument = ""; //600004
	//std::string stock_code = ""; //A332475242
	//ss << ReqHead(fund_id) << "||" << user_id_  << "|" << market_type << "|" << imstrument << "|"
	//	 << stock_code << SepStr(4) << max_num << "|" << "1|RJWT" << SepStr(4);
	ss << ReqHead(fund_id) << "||" << user_id_  << SepStr(7) << max_num << "|1|RJWT" << SepStr(4);
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));
	return req_id_;
}

int JSDTradeApi::QueryOrder(std::string& err)
{
	DateTime date_time(NULL);
	char dest[20];
	sprintf(dest, "%4d%2d%2d", date_time.date.year, date_time.date.month, date_time.date.day);
	std::string date_str = dest;
	int fund_id = 14;
	stringstream ss;
	//ss << ReqHead(fund_id)  << "||" << user_id_ << SepStr(8)  << "RJWT||1"<< SepStr(5) << "0||";
	ss << ReqHead(fund_id)  << "||" << user_id_ << "|" << date_str << "|" << date_str<< SepStr(6)  << "RJWT||1"<< SepStr(5) << "0||";	
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	return req_id_;
}

int JSDTradeApi::QueryValidOrder(std::string& err)
{

	DateTime date_time(NULL);
	char dest[20];
	sprintf(dest, "%4d%2d%2d", date_time.date.year, date_time.date.month, date_time.date.day);
	std::string date_str = dest;
	int fund_id = 14;
	stringstream ss;
	//ss << ReqHead(fund_id)  << "||" << user_id_ << SepStr(8)  << "RJWT||1"<< SepStr(5) << "1||";
	ss << ReqHead(fund_id)  << "||" << user_id_ << "|" << date_str << "|" << date_str<< SepStr(6)  << "RJWT||1"<< SepStr(5) << "1||";		
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	return req_id_;
}

int JSDTradeApi::QuerySpecificOrder(int order_id, std::string& err)
{
	int fund_id = 14;
	stringstream ss;

	ss << ReqHead(fund_id)  << "||" << user_id_ << SepStr(3) << order_id << SepStr(5) << "RJWT||1"<< SepStr(5) << "0||";
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	return req_id_;
}

//13无委托编号，改成122
int JSDTradeApi::QueryTrade(std::string& err)
{
	//int fund_id = 13;
	//int max_num = 10000;
	//stringstream ss;
	//ss << ReqHead(fund_id) << "||" << user_id_ << "|" << max_num << SepStr(4) << "RJWT" << SepStr(7);
	//spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	int fund_id = 122;
	int max_num = 10000;
	stringstream ss;
	ss << ReqHead(fund_id) << "||" << user_id_ << "|||1|" << max_num << "|0|" << SepStr(5);
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	return req_id_;
}

int JSDTradeApi::QueryPlusTrade(const std::string& pos, std::string& err)
{
	int fund_id = 122;
	int max_num = 1000;
	stringstream ss;
	ss << ReqHead(fund_id) << "||" << user_id_ << "|||1|" << max_num << "|0|" << pos << SepStr(5);
	spx_sessions_[SESS_RSP_TRADE]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	return req_id_;
}

int JSDTradeApi::QueryHisTrade(const std::string& begin_date, const std::string& end_date, std::string& err)
{
	int fund_id = 62;
	int max_num = 1000;
	stringstream ss;
	ss << ReqHead(fund_id) << "1||" << user_id_ << "|" << begin_date << "|" 
		<< end_date << "||" << max_num << "|RJWT" << SepStr(6);
	std::string str = ss.str();
	spx_sessions_[SESS_USER_QRY]->AppendReq(SpxReqData(fund_id, true, ss.str()));

	return req_id_;
}

int JSDTradeApi::SubmitOrder(const OrderParamData& param, std::string& err)
{
	if (!is_order_init_) { err = "委托数据未初始化";  return -1; }
	/*if (param.user_tag <= max_order_id_)
	{
		stringstream ss;
		ss << "EntrustID must be grater than " << max_order_id_ << " while receive " << param.user_tag;
		err = ss.str();
		return -1;
	}
	max_order_id_ = param.user_tag;*/

	int fund_id = 3;
	int market_id = 1;
	std::string stock_id = sh_stock_id_;
	if (param.symbol.exchange == EXCHANGE_SZE)
	{
		market_id = 2;
		stock_id = sz_stock_id_;
	}

	stringstream ss;
	ss << ReqHead(fund_id) << market_id << "||" << stock_id << "|" << param.symbol.instrument
		<< "|" << param.direction << param.order_price_flag << "|" << param.volume << "|" << param.limit_price << "|RJWT"
		<< SepStr(12) << "1|";
		//<< SepStr(9) << param.user_tag << SepStr(3) << "1|";
	spx_sessions_[SESS_SUBMIT_ORDER]->AppendReq(SpxReqData(fund_id, false, ss.str()));
	Locker lockdr(&cache_mutex_);
	order_param_cache_[req_id_] = param;
	return req_id_;
}

int JSDTradeApi::CancelOrder(const OrderData& param, std::string& err)
{
	if (!is_order_init_) { err = "委托数据未初始化";  return -1; }

	Locker locker1(&valid_order_mutex_);
	std::map<int, OrderData>::iterator iter = valid_orders_.find(param.order_id);
	if (iter == valid_orders_.end())
	{
		err = "订单不存在或不可撤";  
		return -1;
	}

	int fund_id = 4;
	int market_id = 1;
	std::string stock_id = sh_stock_id_;
	if (param.symbol.exchange == EXCHANGE_SZE)
	{
		market_id = 2;
		stock_id = sz_stock_id_;
	}

	stringstream ss;
	//TODO:委托方式现在定为限价，以后检验是否有必要，如有必要从内存中恢复(好像从登陆就要指定)
	ss << ReqHead(fund_id) << market_id << "||" << stock_id << "|" << param.order_id << "|RJWT|";
	cout  << ss.str() << endl;
	spx_sessions_[SESS_SUBMIT_ORDER]->AppendReq(SpxReqData(fund_id, false, ss.str()));
	//spx_sessions_[SESS_RSP_TRADE]->AppendReq(SpxReqData(fund_id, false, ss.str()));
	Locker lockdr(&cache_mutex_);
	calcel_order_param_cache_[req_id_] = param;
	return req_id_;
}

}
