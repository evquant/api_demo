#ifndef _T_SHOPTION_H_ 
#define _T_SHOPTION_H_
#include "tdef.h"
#define SHOP     8 //�Ͻ�����Ȩ�г�

//-----------------------------------�Ͻ�����Ȩ-------------------------------------------
#define ID_SHOP_BASEINFO    0x01   //��Ȩ������Ϣ ��ӦT_SH_OptionBaseInfo
#define ID_SHOP_MARKETDATA  0x02   //��Ȩ�г����� ��ӦT_SH_OptionMarketData

#pragma pack(push,1)
//1.1 �Ͻ�����Ȩ������Ϣ
typedef struct t_SHOP_BaseInfo
{

	   char  sSecurityID[9];                                //	��Լ����	C8	��Ȩ��Լ�ĺ�Լ����
	   char  sContractID[19];                               //	��Լ���״���	C19	
	   char  sContractSymbol[21];                           //	��Ȩ��Լ���	C20	
	   char  sUnderlyingSecurityID[7];                      //	���֤ȯ����	C6	
	   char  sUnderlyingSymbol[9];                          //	����֤ȯ֤ȯ����	C8	
	   char  sUnderlyingType[4];                            //	���֤ȯ����	C3	EBS �C ETF�� ASH �C A��
	   char  cOptionType;	                                //  ŷʽ��ʽ	C1	��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
	   char  cCallOrPut;                                    //	�Ϲ��Ϲ�	C1	�Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
	   T_U32 uContractMultiplierUnit;                       //	��Լ��λ	N11	������Ȩ��Ϣ������ĺ�Լ��λ
	   T_U32 uExercisePrice;	                            //  ��Ȩ��Ȩ��	N11(4)	������Ȩ��Ϣ���������Ȩ��Ȩ�ۣ���ȷ��0.1��
	   char  sStartDate[9];                                 //	�׸�������	C8	��Ȩ�׸�������,YYYYMMDD
	   char  sEndDate[9];                                   //	�������	C8	��Ȩ�������/��Ȩ�գ�YYYYMMDD 
	   char  sExerciseDate[9];                              //	��Ȩ��Ȩ��	C8	��Ȩ��Ȩ�գ�YYYYMMDD 
	   char  sDeliveryDate[9];                              //	��Ȩ������	C8	��Ȩ�����գ�Ĭ��Ϊ��Ȩ�յ���һ�������գ�YYYYMMDD
	   char  sExpireDate[9];	                            //  ��Ȩ������	C8	��Ȩ�����գ�YYYYMMDD
	   char  cUpdateVersion;	                            //  ��Լ�汾��	C1	��Ȩ��Լ�İ汾��
	   T_I64 iTotalLongPosition;                            //	��ǰ��Լδƽ����	N12	��λ�� ���ţ�
	   T_U32 uSecurityClosePx;                              //	��Լǰ���̼�	N11(4)	�������̼ۣ��Ҷ��룬��λ��Ԫ����ȷ��0.1�壩
	   T_U32 uSettlPrice;                                   //	��Լǰ�����	N11(4)	���ս���ۣ�������Ȩ��Ϣ��Ϊ������Ľ���ۣ���Լ����������д�ο��ۣ����Ҷ��룬��λ��Ԫ����ȷ��0.1�壩
	   T_U32 uUnderlyingClosePx;                            //	���֤ȯǰ����	N11(4)	��Ȩ���֤ȯ��Ȩ��Ϣ�������ǰ���̼۸��Ҷ��룬��λ��Ԫ����ȷ��0.1�壩
	   char  cPriceLimitType;                               //	�ǵ�����������	C1	��N�����ǵ�����������
	   T_U32 uDailyPriceUpLimit;                            //	�Ƿ����޼۸�	N11(4)	������Ȩ��ͣ�۸񣬵�λ��Ԫ����ȷ��0.1�壩
	   T_U32 uDailyPriceDownLimit;                          //	�������޼۸�	N11(4)	������Ȩ��ͣ�۸񣬵�λ��Ԫ����ȷ��0.1�壩
	   T_U32 uMarginUnit;                                   //	��λ��֤��	N16(2)	���ճ���һ�ź�Լ����Ҫ�ı�֤����������ȷ����
	   T_I32 nMarginRatioParam1;                            //	��֤������������һ	N6(2)	��֤������������λ��%
	   T_I32 nMarginRatioParam2;                            //	��֤��������������	N6(2)	��֤������������λ��%
	   T_U32 uRoundLot;                                     //	������	N12	һ�ֶ�Ӧ�ĺ�Լ��
	   T_U32 uLmtOrdMinFloor;                               //	�����޼��걨����	N12	�����޼��걨���걨�������ޡ�
	   T_U32 uLmtOrdMaxFloor;                               //	�����޼��걨����	N12	�����޼��걨���걨�������ޡ�
	   T_U32 uMktOrdMinFloor;                               //	�����м��걨����	N12	�����м��걨���걨�������ޡ�
	   T_U32 uMktOrdMaxFloor;                               //	�����м��걨����	N12	�����м��걨���걨�������ޡ�
	   T_U32 uTickSize;                                     //  ��С���۵�λ	N11(4)	��λ��Ԫ����ȷ��0.1��
	   char  sSecurityStatusFlag[9];                        //	��Ȩ��Լ״̬��Ϣ��ǩ	C8	���ֶ�Ϊ8λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո�
																/*��1λ����0����ʾ�ɿ��֣���1����ʾ�����������֣���.�������ҿ��֣������뿪�֡�
																��2λ����0����ʾδ����ͣ�ƣ���1����ʾ����ͣ�ơ�
																��3λ����0����ʾδ�ٽ������գ���1����ʾ���뵽���ղ���10�������ա�
																��4λ����0����ʾ����δ����������1����ʾ���10���������ں�Լ������������
																��5λ����A����ʾ�����¹��Ƶĺ�Լ����E����ʾ�����ĺ�Լ����D����ʾ����ժ�Ƶĺ�Լ��*/
}T_SHOP_BaseInfo,*PSHOP_BaseInfo;

