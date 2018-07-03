#ifndef _T_SZ_MARKET_
#define _T_SZ_MARKET_
#include "tdef.h"
// 市场编号定义                                                          */

#define     SZ                  2           //深圳市场



//*****************************************************************************************
//以上服务数据ID保留与原系统兼容，以下服务ID针对每个市场分开定义

//-----------------------------------深圳市场--------------------------------------
#define ID_SZ_INDEXDATA		          0x00  //指数(Stock_IndexData)
#define ID_SZ_TRANSACTION		      0x01  //成交(Stock_TransactionEx)
#define ID_SZ_ORDERQUEUE		      0x02  //委托队列(Stock_OrderQueue_Head+Stock_OrderQueue)
#define ID_SZ_STEPORDER			      0x03  //逐笔委托(Stock_StepOrder)
#define ID_SZ_MARKETDATA		      0x04  //行情数据(Stock_MarketData)
#define ID_SZ_MARKETDATA_L1	          0x06  //用于L1行情 深圳(Stock_MarketData_L1)
#define ID_SZ_KLINEDATA               0x07  //深交所个股分钟K线数据(T_SZ_Kline)

#pragma pack(push,1)
// 1.1 股票行情
typedef struct t_SZ_StockMarketDataL2
{
	T_I32		nTime;				     //时间(HHMMSSmmmm)
	T_I32		nStatus;			     //状态
	T_U32       uPreClose;			     //前收盘价
	T_U32       uOpen;				     //开盘价
	T_U32       uHigh;				     //最高价
	T_U32       uLow;				     //最低价
	T_U32       uMatch;				     //最新价
	T_U32       uAskPrice[10];	         //申卖价
	T_U32       uAskVol[10];		     //申卖量
	T_U32       uBidPrice[10];		     //申买价
	T_U32       uBidVol[10];		     //申买量
	T_U32       uNumTrades;			     //成交笔数
	T_I64	    iVolume;				 //成交总量
	T_I64	    iTurnover;				 //成交总金额
	T_I64	    iTotalBidVol;			 //委托买入总量
	T_I64	    iTotalAskVol;			 //委托卖出总量
	T_U32       uWeightedAvgBidPrice;	 //加权平均委买价格
	T_U32       uWeightedAvgAskPrice;    //加权平均委卖价格
	T_I32		nIOPV;					 //IOPV净值估值
	T_I32		nYieldToMaturity;		 //到期收益率
	T_U32       uHighLimited;			 //涨停价
	T_U32       uLowLimited;			 //跌停价
	char	    sPrefix[4];				 //证券信息前缀
	T_I32		nSyl1;					 //市盈率1 2 位小数 股票：价格/上年每股利润 债券：每百元应计利息
	T_I32		nSyl2;					 //市盈率2 2 位小数 股票：价格/本年每股利润 债券：到期收益率 基金：每百份的IOPV 或净值 权证：溢价率
	T_I32		nSD2;					 //升跌2（对比上一笔）
}T_SZ_StockMarketDataL2,*PSZ_StockMarketDataL2;

//1.2 dbf行情，上交所与深交所均用此结构
typedef struct t_SZ_StockMarketDataL1
{
	T_I32		nTime;				     //时间(HHMMSSmmmm)
	T_I32		nStatus;			     //状态
	T_U32       uPreClose;			     //前收盘价
	T_U32       uOpen;				     //开盘价
	T_U32       uHigh;				     //最高价
	T_U32       uLow;				     //最低价
	T_U32       uMatch;				     //最新价
	T_U32       uAskPrice[5];	         //申卖价
	T_U32       uAskVol[5];				 //申卖量
	T_U32       uBidPrice[5];		     //申买价
	T_U32       uBidVol[5];				 //申买量
	T_U32       uNumTrades;			     //成交笔数
	T_I64	    iVolume;				 //成交总量
	T_I64	    iTurnover;				 //成交总金额
	T_U32       uHighLimited;			 //涨停价
	T_U32       uLowLimited;			 //跌停价
}T_SZ_StockMarketDataL1,*PSZ_StockMarketDataL1;

