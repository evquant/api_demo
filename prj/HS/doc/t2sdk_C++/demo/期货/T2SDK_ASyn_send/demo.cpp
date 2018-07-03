#include <stdio.h>
#include "demo.h"
////////////////////////////////

int g_iSystemNo = -1;
bool g_bLogin = false;
char g_cUserToken[USERTOKENLEN] = {0};

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
#if 0
                        int nLength = 0;
                        void *lpData = pUnPacker->GetRawByIndex(k, &nLength);
						
                        // ��2�������ݽ��д���
						if (16 == iIssueType || 17 == iIssueType || 18 == iIssueType)	//�ڻ�������Ϣ
						{
							char ctype;
							if (17 == iIssueType)
							{
								ctype = pUnPacker->GetChar("business_type");
							}
							ShowStructData(ctype, iIssueType, nLength, lpData);
						}
#endif
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

///�ڻ���ʾӦ������
void GetStr(char* &dest, char* &source) //��ȡ�ַ���
{	
	int j = 0;
	dest = source;
	for (;*source != 1 && *source != NULL;source++)
	{
		j++;
	}
	dest[j] = NULL;
	if(source != NULL)
	source ++;
}
void GetInt(int &dest,char* &source) //��ȡ����
{
	int j = 0;
	char* pos = NULL;
	pos = source;
	for (;*source != 1 && *source != NULL;source++)
	{
		j++;
	}
	pos[j] = NULL;
	dest = atoi(pos);
	if(source != NULL)
	source++;
}
void GetDou(double &dest,char* & source) //��ȡ������
{
	int j = 0;
	char* pos = NULL;
	pos = source;
	for (;*source != 1 && *source != NULL;source++)
	{
		j++;
	}
	pos[j] = NULL;
	if(source != NULL)
	source++;
	dest = atof(pos);
}
void GetChar(char &dest,char* &source)
{
	dest = source[0];
	source ++;
	if(source != NULL)
	source ++;
}
void ShowFutuPack(int issue_type, IF2UnPacker *pUnPacker)
{
	int len;
	char* futu_exch_type = NULL;
	char* commodity_type = NULL;
	char* contract_code = NULL;
	char* arbicontract_id = NULL;
	char* first_code = NULL;
	char* second_code = NULL;
	char* futures_account = NULL;
	char* fund_account = NULL;
	char* futu_report_no = NULL;
	char* firm_no = NULL;
	char* operator_no = NULL;
	char* confirm_no = NULL;

	double pre_settlement_price = 0, pre_close_price = 0, pre_delta = 0, futu_open_price = 0, futu_last_price = 0, buy_high_price = 0;
	double sale_low_price = 0, futu_high_price = 0, futu_low_price = 0, uplimited_price = 0, downlimited_price = 0, average_price = 0;
	double change_direction =0, business_balance = 0, futu_close_price = 0, settlement_price = 0,buy_price = 0, buy_total_amount = 0;
	double pre_open_interest = 0, buy_high_amount = 0, sale_low_amount = 0, business_amount = 0, open_interest = 0, buy_amount = 0;
	double sale_price = 0, sale_amount = 0, sale_total_amount = 0, business_price = 0;
	double entrust_amount = 0, business_total_amount = 0, cacel_amount = 0, entrust_price = 0;
	char entrust_status, futu_entrust_type, forceclose_reason, entrust_occasion, weave_type, entrust_bs, entrust_direction, hedge_type;
	char exch_status, entrade_flag, quote_type;
	int client_group = 0 , branch_no = 0, batch_no = 0 ,amount_per_hand = 0, init_date = 0, curr_time = 0, entrust_no = 0;
	int business_no = 0, business_time = 0, quote_length = 0, update_time = 0, quote_index = 0;
	char * QH = (char *)pUnPacker->GetRaw("QH",&len);
	if (issue_type == ISSUE_TYPE_MARKETINFO_FUTU) ///�ڻ����ȱ䶯����
	{
		GetChar(quote_type,QH);
		GetInt(quote_length,QH);
		GetInt(update_time,QH);
		GetInt(quote_index,QH);
		GetStr(futu_exch_type,QH);
		GetStr(commodity_type,QH);
		GetStr(contract_code,QH);
		GetDou(pre_settlement_price,QH);
		GetDou(pre_close_price,QH);
		GetDou(pre_open_interest,QH);
		GetDou(pre_delta,QH);
		GetDou(futu_open_price,QH);
		GetDou(futu_last_price,QH);
		GetDou(buy_high_price,QH);
		GetDou(buy_high_amount,QH);
		GetDou(sale_low_price,QH);
		GetDou(sale_low_amount,QH);
		GetDou(futu_high_price,QH);
		GetDou(futu_low_price,QH);
		GetDou(uplimited_price,QH);
		GetDou(downlimited_price,QH);
		GetDou(average_price,QH);
		GetDou(change_direction,QH);
		GetDou(business_amount,QH);
		GetDou(open_interest,QH);
		GetDou(business_balance,QH);
		GetDou(futu_close_price,QH);
		GetDou(settlement_price,QH);
		printf("quote_type=%c,quote_length=%d,update_time=%d,quote_index=%d\n",quote_type,quote_length,update_time,quote_index);
		printf("futu_exch_type=%s,commodity_type=%s,contract_code=%s,\n",futu_exch_type,commodity_type,contract_code);
		printf("pre_settlement_price=%f,pre_close_price=%f,pre_open_interest=%f,\n",pre_settlement_price,pre_close_price,pre_open_interest);
		printf("pre_delta=%f,futu_open_price=%f,futu_last_price=%f,\n",pre_delta,futu_open_price,futu_last_price);
		printf("buy_high_price=%f,buy_high_amount=%f,sale_low_price=%f,\n",buy_high_price,buy_high_amount,sale_low_price);
		printf("sale_low_amount=%f,futu_high_price=%f,futu_low_price=%f,\n",sale_low_amount,futu_high_price,futu_low_price);
		printf("uplimited_price=%f,downlimited_price=%f,average_price=%f,\n",uplimited_price,downlimited_price,average_price);
		printf("change_direction=%f,business_amount=%f,open_interest=%f,\n",change_direction,business_amount,open_interest);
		printf("business_balance=%f,futu_close_price=%f,settlement_price=%f,\n",business_balance,futu_close_price,settlement_price);
	}
	else if (issue_type == ISSUE_TYPE_ARGMARKETINFO_FUTU)  ///�ڻ���ϱ䶯����
	{
		GetStr(arbicontract_id,QH);
		GetStr(futu_exch_type,QH);
		GetStr(first_code,QH);
		GetStr(second_code,QH);
		GetChar(weave_type,QH);
		GetDou(buy_price,QH);
		GetDou(buy_amount,QH);
		GetDou(buy_total_amount,QH);
		GetDou(sale_price,QH);
		GetDou(sale_amount,QH);
		GetDou(sale_total_amount,QH);
		GetDou(futu_high_price,QH);
		GetDou(futu_low_price,QH);
		GetDou(uplimited_price,QH);
		GetDou(downlimited_price,QH);

		printf("arbicontract_id=%s,futu_exch_type=%s,first_code=%s\n",arbicontract_id,futu_exch_type,first_code);
		printf("second_code=%s,weave_type=%c,buy_price=%f,\n",second_code,weave_type,buy_price);
		printf("buy_amount=%f,buy_total_amount=%f,sale_price=%f,\n",buy_amount,buy_total_amount,sale_price);
		printf("sale_amount=%f,sale_total_amount=%f,futu_high_price=%f,\n",sale_amount,sale_total_amount,futu_high_price);
		printf("futu_low_price=%f,uplimited_price=%f,downlimited_price=%f,\n",futu_low_price,uplimited_price,downlimited_price);
	}
	else if (issue_type == ISSUE_TYPE_REALTIME_FUTU)  ///�ڻ�ί�гɽ�������Ϣ
	{
		char LY = pUnPacker->GetChar("LY");
		if('0' == LY)
		{
			GetInt(entrust_no,QH);
			GetStr(futures_account,QH);
			GetStr(futu_exch_type,QH);
			GetStr(contract_code,QH);
			GetChar(entrust_bs,QH);
			GetChar(entrust_direction,QH);
			GetChar(hedge_type,QH);
			GetStr(fund_account,QH);
			GetStr(futu_report_no,QH);
			GetStr(firm_no,QH);
			GetStr(operator_no,QH);
			GetInt(client_group,QH);
			GetDou(entrust_amount,QH);
			GetDou(business_total_amount,QH);
			GetDou(cacel_amount,QH);
			GetDou(entrust_price,QH);
			GetChar(entrust_status,QH);
			GetInt(branch_no,QH);
			GetInt(batch_no,QH);
			GetChar(futu_entrust_type,QH);
			GetInt(amount_per_hand,QH);
			GetChar(forceclose_reason,QH);
			GetInt(init_date,QH);
			GetInt(curr_time,QH);
			GetStr(confirm_no,QH);
			GetChar(entrust_occasion,QH);

			printf("entrust_no=%d,futures_account=%s,futu_exch_type=%s,\n",entrust_no,futures_account,futu_exch_type);
			printf("contract_code=%s,entrust_bs=%c,entrust_direction=%c,\n",contract_code,entrust_bs,entrust_direction);
			printf("hedge_type=%c,fund_account=%s,futu_report_no=%s,\n",hedge_type,fund_account,futu_report_no);
			printf("firm_no=%s,operator_no=%s,client_group=%d,\n",firm_no,operator_no,client_group);
			printf("entrust_amount=%f,business_total_amount=%f,cacel_amount=%f,\n",entrust_amount,business_total_amount,cacel_amount);
			printf("entrust_price=%f,entrust_status=%c,branch_no=%d,\n",entrust_price,entrust_status,branch_no);
			printf("batch_no=%d,futu_entrust_type=%c,amount_per_hand=%d,\n",batch_no,futu_entrust_type,amount_per_hand);
			printf("forceclose_reason=%c,init_date=%d,curr_time=%d,\n",forceclose_reason,init_date,curr_time);
			printf("confirm_no=%s,entrust_occasion=%c,\n",confirm_no,entrust_occasion);
		}
		else if ('B' == LY)
		{
			GetInt(entrust_no,QH);
			GetStr(futures_account,QH);
			GetStr(futu_exch_type,QH);
			GetInt(business_no,QH);
			GetStr(contract_code,QH);
			GetChar(entrust_bs,QH);
			GetChar(entrust_direction,QH);
			GetChar(hedge_type,QH);
			GetDou(business_price,QH);
			GetDou(business_amount,QH);
			GetStr(fund_account,QH);
			GetStr(futu_report_no,QH);
			GetStr(firm_no,QH);
			GetStr(operator_no,QH);
			GetInt(client_group,QH);
			GetDou(entrust_amount,QH);
			GetDou(business_total_amount,QH);
			GetDou(cacel_amount,QH);
			GetDou(entrust_price,QH);
			GetChar(entrust_status,QH);
			GetInt(branch_no,QH);
			GetInt(batch_no,QH);
			GetChar(futu_entrust_type,QH);
			GetInt(amount_per_hand,QH);
			GetChar(forceclose_reason,QH);
			GetInt(init_date,QH);
			GetInt(business_time,QH);
			GetStr(confirm_no,QH);
			GetChar(entrust_occasion,QH);

			printf("entrust_no=%d,futures_account=%s,futu_exch_type=%s,\n",entrust_no,futures_account,futu_exch_type);
			printf("business_no=%d,contract_code=%s,entrust_bs=%c,\n",business_no,contract_code,entrust_bs);
			printf("entrust_direction=%c,hedge_type=%c,business_price=%f,\n",entrust_direction,hedge_type,business_price);
			printf("business_amount=%f,fund_account=%s,futu_report_no=%s,\n",business_amount,fund_account,futu_report_no);
			printf("firm_no=%s,operator_no=%s,client_group=%d,\n",firm_no,operator_no,client_group);
			printf("entrust_amount=%f,business_total_amount=%f,cacel_amount=%f,\n",entrust_amount,business_total_amount,cacel_amount);
			printf("entrust_price=%f,entrust_status=%c,branch_no=%d,\n",entrust_price,entrust_status,branch_no);
			printf("batch_no=%d,futu_entrust_type=%c,amount_per_hand=%d,\n",batch_no,futu_entrust_type,amount_per_hand);
			printf("forceclose_reason=%c,init_date=%d,business_time=%d,\n",forceclose_reason,init_date,business_time);
			printf("confirm_no=%s,entrust_occasion=%c,\n",confirm_no,entrust_occasion);
		}
	}
	else if (issue_type == ISSUE_TYPE_EXCHSTATUS_FUTU)  ///�ڻ�������״̬��Ϣ
	{
		GetStr(futu_exch_type,QH);
		GetChar(exch_status,QH);
		GetChar(entrade_flag,QH);
		printf("futu_exch_type=%s,exch_status=%c,entrade_flag=%c,\n",futu_exch_type,exch_status,entrade_flag);
	}
}

