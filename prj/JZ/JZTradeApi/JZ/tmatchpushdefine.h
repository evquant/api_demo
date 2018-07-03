
#ifndef _MATCH_PUSH_INFO_INCLUADE_
#define _MATCH_PUSH_INFO_INCLUADE_

#include "tdef.h"
#define ID_MATCH_ABSTRACT                6  //�ɽ��ر�����ID,�ͻ����Ĵ�ID��Ϣ�ɻ�ȡ�ɽ��ر�����Ҫ��Ȩ

#define MATCH_STOCK_KIND        '1'  // '1' ���н���ϵͳ�ɽ��ر�
#define MATCH_FUTURES_KIND      '2'  // 2 �ڻ��ɽ��ر� 
#pragma pack(push,1)
//�ɽ��ر��������� ,���гɽ��ر���������ͷ�ṹ
typedef struct t_Matchinfokind
{
	T_U8 ucMatchkink;  // 1 ֤ȯ�ɽ��ر�, 2 �ڻ��ɽ��ر� 
	T_U8 ucRevs;       // �����ֶ�1
}T_MatchInfoKind,*T_PMatchInfoKind;

typedef struct t_Stock_Match
{
	T_I32				nServerid;			// ��������
	T_I32				nMatchsno;			// �ɽ���ˮ��
	T_I32				nOperdate;			// ��������
	T_I32				nTrddate;			// ��������
	T_I64               iCustid;			// �ͻ�����
	T_I64		        iFundid;			// �ʲ��ʻ�
	char				cMoneytype;			// ���Ҵ���
	char				sOrgid[5];			// ��֧����
	char			    sBrhid[5];			// ������֧
	char     			sSecuid[11];		// �ɶ�����
	char    			sRptsecuid[11];		// ���̹ɶ�����
	char				sBSflag[3];			// ��������
	char				sRptbs[3];			// �걨�����
	char				cMatchtype;			// �ɽ����� '0'��ͨ�ɽ� '1'�����ɽ� '2'�ϵ� '3'�ڲ����� '4'�����ϵ�
	T_I32				nOrdersno;			// �걨���
	char    			sOrderid[11];		// ��ͬ���
	char				cMarket;			// �����г�
	char			    sStkcode[9];		// ֤ȯ����
	char    		    sStkname[9];		// ֤ȯ����
	char				cStktype;			// ֤ȯ���
	T_F64				dOrderprice;		// ί�м۸�
	T_I32				nOrderqty;			// ί������
	char    			sSeat[7];			// ϯλ����
	T_I32				nMatchtime;			// �ɽ�ʱ��
	T_F64				dMatchprice;		// �ɽ��۸�
	T_I32				nMatchqty;			// �ɽ�����
	T_F64				dMatchamt;			// �ɽ����
	T_I32				nMatchcode;			// �ɽ����
	char				cOperway;			// ������ʽ
	char    			sRemark[33];		// ��ע��Ϣ

}T_Stock_Match,*PStock_Match;

#pragma pack(pop)

#endif //_MATCH_PUSH_INFO_INCLUADE_