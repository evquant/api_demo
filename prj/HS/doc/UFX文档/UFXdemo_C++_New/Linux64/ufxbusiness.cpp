#include "ufxbusiness.h"
using namespace std;

UFXBusiness::UFXBusiness()
{
	iSubIndex = -1;
	HtConn = NULL;
	CConfigInterface* BusConfig = NewConfig();
	SubConfig = NewConfig();
	BusConfig->AddRef();
	SubConfig->AddRef();
	int isSucc = BusConfig->Load("t2sdk.ini");
	if (isSucc!=0)
		printf("加载文件失败");
	SubConfig->Load("subscriber.ini");
	BusinessConn = NewConnection(BusConfig);
	SubscribeConn = NewConnection(SubConfig);
	BusinessConn->AddRef();
	SubscribeConn->AddRef();
	int ret = BusinessConn->Create(NULL);
	if (ret != 0)
	{
		cout<<"业务接口连接失败："<<BusinessConn->GetErrorMsg(ret)<<endl;
		BusinessConn->Release();
		BusConfig->Release();
		return;
	}
	//SubCallBack = new CSubCallback();
	ret = SubscribeConn->Create(NULL);
	if (ret != 0)
	{
		cout<<"业务接口连接失败："<<BusinessConn->GetErrorMsg(ret)<<endl;
		SubscribeConn->Release();
		SubConfig->Release();
		return; 
	}
	cout<<"连接成功！"<<endl;
	lpSub=NULL;
}
UFXBusiness::~UFXBusiness()
{
	if(iSubIndex>=0)
	{
		lpSub->CancelSubscribeTopic(iSubIndex);
		printf("取消订阅成功\n");
		lpSub->Release();
	}
	printf("请稍等，系统正在退出。。。\n");
	SubscribeConn->Release();
	BusinessConn->Release();
	if (HtConn!=NULL)
	{
		HtConn->isRun = false;
		pthread_join(pthreadID,NULL);
		delete HtConn;
	}
}


int UFXBusiness::SubTopic()
{
	SubCallBack = new CSubCallback();
	char* bizName = (char*)SubConfig->GetString("subcribe","biz_name","");
	if (lpSub == NULL)
    	lpSub = SubscribeConn->NewSubscriber(SubCallBack,bizName,5000);
	if (!lpSub)
	{
		printf("创建订阅对象失败：%s",SubscribeConn->GetMCLastError());
		return -1;
	}
	lpSub->AddRef();
	CSubscribeParamInterface* lpSubParam = NewSubscribeParam();
	lpSubParam->AddRef();
	char* topicName = (char*)SubConfig->GetString("subcribe","topic_name","");
	lpSubParam->SetTopicName(topicName);
	lpSubParam->SetFromNow(true);
	lpSubParam->SetReplace(true);
	char Appdata[]= "I`m here,always!";
	lpSubParam->SetAppData(Appdata,16);
	int nCount = SubConfig->GetInt("subcribe","filter_count",0);
	for (int i=1;i<=nCount;i++)
	{
		char lName[128]={0};
		sprintf(lName,"filter_name%d",i);
		char* filterName = (char*)SubConfig->GetString("subcribe",lName,"");
		char lValue[128]={0};
		sprintf(lValue,"filter_value%d",i);
		char* filterValue = (char*)SubConfig->GetString("subcribe",lValue,"");
		lpSubParam->SetFilter(filterName,filterValue);
	}
	lpSubParam->SetSendInterval(0);
	IF2Packer* pack = NewPacker(2);
	pack->AddRef();
	//创建一个业务包
    pack->BeginPack();
    pack->AddField("login_operator_no");
	pack->AddField("password");
	pack->AddStr("1000");
	pack->AddStr("0");
	pack->EndPack();
	IF2UnPacker* lpBack = NULL;
	int subscribeIndex = 0;
	printf("开始订阅\n");
	int iRet = lpSub->SubscribeTopic(lpSubParam,5000,&lpBack,pack);
	if(iRet>0)
	{
		iSubIndex = iRet;	
		printf("订阅成功\n");
	}
	else
	{
		if(lpBack!=NULL)
			lpBack->Release();
		printf("订阅失败：%s。",SubscribeConn->GetErrorMsg(iRet));
	}
	return iRet;
}