/////////////////////////////////////////////////////////////
//CBusiness ʵ��
CBusiness::CBusiness()
{
	memset(g_cUserToken, 0, sizeof(g_cUserToken));
}

CBusiness::~CBusiness()
{
	
}

//��¼
int CBusiness::Login()
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
	pPacker->AddField("input_content", 'C');      
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);    
	pPacker->AddField("password", 'S', 10);      
	pPacker->AddField("password_type", 'C');     
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);						
	pPacker->AddChar('L');					
	///����ȯ�̻����ڻ���Ҫ����д
	pPacker->AddStr("1");					
	pPacker->AddInt(1);				
	pPacker->AddChar('1');					
	pPacker->AddStr("302000007");			
	pPacker->AddStr("0");					
	pPacker->AddStr("123456");				
	pPacker->AddChar(/*'2'*/'\0');			
	///�������
	pPacker->EndPack();
	
	if((hSend = g_pConnection->SendBiz(331100,pPacker, ASYNCSEND, 0)) < 0)
	{
		printf("���͹���331100ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("���͹���331100�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

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

//�ڻ�ί��
int CBusiness::FutuEntrust(int iSystemNo)
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
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("futu_exch_type", 'S', 4);
	pPacker->AddField("futures_account", 'S', 12);
	pPacker->AddField("contract_code", 'S', 30);
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("hedge_type", 'C', 1);
	pPacker->AddField("futures_direction", 'C', 1);
	pPacker->AddField("futu_entrust_prop", 'C', 1);
	pPacker->AddField("futu_entrust_price", 'F', 12, 5);
	pPacker->AddField("entrust_amount", 'F', 16, 2);
	pPacker->AddField("entrust_kind",'C',1);
	pPacker->AddField("branch_no", 'I', 5);
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);                             
	pPacker->AddStr("302000007");				     
	pPacker->AddStr(g_cUserToken);		           
	pPacker->AddStr("127.0.0.1;005056C00008");		
	pPacker->AddChar('L');						   
	pPacker->AddStr("F4");						    
	pPacker->AddStr(/*"201310006121"*/"");			 
	pPacker->AddStr("IF1311");					   
	pPacker->AddChar('2');						    
	pPacker->AddChar('0');							
	pPacker->AddChar('1');							
	pPacker->AddChar('0');							
	pPacker->AddDouble(2424);						
	pPacker->AddDouble(1);							
	pPacker->AddChar('0');							
	pPacker->AddInt(1);								
	///�������
	pPacker->EndPack();
		
	if((hSend = g_pConnection->SendBiz(338202,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("���͹���338202ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("���͹���338202�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
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

//�ڻ�UFT�ֲֲ�ѯ
int CBusiness::SearchFutuHold(int iSystemNo /* = 2 */)
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
	LPRET_DATA  pRetData = NULL;
	
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no",'I',5);
	pPacker->AddField("op_entrust_way",'C',1);
    pPacker->AddField("op_station",'S',255);
    pPacker->AddField("branch_no",'I',5);
    pPacker->AddField("client_id",'S',18);
    pPacker->AddField("fund_account",'S',18);
    pPacker->AddField("password",'S',10);
    pPacker->AddField("user_token",'S',40);
    pPacker->AddField("futu_exch_type",'S',4);
    pPacker->AddField("futures_account",'S',12);
    pPacker->AddField("contract_code",'S',30);
    pPacker->AddField("entrust_bs",'C',1);
    pPacker->AddField("query_direction",'C',1);
    pPacker->AddField("query_mode",'C',1);
    pPacker->AddField("action_in",'I',5);
    pPacker->AddField("request_num",'I',8);
    pPacker->AddField("position_str",'S',32);
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);                        
	pPacker->AddChar('L');                   
	pPacker->AddStr("127.0.0.1;005056C00008");          
	pPacker->AddInt(1);                   
	pPacker->AddStr("123411");                  
	pPacker->AddStr("302000007");                 
	pPacker->AddStr("123456");                  
	pPacker->AddStr(g_cUserToken);                 
	pPacker->AddStr("");               
	pPacker->AddStr("");                  
	pPacker->AddStr("");               
	pPacker->AddChar('\0');               
	pPacker->AddChar('\0');              
	pPacker->AddChar('\0');                
	pPacker->AddInt(0);                 
	pPacker->AddInt(10);                  
	pPacker->AddStr("");                  
	///�������
	pPacker->EndPack();

	//ͬ�����ͳֲֲ�ѯ����
	if((hSend = g_pConnection->SendBiz(338303,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("���͹���338303ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("���͹���338303�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
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

//�ڻ�UFTί�в�ѯ
int CBusiness::SearchFutuEntrust(int iSystemNo /* = 2 */)
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
	pPacker->AddField("user_token", 'S', 40);	
	pPacker->AddField("futu_exch_type", 'S', 4);	
	pPacker->AddField("futures_account", 'S', 12);
	pPacker->AddField("contract_code", 'S', 30);
	pPacker->AddField("en_entrust_status", 'S', 10);
	pPacker->AddField("batch_no", 'I', 8);
	pPacker->AddField("entrust_no", 'I', 8);
	pPacker->AddField("futu_entrust_type", 'C', 1);
	pPacker->AddField("query_direction", 'C', 1);
	pPacker->AddField("request_num",'I',8);
	pPacker->AddField("position_str",'S',32);
	
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);					     	
	pPacker->AddChar('L');					    
	///����ȯ�̻����ڻ���Ҫ����д
	pPacker->AddStr("127.0.0.1;005056C00008");
	pPacker->AddInt(1);					
	pPacker->AddStr("123411");		          
	pPacker->AddStr("302000007");			  
	pPacker->AddStr("123456");			    	
	pPacker->AddStr(g_cUserToken);	        	
	pPacker->AddStr("");				       
	pPacker->AddStr("");		               
	pPacker->AddStr("IF1311");				
	pPacker->AddStr("");                   
	pPacker->AddInt(0);                        
	pPacker->AddInt(1000003);                    
	pPacker->AddChar('\0');                   
	pPacker->AddChar('0');                   
	pPacker->AddInt(1);                      
	pPacker->AddStr(" ");                   
	///�������
	pPacker->EndPack();
	
	//�첽����ί�в�ѯ����
	if((hSend = g_pConnection->SendBiz(338301,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("���͹���338301ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("���͹���338301�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);
	
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

int CBusiness::WithdrawFutuEntrust(int iSystemNo /* = 2 */)
{
	int iRet = 0, hSend = 0, iEntrustNo = 0;
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker =NewPacker(2);
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
	pPacker->AddField("op_branch_no",'I',5);
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station",'S',255);
	pPacker->AddField("branch_no",'I',5);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("fund_account",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token",'S',40);
	pPacker->AddField("futu_exch_type",'S',4);
	pPacker->AddField("entrust_no",'I',8);
    ///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);                     
	pPacker->AddChar('L');						
	pPacker->AddStr(" ");					
	pPacker->AddInt(1);						
	pPacker->AddStr("123411");					
	pPacker->AddStr("302000007");
	pPacker->AddStr("123456");					
	pPacker->AddStr(g_cUserToken);			
	pPacker->AddStr(" ");					
	pPacker->AddInt(1030478);				
	///�������
	pPacker->EndPack();

	///�첽���ͳ�������
	if((hSend = g_pConnection->SendBiz(338217,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("���͹���338217ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("���͹���338217�ɹ�, ����ֵ: %d!\r\n", hSend);

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
	pPacker->AddField("contract_code",'S',30);    
	pPacker->AddField("arbicontract_id",'S',30); 
	pPacker->AddField("futu_exch_type",'S',4);	  
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(1);						
	pPacker->AddStr("302000007");				
	
	///����Ϊ�ڻ�UFT��Ҫ���ֶ�
	pPacker->AddInt(0);								
	pPacker->AddChar('L');							
	pPacker->AddStr("");		
	pPacker->AddStr(/*pClientID*/"");					
	pPacker->AddStr("123456");					 
	pPacker->AddStr(g_cUserToken);				
	pPacker->AddInt(issue_type);			
	pPacker->AddStr("IF1311");						
	pPacker->AddStr("*");						
	pPacker->AddStr("*");                   
	///�������
	pPacker->EndPack();

	REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issue_type;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///����
	if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, ASYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	{
		printf("���͹���620001ʧ��, �����: %d, ԭ��: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("���͹���620001�ɹ�, ���ؽ��վ��: %d!\r\n", hSend);

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

int CBusiness::CancelSub(int issueType)
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

	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("user_token",'S',40);  
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(1);					
	pPacker->AddStr("302000007");			
	pPacker->AddStr(g_cUserToken);		    
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
    puts("CCallback::OnClose");
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
	int iIssueType = lpRetData->issueType;
	int iRetCode = lpRetData->returnCode;
	int iKeyInfoLen = lpRetData->keyInfoLen;
	char* pKeyInfo = (char*)lpRetData->lpKeyInfo;
	int iLen = lpRetData->keyInfoLen;
	
	if(iErrNo != 0 || iRetCode != 0)
	{
		g_bLogin = true;
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
	
		if((ret = g_pConnection->SendBizEx(620000, NULL, NULL, ASYNCSEND, 0, 0, 0, &reqData)) < 0)
		{
			puts(g_pConnection->GetErrorMsg(ret));
		}
	}
	else if(620001 == iFuncID)	//���ķ�����Ϣ
	{
		puts("�յ����ķ�����Ϣ620001:");
		IF2UnPacker * lpUnapck = NULL;
		if (lpRetData != NULL && ( lpRetData->issueType == ISSUE_TYPE_MARKETINFO_FUTU || lpRetData->issueType == ISSUE_TYPE_ARGMARKETINFO_FUTU ||lpRetData->issueType == ISSUE_TYPE_REALTIME_FUTU || lpRetData->issueType == ISSUE_TYPE_EXCHSTATUS_FUTU))	//�ɽ�����
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
	else if(620003 == iFuncID)	//�ɽ�����
	{
		//�յ��ɽ�������Ϣ
		puts("�յ�������Ϣ620003:");
		IF2UnPacker * lpUnapck = NULL;
		if (lpRetData != NULL)	//�ɽ�����
		{
			int issue_type = lpRetData->issueType;
			if(issue_type == ISSUE_TYPE_MARKETINFO_FUTU || issue_type == ISSUE_TYPE_ARGMARKETINFO_FUTU ||
				issue_type == ISSUE_TYPE_REALTIME_FUTU || issue_type == ISSUE_TYPE_EXCHSTATUS_FUTU)    ///�ڻ�
			{
				ShowFutuPack(issue_type, (IF2UnPacker *)lpUnpackerOrStr);
			}
		}
	}
	else if (iFuncID == 331100 )
	{
		ShowPacket(0, (IF2UnPacker *)lpUnpackerOrStr);
		g_bLogin = true;
		
		IF2UnPacker * lpUnpack = NULL;
		lpUnpack = NewUnPacker(lpRetData->lpKeyInfo, lpRetData->keyInfoLen);
		if(lpUnpack)
		{
			g_iSystemNo = lpUnpack->GetInt("sysnode_id");
			const char *pUserToken = lpUnpack->GetStr("user_token");
			if(pUserToken)
				strcpy(g_cUserToken, pUserToken);
			ShowPacket(0, lpUnpack);
		}
	}
	else if(iFuncID == 338301 || iFuncID == 338202 || iFuncID == 338303 || iFuncID == 338217)	///ί�в�ѯ�첽Ӧ����
	{
		ShowPacket(0, (IF2UnPacker *)lpUnpackerOrStr);
	}
	printf("�밴�������ʾ���˵�!:\n");
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

CBusiness g_szBusiness;

int main()
{

//	HANDLE hThread = NULL;

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

	
	//���������һ��Ϊ׼
    //[t2sdk] serversָ����Ҫ���ӵ�IP��ַ���˿ڣ������ö�����м��ԡ�;�����//111.207.179.6:8066  18.8.1.171:8066
    //lpConfig->SetString("t2sdk", "servers", "192.168.51.162:3335");
	
    //[t2sdk] license_fileָ�����֤�ļ�·�� HS-HUNDSUN001-FBASE2-0000_3rd.dat
    //lpConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");
	
    //[t2sdk] langָ��������Ϣ�����Ժţ�ȱʡΪ��������2052����1033ΪӢ��
    //lpConfig->SetString("t2sdk", "lang", "1033");
	
    //[t2sdk] send_queue_sizeָ��T2_SDK�ķ��Ͷ��д�С
    //lpConfig->SetString("t2sdk", "send_queue_size", "100");
    
    //[t2sdk] send_queue_sizeָ��T2_SDK�ķ��Ͷ��д�С
    //lpConfig->SetString("t2sdk", "license_pwd", "888888");
	
    //[safe] safe_levelָ�����ӵİ�ȫģʽ����Ҫ��T2ͨ���İ�ȫģʽһ�£���������ʧ��
    //lpConfig->SetString("safe", "safe_level", "ssl");
    
    //[safe] 
    //lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
    
    //[safe] 
    //lpConfig->SetString("safe", "cert_pwd", "111111");
	

	g_szBusiness.SetConfig(lpConfig);
	//ͨ��T2SDK����������������ȡһ���µ�CConnection����ָ��
	g_pConnection = NewConnection(lpConfig);
	
	g_pConnection->AddRef();
	
	// �����Զ�����CCallback�Ķ����ڳ�ʼ�����Ӷ���ʱ�贫�ݴ˶����뿴������룩
	CCallback szCallback;
  
	int iRet = 0, iLogin = 0;
	
	
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
			////////////////////////////�ڻ�����///////////////////////////////////////
			///���ȶ���
	    	iLogin = g_szBusiness.Login();
		
			if( iLogin == 0 )
			{
			//��¼�ɹ����ȴ��ص�����Ӧ��.
WAIT:			
				if(!g_bLogin)
					goto WAIT;
			}
			
			if(g_iSystemNo >= 0)
			{
				int chose = 0;
				printf("------------------------------��ӭʹ��demo------------------------------\n");
				printf("1.����-�ڻ�ί�гɽ��ر�. 2.����-�ڻ���������. 3.����-�ڻ��������. \n4.����-�ڻ�������״̬��Ϣ.5.���ڻ�ί��.6.��ѯ�ڻ�ί��.\n7.ί�г���.8.��ѯ�ֲ�. 0.�˳���ϵͳ!\n��������Ĳ���:");
				while(scanf("%d",&chose),0 != chose)
				{
					getchar();
					switch (chose)
					{
						case 1: g_szBusiness.Subscribe(ISSUE_TYPE_REALTIME_FUTU);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						case 2: g_szBusiness.Subscribe(ISSUE_TYPE_MARKETINFO_FUTU);
							    printf("�밴�������ʾ���˵�!:\n");
							break;
						case 3: g_szBusiness.Subscribe(ISSUE_TYPE_ARGMARKETINFO_FUTU);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						case 4: g_szBusiness.Subscribe(ISSUE_TYPE_EXCHSTATUS_FUTU);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						case 5: g_szBusiness.FutuEntrust(g_iSystemNo);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						case 6: g_szBusiness.SearchFutuEntrust(g_iSystemNo);
								printf("�밴�������ʾ���˵�!:\n");
							break;
						case 7:g_szBusiness.WithdrawFutuEntrust(g_iSystemNo);
							    printf("�밴�������ʾ���˵�!:\n");
							break;
						case 8:g_szBusiness.SearchFutuHold(g_iSystemNo);
							    printf("�밴�������ʾ���˵�!:\n");
							break;
						default:printf("������Ĳ�������ȷ,��������ȷ�Ĳ���!\n");
								printf("�밴�������ʾ���˵�!:\n");
							break;
					}
					getchar();
					printf("1.����-�ڻ�ί�гɽ��ر�. 2.����-�ڻ���������. 3.����-�ڻ��������. n4.����-�ڻ�������״̬��Ϣ.5.���ڻ�ί��.6.��ѯ�ڻ�ί��.\n7.ί�г���.8.��ѯ�ֲ�. .�˳���ϵͳ!\n��������Ĳ���:");
				}				
			}
			else
				printf("ȡ��ϵͳ�ڵ��ʧ��, ���ؽ��: %d!\r\n", g_iSystemNo);
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

	///ȡ������
	g_szBusiness.CancelSub(ISSUE_TYPE_REALTIME_FUTU);      //ȡ������-�ڻ�ί�гɽ��ر�
	g_szBusiness.CancelSub(ISSUE_TYPE_MARKETINFO_FUTU);    //ȡ������-�ڻ���������
	g_szBusiness.CancelSub(ISSUE_TYPE_ARGMARKETINFO_FUTU); //ȡ������-�ڻ��������
	g_szBusiness.CancelSub(ISSUE_TYPE_EXCHSTATUS_FUTU);    //ȡ������-�ڻ�������״̬��Ϣ
	
	lpConfig->Release();
	
	g_pConnection->Close();
	g_pConnection->Release();
	
	return 0;
}