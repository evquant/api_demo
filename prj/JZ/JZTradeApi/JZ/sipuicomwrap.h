//  sipuicomux.h LINUX版接口定义和包装类
#ifndef SIPUICOMUX_H_
#define SIPUICOMUX_H_


#include "./tdr.h"

#include "tzzzsmarket.h"

#ifdef linux
    #ifndef __stdcall
        #define __stdcall
    #endif
#endif

//-----------------------------------------------------------------------------------------
// 接口定义,库中接口的C导出函数真是名参见下面的包装类CUicomWap::Load 方法
typedef T_HANDLE (__stdcall *i_Create)(   // 创建,返回句柄
        const char* slibpath,   // [in]库目录，不含库文件名，如果放在path中，可为NULL
        ONRECEIVEDATA funData,  // [in]数据到达回调函数
        void* pParamData,       // [in]数据到达回调函数的调用者参数
        ONERRORMSG funError,    // [in]错误消息回调函数
        void* pParamMsg,        // [in]错误消息回调函数的调用者参数。
        bool bSnapshot,         // [in]是否使用快照,true/false
        bool bAutoReRss         // [in]是否使用自动重订阅true/false
        );

typedef int    (__stdcall *i_Destroy)(T_HANDLE h);
//以静态方式与订阅端建立连接，可建立多个连接,每个连接订阅相同的数据源或不同的数据，如果订阅相同的数据，只处理最新的数据
typedef int (__stdcall *i_ConnectByStatic)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* sSvrIP,	    //[in] ip地址
        int         nSvrPort,   //[in] 端口号
        const char* sUserName,  //[in] 用户名
        const char* sPassword,  //[in] 密码
        const char* sInfosvrIP, //[in] Info_SVR IP地址,Info_SVR上存储市场信息、证券代码信息、用户信息等
        int     nInfoSvrPort,   //[in] Info_SVR Port地址
        int     nLoginMode,      //[in] 普通登录、级联登录
        int     nTimeOutSec     // [in] 登陆超时秒,0表示异步方式立即返回
        );
    // 以动态方式与服务端建立连接
typedef int (__stdcall *i_ConnectByDynamic)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* sIP,        //[in] IP地址
        int nPort,              //[in] 端口号
        int nIPType,            //[in] 内网或外网标识 SIP_SVR_LAN:内网，SIP_SVR_WAN:外网
        int nWanType,           //[in] 网络类型,移动、电信、网通
        const char* sUserName,  //[in] 用户名
        const char* sPassword,  //[in] 密码
        int nLoginMode,          //[in] 普通登录、级联登录
        int     nTimeOutSec     // [in] 登陆超时秒,0表示异步方式立即返回
        );

typedef bool (__stdcall *i_IsConnected)(
        T_HANDLE    h          //[in] TDR_Create返回的句柄
    );
typedef int  (__stdcall *i_DisConnect)(
        T_HANDLE    h          //[in] TDR_Create返回的句柄
    );
typedef int (__stdcall *i_SubscribeByCode)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCode,
        unsigned char ucMode,
        int nServiceId
        );
typedef int (__stdcall *i_UnsubscribeByCode)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCode,
        int nServiceId
        );
typedef int (__stdcall *i_SubscribeByMarket)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        unsigned char ucMode,
        int nServiceId
        );
typedef int (__stdcall *i_UnsubscribeByMarket)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        int nServiceId
        );
typedef int (__stdcall *i_SubscribeByGroup)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCodes,//订阅的证券代码，多支证券以逗号分隔","，比如600000,000001,000012
        unsigned char ucMode,
        int nServiceId
        );
typedef int (__stdcall *i_UnsubscribeByGroup)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarkets,
        const char* chCodes,//订阅的证券代码，多支证券以逗号分隔","，比如600000,000001,000012
        int nServiceId
        );
typedef int (__stdcall *i_UnsubscribeAll)(
        T_HANDLE    h          //[in] TDR_Create返回的句柄
        );
typedef int (__stdcall *i_GetFuturesMarketData)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,           //[in] 市场代码
        const char* chCode,             //[in] 证券代码
        Futures_MarketData* marketData  //[in] 期货行情
        );