IF2Packer* UFXBusiness::BuildLoginPack()
{
	IF2Packer* pLoginPack = NewPacker(2);
	pLoginPack->AddRef();
	pLoginPack->BeginPack();
	pLoginPack->AddField("operator_no",'S',16,0);
	pLoginPack->AddField("password",'S',32,0);
	pLoginPack->AddField("mac_address",'S',255,0);
	pLoginPack->AddField("op_station",'S',255,0);
	pLoginPack->AddField("ip_address",'S',255,0);
	pLoginPack->AddField("authorization_id",'S',64,0);
	pLoginPack->AddStr("1000");
	pLoginPack->AddStr("0");
	pLoginPack->AddStr("123456");
	pLoginPack->AddStr("192.168.50.167");
	pLoginPack->AddStr("192.168.50.167");
	pLoginPack->AddStr("hd123456");
	pLoginPack->EndPack();
	return pLoginPack;
}

IF2Packer* UFXBusiness::BuildEntrustPack()
{
	IF2Packer* pEnstPack = NewPacker(2);
	pEnstPack->AddRef();
	pEnstPack->BeginPack();
	pEnstPack->AddField("user_token",'S',512,0);
	pEnstPack->AddField("account_code");
	pEnstPack->AddField("combi_no",'S',8,0);
	pEnstPack->AddField("market_no",'S',3,0);
	pEnstPack->AddField("stock_code",'S',16,0);
	pEnstPack->AddField("entrust_direction",'S',1,0);
	pEnstPack->AddField("price_type",'S',1,0);
	pEnstPack->AddField("entrust_price",'F',9,3);
	pEnstPack->AddField("entrust_amount",'F',16,2);
	pEnstPack->AddStr(strToken.c_str());
	pEnstPack->AddStr("106000");
	pEnstPack->AddStr("106A1");
	pEnstPack->AddStr("1");
	pEnstPack->AddStr("600570");
	pEnstPack->AddStr("1");
	pEnstPack->AddStr("0");
	pEnstPack->AddInt(14);
	pEnstPack->AddInt(100);
	pEnstPack->EndPack();
	return pEnstPack;
}

IF2Packer* UFXBusiness::BuildQryEntrustPack()
{
	IF2Packer* pQryEnstPack = NewPacker(2);
	pQryEnstPack->AddRef();
	pQryEnstPack->BeginPack();
	pQryEnstPack->AddField("user_token",'S',512,0);
	pQryEnstPack->AddField("account_code",'S',32,0);
	pQryEnstPack->AddField("combi_no",'S',8,0);
	pQryEnstPack->AddField("request_num",'I',8,0);
	pQryEnstPack->AddStr(strToken.c_str());
	pQryEnstPack->AddStr("106000");
	pQryEnstPack->AddStr("106A1");
	pQryEnstPack->AddInt(50);
	pQryEnstPack->EndPack();
	return pQryEnstPack;
}

IF2Packer* UFXBusiness::BuildQryHoldPack()
{
	IF2Packer* pQryHPack = NewPacker(2);
	pQryHPack->AddRef();
	pQryHPack->BeginPack();
	pQryHPack->AddField("user_token",'S',512,0);
	pQryHPack->AddField("account_code",'S',32,0);
	pQryHPack->AddField("combi_no",'S',8,0);
	pQryHPack->AddField("market_no",'S',2,0);
	pQryHPack->AddStr(strToken.c_str());
	pQryHPack->AddStr("106000");
	pQryHPack->AddStr("106A1");
	pQryHPack->AddStr("1");
	pQryHPack->EndPack();
	return pQryHPack;
}

