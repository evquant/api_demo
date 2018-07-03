#include "tdef.h"
//////////////////////////////////////////////////////////////////////////
//深交所期权行情
//////////////////////////////////////////////////////////////////////////

#define  SZOP        11                 //深交所期权

#define ID_SZOP_BASEINFO      1     //期权基础信息
#define ID_SZOP_MARKETDATA    2     //期权快照行情
#pragma pack(push,1)
//////////////////////////////////////////////////////////////////////////
//深交所所有证券代码共有  securities_YYYYMMDD.xml
typedef struct t_BaseInfo
{
	char    sSecurityID[8];         //证券代码 SecurityID C8
	char    sSymbol[40];            //证券简称 C40
	char    sEnglishName[40];       //英文简称 对于期权填写合约代码 C40
	char    sISIN[12];              //ISIN代码 C12
	char    sSecurityIDSource[4];   //证券代码源 4=ISIN 102=深圳证券交易所 C4
	char    sUnderlyingSecurityID[8];//基础证券代码 C8
	T_U32   uListDate;              //上市日期 N8
	T_U16   usSecurityType;         //证券类别 N4
    char    sCurrency[4];            //货币代码 C4
	T_I64   i64QtyUnit;             //数量单位 委托数量字段必须为该证券数量单位的整数倍 N15(2) 此处整型处理，放大10000倍
	char    cDayTrading;              //是否支持当日回转交易 Y=支持 N=不支持 C1
	T_I64   i64PrevClosePx;         //昨日收盘价 N13(4)
	char    sSecurityStatus[20];    //
	T_I64   i64OutstandingShare;    //总发行量  N18(2)
	T_I64   i64PublicFloatShareQuantity; //流通股数 N18(2)
	T_I64   i64ParValue;//面值 N13(4)
	char    cGageFlag;//是否可作业融资融券可充抵保证金证券 C1  Y=是 N=否
	T_I32   nGageRatio;//可充抵保证金折算率 N5(2)
	char    cCrdBuyUnderlying;//是否为融资标的 C1 Y=是 N=否
	char    cCrdSellUnderlying;//是否为融券标的C1 Y=是 N=否
	char    cPledgeFlag;//是否可质押入库 C1 Y=是 N=否
	T_I32   nContractMultiplier;//对回购标准券折算率 N6(5)
	char    sRegularShare[8];//对应回购标准券 C8

}T_BASEINFO,*PBASEINFO;

//////////////////////////////////////////////////////////////////////////
//期权特有字段   securities_YYYYMMDD.xml
typedef struct t_OptionParams
{
	char          cCallOrPut;  //认购或认沽 C1 C=Call P=Put
	T_I32         nDeliveryDay; //交割日期  N8
	char          cDeliveryType; //交割方式 C1  S=证券结算 C=现金结算
	T_I32         nExerciseBeginDate;//行权起始日期 N8
	T_I32         nExerciseEndDate;//行权结束日期 N8
	T_I64         i64ExercisePrice;//行权价 N13(4)
	char          cExerciseType;//行权方式 C1 A=美式 E=欧式 B=百慕大式
	T_I32         nLastTradeDay;//最后交易日 N8
	//char          cAdjusted;//是否调整 C1 Y=是 N=否
	T_U16         usAdjustTimes;//调整次数 N2
	T_I64         i64ContractUnit;//合约单位 N15(2)
	T_I64         i64PrevClearingPrice;//昨日结算价 N13(4)
	T_I64         i64ContractPosition;//合约持仓量  N18(2)
}T_OPTIONPARAMS,*POPTIONPARAMS;

//衍生品参考信息，目前仅包含期权  derivativeauctionparams_YYYYMMDD.xml

typedef struct t_DerivativeParams
{
	char    sSecurityID[8];  //证券代码 C8
	T_I64   i64BuyQtyUpperLimit;//买委托数量上限 N15(2)
	T_I64   i64SellQtyUpperLimit;//卖委托数量上限 N15(2)
	T_I64   i64BuyQtyUnit;      //买数量单位 N15(2)
	T_I64   i64SellQtyUnit;     //卖数量单位 N15(2)
	T_I64   i64PriceTick;        //价格档位 N13(4)
	T_I64   i64PriceUpperLimit;        //涨停价 N13(4)
	T_I64   i64PriceLowerLimit;        //跌停价 N13(4)
	T_I64   i64LastSellMargin;   //昨志开每张保证金 N18(4)
	T_I64   i64SellMargin;       //今卖开每张保证金 N18(4)
	T_I32   nMarginRatioParam1; //保证金比例计算参数一 N4(2)
	T_I32   nMarginRatioParam2; //保证金比例计算参数一 N4(2)
	char    cMarketMakerFlag;    //做市商标志  C1 Y=是 N=否
}T_DERIVATIVEPARAMS,*PDERIVATIVEPARAMS;


typedef struct t_SZOP_BaseInfo
{
	T_BASEINFO          tBase;      //基础信息
	T_OPTIONPARAMS      tOpParams;  //期权特有
	T_DERIVATIVEPARAMS  tDeParmas;  //衍生品信息

}T_SZOP_BASEINFO,*PSZOP_BASEINFO;  //ID_SZOP_BASEINFO

typedef struct t_SZOP_MarketData
{
	T_I32       nTime; //数据生成时间 HHMMSSmmm格式
	T_U16       usChannelNo; //频道代码
	char        sMDStreamID[3] ;//行情类别
	char        sSecrityID[8];//证券代码
	char        sSecurityIDSource[4];//证券代码源 101=深圳证券交易所
	char        sTradingPhaseCode[8];//产品所处的交易阶段代码 第0位：S=启动（开市前） O=开盘集合竞价 T=连续竞价 B=休市 C=收盘集合竞价 E=已闭市 H=临时停牌 A=盘后交易 第1位：0=正常状态 1=全天停牌
	T_I64       i64PrevClosePx;//昨收价
	T_I64       i64NumTrades; //成交笔数
	T_I64       i64TotalVolumeTrade;//成交总量
	T_I64       i64TotalValueTrade;//成交总金额

	T_I64       i64LastPrice;//最近价
	T_I64       i64OpenPrice;//开盘价
	T_I64       i64HighPrice;//最高价
	T_I64       i64LowPrice;//最低价
	T_I64       i64BuyAvgPrice;//x3=买入汇总（总量及加权平均价）
	T_I64       i64BuyVolumeTrade;//x3=买入汇总（总量及加权平均价）
	T_I64       i64SellAvgPrice;//x4=卖出汇总（总量及加权平均价）
	T_I64       i64SellVolumeTrade;//x4=卖出汇总（总量及加权平均价）
	T_I64       i64OfferPrice[10];//卖委托价格
	T_I64       i64OfferQty[10]; //卖委托量
	T_I64       i64BidPrice[10];//买委托价格
	T_I64       i64BidQty[10];//买委托量
	T_I64       i64PriceUpperLimit;        //涨停价
	T_I64       i64PriceLowerLimit;        //跌停价
	T_I64       i64ContractPosition; //合约持仓量


	//行情条目类别 0=买入 1=卖出 2=最近价 4=开盘价 7=最高价 8=最低价
	//x1= 升跌一 x2=升跌二 x3=买入汇总（总量及加权平均价）x4=卖出汇总（总量及加权平均价）
	//x5=股票市盈率一 x6=股票市盈率二 x7=基金T-1日净值 x8=基金实时参考净值(包括ETF的IOPV)
	//x9=权证溢价率 xe=涨停价 xf=跌停价 xg=合约持仓量
}T_SZOP_MARKETDATA,*PSZOP_MARKETDATA;

#pragma pack(pop)