typedef int (__stdcall *i_GetIndexMarketData)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCode,
        Stock_IndexData* indexData//指数行情
        );
typedef int (__stdcall *i_GetStockMarketData)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCode,
        Stock_MarketData* marketData//股票市场行情
        );
typedef int (__stdcall *i_ReqFuturesMarketData)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,           //[in] 市场代码
        const char* chCode,             //[in] 证券代码
        Futures_MarketData* marketData  //[in] 期货行情
        );
typedef int (__stdcall *i_ReqIndexMarketData)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCode,
        Stock_IndexData* indexData
        );
typedef int (__stdcall *i_ReqStockMarketData)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* chMarket,
        const char* chCode,
        Stock_MarketData* marketData
        );

typedef bool   (__stdcall *i_GetCodeTableFirst)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* sMarket,
        tPOSMAP* pos,
        Security_Code* pcode
        );
typedef bool   (__stdcall *i_GetCodeTableNext)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        const char* sMarket,
        tPOSMAP* pPos,
        Security_Code* pcode
        );

typedef int (__stdcall *i_SubscribeTrand)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        unsigned char ucMode
        );

typedef int (__stdcall *i_UnsubscribeTrand)(
        T_HANDLE    h          //[in] TDR_Create返回的句柄
        );

typedef int (__stdcall *i_SubscribeStrategySignal)(
        T_HANDLE    h,          //[in] TDR_Create返回的句柄
        unsigned char ucMode
        );
typedef int (__stdcall *i_UnSubscribeStrategySignal)(
        T_HANDLE    h          //[in] TDR_Create返回的句柄
        );
typedef int (__stdcall *i_GetMarketDate)(
        T_HANDLE    h,          // [in] TDR_Create返回的句柄
        int nMarket,            // [in] 市场编码
        int* nDate              // [out] 返回SE_O时回填交易日期
        );

typedef int (__stdcall *i_GenerateAbstract)(
	T_HANDLE    h,          // [in] TDR_Create返回的句柄
	const char*  sMarket,
	const char* sCode,
	T_I32 nServiceId,
	T_U32* uId);
typedef int (__stdcall *i_SubscribeByAbstractID)(
	T_HANDLE    h,          // [in] TDR_Create返回的句柄
    T_U32 uId,
    T_U8  ucMode,
    T_U32 uPos);
typedef int (__stdcall *i_SubscribeByAbstractIDS)(
	T_HANDLE    h, 
	T_U32 uId[],
	T_U8 ucMode[],
	T_U32 uPos[],
	T_U8  ucErrorCode[],
	T_I32 nSize);
typedef int (__stdcall *i_GetDataByAbstractID)(
	T_HANDLE    h,          // [in] TDR_Create返回的句柄
    T_AbstractData pAbstractData[], 
    T_I32 nSize);
typedef int (__stdcall *i_ReqDataByAbstractID)(
	T_HANDLE    h,          // [in] TDR_Create返回的句柄
	T_AbstractData pAbstractData[], 
	T_I32 nSize);



typedef int (__stdcall *i_AddTdaUser)(
	T_HANDLE    h,                  // [in] TDR_Create返回的句柄
	T_U32 uDataID,			// 交易数据抽象ID (市场ID = 0x80和 市场ID = 0)
	const char* suser,		// 数据所属用户，NULL表示自己。
	const char* spass		// 密码，suser != NULL时有效
	);// 添加交易数据用户，返回错误码



typedef int (__stdcall  *i_GetTdaUser)(
	T_HANDLE    h,                  // [in] TDR_Create返回的句柄
	T_U32 uDataID,			// [in]交易数据抽象ID（市场ID = 0x80和 市场ID = 0）
	T_U32 uUserID[],		// [out]数据所属用户，NULL表示自己。
	T_U32 uBufSize,			// [in]密码，suser != NULL时有效
	T_U32 *puRetUsers		// [out] 读取到的用户ID个数
	); // 读取订阅交易数据所属用户表,返回错误码