IF2Packer* UFXBusiness::BuildQryDealPack()
{
	IF2Packer* pQryDPack = NewPacker(2);
	pQryDPack->AddRef();
	pQryDPack->BeginPack();
	pQryDPack->AddField("user_token",'S',512,0);
	pQryDPack->AddField("account_code",'S',32,0);
	pQryDPack->AddField("combi_no",'S',8,0);
	pQryDPack->AddField("request_num",'I',8,0);
	pQryDPack->AddStr(strToken.c_str());
	pQryDPack->AddStr("106000");
	pQryDPack->AddStr("106A1");
	pQryDPack->AddInt(50);
	pQryDPack->EndPack();
	return pQryDPack;
}

IF2Packer* UFXBusiness::BuildWithdrawPack()
{
	IF2Packer* pWithDrawPack = NewPacker(2);
	pWithDrawPack->AddRef();
	pWithDrawPack->BeginPack();
	pWithDrawPack->AddField("user_token",'S',512,0);
	pWithDrawPack->AddField("entrust_no",'I',8,3);
	pWithDrawPack->AddStr(strToken.c_str());
	pWithDrawPack->AddInt(iEntrustNo);
	pWithDrawPack->EndPack();
	return pWithDrawPack;
}

IF2Packer* UFXBusiness::BuildLoginOffPack()
{
	IF2Packer* pLogOff = NewPacker(2);
	pLogOff->AddRef();
	pLogOff->BeginPack();
	pLogOff->AddField("user_token",'S',512,0);
	pLogOff->AddStr(strToken.c_str());
	pLogOff->EndPack();
	return pLogOff;
}

int UFXBusiness::CallService(int iFunc, IF2Packer* requestPack)
{
	int ret = BusinessConn->SendBiz(iFunc,requestPack);
	IF2UnPacker* repUnPacker = NULL;
	if (ret > 0)
	{
		void* Point=NULL;
		ret = BusinessConn->RecvBiz(ret,&Point,12000);
		switch(ret)
		{
			case 0:
			case 1:
				repUnPacker = (IF2UnPacker*)Point;
				PrintUnPack(iFunc,repUnPacker);
				break;
			case 2:
				printf("错误:%s/n",(char*)Point);
				break;
			default:
				printf("错误:%s/n",BusinessConn->GetErrorMsg(ret));
				break;
		}
	}
	else
	{
		printf("错误:%s/n",BusinessConn->GetErrorMsg(ret));
	}
	return ret;
}

void UFXBusiness::PrintUnPack(int iFunc, IF2UnPacker* unPacker)
{
	int i = 0, t = 0, j = 0, k = 0;
	for (i = 0; i < unPacker->GetDatasetCount(); ++i)
	{
		// 设置当前结果集
		unPacker->SetCurrentDatasetByIndex(i);
		if (iFunc == 10001 && i==1)
			strToken = unPacker->GetStr("user_token");
		if (iFunc == 91001 && i==1)
			iEntrustNo = unPacker->GetInt("entrust_no");
		// 打印字段
		for (t = 0; t < unPacker->GetColCount(); ++t)
		{
			printf("%20s", unPacker->GetColName(t));
		}												
		putchar('\n');	
		// 打印所有记录	
		for (j = 0; j < (int)unPacker->GetRowCount(); ++j)												
		{
			// 打印每条记录
			for (k = 0; k < unPacker->GetColCount(); ++k) 
			{																
	     		switch (unPacker->GetColType(k))					
				{										
					case 'I':
						printf("%20d", unPacker->GetIntByIndex(k));
						break;					
					case 'C':
						printf("%20c", unPacker->GetCharByIndex(k));	
						break;
					case 'S':
						printf("%20s", unPacker->GetStrByIndex(k));
						break;
     			   case 'F':
						printf("%20f", unPacker->GetDoubleByIndex(k));
						break;
				   case 'R':
						{		
							int nLength;
							void *lpData = unPacker->GetRawByIndex(k, &nLength);
							// 对2进制数据进行处理
							break;
						}					
				   default:
						// 未知数据类型	
						printf("未知数据类型。\n");
						break;										
				}
			}
			putchar('\n');			
			unPacker->Next();
		}
		putchar('\n');
	}
}

