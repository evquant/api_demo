#ifndef __DEMO_H__
#define __DEMO_H__
#include "include/t2sdk_interface.h"
//////////////////////////////////////////////////////////////
//宏
#define SYNCSEND		0	///同步发送
#define ASYNCSEND		1	///异步发送
#define USERTOKENLEN	256		///令牌长度
#define USERSECTION		"user"
#define T2SDK           "t2sdk"
#define ISSUE_TYPE_MARKETINFO_FUTU  33100       ///期货单腿变动行情
#define ISSUE_TYPE_ARGMARKETINFO_FUTU 33102     ///期货组合变动行情
#define ISSUE_TYPE_REALTIME_FUTU      33101     ///期货委托成交反馈信息
#define ISSUE_TYPE_EXCHSTATUS_FUTU  33105     	///期货交易所状态信息

//////////////////////////////////////////////////////////////
// 全局变量
extern CConnectionInterface *g_pConnection;

extern int g_iSystemNo;
extern bool g_bLogin;
extern char g_cUserToken[USERTOKENLEN];

void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker);
void ShowFutuPack(int issue_type, IF2UnPacker *pUnPacker);

///////////////////////////////////////////////////////////////
//CBusiness
class CBusiness
{
public:
	CBusiness();
	virtual ~CBusiness();

	void SetConfig(CConfigInterface *pConfig)
	{
		m_pConfig = pConfig;
	}

	int DealReceive();
	int Subscribe(int issue_type); ///发布类型
	int CancelSub(int issueType);

	/** 
	* 登录，调用331100功能登录
	* @return >=0 返回系统结点号，其他失败
	*/
	int Login();

	/** 
	* 期货委托
	* 先获取pack打包器进行打包，再发送委托，得到应答以后再解包。
	* @param iSystemNo 系统节点号，
	*				对于UF2.0交易系统，需要根据登录功能331100返回的字段sysnode_id来设置iSystemNo。
	* @return 0: 成功, 其他失败!
	*/
	int FutuEntrust(int iSystemNo = 2); 

	/** 
	* 期货委托查询
	* 先获取pack打包器进行打包，再发送委托，得到应答以后再解包。
	* @param iSystemNo 系统节点号，
	*				对于UF2.0交易系统，需要根据登录功能331100返回的字段sysnode_id来设置iSystemNo。
	* @return 0: 成功, 其他失败!
	*/
	int SearchFutuEntrust(int iSystemNo = 2);

	//void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker);
	/** 
	* 期货委托撤单
	*/
	int WithdrawFutuEntrust(int iSystemNo = 2);
	/** 
	* 期货持仓查询
	*/
	int SearchFutuHold(int iSystemNo = 2);
    
private:
	CConfigInterface *m_pConfig;
	char m_cUserToken[USERTOKENLEN];
};

//////////////////////////////////////////////////////////////////////
//Callback
class CCallback : public CCallbackInterface
{
public:
    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();
	
    // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
    // Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
	
};
#endif
