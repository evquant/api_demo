#ifndef _T_CFFEX_MARKET_
#define _T_CFFEX_MARKET_
#include "tdef.h"
// �г���Ŷ���                                                          */

#define     CFFEX               3           //�н���





//*****************************************************************************************
//���Ϸ�������ID������ԭϵͳ���ݣ����·���ID���ÿ���г��ֿ�����

//-----------------------------------�н���-----------------------------------------------
#define ID_CFFEX_MARKETDATA	    0x01  //�ڻ���������(Futures_MarketData)	1016


#pragma pack(push,1)
//1.1 �н����ڻ�����
typedef struct t_CFFEX_FutursMarketData
{
	T_I32	   nTime;					//ʱ��(HHMMSSmmmm)
	T_I32	   nStatus;					//״̬
	T_I64      iPreOpenInterest;		//��ֲ�
	T_U32      uPreClose;				//�����̼�
	T_U32      uPreSettlePrice;		    //�����
	T_U32      uOpen;					//���̼�
	T_U32      uHigh;					//��߼�
	T_U32      uLow;					//��ͼ�
	T_U32      uMatch;				    //���¼�
	T_I64	   iVolume;					//�ɽ�����
	T_I64	   iTurnover;				//�ɽ��ܽ��
	T_I64      iOpenInterest;			//�ֲ�����
	T_U32      uClose;				    //������
	T_U32      uSettlePrice;			//�����
	T_U32      uHighLimited;		    //��ͣ��
	T_U32      uLowLimited;		        //��ͣ��
	T_I32	   nPreDelta;			    //����ʵ��
	T_I32      nCurrDelta;              //����ʵ��
	T_U32      uAskPrice[5];			//������
	T_U32      uAskVol[5];			    //������
	T_U32      uBidPrice[5];			//�����
	T_U32      uBidVol[5];			    //������
}Futures_MarketData,T_CFFEX_FutursMarketData,*PCFFEX_FutursMarketData;
#pragma pack(pop)
#endif //_T_CFFEX_MARKET_