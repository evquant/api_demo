//***********************************************************************************
//行情系统 linux/windows X86/X64 通用版接口定议 v2.0
//作者 马辉  mahui012430@gtjas.com
//版权所有：国泰君安证券股份有限公司 
//创建日期:2013.07.29
//说明：该接口提供三种获取行情数据的方式
//1)订阅-推送方式,通过回调函数获取Level2行情，适用于每一笔tick数据均要处理的情形。
//2)订阅-设置本地行情快照存储。通过TDR_GetXXX接口从本地内存获取最新行情。
//3)请求-应答。通过TDR_ReqXXX接口，向服务器请求行情数据。
//***********************************************************************************


//***********************************************************************************
//行情接口使用步骤    v2.3
//1)TDR_Create...
//2)TDR_ConnectByDynamic/TDR_ConnectByStatic
//3)TDR_SubscribeXXX 订阅行情
//4)通过回调函数或TDR_GetXXX接口获取行情
//5)TDR_UnSubscribeXXX 取消订阅
//6)TDR_DisConnect 断开连接
//7)TDR_Destroy   销毁句柄
//***********************************************************************************



//  tdr.h   LINUX/windows   X86/X64 通用版接口定义

//  由于使用了动态库，在编译连接时要加-ldl选项.
//  如果在code::block 集成环境,Project->Build Options->Linker Setting->Other Linker Options加-ldl选项
//
//  如果您使用了多线程，需要为GCC加上pthread链接库，
//  如果在code::block集成环境，则Project->Build Options->Linker Setting->Link Libraries中加入pthread

// LINUX中 GCC类型长度(字节)
// type         x86_sizeof(type)    x64_sizeof(type)
// char         1                   1
// short        2                   2
// int          4                   4
// long         4                   8
// void*        4                   8
// long long    8                   8
// float        4                   4
// double       8                   8

//__GNUC__
//linux
//__i386__  是x86
//__x86_64__  是X64


//windows VC编译器
// type         x86_sizeof(type)    x64_sizeof(type)
// char         1                   1
// short        2                   2
// int          4                   4
// long         4                   4
// void*        4                   8
// long long    8                   8
// float        4                   4
// double       8                   8

//_WIN32
//	Defined for applications for Win32 and Win64. Always defined.
//_WIN64
//	Defined for applications for Win64.
//_WINDOWS_ 在Windows.h中定义的#ifndef _WINDOWS_
//#define _WINDOWS_

#include "tdef.h"

#ifndef TDR_H_INCLUDED
#define TDR_H_INCLUDED




//--------------------------------------------------------------------------------------------
//参数命名规范
//1 单个字符 char 以小写字母‘c’打头，若不作说明，均指小写字母。
//2 char* char[] 类型，以小写字母's'打头。
//3 有符号整型T_I32 即 n打头。
//4 无符号整型T_U32 即 u打头。
//5 long long类型以T_I64 i 打头


//-----------------------------------------------------------------------------------------
// 错误码
#define SE_OK				0x00	// 成功
#define SE_ERR				0x01	// 未知错误
#define SE_ERRHANDLE		0x02	// 无效句柄
#define	SE_ARGS				0x03	// 参数错误
#define SE_ERRDATA			0x04	// 错误的数据或数据无效
#define SE_EXP				0x05	// 错误的表达式
#define SE_FRMERR			0x07	// 报文错误
#define SE_UNINITIONAL      0x08    // 未初始化

#define SE_NOTSUPPORT		0x0F	// 不支持的命令

#define SE_NOCONNECT		0x10	// 网络未连接
#define	SE_TIMEOUT			0x11	// 网络超时
#define SE_CONNECTFULL		0x12	// 服务器连接满
#define SE_NETERR			0x13	// 网络错误

#define SE_OP_NOUSER		0x20	// 无此用户
#define SE_OP_NOPOWER		0x21	// 无此权限
#define	SE_OP_NOACTIVE		0x22	// 帐号不活动
#define	SE_OP_PASSWORD		0x23	// 密码错误