typedef int	(__stdcall *i_DelTdaUser)( 
	T_HANDLE    h,                  // [in] TDR_Create返回的句柄
	T_U32 uDataID,		// [in] 交易数据抽象ID (市场ID = 0x80 和 市场ID = 0)
	T_U32 uDelUserID    // [in] 要删除关联的用户ID
	); // 删除交易数据ID的所属用户,返回错误码


//-----------------------------------------------------------------------------------------
// 接口封装，封装了so库的加载卸载，隐藏了接口句柄等，仅供参考。
#ifdef linux
	#include <dlfcn.h>
#else
	#include <windows.h>
#endif
class CUicomWrap
{
public:
    CUicomWrap(){
        m_dlhandle = NULL;
        m_handle = 0;
        iZero();
    }
    virtual ~CUicomWrap()
    {
        Unload(); // 卸载库
    }
protected:

#ifdef linux
    void        *m_dlhandle;
#else  // windows
    #define     dlclose(a)     FreeLibrary(a)
    #define     dlsym(a,b)     GetProcAddress(a,b)
    HINSTANCE   m_dlhandle;
#endif

    T_HANDLE    m_handle;

    i_Create                    mi_Create;
    i_Destroy                   mi_Destroy;
    i_ConnectByStatic           mi_ConnectByStatic;
    i_ConnectByDynamic          mi_ConnectByDynamic;
    i_IsConnected               mi_IsConnected;
    i_DisConnect                mi_DisConnect;
    i_SubscribeByCode           mi_SubscribeByCode;
    i_UnsubscribeByCode         mi_UnsubscribeByCode;
    i_SubscribeByMarket         mi_SubscribeByMarket;
    i_UnsubscribeByMarket       mi_UnsubscribeByMarket;
    i_SubscribeByGroup          mi_SubscribeByGroup;
    i_UnsubscribeByGroup        mi_UnsubscribeByGroup;
    i_UnsubscribeAll            mi_UnsubscribeAll;
    i_GetFuturesMarketData      mi_GetFuturesMarketData;
    i_GetIndexMarketData        mi_GetIndexMarketData;
    i_GetStockMarketData        mi_GetStockMarketData;
    i_ReqFuturesMarketData      mi_ReqFuturesMarketData;
    i_ReqIndexMarketData        mi_ReqIndexMarketData;
    i_ReqStockMarketData        mi_ReqStockMarketData;
    i_GetCodeTableFirst         mi_GetCodeTableFirst;
    i_GetCodeTableNext          mi_GetCodeTableNext;

