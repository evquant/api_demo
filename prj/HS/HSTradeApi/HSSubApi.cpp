#include "HSSubApi.h"


namespace kiiik {


void FUNCTION_CALL_MODE HSSubApi::OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo)
{
}

void FUNCTION_CALL_MODE HSSubApi::OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData)
{
	if (lpData == NULL)
	{
		cout << "��������ָ��Ϊ��" << endl;
		return;
	}

	IF2UnPacker* lpUnPack = NewUnPacker((void*)lpData,nLength);
	cout << endl;
	cout << endl;
	cout << "HSSubApi::OnReceived: SubscribeIndex(" << subscribeIndex << "), MsgType(" << lpUnPack->GetStr("msgtype") << "), DataLength(" << nLength << ")." << endl;
	ShowPacket(lpUnPack);

	if (lpRecvData == NULL)
	{
		cout << "��������ָ��Ϊ��" << endl;
	}
	else
	{
		ShowSubscribe(subscribeIndex,lpRecvData);
	}

	lpUnPack->SetCurrentDatasetByIndex(0);
	lpUnPack->First();

	string sMsgType = lpUnPack->GetStr("msgtype");  //������Ϣ����
	string sMarketNo = lpUnPack->GetStr("market_no");
	string sOperatorNo = lpUnPack->GetStr("operator_no");
	if (sMsgType == "a")
	{
		string sBusiDate = lpUnPack->GetStr("business_date");
		string sBusiTime = lpUnPack->GetStr("business_time");
		string sBatchNo = lpUnPack->GetStr("batch_no");
		string sEntrustNo = lpUnPack->GetStr("entrust_no");
		string sReportNo = lpUnPack->GetStr("report_no");
		string sAccountCode = lpUnPack->GetStr("account_code");
		string sCombiNo = lpUnPack->GetStr("combi_no");
		string sInstanceNo = lpUnPack->GetStr("instance_no");
		string sStockholderId = lpUnPack->GetStr("stockholder_id");
		string sReportSeat = lpUnPack->GetStr("report_seat");
		string sStockCode = lpUnPack->GetStr("stock_code");
		string sEntrustDirection = lpUnPack->GetStr("entrust_direction");
		string sFuturesDirection = lpUnPack->GetStr("futures_direction");
		string sPriceType = lpUnPack->GetStr("price_type");
		string sEntrustPrice = lpUnPack->GetStr("entrust_price");
		string sEntrustAmount = lpUnPack->GetStr("entrust_amount");
		string sInvestType = lpUnPack->GetStr("invest_type");
		string sEntrustState = lpUnPack->GetStr("entrust_state");
		string sExtsystemId = lpUnPack->GetStr("extsystem_id");
		string sThirdReff = lpUnPack->GetStr("third_reff");
	}
	else if (sMsgType == "b")
	{
		//��a��������ͬ,�ֶνṹ�ο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶.xls����[��Ϣ����]ҳ
	}
	else if (sMsgType == "c")
	{
		//��a��������ͬ,�ֶνṹ�ο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶.xls����[��Ϣ����]ҳ
	}
	else if (sMsgType == "d")
	{
		//��a��������ͬ,�ֶνṹ�ο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶.xls����[��Ϣ����]ҳ
	}
	else if (sMsgType == "e")
	{
		//��a��������ͬ,�ֶνṹ�ο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶.xls����[��Ϣ����]ҳ
	}
	else if (sMsgType == "f")
	{
		//��a��������ͬ,�ֶνṹ�ο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶.xls����[��Ϣ����]ҳ
	}
	else if (sMsgType == "g")
	{
		//��a��������ͬ,�ֶνṹ�ο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶.xls����[��Ϣ����]ҳ
	}
}
void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo);

};