#define SE_NOKEY			0x30	// 主键不存在
#define SE_KEYEXIST			0x31	// 主键已经存在
#define SE_NOTDEFITEM		0x32	// 没有定义的抽象数据类型

#define SE_SUBSCRIPT_FULL	0x40	// 订阅满
#define SE_NOTSUBSCRIPT		0x41	// 没有订阅
#define SE_INFOSVRNOTRUN	0x42	// info_svr没有运行或者连接失败!
#define SE_USERMAXCONNECTED	0x43	// 用户已达到最大同时登录数，不能再连接
#define SE_IPMASK			0x44	// IP被屏蔽
#define SE_VER				0x45	// 版本错误
#define SE_NOTUPLINK		0x46	// 不容许级联
#define	SE_PGMCONNECTERR	0x47	// 加入可靠多播错误
#define SE_READBUFOVERFLOW	0x48	// 接收缓冲溢出
#define SE_NOPGMRECVPOWER	0x49	// 无PGM读取权限


#define SE_LIBUI            0x51    // UI库错误
#define SE_LIBMI            0x52    // MI库错误
#define SE_NOSERVER         0x53    // 没有服务器，动态连接时，当没有sip_svr运行时会返回这个错误代码.
#define SE_NOMARKET         0x54    // 无此市场
#define SE_NOCODE           0x55    // 无此编码，当不能转换为抽象ID时，返回此错误
#define SE_USERNAMENOTDIGIT 0x56    // 用户名非数字格式
#define SE_OUTSIDEUSERS     0x57    // 超出最大用户数,TDR_SubscribeTrand可能会返回这个错误
#define SE_NOSNAPSHOT       0x58    // 没有快照，是指创建时不使用快照，Get接口会返回这个错误码
#define SE_LIBERR           0x59    // 库错误，一般是没有找到库或者使用了错误的库


#define SE_NOTTDAID			0x61	// 非交易数据ID
#define SE_TDAUSERSFULL     0x62	// 交易数据抽象ID用户数满，每个交易抽象ID可最大同时订阅1024个用户
#define SE_TDAUSERERR		0x63	// 订阅的交易数据用户帐号错误
#define SE_NOTRSSATLNK		0x64	// 级联模式下不允许订阅交易数据

#define SE_PROXYCONNECT		0x71	// 代理服务器连接错误
#define SE_PROXYAUTH		0x72	// 代理服务器验证错误

//-----------------------------------------------------------------------------------------
// 基本数据结构
#ifndef _TPOSMAP
    #define _TPOSMAP
    struct tPOSMAP  // 位置结构，用于遍历代码表
    {
        T_U32 npos;  //hash位置
        T_U32 nlist; //List位置
    };
#endif

//-----------------------------------------------------------------------------------------
// 回调函数定义

// 数据到达回调，pParam为调用着自己的参数，即TDR_Create中的pParamData参数
typedef  void   (__stdcall *ONRECEIVEDATA)(void* pUserParam, T_I32 nDate, T_I32 nMarketId,const char* sCode,const char* sName,T_I32 nServiceId,void* pData,T_I32 nLen);

// 错误信息回调，pParam为调用着自己的参数，即TDR_Create中的pParamMsg参数
typedef  void   (__stdcall *ONERRORMSG)(void* pUserParam,T_I32 nError,T_I32 nErrSource,T_U32 uData);



//-----------------------------------------------------------------------------------------
// 证券业务数据结构


// 外网运营商
#define     WAN_TC				0           // 电信
#define     WAN_NC				1           // 网通
#define     WAN_UC				2           // 联通
#define     WAN_MC				3           // 移动
#define     WAN_CC				4           // 广电

// 订阅方式
#define		RSS_MODE_NEW		0			// 最新订阅
#define		RSS_MODE_INC		1			// 增量订阅

// 订阅推送位置
#define     UI_RSS_POSSTART		0			// 订阅推送位置从最开始
#define     UI_RSS_POSCUR		0XFFFFFFFF	// 订阅推送位置从当前

// 错误源定义,cb_ErrMsg回调中使用
#define     ERRMSGSRC_CONNECT	0x81		// 连接
#define     ERRMSGSRC_LOGIN		0x82		// 登录


