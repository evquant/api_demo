#ifndef _T_SZ_MARKET_
#define _T_SZ_MARKET_
#include "tdef.h"
// �г���Ŷ���                                                          */

#define     SZ                  2           //�����г�



//*****************************************************************************************
//���Ϸ�������ID������ԭϵͳ���ݣ����·���ID���ÿ���г��ֿ�����

//-----------------------------------�����г�--------------------------------------
#define ID_SZ_INDEXDATA		          0x00  //ָ��(Stock_IndexData)
#define ID_SZ_TRANSACTION		      0x01  //�ɽ�(Stock_TransactionEx)
#define ID_SZ_ORDERQUEUE		      0x02  //ί�ж���(Stock_OrderQueue_Head+Stock_OrderQueue)
#define ID_SZ_STEPORDER			      0x03  //���ί��(Stock_StepOrder)
#define ID_SZ_MARKETDATA		      0x04  //��������(Stock_MarketData)
#define ID_SZ_MARKETDATA_L1	          0x06  //����L1���� ����(Stock_MarketData_L1)
#define ID_SZ_KLINEDATA               0x07  //������ɷ���K������(T_SZ_Kline)

#pragma pack(push,1)
// 1.1 ��Ʊ����
typedef struct t_SZ_StockMarketDataL2
{
	T_I32		nTime;				     //ʱ��(HHMMSSmmmm)
	T_I32		nStatus;			     //״̬
	T_U32       uPreClose;			     //ǰ���̼�
	T_U32       uOpen;				     //���̼�
	T_U32       uHigh;				     //��߼�
	T_U32       uLow;				     //��ͼ�
	T_U32       uMatch;				     //���¼�
	T_U32       uAskPrice[10];	         //������
	T_U32       uAskVol[10];		     //������
	T_U32       uBidPrice[10];		     //�����
	T_U32       uBidVol[10];		     //������
	T_U32       uNumTrades;			     //�ɽ�����
	T_I64	    iVolume;				 //�ɽ�����
	T_I64	    iTurnover;				 //�ɽ��ܽ��
	T_I64	    iTotalBidVol;			 //ί����������
	T_I64	    iTotalAskVol;			 //ί����������
	T_U32       uWeightedAvgBidPrice;	 //��Ȩƽ��ί��۸�
	T_U32       uWeightedAvgAskPrice;    //��Ȩƽ��ί���۸�
	T_I32		nIOPV;					 //IOPV��ֵ��ֵ
	T_I32		nYieldToMaturity;		 //����������
	T_U32       uHighLimited;			 //��ͣ��
	T_U32       uLowLimited;			 //��ͣ��
	char	    sPrefix[4];				 //֤ȯ��Ϣǰ׺
	T_I32		nSyl1;					 //��ӯ��1 2 λС�� ��Ʊ���۸�/����ÿ������ ծȯ��ÿ��ԪӦ����Ϣ
	T_I32		nSyl2;					 //��ӯ��2 2 λС�� ��Ʊ���۸�/����ÿ������ ծȯ������������ ����ÿ�ٷݵ�IOPV ��ֵ Ȩ֤�������
	T_I32		nSD2;					 //����2���Ա���һ�ʣ�
}T_SZ_StockMarketDataL2,*PSZ_StockMarketDataL2;

//1.2 dbf���飬�Ͻ�����������ô˽ṹ
typedef struct t_SZ_StockMarketDataL1
{
	T_I32		nTime;				     //ʱ��(HHMMSSmmmm)
	T_I32		nStatus;			     //״̬
	T_U32       uPreClose;			     //ǰ���̼�
	T_U32       uOpen;				     //���̼�
	T_U32       uHigh;				     //��߼�
	T_U32       uLow;				     //��ͼ�
	T_U32       uMatch;				     //���¼�
	T_U32       uAskPrice[5];	         //������
	T_U32       uAskVol[5];				 //������
	T_U32       uBidPrice[5];		     //�����
	T_U32       uBidVol[5];				 //������
	T_U32       uNumTrades;			     //�ɽ�����
	T_I64	    iVolume;				 //�ɽ�����
	T_I64	    iTurnover;				 //�ɽ��ܽ��
	T_U32       uHighLimited;			 //��ͣ��
	T_U32       uLowLimited;			 //��ͣ��
}T_SZ_StockMarketDataL1,*PSZ_StockMarketDataL1;