    i_GetMarketDate             mi_GetMarketDate;
	i_GenerateAbstract          mi_GenerateAbstract;
	i_SubscribeByAbstractID     mi_SubscribeByAbstractID;
	i_SubscribeByAbstractIDS     mi_SubscribeByAbstractIDS;
	i_GetDataByAbstractID       mi_GetDataByAbstractID;
	i_ReqDataByAbstractID       mi_ReqDataByAbstractID;
	i_AddTdaUser                mi_AddTdaUser;
	i_GetTdaUser                mi_GetTdaUser;
	i_DelTdaUser                mi_DelTdaUser;
	

	
protected:
    void iZero()
    {
        mi_Create = NULL;
        mi_Destroy = NULL;
        mi_ConnectByStatic = NULL;
        mi_ConnectByDynamic = NULL;
        mi_IsConnected = NULL;
        mi_DisConnect = NULL;
        mi_SubscribeByCode = NULL;
        mi_UnsubscribeByCode = NULL;
        mi_SubscribeByMarket = NULL;
        mi_UnsubscribeByMarket = NULL;
        mi_SubscribeByGroup = NULL;
        mi_UnsubscribeByGroup = NULL;
        mi_UnsubscribeAll = NULL;
        mi_GetFuturesMarketData = NULL;
        mi_GetIndexMarketData = NULL;
        mi_GetStockMarketData = NULL;
        mi_ReqFuturesMarketData = NULL;
        mi_ReqIndexMarketData = NULL;
        mi_ReqStockMarketData = NULL;
        mi_GetCodeTableFirst = NULL;
        mi_GetCodeTableNext = NULL;

        mi_GetMarketDate = NULL;
		mi_GenerateAbstract = NULL;
		mi_SubscribeByAbstractID = NULL;
		mi_GetDataByAbstractID = NULL;
		mi_ReqDataByAbstractID = NULL;
	    mi_AddTdaUser            = NULL;
		mi_GetTdaUser            = NULL;
		mi_DelTdaUser            = NULL;
    }

public:
    bool    IsCreate(){   return  (m_handle != 0);   }
    bool    IsLoad(){     return (m_dlhandle != NULL);  }
    bool    Load(const char* slibfile) // 加载库和提取接口
    {
        if(m_dlhandle)
            Unload();
    #ifdef linux
        m_dlhandle = dlopen( slibfile, RTLD_LAZY );
    #else
        m_dlhandle = LoadLibrary(slibfile);
    #endif

        if (!m_dlhandle)
        {
            return false;
        }

        mi_Create = (i_Create)dlsym(m_dlhandle,"TDR_Create");
        mi_Destroy = (i_Destroy)dlsym(m_dlhandle,"TDR_Destroy");
        mi_ConnectByStatic = (i_ConnectByStatic)dlsym(m_dlhandle,"TDR_ConnectByStatic");
        mi_ConnectByDynamic = (i_ConnectByDynamic)dlsym(m_dlhandle,"TDR_ConnectByDynamic");
        mi_IsConnected = (i_IsConnected)dlsym(m_dlhandle,"TDR_IsConnected");
        mi_DisConnect = (i_DisConnect)dlsym(m_dlhandle,"TDR_DisConnect");
        mi_SubscribeByCode = (i_SubscribeByCode)dlsym(m_dlhandle,"TDR_SubscribeByCode");
        mi_UnsubscribeByCode   = (i_UnsubscribeByCode)dlsym(m_dlhandle,"TDR_UnsubscribeByCode");
        mi_SubscribeByMarket= (i_SubscribeByMarket)dlsym(m_dlhandle,"TDR_SubscribeByMarket");
        mi_UnsubscribeByMarket   = (i_UnsubscribeByMarket)dlsym(m_dlhandle,"TDR_UnsubscribeByMarket");
        mi_SubscribeByGroup = (i_SubscribeByGroup)dlsym(m_dlhandle,"TDR_SubscribeByGroup");
        mi_UnsubscribeByGroup = (i_UnsubscribeByGroup)dlsym(m_dlhandle,"TDR_UnsubscribeByGroup");
        mi_UnsubscribeAll = (i_UnsubscribeAll)dlsym(m_dlhandle,"TDR_UnsubscribeAll");
        mi_GetFuturesMarketData = (i_GetFuturesMarketData)dlsym(m_dlhandle,"TDR_GetFuturesMarketData");
        mi_GetIndexMarketData = (i_GetIndexMarketData)dlsym(m_dlhandle,"TDR_GetIndexMarketData");
        mi_GetStockMarketData = (i_GetStockMarketData)dlsym(m_dlhandle,"TDR_GetStockMarketData");
        mi_ReqFuturesMarketData = (i_ReqFuturesMarketData)dlsym(m_dlhandle,"TDR_ReqFuturesMarketData");
        mi_ReqIndexMarketData = (i_ReqIndexMarketData)dlsym(m_dlhandle,"TDR_ReqIndexMarketData");
        mi_ReqStockMarketData = (i_ReqStockMarketData)dlsym(m_dlhandle,"TDR_ReqStockMarketData");
        mi_GetCodeTableFirst = (i_GetCodeTableFirst)dlsym(m_dlhandle,"TDR_GetCodeTableFirst");
        mi_GetCodeTableNext = (i_GetCodeTableNext)dlsym(m_dlhandle,"TDR_GetCodeTableNext");

        mi_GetMarketDate = (i_GetMarketDate)dlsym(m_dlhandle,"TDR_GetMarketDate");
		mi_GenerateAbstract = (i_GenerateAbstract)dlsym(m_dlhandle,"TDR_GenerateAbstract");
		mi_SubscribeByAbstractID = (i_SubscribeByAbstractID)dlsym(m_dlhandle,"TDR_SubscribeByAbstractID");
		mi_SubscribeByAbstractIDS = (i_SubscribeByAbstractIDS)dlsym(m_dlhandle,"TDR_SubscribeByAbstractIDS");
		mi_GetDataByAbstractID = (i_GetDataByAbstractID)dlsym(m_dlhandle,"TDR_GetDataByAbstractID");
		mi_ReqDataByAbstractID = (i_ReqDataByAbstractID)dlsym(m_dlhandle,"TDR_ReqDataByAbstractID");
		mi_AddTdaUser            = (i_AddTdaUser)dlsym(m_dlhandle,"TDR_AddTdaUser");
		mi_GetTdaUser            = (i_GetTdaUser)dlsym(m_dlhandle,"TDR_GetTdaUser");
		mi_DelTdaUser            = (i_DelTdaUser)dlsym(m_dlhandle,"TDR_DelTdaUser");
        return true;
    };
    bool    Unload() // 卸载库
    {
        if(!m_dlhandle)
            return true;
        DisConnect();
        Destroy();
        dlclose( m_dlhandle );
        m_dlhandle = NULL;
        iZero();
        return true;
    }
    // 下面是接口映射
    bool Create(   // 创建,返回句柄
        const char* slibpath,   // [in]库目录，不含库文件名，如果放在path中，可为NULL
        ONRECEIVEDATA funData,  // [in]数据到达回调函数
        void* pParamData,       // [in]数据到达回调函数的调用者参数
        ONERRORMSG funError,    // [in]错误消息回调函数
        void* pParamMsg,        // [in]错误消息回调函数的调用者参数。
        bool bSnapshot,         // [in]是否使用快照,true/false
        bool bAutoReRss         // [in]是否使用自动重订阅true/false
        )
    {
        if(m_handle)
            return true;
        if(mi_Create)
            m_handle = mi_Create(slibpath,funData,pParamData,funError,pParamMsg,bSnapshot,bAutoReRss);
        return (m_handle != 0);
    }
    int Destroy()
    {
        if(m_handle && mi_Destroy)
        {
            mi_Destroy(m_handle);
            m_handle = 0;
            return SE_OK;
        }
        return SE_LIBERR;
    }
    int ConnectByStatic(
        const char* sSvrIP,	    //[in] ip地址
        int         nSvrPort,   //[in] 端口号
        const char* sUserName,  //[in] 用户名
        const char* sPassword,  //[in] 密码
        const char* sInfosvrIP, //[in] Info_SVR IP地址,Info_SVR上存储市场信息、证券代码信息、用户信息等
        int     nInfoSvrPort,   //[in] Info_SVR Port地址
        int     nLoginMode,     //[in] 普通登录、级联登录
        int     nTimeOutSec     //[in] 登陆超时秒,0表示异步方式立即返回
        )
    {
        if(m_handle && mi_ConnectByStatic)
            return mi_ConnectByStatic(m_handle,sSvrIP,nSvrPort,sUserName,sPassword,sInfosvrIP,nInfoSvrPort,nLoginMode,nTimeOutSec);
        return SE_LIBERR;
    }
    int ConnectByDynamic(
        const char* sIP,        //[in] IP地址
        int nPort,              //[in] 端口号
        int nIPType,            //[in] 内网或外网标识 SIP_SVR_LAN:内网，SIP_SVR_WAN:外网
        int nWanType,           //[in] 网络类型,移动、电信、网通
        const char* sUserName,  //[in] 用户名
        const char* sPassword,  //[in] 密码
        int nLoginMode,         //[in] 普通登录、级联登录
        int     nTimeOutSec     //[in] 登陆超时秒,0表示异步方式立即返回
        )
    {
        if(m_handle && mi_ConnectByDynamic)
            return mi_ConnectByDynamic(m_handle,sIP,nPort,nIPType,nWanType,sUserName,sPassword,nLoginMode,nTimeOutSec);
        return SE_LIBERR;
    }