// 登录方式
// 登录方式
#define     UI_LOGIN_NORMAL		0			// 普通模式
#define     UI_LOGIN_UPLINK		1			// 级联模式
#define     UI_LOGIN_PGM		2			// PGM模式

// 网络模式
#define		SIP_SVR_WAN			0			// SIP_SVR的外网IP
#define		SIP_SVR_LAN			1			// SIP_SVR的内网IP

//代理类型
#define TCP_PROXY_NONE		0
#define TCP_PROXY_SOCKET5	1
#define TCP_PROXY_HTTP		2   //暂不支持



//////////////////////////////////////////////////////////////////////////
//现有市场编号定义到8，若扩展期它市场，在此添加备注信息，各市场定义的宏参
//见各市场定义定义头文件
//////////////////////////////////////////////////////////////////////////
//// 市场编号定义                                                          */
//#define     SH                  1           //上海市场
//#define     SZ                  2           //深圳市场
//#define     CFFEX               3           //中金所
//#define     CZCE                4           //郑商所
//#define     DCE                 5           //大商所
//#define     SHFE                6           //上期所
//#define     ZZZS                7           //中证指数市场
//#define     SHOP                8           //上交所期权市场
//#define     HK                  9           //香港市场

#include "tshmarket.h"      //上交所证券行情
#include "tszmarket.h"      //深交所证券行情
#include "tcffexmarket.h"   //中金所期货行情
#include "tczcemarket.h"    //郑商所期货行情
#include "tshfemarket.h"    //上期所期货行情
#include "tdcemarket.h"     //大商所期货行情
#include "tshopmarket.h"    //上交所期权行情
#include "tzzzsmarket.h"    //中证指数行情
#include "thkmarket.h"      //港交所omd行情
#include "tszopmarket.h"    //深交所期权行情

#include "tmatchpushdefine.h" //标准成交回报




#pragma pack(push,1)                // 结构1字节对起，直到后面的pack(pop)，仅对业务数据结构要求。

//   1.1 市场信息
typedef struct
{
	unsigned char   uNumber;        //市场编号，对应抽象ID的Byte3 ，作为唯一key
	unsigned char   ucRes1;         //保留1
	unsigned char   ucRes2[2];      //保留2
	char            sName[20];      //市场名称
	char			sCode[8];       //市场代码SZ,SH,CF

}Security_MarketInfo; //sizeof() = 32

//   1.2 证券代码
typedef struct
{
	T_U32  uKey;                    //主键key,Byte0-Byte1:与抽象ID的Byte1-Byte2对应，取值[1-65534]；Byte2:市场编号,与T_MARKETINFO中的uNumber相对应,Byte3:0x00
	T_U32  ucRes1;                  //保留 add 2013.06.21 证券类型，用0x10表示指数
	char   sCode[28];               //普通证券代码6位，但考虑到期权代码扩至16位
	char   sName[28];               //证券名称

}Security_Code;//sizeof()= 64

#pragma pack(pop)  // 1字节对其结束

//抽象数据ID
typedef struct t_AbstractData
{
	T_U32 uId;              //抽象ID,抽象ID,由TDR_GenerateAbstract生成
	T_I32 nErr;             //错误码
	char sData[8176];       //抽象数据，需要强制转换成对应的结构体
	T_U8 uRes1[4];          //保留字段1
	T_U32 uRes2;            //保留字段2
	char sMarket[8]; //证券市场
	char sCode[28];  //证券代码
}T_AbstractData,*T_PAbstractData;

//当日历史数据
typedef struct t_HistoryData
{
	T_U16 usLen; //sData中有效数据的长度
	T_U32 uPos;  //当日历史数据在内存中的存储位置
	char sData[8186];//抽象数据类型，根据请求的类型定
}T_HistoryData;//

