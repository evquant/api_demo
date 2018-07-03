#ifndef _T_HKMARKET_H_ 
#define _T_HKMARKET_H_
#include "tdef.h"
#define HK     9 //香港市场

#define ID_HK_ORGMSG				1	// 原始OMD消息数据，需要开启内存存储，每个证券至少2个页面
#define ID_HK_EXMSG				    2	// 扩展消息，目前只有 消息号为90 的消息(10档行情),如果大于一个，也需要开启内存存储
#define ID_HK_CUR					3	// 汇率 对应omd 14
#define ID_HK_INDEX				    4	// 指数 对应omd 71
#define ID_HK_BASEINFO			    5	// 基础信息 对应omd 11
#define ID_HK_COUNT				    6   // 统计  对应 omd 60
#define ID_HK_CLOSEPRICE			7   // 收盘价 对应 omd 62

#pragma pack(push,1)

typedef struct t_omdpacket		// 3.3 PACKET HEADER 报文头--------------------------------
{
	T_U16 usPkgSize;			//报文长度，含本报文头
	T_U8  ucMsgCout;			//消息个数
	T_U8  ucFiller1;			//填充 
	T_U32 uSeqNum;				//第一个消息的序号
	T_U64 ulSendTime;			//发送时间，自1970-1-1 0：0：0 GMT时标，单位纳秒
	T_U8  pdata[];
}OMD_PACKET,*POMDPACKET; // sizeof() = 16

typedef struct t_omdmsg			// OMD消息通用格式
{
	T_U16	usMsgSize;			//消息长度
	T_U16	usMsgType;			//消息类型
	T_U8	pdata[];
}OMD_MSG,*POMDMSG;   // sizeof() = 4

typedef struct t_securitydefine			// 3.7.2 Security Definition (11) 证券基础信息-----------------
{
	T_U16	usMsgSize;				//消息长度
	T_U16	usMsgType;				//消息类型(11)
	T_U32	uSecurityCode;			//证券代码,1-99999的5位十进制
	char	sMarketCode[4];			//市场代码,MAIN,GEM,NASD,ETS,注意末尾可能没有0字符
	char	sISINCode[12];			//ISIN代码,证券国际代码.
	char	sInstrumentType[4];		// BOND(债券),BWRT(篮子权证),EQTY(股票),TRST(信托),WRNT(权证)
	char	sSpreadTableCode[2];	// '01' PArtA , '02' PartB
	char	sSecurityShortName[40];	// 证券短名
	char	sCurrencyCode[3];		// 货币代码,HKD,USD,EUR,JPY,GBP,CAD,SGD,CNY
	T_U8	usGCCSName[60];			// 香港中文繁体UnicodeUTF-16LE编码
	T_U8	usGBName[60];			// GB简体UnicodeUTF-16LE编码
	T_U32	uLotSize;				// 买卖单位，Board lot size for the security
	T_I32	iPrelosePrice;			// 前收市价
	char	cFiller1;				// res1
	char	cShortSellFlag;			// Y Short-sell allowed,N Short-sell not allowed
	char	cFiller2;				// res2
	char	cCCASSFlag;				// Y CCASS security,N Non CCASS security
	char	cDummySecurityFlag;		// Y Dummy security,N Normal security
	char	cTestSecurityFlag;		// Y Test security,N Normal security
	char	cStampDutyFlag;			// Y Stamp duty required,N Stamp duty not required
	char	cFiller3;				// res3
	T_U32	uListingDate;			// 上市日期 YYYYMMDD,19000101 表示未知
	T_U32	uDelistingDate;			// 退市日期 YYYYMMDD,0 表示未知
	char	sFreeText[38];			// 固定长度的FreeText,如果没有，填写空格
	char	cEFNFlag;				// Y EFN,N Non-EFN
	T_U32	uAccruedInterest;		// Accrued Interest,约定3位小数
	T_U32	uCouponRate;			// Coupon Rate,约定3位小数
	T_U32	uConversionRatio;		// Conversion Ratio,约定3位小数
	T_I32	iStrikePrice;			// Strike Price行权价，约定3位小数
	T_U32	uMaturityDate;			// 到期日，YYYYMMDD
	char	cCallPutFlag;			// Derivative Warrants/Basket: C Call,P Put
	// ELI & CBBC: C Bull,P Bear / Rang
	char	cStyle;					// Style of the basket warrant:A American style,E European style,<blank> Other
	T_U16	usNoUnderlyingSecurities;// 0 to 20 for Basket Warrants;0 to 1 for Warrants and Structured Product
	struct t_uls
	{
		T_U32 uUnderlyingSecurityCode; // 5-digit code identifying the underlying security.
		T_U32 uUnderlyingSecurityWeight;//The weight of the underlying security code.
	} uls[];
}OMDMSG_SECURITYDEFINE; // sizeof() = 280 + 8*nu   (nu = usNoUnderlyingSecurities)