    bool IsConnected()
    {
        if(m_handle && mi_IsConnected)
            return mi_IsConnected(m_handle);
        return false;
    }
    int  DisConnect()
    {
        if(m_handle && mi_DisConnect)
            return mi_DisConnect(m_handle);
        return SE_LIBERR;
    }
    int SubscribeByCode(
        const char* chMarket,
        const char* chCode,
        unsigned char ucMode,
        int nServiceId
        )
    {
        if(m_handle && mi_SubscribeByCode)
            return mi_SubscribeByCode(m_handle,chMarket,chCode,ucMode,nServiceId);
        return SE_LIBERR;
    }
    int UnsubscribeByCode(
        const char* chMarket,
        const char* chCode,
        int nServiceId
        )
    {
        if(m_handle && mi_UnsubscribeByCode)
            return mi_UnsubscribeByCode(m_handle,chMarket,chCode,nServiceId);
        return SE_LIBERR;
    }
    int SubscribeByMarket(
        const char* chMarket,
        unsigned char ucMode,
        int nServiceId
        )
    {
        if(m_handle && mi_SubscribeByMarket)
            return mi_SubscribeByMarket(m_handle,chMarket,ucMode,nServiceId);
        return SE_LIBERR;
    }
    int UnsubscribeByMarket(
        const char* chMarket,
        int nServiceId
        )
    {
        if(m_handle && mi_UnsubscribeByMarket)
            return mi_UnsubscribeByMarket(m_handle,chMarket,nServiceId);
        return SE_LIBERR;
    }
    int SubscribeByGroup(
        const char* chMarket,
        const char* chCodes,//订阅的证券代码，多支证券以逗号分隔","，比如600000,000001,000012
        unsigned char ucMode,
        int nServiceId
        )
    {
        if(m_handle && mi_SubscribeByGroup)
            return mi_SubscribeByGroup(m_handle,chMarket,chCodes,ucMode,nServiceId);
        return SE_LIBERR;
    }
    int UnsubscribeByGroup(
        const char* chMarkets,
        const char* chCodes,//订阅的证券代码，多支证券以逗号分隔","，比如600000,000001,000012
        int nServiceId
        )
    {
        if(m_handle && mi_UnsubscribeByGroup)
            return mi_UnsubscribeByGroup(m_handle,chMarkets,chCodes,nServiceId);
        return SE_LIBERR;
    }
    int UnsubscribeAll( )
    {
        if(m_handle && mi_UnsubscribeAll)
            return mi_UnsubscribeAll(m_handle);
        return SE_LIBERR;
    }
    int GetFuturesMarketData(
        const char* chMarket,           //[in] 市场代码
        const char* chCode,             //[in] 证券代码
        Futures_MarketData* marketData  //[in] 期货行情
        )
    {
        if(m_handle && mi_GetFuturesMarketData)
            return mi_GetFuturesMarketData(m_handle,chMarket,chCode,marketData);
        return SE_LIBERR;
    }
    int GetIndexMarketData(
        const char* chMarket,
        const char* chCode,
        Stock_IndexData* indexData//指数行情
        )
    {
        if(m_handle && mi_GetIndexMarketData)
            return mi_GetIndexMarketData(m_handle,chMarket,chCode,indexData);
        return SE_LIBERR;
    }
    int GetStockMarketData(
        const char* chMarket,
        const char* chCode,
        Stock_MarketData* marketData//股票市场行情
        )
    {
        if(m_handle && mi_GetStockMarketData)
            return mi_GetStockMarketData(m_handle,chMarket,chCode,marketData);
        return SE_LIBERR;
    }
    int ReqFuturesMarketData(
        const char* chMarket,           //[in] 市场代码
        const char* chCode,             //[in] 证券代码
        Futures_MarketData* marketData  //[in] 期货行情
        )
    {
        if(m_handle && mi_ReqFuturesMarketData)
            return mi_ReqFuturesMarketData(m_handle,chMarket,chCode,marketData);
        return SE_LIBERR;
    }
    int ReqIndexMarketData(
        const char* chMarket,
        const char* chCode,
        Stock_IndexData* indexData
        )
    {
        if(m_handle && mi_ReqIndexMarketData)
            return mi_ReqIndexMarketData(m_handle,chMarket,chCode,indexData);
        return SE_LIBERR;
    }
    int ReqStockMarketData(
        const char* chMarket,
        const char* chCode,
        Stock_MarketData* marketData
        )
    {
        if(m_handle && mi_ReqStockMarketData)
            return mi_ReqStockMarketData(m_handle,chMarket,chCode,marketData);
        return SE_LIBERR;
    }

