#include "HSSubApi.h"


namespace kiiik {


void FUNCTION_CALL_MODE HSSubApi::OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo)
{
}

void FUNCTION_CALL_MODE HSSubApi::OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData)
{
	if (lpData == NULL)
	{
		cout << "主推数据指针为空" << endl;
		return;
	}

	IF2UnPacker* lpUnPack = NewUnPacker((void*)lpData,nLength);
	cout << endl;
	cout << endl;
	cout << "HSSubApi::OnReceived: SubscribeIndex(" << subscribeIndex << "), MsgType(" << lpUnPack->GetStr("msgtype") << "), DataLength(" << nLength << ")." << endl;
	ShowPacket(lpUnPack);

	if (lpRecvData == NULL)
	{
		cout << "订阅数据指针为空" << endl;
	}
	else
	{
		ShowSubscribe(subscribeIndex,lpRecvData);
	}

	lpUnPack->SetCurrentDatasetByIndex(0);
	lpUnPack->First();

	string sMsgType = lpUnPack->GetStr("msgtype");  //主推消息类型
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
		//和a的做法相同,字段结构参考《恒生投资管理系统O3.2_周边接口规范.xls》的[消息推送]页
	}
	else if (sMsgType == "c")
	{
		//和a的做法相同,字段结构参考《恒生投资管理系统O3.2_周边接口规范.xls》的[消息推送]页
	}
	else if (sMsgType == "d")
	{
		//和a的做法相同,字段结构参考《恒生投资管理系统O3.2_周边接口规范.xls》的[消息推送]页
	}
	else if (sMsgType == "e")
	{
		//和a的做法相同,字段结构参考《恒生投资管理系统O3.2_周边接口规范.xls》的[消息推送]页
	}
	else if (sMsgType == "f")
	{
		//和a的做法相同,字段结构参考《恒生投资管理系统O3.2_周边接口规范.xls》的[消息推送]页
	}
	else if (sMsgType == "g")
	{
		//和a的做法相同,字段结构参考《恒生投资管理系统O3.2_周边接口规范.xls》的[消息推送]页
	}
}
void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo);

};