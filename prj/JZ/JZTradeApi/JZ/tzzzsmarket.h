#ifndef _T_ZZZS_MARKET_
#define _T_ZZZS_MARKET_
#include "tdef.h"
#pragma pack(push,1)

//市场定义
#define ZZZS      7    //中证指数行情市场

#define  ID_ZZZS_INDEX    0x01  //中证指数行情 对应T_ZZZS_IndexMarketData
#define  ID_ZZZS_WEIGHT   0x02  //权重信息 对应T_ZZZS_IndexWeight
#define  ID_ZZZS_ETFIOPV  0x03  //ETF的IOPV 对应T_ZZZS_EtfIopv


//1.1 指数行情信息
typedef struct t_ZZZS_IndexMarketData
{
	T_U16 nRecordType;				//记录类型，01：指数行情信息。02：指数权重信息。03：ETF的IOPV（参考净值）	对应JLLX
	T_I32 nTime;                    //时间 HHMMSSmmm
	char szStandby[5];				//备用字段，全部用空格填充		 
	char szIndexCode[7];			//指数代码	对应ZSDM
	char szIndexReferred[21];		//指数简称	对应JC
	T_U16 nMarketCode;				//市场代码 1:上证所。2:深交所。3:沪深。4:香港。5:亚太。0:全球。 对应SCDM
	T_U64 iRealtimeIndex;			//实时指数，当前指数值 对应SSZS										放大1w倍
	T_U64 iOpenValueOfToday;		//当日开盘值，初始为0.0000，当其为0.0000表示未开盘。	对应DRKP	放大1w倍
	T_U64 iMaximumOfDay;			//当日最大值，当前交易日最大指数值。	对应DRZD					放大1w倍
	T_U64 iMinimumOfDay;			//当日最小值，当前交易日最小指数值。	对应DRZX					放大1w倍
	T_U64 iCloseValueOfToday;		//当日收盘值，初始为0.0000，当其不为0.0000表示已收盘。	对应DRSP	放大1w倍
	T_U64 iCloseValueOfYesterday;	//昨日收盘值，上一交易日的收盘值。	对应ZRSP						放大1w倍
	T_I64 iRiseAndFall;				//涨跌	对应ZD														放大1w倍
	T_I64 iRiseAndFallRange;		//涨跌幅	对应ZDF													放大1w倍
	T_U64 iMatchVolume;				//成交量单位为股，如果是债券指数则单位为张。	对应CJL				
	T_U64 iMatchAmount;				//成交金额，单位为万元  对应CJJE									放大10w倍
	T_U64 iExchangeRate;			//汇率，盘中为0.00000000.收盘后为收盘时计算指数所使用的汇率	对应HL	放大1亿倍
	T_U16 nMoneyType;				//货币种类。0：人民币；1：港币；2：美 元 ；3：台币；4：日元	对应BZBZ
	T_U32 nIndexSerial;				//指数展示序号 对应ZSXH
	T_U64 iCloseValueOfToday2;		//当日收盘值2，若该指数为全球指数，该收盘值为当日亚太区收盘值。
	//初始值为0.0000。当值不为0.0000时，说明指数亚太区已收盘。对应DRSP2 放大1w倍
	T_U64 iCloseValueOfToday3;		//当日收盘值3，若该指数为全球指数，该收盘值为当日欧洲区收盘值。
	//初始值为0.0000。当值不为0.0000时，说明指数欧洲区已收盘。对应DRSP3 放大1w倍


}T_ZZZS_IndexMarketData, *PZZZS_IndexMarketData;

//1.2 指数权重信息结构体
typedef struct t_ZZ_IndexWeight
{
	T_U16 nRecordType;				//记录类型，01：指数行情信息。02：指数权重信息。03：ETF的IOPV（参考净值）	对应JLLX
	T_I32 nTime;                    //时间 HHMMSSmmm
	char szStandby[5];				//备用字段，全部用空格填充		 
	char szIndexCode[7];			//指数代码	对应ZSDM
	char szIndexName[21];			//指数名称	对应ZSMC
	char szStockCode[9];			//证券代码	对应ZQDM
	char szStockName[9];			//证券名称	对应ZQMC
	T_U64 iWeightRate;				//权重比	对应QZ											放大10w倍
	T_U64 iCurrentIndexValue;		//当前指数值 对应DQZS										放大1w倍
	T_I64 iImpactPointNumerical;	//影响点数值  该股票在当前时点对指数的贡献点数 对应YXDS		放大1w倍

}T_ZZZS_IndexWeight, *PZZZS_IndexWeight;

//1.3 ETF的IOPV净值
typedef struct t_ZZZS_EtfIopv
{
	T_U16 nRecordType;				//记录类型，01：指数行情信息。02：指数权重信息。03：ETF的IOPV（参考净值）	对应JLLX
	T_I32 nTime;                    //时间 HHMMSSmmm
	char szStandby[5];				//备用字段，全部用空格填充		 
	char szStockCode[9];			//ETF的证券代码	对应ZQDM
	char szStockName[9];			//ETF的证券名称	对应ZQMC
	T_U16 nMarketCode;				//市场代码 1:上证所。2:深交所。3:沪深。4:香港。5:亚太。0:全球。 对应SCDM
	T_I64 iIOPV;					//基金参考净值（IOPV）	对应IOPV							放大1w倍
}T_ZZZS_EtfIopv, *PZZZS_EtfIopv;

#pragma pack(pop)

#endif //_T_ZZZS_MARKET_