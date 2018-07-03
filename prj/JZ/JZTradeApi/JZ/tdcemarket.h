#ifndef _T_DCE_MARKET_
#define _T_DCE_MARKET_
#include "tdef.h"
// 市场编号定义                                                          */

#define     DCE                 5           //大商所

//*****************************************************************************************
//以上服务数据ID保留与原系统兼容，以下服务ID针对每个市场分开定义

//-----------------------------------大商所-----------------------------------------------
#define ID_DCE_MARKETDATA	    0x01  //期货行情数据(Futures_MarketData)	1016


#pragma pack(push,1)
//1.1 大商所期货行情
typedef struct t_DCE_FutursMarketData
{
	T_I32	   nTime;					//时间(HHMMSSmmmm)
	T_I32	   nStatus;					//状态
	T_I64      iPreOpenInterest;		//昨持仓
	T_U32      uPreClose;				//昨收盘价
	T_U32      uPreSettlePrice;		    //昨结算
	T_U32      uOpen;					//开盘价
	T_U32      uHigh;					//最高价
	T_U32      uLow;					//最低价
	T_U32      uMatch;				    //最新价
	T_I64	   iVolume;					//成交总量
	T_I64	   iTurnover;				//成交总金额
	T_I64      iOpenInterest;			//持仓总量
	T_U32      uClose;				    //今收盘
	T_U32      uSettlePrice;			//今结算
	T_U32      uHighLimited;		    //涨停价
	T_U32      uLowLimited;		        //跌停价
	T_I32	   nPreDelta;			    //昨虚实度
	T_I32      nCurrDelta;              //今虚实度
	T_U32      uAskPrice[5];			//申卖价
	T_U32      uAskVol[5];			    //申卖量
	T_U32      uBidPrice[5];			//申买价
	T_U32      uBidVol[5];			    //申买量
}T_DCE_FutursMarketData,*PDCE_FutursMarketData;
#pragma pack(pop)
#endif //_T_DCE_MARKET_