    bool   GetCodeTableFirst(
        const char* sMarket,
        tPOSMAP* pPos,
        Security_Code* pcode
        )
    {
        if(m_handle && mi_GetCodeTableFirst)
            return mi_GetCodeTableFirst(m_handle,sMarket,pPos,pcode);
        return false;
    }
    bool  GetCodeTableNext(
        const char* sMarket,
        tPOSMAP* pPos,
        Security_Code* pcode
        )
    {
        if(m_handle && mi_GetCodeTableNext)
            return mi_GetCodeTableNext(m_handle,sMarket,pPos,pcode);
        return false;
    }

   
    int GetMarketDate(
        int nMarket,
        int* nDate
        )
    {
        if(m_handle && mi_GetMarketDate)
            return mi_GetMarketDate(m_handle,nMarket,nDate);
        return SE_LIBERR;
    }

	int GenerateAbstract(const char*  sMarket,const char* sCode,T_I32 nServiceId,T_U32* uId)
	{
		if(m_handle && mi_GenerateAbstract)
			return mi_GenerateAbstract(m_handle,sMarket,sCode,nServiceId,uId);
		return SE_LIBERR;

	}
	int SubscribeByAbstractID(T_U32 uId,T_U8  ucMode,T_U32 uPos)
	{
		if(m_handle && mi_SubscribeByAbstractID)
			return mi_SubscribeByAbstractID(m_handle,uId,ucMode,uPos);
		return SE_LIBERR;

	}
	int SubscribeByAbstractIDS(T_U32 uId[], T_U8  ucMode[], T_U32 uPos[],T_U8  ucErrorCode[],T_I32 nSize)
	{
		if(m_handle && mi_SubscribeByAbstractIDS)
			return mi_SubscribeByAbstractIDS(m_handle,uId,ucMode,uPos,ucErrorCode,nSize);
		return SE_LIBERR;

	}
	int GetDataByAbstractID(T_AbstractData pAbstractData[], T_I32 nSize)
	{
		if(m_handle && mi_GetDataByAbstractID)
			return mi_GetDataByAbstractID(m_handle,pAbstractData,nSize);
		return SE_LIBERR;

	}
	int ReqDataByAbstractID(T_AbstractData pAbstractData[], T_I32 nSize)
	{
		if(m_handle && mi_ReqDataByAbstractID)
			return mi_ReqDataByAbstractID(m_handle,pAbstractData,nSize);
		return SE_LIBERR;

	}


