#include <stdio.h>
#include "demo.h"
////////////////////////////////
//打印应答包
void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker)
{
	int i = 0, t = 0, j = 0, k = 0;
	
    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集
		pUnPacker->SetCurrentDatasetByIndex(i);
		
        // 打印字段
        for (t = 0; t < pUnPacker->GetColCount(); ++t)
        {
            printf("%20s", pUnPacker->GetColName(t));
        }
		
        putchar('\n');
		
        // 打印所有记录
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
            // 打印每条记录
            for (k = 0; k < pUnPacker->GetColCount(); ++k)
            {
                switch (pUnPacker->GetColType(k))
                {
                case 'I':
                    printf("%20d", pUnPacker->GetIntByIndex(k));
                    break;
					
                case 'C':
                    printf("%20c", pUnPacker->GetCharByIndex(k));
                    break;
					
                case 'S':
                    printf("%20s", pUnPacker->GetStrByIndex(k));
                    break;
					
                case 'F':
                    printf("%20f", pUnPacker->GetDoubleByIndex(k));
                    break;
					
                case 'R':
                    {
                        break;
                    }				
                default:
                    // 未知数据类型
                    printf("未知数据类型。\n");
                    break;
                }
            }		
            putchar('\n');		
            pUnPacker->Next();
        }
        putchar('\n');
    }
}

/////////////////////////////////////////////////////////////
//CBusiness 实现
CBusiness::CBusiness()
{
	memset(m_cUserToken, 0, sizeof(m_cUserToken));
}

CBusiness::~CBusiness()
{
	
}