//1.3 �����ʳɽ�
typedef struct t_SZ_StockStepTrade
{
	T_U32		uSetno;             //֤ȯ������ 
	T_U32		uRecno;             //�ɽ����� ָ��֤ȯ����Ψһ��¼��ʶ ��1 ��ʼ����
	T_U32		uBuyOrderRecNo;     //��ί������ ��1 ��ʼ������0 ��ʾ�޶�Ӧί��
	T_U32		uSellOrderRecNo;    //����ί������ ��1 ��ʼ������0 ��ʾ�޶�Ӧί��
	T_U32		uPrice;             //�ɽ��۸� ����10000��
	T_U32		uTradeQty;          //�ɽ�����
	char		cOrderKind;         //�ɽ����
	char		sResv1[3];          //�����ֶ�1
	char		cFunctionCode;      //�ɽ�����
	char		sResv2[3];          //�����ֶ�2
	T_I32		nTradeTime;         //�ɽ�ʱ��

}Stock_Transaction_SZ,T_SZ_StockStepTrade,*PSZ_StockStepTrade; //�����ʳɽ�����Ӧ���UA202

//1.4 ���ί��(Order) ���������
typedef struct t_SZ_StockStepOrder
{
	T_U32		uSetno;             //֤ȯ������ 
	T_U32		uRecno;             //�ɽ����� ָ��֤ȯ����Ψһ��¼��ʶ ��1 ��ʼ����
	T_U32 	    uPrice;		        //ί�б��
	T_U32		uOrderQty;	        //ί�м۸�
	char		cOrderKind;         //�ɽ����
	char		sResv1[3];          //�����ֶ�1
	char		cFunctionCode;      //�ɽ�����
	char		sResv2[3];          //�����ֶ�2
	T_I32       nOrderEntryTime;    //ί��ʱ��
}Stock_StepOrder_SZ,T_SZ_StockStepOrder,*PSZ_StockStepOrder; //������ί�� ��ӦUA201




//1.5 ��������(Queue)
#ifndef _ORDER_QUEUE_
#define _ORDER_QUEUE_


typedef struct t_OrderQueueHead
{
	T_I32 nItem;                //���ݸ���
}T_OrderQueueHead,*POrderQueueHead;
typedef struct t_OrderQueueItem
{
	T_I32 	   nTime;			//����ʱ��(HHMMSSmmmm)
	T_I32      nSide;			//��������('B':Bid 'S':Ask)
	T_I32	   nPrice;			//�ɽ��۸�
	T_I32 	   nOrders;		    //��������
	T_I32 	   nABItems;		//��ϸ����
	T_I32 	   nABVolume[200];	//������ϸ
}T_OrderQueueItem,*POrderQueueItem;

#endif //#define _ORDER_QUEUE_
typedef struct t_SZ_StockOrderQueue
{
	T_OrderQueueHead  tHead;
	T_OrderQueueItem  tItem[0];

}T_SZ_StockOrderQueue,*PSZ_StockOrderQueue;


//1.6 ָ��
typedef struct t_SZ_StockIndex
{
	T_I32     nTime;			//ʱ��(HHMMSSmmmm)
	T_I32	  nOpenIndex;		//����ָ��
	T_I32 	  nHighIndex;		//���ָ��
	T_I32 	  nLowIndex;		//���ָ��
	T_I32 	  nLastIndex;		//����ָ��
	T_I64	  iTotalVolume;	    //���������Ӧָ���Ľ�������
	T_I64	  iTurnover;		//���������Ӧָ���ĳɽ����
	T_I32	  nPreCloseIndex;	//ǰ��ָ��
}T_SZ_StockIndex,*PSZ_StockIndex;

//1.7 ����K��
typedef struct t_SZ_Kline
{
	T_U32   uDay;			    // ����				YYYYMMDD
	T_I32	nTime;			    // ʱ��(����ʱ��)	HHMM
	T_I32   nPreClose;                  // ǰ���̼�         ��λ��1/100��
	T_I32	nValOpen;		// ���̼�			��λ��1/100��,����1��ʾ0.0001Ԫ
	T_I32	nValHigh;		// ��߼�			��λ��1/100��
	T_I32	nValLow;		// ��ͼ�			��λ��1/100��
	T_I32	nValClose;		// ���̼�			��λ��1/100��
	T_I64	i64Volume;		// �����ڳɽ���		��λ����֤ȯ����С���׵�λ�������ƱΪ���ɡ�
	T_I64	i64ValTotal;	         // �����ڳɽ���		��λ��Ԫ
	T_I64	i64TotalVol;		// �ۼƳɽ���		��λ����֤ȯ����С���׵�λ�������ƱΪ���ɡ�
	T_I64	i64TotalTurnOver;	// �ۼƳɽ����		��λ��Ԫ
	T_I32	nTurover;		// ����(�ٷ���)		��λ��1/10000������1��ʾ0.01%
	T_I32	nValIncrease;	// �ǵ�ֵ			��λ��1/100��
}T_SZ_Kline,*PSZ_Kline; // sizeof() = 48
#pragma pack(pop)
#endif //_T_SZ_MARKET_