	int AddTdaUser(
		T_U32 uDataID,			// 交易数据抽象ID (市场ID = 0x80和 市场ID = 0)
		const char* suser,		// 数据所属用户，NULL表示自己。
		const char* spass		// 密码，suser != NULL时有效
		)// 添加交易数据用户，返回错误码

	{
		if(m_handle && mi_AddTdaUser)
			return mi_AddTdaUser(m_handle,uDataID,suser,spass);
		return SE_LIBERR;
	
	}

	int   GetTdaUser(
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_U32 uDataID,			// [in]交易数据抽象ID（市场ID = 0x80和 市场ID = 0）
		T_U32 uUserID[],		// [out]数据所属用户，NULL表示自己。
		T_U32 uBufSize,			// [in]密码，suser != NULL时有效
		T_U32 *puRetUsers		// [out] 读取到的用户ID个数
		) // 读取订阅交易数据所属用户表,返回错误码
	{
		if(m_handle && mi_GetTdaUser)
			return mi_GetTdaUser(m_handle,uDataID,uUserID,uBufSize,puRetUsers);
		return SE_LIBERR;
	}

	int	 DelTdaUser( 
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_U32 uDataID,		// [in] 交易数据抽象ID (市场ID = 0x80 和 市场ID = 0)
		T_U32 uDelUserID    // [in] 要删除关联的用户ID
		) // 删除交易数据ID的所属用户,返回错误码
	{
		if(m_handle && mi_DelTdaUser)
			return mi_DelTdaUser(m_handle,uDataID,uDelUserID);
		return SE_LIBERR;
	}
};
#endif // SIPUICOMUX_H_