//登录
int CBusiness::Login()
{
	int iRet = 0, hSend = 0, iSystemNo = -1;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);  
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);       
	pPacker->AddField("input_content", 'C');     
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);   
	pPacker->AddField("password", 'S', 10);       
	pPacker->AddField("password_type", 'C');      
	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('L');					
	pPacker->AddStr("1");				
	pPacker->AddInt(1);				
	pPacker->AddChar('1');				
	pPacker->AddStr("302000007");			
	pPacker->AddStr("0");					
	pPacker->AddStr("123456");			
	pPacker->AddChar(/*'2'*/'\0');				
	///结束打包
	pPacker->EndPack();
	
	if((hSend = g_pConnection->SendBiz(331100,pPacker, SYNCSEND, 0)) < 0)
	{
		printf("发送功能331100失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能331100成功, 返回接收句柄: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("接收功能331100失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("331100 业务错误, 错误号: %d, 原因: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("331100系统错误, 原因: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("其他错误, 错误号: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pUnPacker)
	{
		iSystemNo = pUnPacker->GetInt("sysnode_id");
		const char *pUserToken = pUnPacker->GetStr("user_token");
		if(pUserToken)
			strcpy(m_cUserToken, pUserToken);
		ShowPacket(0, pUnPacker);
	}
	else
		printf("接收到的数据存在问题, 解包器为空!\r\n");

EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iSystemNo;
}

//证券委托 333002
int CBusiness::SecuEntrust(int iSystemNo)
{
	int iRet = 0, hSend = 0, iEntrustNo = 0;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'F', 18, 3);	
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("entrust_prop", 'S', 3);
	pPacker->AddField("batch_no", 'I', 8);
	
	///加入对应的字段值
	pPacker->AddInt(0);				
	pPacker->AddChar('1');				
	pPacker->AddStr("hs");					
	pPacker->AddInt(1);			
	pPacker->AddStr("123411");			 
	pPacker->AddStr("302000007");			
	pPacker->AddStr("123456");			
	pPacker->AddChar('2');				
	pPacker->AddStr(m_cUserToken);		
	printf("%s\n",m_cUserToken);
	pPacker->AddStr("1");				
	pPacker->AddStr("A083885890");		
	pPacker->AddStr("600570");			
	pPacker->AddDouble(100);			 
	pPacker->AddDouble(13.01);			
	pPacker->AddChar('1');				
	pPacker->AddStr("0");		
	pPacker->AddInt(0);		
	
	///结束打包
	pPacker->EndPack();
	
	if((hSend = g_pConnection->SendBiz(333002,pPacker, SYNCSEND, iSystemNo)) < 0)
	{
		printf("发送功能333002失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能333002成功, 返回接收句柄: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("接收功能333002失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("333002业务错误, 错误号: %d, 原因: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("333002系统错误, 原因: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("其他错误, 错误号: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pUnPacker)
	{
		iEntrustNo = pUnPacker->GetInt("entrust_no");
		printf("接收功能333002成功, 返回值: %d, 委托号: %d!\r\n", iRet, iEntrustNo);
	}
	else
		printf("接收到的数据存在问题, 解包器为空!\r\n");

EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//证券委托查询 333101
int CBusiness::SearchSecuEntrust(int iSystemNo /* = 2 */)
{
	int iRet = 0, hSend = 0;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///定义解包器
	IF2UnPacker *pUnPacker = NULL;
	
	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);				
	pPacker->AddField("client_id", 'S', 18);				
	pPacker->AddField("fund_account", 'S', 18);				
	pPacker->AddField("password", 'S', 10);			
	pPacker->AddField("password_type", 'C', 1);			
	pPacker->AddField("user_token", 'S', 40);			
	
	///加入对应的字段值
	pPacker->AddInt(0);					
	pPacker->AddChar('1');					
	pPacker->AddStr("hs");			
	pPacker->AddInt(1);			
	pPacker->AddStr("123411");			 
	pPacker->AddStr("302000007");			
	pPacker->AddStr("123456");		
	pPacker->AddChar('2');		
	pPacker->AddStr(m_cUserToken);		
	
	///结束打包
	pPacker->EndPack();
	
	///异步发送委托查询功能，应答在回调函数OnReceivedBizEx中处理。
	if((hSend = g_pConnection->SendBiz(333101,pPacker, SYNCSEND, iSystemNo)) < 0)
	{
		printf("发送功能333101失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("发送功能333101成功, 返回接收句柄: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
	if(iRet < 0)
	{
		printf("接收功能333101失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("333101业务错误, 错误号: %d, 原因: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("333101系统错误, 原因: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("其他错误, 错误号: %d!\r\n", iRet);
		goto EXIT;
	}
	
	if(pUnPacker)
	{
		printf("接收功能333101成功, 返回值: %d!\r\n", iRet);
		ShowPacket(0, (IF2UnPacker *)pUnPacker);
	}
	else
		printf("接收到的数据存在问题, 解包器为空!\r\n");
	
EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

int CBusiness::Subscribe(int issue_type)
{
	int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);  
	
	///加入对应的字段值
	pPacker->AddInt(1);					
//	pPacker->AddStr("");
	pPacker->AddStr("302000007");				
	pPacker->AddInt(0);							
	pPacker->AddChar('L');						
	pPacker->AddStr("");		
	pPacker->AddStr(/*pClientID*/"");				
	pPacker->AddStr("123456");					
	pPacker->AddStr(m_cUserToken);		
	pPacker->AddInt(issue_type);			
	///结束打包
	pPacker->EndPack();

	REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issue_type;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///订阅
	if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, SYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	{
		printf("发送功能620001失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("发送功能620001成功, 返回接收句柄: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("接收功能620001失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("620001业务错误, 错误号: %d, 原因: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("620001系统错误, 原因: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("其他错误, 错误号: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pRetData == NULL || pRetData->lpKeyInfo == NULL)
	{
		printf("接收到的keyinfo中的数据非法!\r\n");
		goto EXIT;
	}

	pMCUnPacker = NewUnPacker(pRetData->lpKeyInfo, pRetData->keyInfoLen);
		
	if(pMCUnPacker)
	{
		ShowPacket(0,pMCUnPacker);
	}
	else
		printf("接收到的数据存在问题, 解包器为空!\r\n");

EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}


	///自已申请的，需要释放
	if(pMCUnPacker)
		pMCUnPacker->Release();
	
	return iRet;
}

int CBusiness::CancelSub(int issueType)
{
	int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("user_token",'S',40);    
	///加入对应的字段值
	pPacker->AddInt(1);				
	pPacker->AddStr("302000007");			
	pPacker->AddStr(m_cUserToken);		 
	///结束打包
	pPacker->EndPack();

	REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issueType;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///取消订阅
	if((hSend = g_pConnection->SendBizEx(620002,pPacker, NULL, SYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	{
		printf("发送功能620002失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能620002成功, 返回接收句柄: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("接收功能620002失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("620001业务错误, 错误号: %d, 原因: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("620002系统错误, 原因: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("其他错误, 错误号: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pRetData == NULL || pRetData->lpKeyInfo == NULL)
	{
		printf("接收到的keyinfo中的数据非法!\r\n");
		goto EXIT;
	}

	pMCUnPacker = NewUnPacker(pRetData->lpKeyInfo, pRetData->keyInfoLen);

	if(pMCUnPacker)
	{
		pMCUnPacker->AddRef();
		int iSubscribeResult = pMCUnPacker->GetInt("subscribe_result");
		const char *pResultInfo = pMCUnPacker->GetStr("result_info");
		printf("接收功能620002成功, 返回值: %d, 订阅结果: %d, %s!\r\n", iRet, iSubscribeResult, pResultInfo);
	}
	else
		printf("接收到的数据存在问题, 解包器为空!\r\n");

EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}


	///自已申请的，需要释放
	if(pMCUnPacker)
		pMCUnPacker->Release();
	
	return iRet;
}

////////////////////////////////////////////////////////////
//Callback 实现
unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CCallback::AddRef()
{
    return 0;
}

unsigned long CCallback::Release()
{
    return 0;
}

// 以下各回调方法的实现仅仅为演示使用
void CCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnConnect");
}

void CCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnSafeConnect");
}

void CCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnRegister");
}

void CCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnClose");;
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
	puts("CCallback::OnSent");
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
	puts("CCallback::Reserved1");
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
	puts("CCallback::Reserved2");
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	puts("CCallback::OnReceivedBizEx");
	
	int ret = 0;
	
	char* pErrInfo = lpRetData->errorInfo;
	int iErrNo=lpRetData->errorNo;
	int iFuncID = lpRetData->functionID;
	int iRetCode = lpRetData->returnCode;
	
	if(iErrNo != 0 || iRetCode != 0)
	{
		printf("功能号: %d, 返回错误号: %d, 返回码: %d, 错误信息: %s!\r\n",\
			iFuncID, iErrNo, iRetCode, pErrInfo ? pErrInfo : "NULL");
		return;
	}
	
	if(620000 == iFuncID)	//心跳
	{
		puts("收到心跳620000:");	
    void *Pointer = NULL;
		REQ_DATA reqData;
		memset(&reqData,0,sizeof(REQ_DATA));
		reqData.issueType = 12;		//此处需要确认，心跳应答是否区分订阅类型
		reqData.packetType = 1;		//转换为应答包
		memcpy(&reqData.routeInfo, &lpRetData->sendInfo, sizeof(reqData.routeInfo));
		//消息中心发包
		if((ret = g_pConnection->SendBizEx(620000, NULL, NULL, ASYNCSEND, 0, 0, 0, &reqData)) < 0)
		{
			puts(g_pConnection->GetErrorMsg(ret));
		}
	}
	else if(620003 == iFuncID)	//成交主推
	{
		//收到成交主推消息
		puts("收到主推消息620003:");
		IF2UnPacker * lpUnapck = NULL;
		if (lpRetData != NULL)	//成交主推
		{
			int issue_type = lpRetData->issueType;
			if(issue_type == ISSUE_TYPE_REALTIME_SECU) ///证券成交类型
			{
				lpUnapck = NewUnPacker(lpRetData->lpKeyInfo, lpRetData->keyInfoLen);
				if(lpUnapck)
				{
					lpUnapck->AddRef();
					ShowPacket(0, (IF2UnPacker *)lpUnapck);
					lpUnapck->Release();
				}
			}
		}
	}
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    puts("CCallback::OnReceivedBiz");
}

int  CCallback::Reserved3()
{
	puts("CCallback::Reserved3");
    return 0;
}

void CCallback::Reserved4()
{
	puts("CCallback::Reserved4");
}

void CCallback::Reserved5()
{
	puts("CCallback::Reserved5");
}

void CCallback::Reserved6()
{
	puts("CCallback::Reserved6");
}

void CCallback::Reserved7()
{
	puts("CCallback::Reserved7");
}

////////////////////////////////////////////////////////////////
//main
// 全局连接对象
CConnectionInterface *g_pConnection = NULL;
CConnectionInterface *g_pConnectionHq = NULL;

CBusiness g_szBusiness;
CBusiness g_szBusinessHq;

int main()
{
	//通过T2SDK的引出函数，来获取一个新的CConfig对象
	//此对象在创建连接对象时被传递，用于配置所创建的连接对象的各种属性（比如服务器IP地址、安全模式等）
	//值得注意的是，在向配置对象设置配置信息时，配置信息既可以从ini文件中载入，
	//也可以在程序代码中设定，或者是2者的混合，如果对同一个配置项设不同的值，则以最近一次设置为准
	CConfigInterface * lpConfig = NewConfig();

	//通过T2SDK的引出函数NewXXXX返回的对象，需要调用对象的Release方法释放，而不能直接用delete
	//因为t2sdk.dll和调用程序可能是由不同的编译器、编译模式生成，delete可能会导致异常
	//为了适合Delphi等使用（Delphi对接口自动调用AddRef方法），用C/C++开发的代码，需要在NewXXXX之后调用一下AddRef
	//以保证引用计数正确
	lpConfig->AddRef();

	///从配置文件中加载配置
	lpConfig->Load("t2sdk.ini");

	///可以在代码中对lpConfig对像中的具体值进行设置

    //[t2sdk] servers指定需要连接的IP地址及端口，可配置多个，中间以“;”间隔//111.207.179.6:8066  18.8.1.171:8066
	/*
    lpConfig->SetString("t2sdk", "servers", "192.168.51.162:3335");
	
    //[t2sdk] license_file指定许可证文件路径
    lpConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");
	
    //[t2sdk] lang指定错误信息的语言号（缺省为简体中文2052），1033为英文
    lpConfig->SetString("t2sdk", "lang", "1033");
	
    //[t2sdk] send_queue_size指定T2_SDK的发送队列大小
    lpConfig->SetString("t2sdk", "send_queue_size", "100");
    
    //[t2sdk] send_queue_size指定T2_SDK的发送队列大小
    lpConfig->SetString("t2sdk", "license_pwd", "888888");
	
    //[safe] safe_level指定连接的安全模式，需要和T2通道的安全模式一致，否则连接失败
    lpConfig->SetString("safe", "safe_level", "ssl");
    
    //[safe] 
    lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
    
    //[safe] 
    lpConfig->SetString("safe", "cert_pwd", "111111");
	*/

	g_szBusiness.SetConfig(lpConfig);
  //通过T2SDK的引出函数，来获取一个新的CConnection对象指针
	g_pConnection = NewConnection(lpConfig);
	
	g_pConnection->AddRef();
	
	// 创建自定义类CCallback的对象（在初始化连接对象时需传递此对象，请看下面代码）
	CCallback szCallback;
	
	int iRet = 0, iSystemNo = 0;
	
	//初始化连接对象，返回0表示初始化成功，注意此时并没开始连接服务器
	if ((0 == (iRet = g_pConnection->CreateEx(&szCallback))))
	{
			// 开启断开重连线程，由于在非断开情况下，该线程处于Wait状态，故对应用性能影响甚微
		//正式开始连接，参数5000为超时参数，单位毫秒
		if (iRet = g_pConnection->Connect(5000))
		{
        //puts(g_pConnection->GetErrorMsg(iRet));
				printf("连接服务器失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(iRet));
		}	
		else
		{
			printf("连接服务器成功!\r\n");
			////////////////////////////证券测试//////////////////////////////////////
			//Subscribe(12, 8113, "811300000008");		//发送订阅请求:成交主推 12
			iSystemNo = g_szBusiness.Login();
			if(iSystemNo >= 0)
			{
				int chose = 0;
				printf("------------------------------欢迎使用demo------------------------------\n");
				printf("1.订阅-证券成交回报. 2.证券委托 3.证券委托查询 0.退出该系统!\n请输入你的操作:");
				while(scanf("%d",&chose),0 != chose)
				{
					getchar();
					switch (chose)
					{
						case 1: g_szBusiness.Subscribe(ISSUE_TYPE_REALTIME_SECU);
								printf("请按任意键显示主菜单!:\n");
							break;
						case 2: g_szBusiness.SecuEntrust(iSystemNo);
							    printf("请按任意键显示主菜单!:\n");
							break;
						case 3: g_szBusiness.SearchSecuEntrust(iSystemNo);
								printf("请按任意键显示主菜单!:\n");
							break;
						default:printf("您输入的操作不正确,请输入正确的操作!\n");
								printf("请按任意键显示主菜单!:\n");
							break;
					}
					getchar();
					printf("1.订阅-证券成交回报. 2.证券委托 3.证券委托查询 0.退出该系统!\n请输入你的操作:");
				}
		    }
		    else
				printf("取得系统节点号失败, 返回结果: %d!\r\n", iSystemNo);
		}
	}
	else
	{
		if(0 != iRet)
	      puts(g_pConnection->GetErrorMsg(iRet));
	}
	
	//通过getchar阻塞线程，等待服务端应答包到达
	printf("按任意键出退!\r\n");
	getchar();
	
	g_pConnection->Close();
	g_pConnection->Release();
	lpConfig->Release();
	
	return 0;
}