typedef struct	t_currencyrate			// 3.7.4 Currency Rate (14) 汇率---------------------------
{
	T_U16	usMsgSize;			//消息长度
	T_U16	usMsgType;			//消息类型(14)
	char	sCurrencyCode[3];	//货币代码,HKD,USD,EUR,JPY,GBP,CAD,SGD,CNY
	char	cFiller;			
	T_U16	usCurrencyFactor;	// 非0表示价格乘以10 n次方
	char	sFiller[2];
	T_U32	uCurrencyRate;		// HKD表示的外币单位，约定4位小数
}OMDMSG_CURRENCERATE;	// sizeof() = 16

typedef struct t_statistics				// 3.11.1 Statistics (60) 统计-----------------------------
{
	T_U16	usMsgSize;			//消息长度
	T_U16	usMsgType;			//消息类型(60)
	T_U32	uSecurityCode;		//证券代码,1-99999的5位十进制
	T_U64	ulSharesTraded;		//交易量(股)
	T_I64	lTurnover;			//交易额, 约定3位小数
	T_I32	iHighPrice;			//最高价，约定3位小数
	T_I32	iLowPrice;			//最低价，约定3位小数
	T_I32	iLastPrice;			//最后价，约定3位小数
	T_I32	iVWAP;				//Volume-Weighted Average Price成交量加权平均价格，约定3位小数
	T_U32	uShortSellShares;	//Number of short-sell shares卖空股数
	T_I64	lShortSellTurnover;	//short-sell turnover卖空成交量(金额)，约定3位小数
}OMDMSG_STATISTICS;	// sizeof() = 52

typedef struct t_closeprice				// 3.10.4 Closing Price (62) 收市价------------------------
{
	T_U16	usMsgSize;			//消息长度
	T_U16	usMsgType;			//消息类型(62)
	T_U32	uSecurityCode;		//证券代码,1-99999的5位十进制
	T_I32	iPrice;				//收市价格，约定3位小数
	T_U32	uNumberOfTrades;	//交易数,Total Number of Trades performed on the given instrument
}OMDMSG_CLOSEPRICE;	//sizeof() = 16

typedef struct t_indexdata				// 3.13.2 Index Data (71) 指数数据-------------------------
{
	T_U16	usMsgSize;			//消息长度
	T_U16	usMsgType;			//消息类型(71)
	char	sIndexCode[11];		//Upstream source’s index code
	char	cIndexStatus;		//Index status.
	//C Closing value
	//I Indicative
	//O Opening index
	//P Last close value (prev. ses.)
	//R Preliminary close
	//S Stop loss index
	//T Real-time index value
	T_I64	lIndexTime;			//自1970-1-1 0：0：0 GMT时标，单位纳秒	
	T_I64	lIndexValue;		//4位小数，Current value of the index
	T_I64	lNetChgPrevDay;		//4位小数，Net change in value from previous day’s closing value versus last index value 
	T_I64	lHighValue;			//4位小数
	T_I64	lLowValue;			//4位小数
	T_I64	lEASValue;			//2位小数,Estimated Average Settlement Value
	T_I64	lIndexTurnover;		//4位小数
	T_I64	lOpeningValue;		//4位小数
	T_I64	lClosingValue;		//4位小数
	T_I64	lPreviousSesClose;	//4位小数,Previous session closing value
	T_I64	lIndexVolume;		//4位小数,Index volume of underlying constituents.,Only applicable for CSI.
	T_I32	iNetChgPrevDayPct;	//4位小数,Net change in percentage from previous day’s closing value versus last value
	char	cException;			//Exception indicator
	//# Index with HSIL defined exceptional rule applied
	//' ' Normal index (empty string)
	char	sFiller[3];
}OMDMSG_INDEXDATA;	//sizeof() = 112

// MsgType = 90 , 根据OMD 53消息还原的10档行情
#define OMD_LEVELS	10
typedef struct t_omdmsgex_level10
{
	T_U16 usMsgSize;			//消息长度
	T_U16 usMsgType;			//消息类型(90) 扩展消息
	T_U32 uSecurityCode;		//证券代码,1-99999的5位十进制

	T_I64 ltime;				// 时间OMD中定义的时间,自1970-1-1 0：0：0 GMT时标，单位纳秒,0表示数据无效
	T_I32 iPrelosePrice;		// 前一天收盘价，来自11
	T_I32 iPrice;				//按盘价格，约定4位小数 ,来自40

	T_U64 ulSharesTraded;		//交易量(股) ,随后的6个来自60
	T_I64 lTurnover;			//交易额,约定4位小数
	T_I32 iHighPrice;			//最高价，约定4位小数
	T_I32 iLowPrice;			//最低价，约定4位小数
	T_I32 iLastPrice;			//最后价，约定4位小数
	T_I32 iVWAP;				//Volume-Weighted Average Price成交量加权平均价格，约定3位小数

	//以下按照53撮合
	T_I32 iPrice_b[OMD_LEVELS];
	T_U64 uQuantity_b[OMD_LEVELS];

	T_I32 iPrice_s[OMD_LEVELS];	
	T_U64 uQuantity_s[OMD_LEVELS];
}OMDMSGEX_LEVEL10,*POMDMSGEX_LEVEL10; // sizeof() = 256

#pragma pack(pop)
#endif //_T_HKMARKET_H_