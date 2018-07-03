#include <stdio.h>
#include "demo.h"
////////////////////////////////

int g_iSystemNo = -1;
bool g_bLogin = false;
char g_cUserToken[USERTOKENLEN] = {0};

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
#if 0
                        int nLength = 0;
                        void *lpData = pUnPacker->GetRawByIndex(k, &nLength);
						
                        // 对2进制数据进行处理
						if (16 == iIssueType || 17 == iIssueType || 18 == iIssueType)	//期货主推消息
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

///期货显示应答包结果
void GetStr(char* &dest, char* &source) //获取字符型
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
void GetInt(int &dest,char* &source) //获取整型
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
void GetDou(double &dest,char* & source) //获取浮点型
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
	if (issue_type == ISSUE_TYPE_MARKETINFO_FUTU) ///期货单腿变动行情
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
	else if (issue_type == ISSUE_TYPE_ARGMARKETINFO_FUTU)  ///期货组合变动行情
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
	else if (issue_type == ISSUE_TYPE_REALTIME_FUTU)  ///期货委托成交反馈信息
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
	else if (issue_type == ISSUE_TYPE_EXCHSTATUS_FUTU)  ///期货交易所状态信息
	{
		GetStr(futu_exch_type,QH);
		GetChar(exch_status,QH);
		GetChar(entrade_flag,QH);
		printf("futu_exch_type=%s,exch_status=%c,entrade_flag=%c,\n",futu_exch_type,exch_status,entrade_flag);
	}
}

/////////////////////////////////////////////////////////////
//CBusiness 实现
CBusiness::CBusiness()
{
	memset(g_cUserToken, 0, sizeof(g_cUserToken));
}

CBusiness::~CBusiness()
{
	
}

