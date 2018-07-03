#include <stdio.h>
#include "demo.h"
////////////////////////////////
//��ӡӦ���
void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker)
{
	int i = 0, t = 0, j = 0, k = 0;
	
    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        // ���õ�ǰ�����
		pUnPacker->SetCurrentDatasetByIndex(i);
		
        // ��ӡ�ֶ�
        for (t = 0; t < pUnPacker->GetColCount(); ++t)
        {
            printf("%20s", pUnPacker->GetColName(t));
        }
		
        putchar('\n');
		
        // ��ӡ���м�¼
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
            // ��ӡÿ����¼
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
                    // δ֪��������
                    printf("δ֪�������͡�\n");
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
//CBusiness ʵ��
CBusiness::CBusiness()
{
	memset(m_cUserToken, 0, sizeof(m_cUserToken));
}

CBusiness::~CBusiness()
{
	
}

//��¼
int CBusiness::Login()
{
	int iRet = 0, hSend = 0, iSystemNo = -1;
	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	IF2UnPacker *pUnPacker = NULL;

	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);  
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);       
	pPacker->AddField("input_content", 'C');     
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);   
	pPacker->AddField("password", 'S', 10);       
	pPacker->AddField("password_type", 'C');      
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);						
	pPacker->AddChar('L');					
	pPacker->AddStr("1");				
	pPacker->AddInt(1);				
	pPacker->AddChar('1');				
	pPacker->AddStr("302000007");			
	pPacker->AddStr("0");					
	pPacker->AddStr("123456");			
	pPacker->AddChar(/*'2'*/'\0');				
	///�������
	pPacker->EndPack();
	
	if((hSend = g_pConnection->SendBiz(331100,pPacker, SYNCSEND, 0)) < 0)
	{
		printf("���͹���331100ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("���͹���331100�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("���չ���331100ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("331100 ҵ�����, �����: %d, ԭ��: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("331100ϵͳ����, ԭ��: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("��������, �����: %d!\r\n", iRet);
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
		printf("���յ������ݴ�������, �����Ϊ��!\r\n");

EXIT:
	///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///�ͷ������pack
		pPacker->Release();
	}
	
	return iSystemNo;
}

//֤ȯί�� 333002
int CBusiness::SecuEntrust(int iSystemNo)
{
	int iRet = 0, hSend = 0, iEntrustNo = 0;
	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	IF2UnPacker *pUnPacker = NULL;

	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
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
	
	///�����Ӧ���ֶ�ֵ
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
	
	///�������
	pPacker->EndPack();
	
	if((hSend = g_pConnection->SendBiz(333002,pPacker, SYNCSEND, iSystemNo)) < 0)
	{
		printf("���͹���333002ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("���͹���333002�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("���չ���333002ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("333002ҵ�����, �����: %d, ԭ��: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("333002ϵͳ����, ԭ��: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("��������, �����: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pUnPacker)
	{
		iEntrustNo = pUnPacker->GetInt("entrust_no");
		printf("���չ���333002�ɹ�, ����ֵ: %d, ί�к�: %d!\r\n", iRet, iEntrustNo);
	}
	else
		printf("���յ������ݴ�������, �����Ϊ��!\r\n");

EXIT:
	///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///�ͷ������pack
		pPacker->Release();
	}
	
	return iRet;
}

//֤ȯί�в�ѯ 333101
int CBusiness::SearchSecuEntrust(int iSystemNo /* = 2 */)
{
	int iRet = 0, hSend = 0;
	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///��������
	IF2UnPacker *pUnPacker = NULL;
	
	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);				
	pPacker->AddField("client_id", 'S', 18);				
	pPacker->AddField("fund_account", 'S', 18);				
	pPacker->AddField("password", 'S', 10);			
	pPacker->AddField("password_type", 'C', 1);			
	pPacker->AddField("user_token", 'S', 40);			
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);					
	pPacker->AddChar('1');					
	pPacker->AddStr("hs");			
	pPacker->AddInt(1);			
	pPacker->AddStr("123411");			 
	pPacker->AddStr("302000007");			
	pPacker->AddStr("123456");		
	pPacker->AddChar('2');		
	pPacker->AddStr(m_cUserToken);		
	
	///�������
	pPacker->EndPack();
	
	///�첽����ί�в�ѯ���ܣ�Ӧ���ڻص�����OnReceivedBizEx�д���
	if((hSend = g_pConnection->SendBiz(333101,pPacker, SYNCSEND, iSystemNo)) < 0)
	{
		printf("���͹���333101ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("���͹���333101�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
	if(iRet < 0)
	{
		printf("���չ���333101ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("333101ҵ�����, �����: %d, ԭ��: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("333101ϵͳ����, ԭ��: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("��������, �����: %d!\r\n", iRet);
		goto EXIT;
	}
	
	if(pUnPacker)
	{
		printf("���չ���333101�ɹ�, ����ֵ: %d!\r\n", iRet);
		ShowPacket(0, (IF2UnPacker *)pUnPacker);
	}
	else
		printf("���յ������ݴ�������, �����Ϊ��!\r\n");
	
EXIT:
	///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///�ͷ������pack
		pPacker->Release();
	}
	
	return iRet;
}

int CBusiness::Subscribe(int issue_type)
{
	int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	IF2UnPacker *pUnPacker = NULL;

	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);  
	
	///�����Ӧ���ֶ�ֵ
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
	///�������
	pPacker->EndPack();

	REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issue_type;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///����
	if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, SYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	{
		printf("���͹���620001ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("���͹���620001�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("���չ���620001ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("620001ҵ�����, �����: %d, ԭ��: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("620001ϵͳ����, ԭ��: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("��������, �����: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pRetData == NULL || pRetData->lpKeyInfo == NULL)
	{
		printf("���յ���keyinfo�е����ݷǷ�!\r\n");
		goto EXIT;
	}

	pMCUnPacker = NewUnPacker(pRetData->lpKeyInfo, pRetData->keyInfoLen);
		
	if(pMCUnPacker)
	{
		ShowPacket(0,pMCUnPacker);
	}
	else
		printf("���յ������ݴ�������, �����Ϊ��!\r\n");

EXIT:
	///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///�ͷ������pack
		pPacker->Release();
	}


	///��������ģ���Ҫ�ͷ�
	if(pMCUnPacker)
		pMCUnPacker->Release();
	
	return iRet;
}

int CBusiness::CancelSub(int issueType)
{
	int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	IF2UnPacker *pUnPacker = NULL;

	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("user_token",'S',40);    
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(1);				
	pPacker->AddStr("302000007");			
	pPacker->AddStr(m_cUserToken);		 
	///�������
	pPacker->EndPack();

	REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issueType;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///ȡ������
	if((hSend = g_pConnection->SendBizEx(620002,pPacker, NULL, SYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	{
		printf("���͹���620002ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("���͹���620002�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
	iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
	if(iRet < 0)
	{
		printf("���չ���620002ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(hSend));
		goto EXIT;
	}
	else if(iRet == 1)
	{
		int iErrorNo = pUnPacker->GetInt("error_no");
		const char *pErrorInfo = pUnPacker->GetStr("error_info");
		printf("620001ҵ�����, �����: %d, ԭ��: %s!\r\n", iErrorNo, pErrorInfo);
		goto EXIT;
	}
	else if(iRet == 2)
	{
		const char *pErrorInfo = (const char *)pUnPacker;
		printf("620002ϵͳ����, ԭ��: %s!\r\n", pErrorInfo);
		goto EXIT;
	}
	else if(iRet != 0)
	{
		printf("��������, �����: %d!\r\n", iRet);
		goto EXIT;
	}

	if(pRetData == NULL || pRetData->lpKeyInfo == NULL)
	{
		printf("���յ���keyinfo�е����ݷǷ�!\r\n");
		goto EXIT;
	}

	pMCUnPacker = NewUnPacker(pRetData->lpKeyInfo, pRetData->keyInfoLen);

	if(pMCUnPacker)
	{
		pMCUnPacker->AddRef();
		int iSubscribeResult = pMCUnPacker->GetInt("subscribe_result");
		const char *pResultInfo = pMCUnPacker->GetStr("result_info");
		printf("���չ���620002�ɹ�, ����ֵ: %d, ���Ľ��: %d, %s!\r\n", iRet, iSubscribeResult, pResultInfo);
	}
	else
		printf("���յ������ݴ�������, �����Ϊ��!\r\n");

EXIT:
	///�ͷ�pack��������ڴ棬������ͷž�ֱ��release���ܻ��ڴ�й©
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///�ͷ������pack
		pPacker->Release();
	}


	///��������ģ���Ҫ�ͷ�
	if(pMCUnPacker)
		pMCUnPacker->Release();
	
	return iRet;
}

////////////////////////////////////////////////////////////
//Callback ʵ��
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

// ���¸��ص�������ʵ�ֽ���Ϊ��ʾʹ��
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
		printf("���ܺ�: %d, ���ش����: %d, ������: %d, ������Ϣ: %s!\r\n",\
			iFuncID, iErrNo, iRetCode, pErrInfo ? pErrInfo : "NULL");
		return;
	}
	
	if(620000 == iFuncID)	//����
	{
		puts("�յ�����620000:");	
    void *Pointer = NULL;
		REQ_DATA reqData;
		memset(&reqData,0,sizeof(REQ_DATA));
		reqData.issueType = 12;		//�˴���Ҫȷ�ϣ�����Ӧ���Ƿ����ֶ�������
		reqData.packetType = 1;		//ת��ΪӦ���
		memcpy(&reqData.routeInfo, &lpRetData->sendInfo, sizeof(reqData.routeInfo));
		//��Ϣ���ķ���
		if((ret = g_pConnection->SendBizEx(620000, NULL, NULL, ASYNCSEND, 0, 0, 0, &reqData)) < 0)
		{
			puts(g_pConnection->GetErrorMsg(ret));
		}
	}
	else if(620003 == iFuncID)	//�ɽ�����
	{
		//�յ��ɽ�������Ϣ
		puts("�յ�������Ϣ620003:");
		IF2UnPacker * lpUnapck = NULL;
		if (lpRetData != NULL)	//�ɽ�����
		{
			int issue_type = lpRetData->issueType;
			if(issue_type == ISSUE_TYPE_REALTIME_SECU) ///֤ȯ�ɽ�����
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
// ȫ�����Ӷ���
CConnectionInterface *g_pConnection = NULL;
CConnectionInterface *g_pConnectionHq = NULL;

CBusiness g_szBusiness;
CBusiness g_szBusinessHq;

int main()
{
	//ͨ��T2SDK����������������ȡһ���µ�CConfig����
	//�˶����ڴ������Ӷ���ʱ�����ݣ��������������������Ӷ���ĸ������ԣ����������IP��ַ����ȫģʽ�ȣ�
	//ֵ��ע����ǣ��������ö�������������Ϣʱ��������Ϣ�ȿ��Դ�ini�ļ������룬
	//Ҳ�����ڳ���������趨��������2�ߵĻ�ϣ������ͬһ���������費ͬ��ֵ���������һ������Ϊ׼
	CConfigInterface * lpConfig = NewConfig();

	//ͨ��T2SDK����������NewXXXX���صĶ�����Ҫ���ö����Release�����ͷţ�������ֱ����delete
	//��Ϊt2sdk.dll�͵��ó���������ɲ�ͬ�ı�����������ģʽ���ɣ�delete���ܻᵼ���쳣
	//Ϊ���ʺ�Delphi��ʹ�ã�Delphi�Խӿ��Զ�����AddRef����������C/C++�����Ĵ��룬��Ҫ��NewXXXX֮�����һ��AddRef
	//�Ա�֤���ü�����ȷ
	lpConfig->AddRef();

	///�������ļ��м�������
	lpConfig->Load("t2sdk.ini");

	///�����ڴ����ж�lpConfig�����еľ���ֵ��������

    //[t2sdk] serversָ����Ҫ���ӵ�IP��ַ���˿ڣ������ö�����м��ԡ�;�����//111.207.179.6:8066  18.8.1.171:8066
	/*
    lpConfig->SetString("t2sdk", "servers", "192.168.51.162:3335");
	
    //[t2sdk] license_fileָ�����֤�ļ�·��
    lpConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");
	
    //[t2sdk] langָ��������Ϣ�����Ժţ�ȱʡΪ��������2052����1033ΪӢ��
    lpConfig->SetString("t2sdk", "lang", "1033");
	
    //[t2sdk] send_queue_sizeָ��T2_SDK�ķ��Ͷ��д�С
    lpConfig->SetString("t2sdk", "send_queue_size", "100");
    
    //[t2sdk] send_queue_sizeָ��T2_SDK�ķ��Ͷ��д�С
    lpConfig->SetString("t2sdk", "license_pwd", "888888");
	
    //[safe] safe_levelָ�����ӵİ�ȫģʽ����Ҫ��T2ͨ���İ�ȫģʽһ�£���������ʧ��
    lpConfig->SetString("safe", "safe_level", "ssl");
    
    //[safe] 
    lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
    
    //[safe] 
    lpConfig->SetString("safe", "cert_pwd", "111111");
	*/

	g_szBusiness.SetConfig(lpConfig);
  //ͨ��T2SDK����������������ȡһ���µ�CConnection����ָ��
	g_pConnection = NewConnection(lpConfig);
	
	g_pConnection->AddRef();
	
	// �����Զ�����CCallback�Ķ����ڳ�ʼ�����Ӷ���ʱ�贫�ݴ˶����뿴������룩
	CCallback szCallback;
	
	int iRet = 0, iSystemNo = 0;
	
	//��ʼ�����Ӷ��󣬷���0��ʾ��ʼ���ɹ���ע���ʱ��û��ʼ���ӷ�����
	if ((0 == (iRet = g_pConnection->CreateEx(&szCallback))))
	{
			// �����Ͽ������̣߳������ڷǶϿ�����£����̴߳���Wait״̬���ʶ�Ӧ������Ӱ����΢
		//��ʽ��ʼ���ӣ�����5000Ϊ��ʱ��������λ����
		if (iRet = g_pConnection->Connect(5000))
		{
        //puts(g_pConnection->GetErrorMsg(iRet));
				printf("���ӷ�����ʧ��, �����: %d, ԭ��: %s!\r\n", iRet, g_pConnection->GetErrorMsg(iRet));
		}	
		else
		{
			printf("���ӷ������ɹ�!\r\n");
			////////////////////////////֤ȯ����//////////////////////////////////////
			//Subscribe(12, 8113, "811300000008");		//���Ͷ�������:�ɽ����� 12
			iSystemNo = g_szBusiness.Login();
			if(iSystemNo >= 0)
			{
				int chose = 0;
				printf("------------------------------��ӭʹ��demo------------------------------\n");
				printf("1.����-֤ȯ�ɽ��ر�. 2.֤ȯί�� 3.֤ȯί�в�ѯ 0.�˳���ϵͳ!\n��������Ĳ���:");
				while(scanf("%d",&chose),0 != chose)
				{
					getchar();
					switch (chose)
					{
						case 1: g_szBusiness.Subscribe(ISSUE_TYPE_REALTIME_SECU);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						case 2: g_szBusiness.SecuEntrust(iSystemNo);
							    printf("�밴�������ʾ���˵�!:\n");
							break;
						case 3: g_szBusiness.SearchSecuEntrust(iSystemNo);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						default:printf("������Ĳ�������ȷ,��������ȷ�Ĳ���!\n");
								printf("�밴�������ʾ���˵�!:\n");
							break;
					}
					getchar();
					printf("1.����-֤ȯ�ɽ��ر�. 2.֤ȯί�� 3.֤ȯί�в�ѯ 0.�˳���ϵͳ!\n��������Ĳ���:");
				}
		    }
		    else
				printf("ȡ��ϵͳ�ڵ��ʧ��, ���ؽ��: %d!\r\n", iSystemNo);
		}
	}
	else
	{
		if(0 != iRet)
	      puts(g_pConnection->GetErrorMsg(iRet));
	}
	
	//ͨ��getchar�����̣߳��ȴ������Ӧ�������
	printf("�����������!\r\n");
	getchar();
	
	g_pConnection->Close();
	g_pConnection->Release();
	lpConfig->Release();
	
	return 0;
}