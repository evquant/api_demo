#ifndef _T_DCE_MARKET_
#define _T_DCE_MARKET_
#include "tdef.h"
// �г���Ŷ���                                                          */

#define     DCE                 5           //������

//*****************************************************************************************
//���Ϸ�������ID������ԭϵͳ���ݣ����·���ID���ÿ���г��ֿ�����

//-----------------------------------������-----------------------------------------------
#define ID_DCE_MARKETDATA	    0x01  //�ڻ���������(Futures_MarketData)	1016


#pragma pack(push,1)
//1.1 �������ڻ�����
typedef struct t_DCE_FutursMarketData
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
}T_DCE_FutursMarketData,*PDCE_FutursMarketData;
#pragma pack(pop)
#endif //_T_DCE_MARKET_