//登录
int CBusiness::Login()
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
	pPacker->AddField("input_content", 'C');      
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);    
	pPacker->AddField("password", 'S', 10);      
	pPacker->AddField("password_type", 'C');     
	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('L');					
	///根据券商或是期货商要求填写
	pPacker->AddStr("1");					
	pPacker->AddInt(1);				
	pPacker->AddChar('1');					
	pPacker->AddStr("302000007");			
	pPacker->AddStr("0");					
	pPacker->AddStr("123456");				
	pPacker->AddChar(/*'2'*/'\0');			
	///结束打包
	pPacker->EndPack();
	
	if((hSend = g_pConnection->SendBiz(331100,pPacker, ASYNCSEND, 0)) < 0)
	{
		printf("发送功能331100失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能331100成功, 返回接收句柄: %d!\r\n", hSend);

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

//期货委托
int CBusiness::FutuEntrust(int iSystemNo)
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
	///加入对应的字段值
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
	///结束打包
	pPacker->EndPack();
		
	if((hSend = g_pConnection->SendBiz(338202,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("发送功能338202失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("发送功能338202成功, 返回接收句柄: %d!\r\n", hSend);
	
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

//期货UFT持仓查询
int CBusiness::SearchFutuHold(int iSystemNo /* = 2 */)
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
	LPRET_DATA  pRetData = NULL;
	
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
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
	
	///加入对应的字段值
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
	///结束打包
	pPacker->EndPack();

	//同步发送持仓查询功能
	if((hSend = g_pConnection->SendBiz(338303,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("发送功能338303失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("发送功能338303成功, 返回接收句柄: %d!\r\n", hSend);
	
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

//期货UFT委托查询
int CBusiness::SearchFutuEntrust(int iSystemNo /* = 2 */)
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
	
	
	///加入对应的字段值
	pPacker->AddInt(0);					     	
	pPacker->AddChar('L');					    
	///根据券商或是期货商要求填写
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
	///结束打包
	pPacker->EndPack();
	
	//异步发送委托查询功能
	if((hSend = g_pConnection->SendBiz(338301,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("发送功能338301失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能338301成功, 返回接收句柄: %d!\r\n", hSend);
	
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

int CBusiness::WithdrawFutuEntrust(int iSystemNo /* = 2 */)
{
	int iRet = 0, hSend = 0, iEntrustNo = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker =NewPacker(2);
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
    ///加入对应的字段值
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
	///结束打包
	pPacker->EndPack();

	///异步发送撤单功能
	if((hSend = g_pConnection->SendBiz(338217,pPacker, ASYNCSEND, iSystemNo)) < 0)
	{
		printf("发送功能338217失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能338217成功, 返回值: %d!\r\n", hSend);

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
	pPacker->AddField("contract_code",'S',30);    
	pPacker->AddField("arbicontract_id",'S',30); 
	pPacker->AddField("futu_exch_type",'S',4);	  
	///加入对应的字段值
	pPacker->AddInt(1);						
	pPacker->AddStr("302000007");				
	
	///以下为期货UFT需要的字段
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
	///结束打包
	pPacker->EndPack();

	REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issue_type;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///订阅
	if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, ASYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	{
		printf("发送功能620001失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	printf("发送功能620001成功, 返回接收句柄: %d!\r\n", hSend);

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

int CBusiness::CancelSub(int issueType)
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

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("user_token",'S',40);  
	///加入对应的字段值
	pPacker->AddInt(1);					
	pPacker->AddStr("302000007");			
	pPacker->AddStr(g_cUserToken);		    
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
	
		if((ret = g_pConnection->SendBizEx(620000, NULL, NULL, ASYNCSEND, 0, 0, 0, &reqData)) < 0)
		{
			puts(g_pConnection->GetErrorMsg(ret));
		}
	}
	else if(620001 == iFuncID)	//订阅返回消息
	{
		puts("收到订阅返回消息620001:");
		IF2UnPacker * lpUnapck = NULL;
		if (lpRetData != NULL && ( lpRetData->issueType == ISSUE_TYPE_MARKETINFO_FUTU || lpRetData->issueType == ISSUE_TYPE_ARGMARKETINFO_FUTU ||lpRetData->issueType == ISSUE_TYPE_REALTIME_FUTU || lpRetData->issueType == ISSUE_TYPE_EXCHSTATUS_FUTU))	//成交主推
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
	else if(620003 == iFuncID)	//成交主推
	{
		//收到成交主推消息
		puts("收到主推消息620003:");
		IF2UnPacker * lpUnapck = NULL;
		if (lpRetData != NULL)	//成交主推
		{
			int issue_type = lpRetData->issueType;
			if(issue_type == ISSUE_TYPE_MARKETINFO_FUTU || issue_type == ISSUE_TYPE_ARGMARKETINFO_FUTU ||
				issue_type == ISSUE_TYPE_REALTIME_FUTU || issue_type == ISSUE_TYPE_EXCHSTATUS_FUTU)    ///期货
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
	else if(iFuncID == 338301 || iFuncID == 338202 || iFuncID == 338303 || iFuncID == 338217)	///委托查询异步应答处理
	{
		ShowPacket(0, (IF2UnPacker *)lpUnpackerOrStr);
	}
	printf("请按任意键显示主菜单!:\n");
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

CBusiness g_szBusiness;

int main()
{

//	HANDLE hThread = NULL;

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

	
	//设置以最后一次为准
    //[t2sdk] servers指定需要连接的IP地址及端口，可配置多个，中间以“;”间隔//111.207.179.6:8066  18.8.1.171:8066
    //lpConfig->SetString("t2sdk", "servers", "192.168.51.162:3335");
	
    //[t2sdk] license_file指定许可证文件路径 HS-HUNDSUN001-FBASE2-0000_3rd.dat
    //lpConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");
	
    //[t2sdk] lang指定错误信息的语言号（缺省为简体中文2052），1033为英文
    //lpConfig->SetString("t2sdk", "lang", "1033");
	
    //[t2sdk] send_queue_size指定T2_SDK的发送队列大小
    //lpConfig->SetString("t2sdk", "send_queue_size", "100");
    
    //[t2sdk] send_queue_size指定T2_SDK的发送队列大小
    //lpConfig->SetString("t2sdk", "license_pwd", "888888");
	
    //[safe] safe_level指定连接的安全模式，需要和T2通道的安全模式一致，否则连接失败
    //lpConfig->SetString("safe", "safe_level", "ssl");
    
    //[safe] 
    //lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
    
    //[safe] 
    //lpConfig->SetString("safe", "cert_pwd", "111111");
	

	g_szBusiness.SetConfig(lpConfig);
	//通过T2SDK的引出函数，来获取一个新的CConnection对象指针
	g_pConnection = NewConnection(lpConfig);
	
	g_pConnection->AddRef();
	
	// 创建自定义类CCallback的对象（在初始化连接对象时需传递此对象，请看下面代码）
	CCallback szCallback;
  
	int iRet = 0, iLogin = 0;
	
	
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
			////////////////////////////期货测试///////////////////////////////////////
			///首先订阅
	    	iLogin = g_szBusiness.Login();
		
			if( iLogin == 0 )
			{
			//登录成功，等待回调函数应答.
WAIT:			
				if(!g_bLogin)
					goto WAIT;
			}
			
			if(g_iSystemNo >= 0)
			{
				int chose = 0;
				printf("------------------------------欢迎使用demo------------------------------\n");
				printf("1.订阅-期货委托成交回报. 2.订阅-期货单腿行情. 3.订阅-期货组合行情. \n4.订阅-期货交易所状态信息.5.下期货委托.6.查询期货委托.\n7.委托撤单.8.查询持仓. 0.退出该系统!\n请输入你的操作:");
				while(scanf("%d",&chose),0 != chose)
				{
					getchar();
					switch (chose)
					{
						case 1: g_szBusiness.Subscribe(ISSUE_TYPE_REALTIME_FUTU);
								printf("请按任意键显示主菜单!:\n");
							break;
						case 2: g_szBusiness.Subscribe(ISSUE_TYPE_MARKETINFO_FUTU);
							    printf("请按任意键显示主菜单!:\n");
							break;
						case 3: g_szBusiness.Subscribe(ISSUE_TYPE_ARGMARKETINFO_FUTU);
								printf("请按任意键显示主菜单!:\n");
							break;
						case 4: g_szBusiness.Subscribe(ISSUE_TYPE_EXCHSTATUS_FUTU);
								printf("请按任意键显示主菜单!:\n");
							break;
						case 5: g_szBusiness.FutuEntrust(g_iSystemNo);
								printf("请按任意键显示主菜单!:\n");
							break;
						case 6: g_szBusiness.SearchFutuEntrust(g_iSystemNo);
								printf("请按任意键显示主菜单!:\n");
							break;
						case 7:g_szBusiness.WithdrawFutuEntrust(g_iSystemNo);
							    printf("请按任意键显示主菜单!:\n");
							break;
						case 8:g_szBusiness.SearchFutuHold(g_iSystemNo);
							    printf("请按任意键显示主菜单!:\n");
							break;
						default:printf("您输入的操作不正确,请输入正确的操作!\n");
								printf("请按任意键显示主菜单!:\n");
							break;
					}
					getchar();
					printf("1.订阅-期货委托成交回报. 2.订阅-期货单腿行情. 3.订阅-期货组合行情. n4.订阅-期货交易所状态信息.5.下期货委托.6.查询期货委托.\n7.委托撤单.8.查询持仓. .退出该系统!\n请输入你的操作:");
				}				
			}
			else
				printf("取得系统节点号失败, 返回结果: %d!\r\n", g_iSystemNo);
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

	///取消订阅
	g_szBusiness.CancelSub(ISSUE_TYPE_REALTIME_FUTU);      //取消订阅-期货委托成交回报
	g_szBusiness.CancelSub(ISSUE_TYPE_MARKETINFO_FUTU);    //取消订阅-期货单腿行情
	g_szBusiness.CancelSub(ISSUE_TYPE_ARGMARKETINFO_FUTU); //取消订阅-期货组合行情
	g_szBusiness.CancelSub(ISSUE_TYPE_EXCHSTATUS_FUTU);    //取消订阅-期货交易所状态信息
	
	lpConfig->Release();
	
	g_pConnection->Close();
	g_pConnection->Release();
	
	return 0;
}