//1.2 �Ϻ�����Ȩ����
typedef struct t_SHOP_MarketData
{
	T_I32 nDataTimestamp;  //ʱ��� HHMMSSmmm  �����Ȩ��Լ�Ĳ�Ʒ����Ϊ��00000000�������ʾ����ʱ�䣻
	T_I64 iPreSettlPrice;  //���ս���� 4 decimal places
	T_I64 iSettlPrice;     //���ս����	4 decimal places
	T_I64 iOpenPx;         //���̼�	Today��s open price 4 decimal places �����Ȩ��Լ�Ĳ�Ʒ����Ϊ��00000000�������ʾ���̱�־��	111111��ʾ����
	T_I64 iHighPx;         //��߼�	Today��s high 4 decimal places
	T_I64 iLowPx;          //��ͼ�	Today��s low  4 decimal places
	T_I64 iLastPx;         //�ּ� Last price 4 decimal places �����Ȩ��Լ�Ĳ�Ʒ����Ϊ��00000000�������ʾ��¼��
	T_I64 iAuctionPrice;   //�������жϲο���	4 decimal places
	T_I64 iAuctionQty;     //�������жϼ��Ͼ�������ƥ����
	T_I64 iTotalLongPosition; //��ǰ��Լδƽ������
	T_I64 iBidSize[5];     // ������
	T_I64 iBidPx[5];       // �����
	T_I64 iOfferSize[5];   // ������
	T_I64 iOfferPx[5];     // ������
	T_I64 iTotalVolumeTrade; //�ɽ���	Trade volume of this security
	T_I64 iTotalValueTrade;  //�ɽ���2λС����Turnover of this security
	char  sTradingPhaseCode[5];
	   /* ���ֶ�Ϊ4 λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո�
		��1 λ����S����ʾ����������ǰ��ʱ�Σ���C����ʾ���Ͼ���ʱ�Σ���T����ʾ��������ʱ�Σ���B����ʾ����ʱ�Σ���E����ʾ����ʱ�Σ���V����ʾ�������жϣ���P����ʾ��ʱͣ�ơ�
		��2 λ����0����ʾδ����ͣ�ƣ���1����ʾ����ͣ�ơ�
		�����Ȩ��Լ�Ĳ�Ʒ����Ϊ��00000000�������ʾȫ�г�����״̬��
		���ֶ�Ϊ8 λ�ַ���������ÿλ��ʾ�ض��ĺ��壬�޶�������ո�
		��1 λ����S����ʾȫ�г������ڼ䣨����ǰ������T����ʾȫ�г����ڽ����ڼ䣨���м����У��� ��E����ʾȫ�г����ڱ����ڼ䡣
		��2 λ����1����ʾ���̼��Ͼ��۽�����־��δ����ȡ��0����
		��3 λ����1����ʾ�г�������б�־��δ����ȡ��0����*/

}T_SHOP_MarketData,*PSHOP_MarketData;

#pragma pack(pop)
#endif //_T_SHOPTION_H_

    