void* KeepConnection(void *lpParam)
{
	HeartConn* ConnParam = (HeartConn*)lpParam;
	IF2Packer* lpHeartBeat = NewPacker(2);
	lpHeartBeat->AddRef();
	lpHeartBeat->BeginPack();
	lpHeartBeat->AddField("user_token",'S',512,0);
	lpHeartBeat->AddStr(ConnParam->strToken.c_str());
	lpHeartBeat->EndPack();
	while(ConnParam->isRun)
	{
		sleep(30);
		int ret = ConnParam->ufxConn->SendBiz(10000,lpHeartBeat);
		if (ret>0)
		{
			void* point;
			ret = ConnParam->ufxConn->RecvBiz(ret,&point,3000);
			if(ret == 0)
			{
				IF2UnPacker* lp = (IF2UnPacker*)point;
				if(lp->GetInt("ErrorCode")==0)
				{
					printf("刷新心跳成功\n");
				}
			}
		}
		if (ret != 0)
		{
			printf("刷新心跳失败\n");
			break;
		}
	}
	lpHeartBeat->FreeMem(lpHeartBeat->GetPackBuf());
	lpHeartBeat->Release();
}

void UFXBusiness::HeartBeat()
{
	HtConn = new HeartConn();
	HtConn->strToken = strToken;
	HtConn->isRun = true;
	CConfigInterface* config = NewConfig();
	config->AddRef();
	config->Load("t2sdk.ini");
	HtConn->ufxConn = NewConnection(config);
	HtConn->ufxConn->AddRef();
	int ret = HtConn->ufxConn->Create(NULL);
	if (ret!=0)
	{
		printf("创建连接失败！");
		HtConn->ufxConn->Release();
		config->Release();
		delete HtConn;
		return;
	}
	
	ret = HtConn->ufxConn->Connect(3000);
	if (ret!=0)
	{
		printf("创建连接失败！");
		HtConn->ufxConn->Release();
		config->Release();
		delete HtConn;
		return;
	}
	pthread_create(&pthreadID,NULL,KeepConnection,HtConn);
}

void UFXBusiness::Login()
{
	IF2Packer* lpLogin = BuildLoginPack();
	int ret = CallService(10001,lpLogin);
	lpLogin->FreeMem(lpLogin->GetPackBuf());
	lpLogin->Release();
	HeartBeat();
}

void UFXBusiness::Entrust()
{
	IF2Packer* lpEntrust = BuildEntrustPack();
	int ret = CallService(91001,lpEntrust);
	lpEntrust->FreeMem(lpEntrust->GetPackBuf());
	lpEntrust->Release();
}

void UFXBusiness::QryEntrust()
{
	IF2Packer* lpQryEntrust = BuildQryEntrustPack();
	int ret = CallService(32001,lpQryEntrust);
	lpQryEntrust->FreeMem(lpQryEntrust->GetPackBuf());
	lpQryEntrust->Release();
}

void UFXBusiness::QryHold()
{
	IF2Packer* lpQryHold = BuildQryHoldPack();
	int ret = CallService(31001,lpQryHold);
	lpQryHold->FreeMem(lpQryHold->GetPackBuf());
	lpQryHold->Release();
}

void UFXBusiness::QryDeal()
{
	IF2Packer* lpQryDeal = BuildQryDealPack();
	int ret = CallService(33001,lpQryDeal);
	lpQryDeal->FreeMem(lpQryDeal->GetPackBuf());
	lpQryDeal->Release();
}

void UFXBusiness::WithDraw()
{
	IF2Packer* lpWithDraw = BuildWithdrawPack();
	int ret = CallService(91101,lpWithDraw);
	lpWithDraw->FreeMem(lpWithDraw->GetPackBuf());
	lpWithDraw->Release();
}

void UFXBusiness::Logoff()
{
	IF2Packer* lpLogOff = BuildLoginOffPack();
	int ret = CallService(10002,lpLogOff);
	lpLogOff->FreeMem(lpLogOff->GetPackBuf());
	lpLogOff->Release();
}

