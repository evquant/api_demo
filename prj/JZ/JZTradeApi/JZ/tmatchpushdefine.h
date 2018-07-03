
#ifndef _MATCH_PUSH_INFO_INCLUADE_
#define _MATCH_PUSH_INFO_INCLUADE_

#include "tdef.h"
#define ID_MATCH_ABSTRACT                6  //成交回报抽象ID,客户订阅此ID信息可获取成交回报，需要授权

#define MATCH_STOCK_KIND        '1'  // '1' 集中交易系统成交回报
#define MATCH_FUTURES_KIND      '2'  // 2 期货成交回报 
#pragma pack(push,1)
//成交回报数据类型 ,所有成交回报均包含此头结构
typedef struct t_Matchinfokind
{
	T_U8 ucMatchkink;  // 1 证券成交回报, 2 期货成交回报 
	T_U8 ucRevs;       // 保留字段1
}T_MatchInfoKind,*T_PMatchInfoKind;

typedef struct t_Stock_Match
{
	T_I32				nServerid;			// 机器编码
	T_I32				nMatchsno;			// 成交流水号
	T_I32				nOperdate;			// 发生日期
	T_I32				nTrddate;			// 交易日期
	T_I64               iCustid;			// 客户代码
	T_I64		        iFundid;			// 资产帐户
	char				cMoneytype;			// 货币代码
	char				sOrgid[5];			// 分支机构
	char			    sBrhid[5];			// 机构分支
	char     			sSecuid[11];		// 股东代码
	char    			sRptsecuid[11];		// 报盘股东代码
	char				sBSflag[3];			// 买卖方向
	char				sRptbs[3];			// 申报填类别
	char				cMatchtype;			// 成交类型 '0'普通成交 '1'撤单成交 '2'废单 '3'内部撤单 '4'撤单废单
	T_I32				nOrdersno;			// 申报编号
	char    			sOrderid[11];		// 合同序号
	char				cMarket;			// 交易市场
	char			    sStkcode[9];		// 证券代码
	char    		    sStkname[9];		// 证券名称
	char				cStktype;			// 证券类别
	T_F64				dOrderprice;		// 委托价格
	T_I32				nOrderqty;			// 委托数量
	char    			sSeat[7];			// 席位代码
	T_I32				nMatchtime;			// 成交时间
	T_F64				dMatchprice;		// 成交价格
	T_I32				nMatchqty;			// 成交数量
	T_F64				dMatchamt;			// 成交金额
	T_I32				nMatchcode;			// 成交编号
	char				cOperway;			// 操作方式
	char    			sRemark[33];		// 备注信息

}T_Stock_Match,*PStock_Match;

#pragma pack(pop)

#endif //_MATCH_PUSH_INFO_INCLUADE_