//-------------------------------------------------------------------------------------------
//-------------------------接口调用----------------------------------------------------------
#define TDRAPI(type) type __stdcall
extern "C"
{

	TDRAPI(T_HANDLE) TDR_Create(// 创建,返回句柄
		const char* slibpath,   // [in]库目录，不含库文件名，如果放在path中，可为NULL
		ONRECEIVEDATA funData,  // [in]数据到达回调函数
		void* pParamData,       // [in]数据到达回调函数的调用者参数
		ONERRORMSG funError,    // [in]错误消息回调函数
		void* pParamMsg,        // [in]错误消息回调函数的调用者参数。
		bool bSnapshot,         // [in]是否使用快照,true/false
		bool bAutoReRss         // [in]是否使用自动重订阅true/false
		);

	TDRAPI(T_I32)    TDR_Destroy(T_HANDLE h);
	//以静态方式与订阅端建立连接，可连接指定的行情服务器
	TDRAPI(T_I32)  TDR_ConnectByStatic(
		T_HANDLE    h,            //[in] TDR_Create返回的句柄
		const char* sSvrIP,	      //[in] ip地址
		T_I32         nSvrPort,   //[in] 端口号
		const char* sUserName,    //[in] 用户名
		const char* sPassword,    //[in] 密码
		const char* sInfosvrIP,   //[in] Info_SVR IP地址,Info_SVR上存储市场信息、证券代码信息、用户信息等
		T_I32       nInfoSvrPort, //[in] Info_SVR Port地址
		T_I32       nLoginMode,   //[in] 普通登录、级联登录
		int         nTimeOutSec   //[in] 登陆超时秒,0表示异步方式立即返回
		);
	// 以动态方式与服务端建立连接,会选择一台负载最小的服务器与之连接
	TDRAPI(T_I32) TDR_ConnectByDynamic(
		T_HANDLE    h,            //[in] TDR_Create返回的句柄
		const char* sIP,          //[in] IP地址
		T_I32 nPort,              //[in] 端口号
		T_I32 nIPType,            //[in] 内网或外网标识 SIP_SVR_LAN:内网，SIP_SVR_WAN:外网
		T_I32 nWanType,           //[in] 网络类型,移动、电信、网通
		const char* sUserName,    //[in] 用户名
		const char* sPassword,    //[in] 密码
		T_I32 nLoginMode,         //[in] 普通登录、级联登录
		int     nTimeOutSec       //[in] 登陆超时秒,0表示异步方式立即返回
		);

	TDRAPI(bool) TDR_IsConnected(
		T_HANDLE    h             //[in] TDR_Create返回的句柄
		);
	TDRAPI(T_I32)  TDR_DisConnect(
		T_HANDLE    h             //[in] TDR_Create返回的句柄
		);
	TDRAPI(T_I32) TDR_SubscribeByCode(
		T_HANDLE    h,            //[in] TDR_Create返回的句柄
		const char* sMarket,
		const char* sCode,
		unsigned char ucMode,
		T_I32 nServiceId
		);
	TDRAPI(T_I32) TDR_UnsubscribeByCode(
		T_HANDLE    h,          //[in] TDR_Create返回的句柄
		const char* sMarket,
		const char* sCode,
		T_I32 nServiceId
		);
	TDRAPI(T_I32) TDR_SubscribeByMarket(
		T_HANDLE    h,          //[in] TDR_Create返回的句柄
		const char* sMarket,
		unsigned char ucMode,
		T_I32 nServiceId
		);
	TDRAPI(T_I32) TDR_UnsubscribeByMarket(
		T_HANDLE    h,          //[in] TDR_Create返回的句柄
		const char* sMarket,
		T_I32 nServiceId
		);
	TDRAPI(T_I32) TDR_SubscribeByGroup(
		T_HANDLE    h,          //[in] TDR_Create返回的句柄
		const char* sMarket,    //市场代码,SH、SZ、SHFE、
		const char* sCodes,     //订阅的证券代码，多支证券以逗号分隔","，比如600000,000001,000012
		unsigned char ucMode,
		T_I32 nServiceId
		);
	TDRAPI(T_I32) TDR_UnsubscribeByGroup(
		T_HANDLE    h,          //[in] TDR_Create返回的句柄
		const char* sMarkets,
		const char* sCodes,     //订阅的证券代码，多支证券以逗号分隔","，比如600000,000001,000012
		T_I32 nServiceId
		);
	TDRAPI(T_I32) TDR_UnsubscribeAll(
		T_HANDLE    h          //[in] TDR_Create返回的句柄
		);
	TDRAPI(T_I32) TDR_GetFuturesMarketData(
		T_HANDLE    h,                 //[in] TDR_Create返回的句柄
		const char* sMarket,           //[in] 市场代码
		const char* sCode,             //[in] 证券代码
		Futures_MarketData* marketData //[in] 期货行情
		);
	TDRAPI(T_I32) TDR_GetIndexMarketData(
		T_HANDLE    h,                 //[in] TDR_Create返回的句柄
		const char* sMarket,           //[in] 市场代码
		const char* sCode,             //[in] 证券代码
		Stock_IndexData* pIndexData    //指数行情
		);
	TDRAPI(T_I32) TDR_GetStockMarketData(
		T_HANDLE    h,				  //[in] TDR_Create返回的句柄
		const char* sMarket,          //[in] 市场代码
		const char* sCode,            //[in] 证券代码
		Stock_MarketData* pMarketData //股票市场行情
		);
	TDRAPI(T_I32) TDR_ReqFuturesMarketData(
		T_HANDLE    h,                  //[in] TDR_Create返回的句柄
		const char* sMarket,            //[in] 市场代码
		const char* sCode,              //[in] 证券代码
		Futures_MarketData* pMarketData //[in] 期货行情
		);
	TDRAPI(T_I32) TDR_ReqIndexMarketData(
		T_HANDLE    h,                 //[in] TDR_Create返回的句柄
		const char* sMarket,           //[in] 市场代码
		const char* sCode,             //[in] 证券代码
		Stock_IndexData* pIndexData    //[in]
		);
	TDRAPI(T_I32) TDR_ReqStockMarketData(
		T_HANDLE    h,                 //[in] TDR_Create返回的句柄
		const char* sMarket,
		const char* sCode,
		Stock_MarketData* pMarketData
		);

	TDRAPI(bool)   TDR_GetCodeTableFirst(
		T_HANDLE    h,                //[in] TDR_Create返回的句柄
		const char* sMarket,
		tPOSMAP* pos,
		Security_Code* pcode
		);

	TDRAPI(bool)   TDR_GetCodeTableNext(
		T_HANDLE    h,                //[in] TDR_Create返回的句柄
		const char* sMarket,
		tPOSMAP* pPos,
		Security_Code* pcode
		);

	//TDRAPI(T_I32) TDR_SubscribeTrand(
	//	T_HANDLE    h,               //[in] TDR_Create返回的句柄
	//	unsigned char ucMode
	//	);

	//TDRAPI(T_I32) TDR_UnsubscribeTrand(
	//	T_HANDLE    h                //[in] TDR_Create返回的句柄
	//	);

	//TDRAPI(T_I32) TDR_SubscribeStrategySignal(
	//	T_HANDLE    h,               //[in] TDR_Create返回的句柄
	//	unsigned char ucMode
	//	);
	//TDRAPI(T_I32) TDR_UnSubscribeStrategySignal(
	//	T_HANDLE    h                //[in] TDR_Create返回的句柄
	//	);
	TDRAPI(T_I32) TDR_GetMarketDate(
		T_HANDLE    h,               // [in] TDR_Create返回的句柄
		T_I32  nMarket,              // [in] 市场编码
		T_I32* nDate                 // [out] 返回SE_O时回填交易日期
		);

	//2013-11-20 新增加接口
	TDRAPI(T_I32) TDR_GenerateAbstract(
		T_HANDLE    h,               // [in] TDR_Create返回的句柄
		const char*  sMarket,        // [in] 市场代码
		const char* sCode,           // [in] 证券代码
		T_I32 nServiceId,            // [in] 服务数据ID
		T_U32* uId                   // [out] 返回抽象ID
		);
	//可订阅各类数据，灵活多变,可参考示例
	TDRAPI(T_I32) TDR_SubscribeByAbstractID(
		T_HANDLE    h,               // [in] TDR_Create返回的句柄
		T_U32 uId,                   // [in] 抽象ID,由TDR_GenerateAbstract生成
		T_U8  ucMode,                // [in] RSS_MODE_NEW  RSS_MODE_INC 
		T_U32 uPos                   // [in] 订阅位置 UI_RSS_POSSTART UI_RSS_POSCUR
		);
	TDRAPI(T_I32) TDR_UnSubscribeByAbstractID(
		T_HANDLE    h,               // [in] TDR_Create返回的句柄
		T_U32 uId                    // [in] 抽象ID
		);
	TDRAPI(T_I32) TDR_SubscribeByAbstractIDS(
		T_HANDLE    h,               // [in] TDR_Create返回的句柄
		T_U32 uId[],                   // [in] 抽象ID,由TDR_GenerateAbstract生成
		T_U8  ucMode[],                // [in] RSS_MODE_NEW  RSS_MODE_INC 
		T_U32 uPos[],                   // [in] 订阅位置 UI_RSS_POSSTART UI_RSS_POSCUR
		T_U8  ucErrorCode[],           // [in] 错误码
		T_I32 nSize
		);


	TDRAPI(T_I32) TDR_UnSubscribeByAbstractIDS(
		T_HANDLE    h,               // [in] TDR_Create返回的句柄
		T_U32 uId[],                   // [in] 抽象ID
		T_I32 nSize);

	//从本地内存批量获取各种数据,,可参考示例
	TDRAPI(T_I32) TDR_GetDataByAbstractID(
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_AbstractData pAbstractData[], // [in] 获取抽象数据,存储空间由调用者自行分配空间
		T_I32 nSize                     // [in] pAbstractData数据组中元素个数
		);
	TDRAPI(T_I32) TDR_ReqDataByAbstractID(
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_AbstractData pAbstractData[], // [in] 获取抽象数据,存储空间由调用者自行分配空间
		T_I32 nSize                     // [in] pAbstractData数据组中元素个数
		);
	TDRAPI(T_I32) TDR_ReqHistoryData(
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		const char* sMarket,            // [in] 市场代码
		const char* sCode,              // [in] 证券代码或合约代码
		T_I32 nServiceId,               // [in] 服务数据类型
		T_U32 uPos,                     // [in] 请求数据位置，填0表示从头开始请求
		T_HistoryData pHistoryData[],   // [in/out] 返回数据存储空间
		T_I32 nSize,                    // [in] T_HistoryData数据组大小
		T_I32* pnRecs                   //[out] 返回的实际数据大小
		);

	TDRAPI(T_I32) TDR_AddTdaUser(
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_U32 uDataID,			        // [in]交易数据抽象ID (市场ID = 0x80和 市场ID = 0)
		const char* suser,		        // [in]数据所属用户，NULL表示自己。
		const char* spass		        // [in]密码，suser != NULL时有效
		);

	TDRAPI(T_I32)  TDR_GetTdaUser(
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_U32 uDataID,			        // [in]交易数据抽象ID（市场ID = 0x80和 市场ID = 0）
		T_U32 uUserID[],		        // [out]数据所属用户，NULL表示自己。
		T_U32 uBufSize,			        // [in]密码，suser != NULL时有效
		T_U32 *puRetUsers		        // [out] 读取到的用户ID个数
		);

	TDRAPI(T_I32) TDR_DelTdaUser( 
		T_HANDLE    h,                  // [in] TDR_Create返回的句柄
		T_U32 uDataID,		            // [in] 交易数据抽象ID (市场ID = 0x80 和 市场ID = 0)
		T_U32 uDelUserID                // [in] 要删除关联的用户ID
		);
	TDRAPI(T_I32) TDR_SetProxyServer(T_HANDLE h,				// [in] UI_Create创建的句柄
		T_U16	sutype,			// [in] 代理类型
		const char* sip,		// [in] 代理服务器IP
		T_U16 suport,			// [in] 代理服务器端口
		const char *suser,		// [in] 连接代理服务器的用户名
		const char* spass);		// [in] 连接代理服务器的密码

}

#endif // TDR_H_INCLUDED