//1.3 深交所逐笔成交
typedef struct t_SZ_StockStepTrade
{
	T_U32		uSetno;             //证券集代码 
	T_U32		uRecno;             //成交索引 指定证券集上唯一记录标识 从1 开始计数
	T_U32		uBuyOrderRecNo;     //买方委托索引 从1 开始计数，0 表示无对应委托
	T_U32		uSellOrderRecNo;    //卖方委托索引 从1 开始计数，0 表示无对应委托
	T_U32		uPrice;             //成交价格 扩大10000倍
	T_U32		uTradeQty;          //成交数量
	char		cOrderKind;         //成交类别
	char		sResv1[3];          //保留字段1
	char		cFunctionCode;      //成交代码
	char		sResv2[3];          //保留字段2
	T_I32		nTradeTime;         //成交时间

}Stock_Transaction_SZ,T_SZ_StockStepTrade,*PSZ_StockStepTrade; //深交所逐笔成交，对应深交所UA202

//1.4 逐笔委托(Order) 仅深交所所有
typedef struct t_SZ_StockStepOrder
{
	T_U32		uSetno;             //证券集代码 
	T_U32		uRecno;             //成交索引 指定证券集上唯一记录标识 从1 开始计数
	T_U32 	    uPrice;		        //委托编号
	T_U32		uOrderQty;	        //委托价格
	char		cOrderKind;         //成交类别
	char		sResv1[3];          //保留字段1
	char		cFunctionCode;      //成交代码
	char		sResv2[3];          //保留字段2
	T_I32       nOrderEntryTime;    //委托时间
}Stock_StepOrder_SZ,T_SZ_StockStepOrder,*PSZ_StockStepOrder; //深交所逐笔委托 对应UA201




//1.5 订单队列(Queue)
#ifndef _ORDER_QUEUE_
#define _ORDER_QUEUE_


typedef struct t_OrderQueueHead
{
	T_I32 nItem;                //数据个数
}T_OrderQueueHead,*POrderQueueHead;
typedef struct t_OrderQueueItem
{
	T_I32 	   nTime;			//订单时间(HHMMSSmmmm)
	T_I32      nSide;			//买卖方向('B':Bid 'S':Ask)
	T_I32	   nPrice;			//成交价格
	T_I32 	   nOrders;		    //订单数量
	T_I32 	   nABItems;		//明细个数
	T_I32 	   nABVolume[200];	//订单明细
}T_OrderQueueItem,*POrderQueueItem;

#endif //#define _ORDER_QUEUE_
typedef struct t_SZ_StockOrderQueue
{
	T_OrderQueueHead  tHead;
	T_OrderQueueItem  tItem[0];

}T_SZ_StockOrderQueue,*PSZ_StockOrderQueue;


//1.6 指数
typedef struct t_SZ_StockIndex
{
	T_I32     nTime;			//时间(HHMMSSmmmm)
	T_I32	  nOpenIndex;		//今开盘指数
	T_I32 	  nHighIndex;		//最高指数
	T_I32 	  nLowIndex;		//最低指数
	T_I32 	  nLastIndex;		//最新指数
	T_I64	  iTotalVolume;	    //参与计算相应指数的交易数量
	T_I64	  iTurnover;		//参与计算相应指数的成交金额
	T_I32	  nPreCloseIndex;	//前盘指数
}T_SZ_StockIndex,*PSZ_StockIndex;

//1.7 分钟K线
typedef struct t_SZ_Kline
{
	T_U32   uDay;			    // 日期				YYYYMMDD
	T_I32	nTime;			    // 时间(北京时间)	HHMM
	T_I32   nPreClose;                  // 前收盘价         单位：1/100分
	T_I32	nValOpen;		// 开盘价			单位：1/100分,比如1表示0.0001元
	T_I32	nValHigh;		// 最高价			单位：1/100分
	T_I32	nValLow;		// 最低价			单位：1/100分
	T_I32	nValClose;		// 收盘价			单位：1/100分
	T_I64	i64Volume;		// 分钟内成交量		单位：该证券的最小交易单位，比如股票为“股”
	T_I64	i64ValTotal;	         // 分钟内成交额		单位：元
	T_I64	i64TotalVol;		// 累计成交量		单位：该证券的最小交易单位，比如股票为“股”
	T_I64	i64TotalTurnOver;	// 累计成交金额		单位：元
	T_I32	nTurover;		// 换手(百分数)		单位：1/10000，比如1表示0.01%
	T_I32	nValIncrease;	// 涨跌值			单位：1/100分
}T_SZ_Kline,*PSZ_Kline; // sizeof() = 48
#pragma pack(pop)
#endif //_T_SZ_MARKET_