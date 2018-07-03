
#include "HSHeader.h"
#include "HSSubApi.h"
#include "HSTradeApi.h"



namespace kiiik {

	

	string gOperatorNo  = "00300000";  //操作员
	string gPassword    = "pb88888";         //操作员密码
	string gUserToken;
	int    gBatchNo     = 500000;     //委托批号(如果要自己指定请注意: O32的有效委托批号必须在1到5000000之间, UFT的委托批号必须大于5000000)
	int    gEntrustNo   = 0;
	int    gEntrustNo2  = 0;
	bool   gbIsRun = false;
	map<int,CSubscribeParamInterface*> gAllSubscribeParam;
	HANDLE ghThreadReportProcess = NULL;

	int Connect(const char* serverAddr, CConnectionInterface** connection, HSTradeApi* callback)
	{
		//CConfigInterface、CConnectionInterface的使用可以参考《T2SDK 外部版开发指南.docx》
		//创建T2SDK配置对象，并设置UFX服务器地址以及License文件
		CConfigInterface * lpConfig = NewConfig();
		lpConfig->AddRef();
		lpConfig->SetString("t2sdk", "servers", serverAddr);
		lpConfig->SetString("t2sdk", "license_file", "license.dat");

		//创建连接对象，并注册回调
		(*connection) = NewConnection(lpConfig);
		(*connection)->AddRef();
		int ret = (*connection)->Create2BizMsg(callback);
		if (ret != 0)
		{
			lpConfig->Release();
			return ret;
		}

		//连接UFX服务器，参数3000为超时参数，单位毫秒
		ret = (*connection)->Connect(9000); 
		if (ret != 0)
		{
			lpConfig->Release();
			return ret;
		}

		lpConfig->Release();
		return ret;
	}

	void ShowPacket(IF2UnPacker* unPacker)
	{
		int i = 0, t = 0, j = 0, k = 0;

		for (i = 0; i < unPacker->GetDatasetCount(); ++i)
		{
			// 设置当前结果集
			unPacker->SetCurrentDatasetByIndex(i);

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
							int nLength = 0;
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

	void ShowSubscribe(int subIndex,LPSUBSCRIBE_RECVDATA lpRecvData)
	{
		map<int,CSubscribeParamInterface*>::iterator it = gAllSubscribeParam.find(subIndex);
		if (it == gAllSubscribeParam.end())
		{
			printf("没有这个订阅项\n");
			return;
		}
		CSubscribeParamInterface* lpSubParam = (*it).second;

		printf("----------订阅项部分-------\n");
		printf("主题名字：           %s\n",lpSubParam->GetTopicName());
		printf("附加数据长度：       %d\n",lpRecvData->iAppDataLen);
		if (lpRecvData->iAppDataLen > 0)
		{
			printf("附加数据：           %s\n",lpRecvData->lpAppData);
		}
		printf("过滤字段部分：\n");
		if (lpRecvData->iFilterDataLen > 0)
		{
			IF2UnPacker* lpUnpack = NewUnPacker(lpRecvData->lpFilterData,lpRecvData->iFilterDataLen);
			lpUnpack->AddRef();
			ShowPacket(lpUnpack);
			lpUnpack->Release();
		}
		printf("---------------------------\n");
	}

	int SubConnect(CConnectionInterface** connection, HSTradeApi* callback, CSubscribeInterface** subscribe, HSSubApi* subcallback)
	{
		//通过T2SDK的引出函数，来获取一个新的CConfig对象指针
		//此对象在创建连接对象时被传递，用于配置所创建的连接对象的各种属性（比如服务器IP地址、安全模式等）
		//值得注意的是，在向配置对象设置配置信息时，配置信息既可以从ini文件中载入，
		//也可以在程序代码中设定，或者是2者的混合，如果对同一个配置项设不同的值，则以最近一次设置为准
		CConfigInterface * lpConfig = NewConfig();
		lpConfig->AddRef();
		lpConfig->Load("subscriber.ini");
		//如果需要使用发布订阅功能，必须配置配置mc标签下面的client_name项，配置文件里面有了，不需要下面这句代码添加
		//lpConfig->SetString("mc","client_name","xuxp");

		//创建连接对象，并注册回调(连接地址和端口在subscriber.ini文件中配置)
		(*connection) = NewConnection(lpConfig);
		(*connection)->AddRef();
		int ret = (*connection)->Create2BizMsg(callback);
		if (ret != 0)
		{
			cout << "Create2BizMsg Error: " << (*connection)->GetErrorMsg(ret) << endl;
			lpConfig->Release();
			return ret;
		}

		//连接UFX服务器，参数3000为超时参数，单位毫秒
		ret = (*connection)->Connect(3000);
		if (ret != 0)
		{
			cout << "Connect Error: " << (*connection)->GetErrorMsg(ret) << endl;
			lpConfig->Release();
			return ret;
		}

		//创建订阅对象
		char* bizName = (char*)lpConfig->GetString("subcribe","biz_name","");
		(*subscribe) = (*connection)->NewSubscriber(subcallback,bizName,5000);
		if ((*subscribe) == NULL)
		{
			cout << "NewSubscribe Error: " << (*connection)->GetMCLastError() << endl;
			lpConfig->Release();
			return -1;
		}
		(*subscribe)->AddRef();

		/****************************** 获取订阅参数 ******************************/
		CSubscribeParamInterface* lpSubscribeParam = NewSubscribeParam();
		lpSubscribeParam->AddRef();

		char* topicName = (char*)lpConfig->GetString("subcribe","topic_name","");//主题名字
		lpSubscribeParam->SetTopicName(topicName);

		char* isFromNow = (char*)lpConfig->GetString("subcribe","is_rebulid","");//是否补缺
		if (strcmp(isFromNow,"true")==0)
		{
			lpSubscribeParam->SetFromNow(true);
		}
		else
		{
			lpSubscribeParam->SetFromNow(false);
		}

		char* isReplace = (char*)lpConfig->GetString("subcribe","is_replace","");//是否覆盖
		if (strcmp(isReplace,"true")==0)
		{
			lpSubscribeParam->SetReplace(true);
		}
		else
		{
			lpSubscribeParam->SetReplace(false);
		}

		char* lpApp = "lixuebin";
		lpSubscribeParam->SetAppData(lpApp,8);//添加附加数据

		//添加过滤字段
		int nCount = lpConfig->GetInt("subcribe","filter_count",0);
		for (int i=1;i<=nCount;i++)
		{
			char lName[128]={0};
			sprintf(lName,"filter_name%d",i);
			char* filterName = (char*)lpConfig->GetString("subcribe",lName,"");
			char lValue[128]={0};
			sprintf(lValue,"filter_value%d",i);
			char* filterValue = (char*)lpConfig->GetString("subcribe",lValue,"");
			lpSubscribeParam->SetFilter(filterName,filterValue);
		}

		//添加发送频率
		lpSubscribeParam->SetSendInterval(lpConfig->GetInt("subcribe","send_interval",0));

		//添加返回字段
		nCount = lpConfig->GetInt("subcribe","return_count",0);
		for (int k=1;k<=nCount;k++)
		{
			char lName[128]={0};
			sprintf(lName,"return_filed%d",k);
			char* filedName = (char*)lpConfig->GetString("subcribe",lName,"");
			lpSubscribeParam->SetReturnFiled(filedName);
		}

		//创建一个业务包
		IF2Packer* pack = NewPacker(2);
		pack->AddRef();
		pack->BeginPack();
		pack->AddField("login_operator_no");
		pack->AddField("password");
		pack->AddStr(gOperatorNo.c_str());//这里填你的操作员
		pack->AddStr(gPassword.c_str());//这里填你的操作员密码
		pack->EndPack();
		IF2UnPacker* lpBack = NULL;

		cout << "开始订阅" << endl;
		int iRet = (*subscribe)->SubscribeTopic(lpSubscribeParam,5000,&lpBack,pack);
		if (iRet <= 0)
		{
			if (lpBack != NULL) ShowPacket(lpBack);
			cout << "SubscribeTopic info:[" << iRet << "] " << (*connection)->GetErrorMsg(iRet) << endl;
			pack->FreeMem(pack->GetPackBuf());
			pack->Release();
			lpSubscribeParam->Release();
			lpConfig->Release();
			return -1;
		}
		int subscribeIndex = iRet;
		cout << "SubscribeTopic info:[" << iRet << "] 成功" << endl;
		gAllSubscribeParam[subscribeIndex] = lpSubscribeParam;//保存到map中，用于以后的取消订阅

		pack->FreeMem(pack->GetPackBuf());
		pack->Release();
		lpConfig->Release();
		return ret;
	}

	int CallService(CConnectionInterface* connection, IBizMessage* requestBizMessage)
	{
		return connection->SendBizMsg(requestBizMessage,1);
	}

	IF2Packer* MakeLoginPacker()
	{
		//业务请求消息是由接口规范定义的，可以参考《恒生投资管理系统O3.2_周边接口规范_x.x.x.x.xls》
		IF2Packer* requestPacker = NewPacker(2);
		requestPacker->AddRef();
		requestPacker->BeginPack();
		requestPacker->AddField("operator_no",     'S',16, 0);
		requestPacker->AddField("password",        'S',32, 0);
		requestPacker->AddField("mac_address",     'S',255,0);
		requestPacker->AddField("op_station",      'S',255,0);
		requestPacker->AddField("ip_address",      'S',32, 0);
		requestPacker->AddField("authorization_id",'S',64, 0);
		requestPacker->AddStr(gOperatorNo.c_str());
		requestPacker->AddStr(gPassword.c_str());
		requestPacker->AddStr("123456");
		requestPacker->AddStr("1");
		requestPacker->AddStr("192.168.113.128");
		requestPacker->AddStr("hd123456");
		requestPacker->EndPack();
		return requestPacker;
	}

	int Login(CConnectionInterface* connection)
	{
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		//登录功能号：10001
		lpBizMessage->SetFunction(10001);
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		IF2Packer* lpPacker = MakeLoginPacker();
		lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
		int ret = CallService(connection,lpBizMessage);
		lpPacker->FreeMem(lpPacker->GetPackBuf());
		lpPacker->Release();
		lpBizMessage->Release();
		return ret;
	}

	IF2Packer* MakeSingleEntrustPacker()
	{
		IF2Packer* requestPacker = NewPacker(2);
		requestPacker->AddRef();
		requestPacker->BeginPack();
		requestPacker->AddField("user_token",        'S',512,0);
		requestPacker->AddField("batch_no",          'I',8,0);
		requestPacker->AddField("account_code",      'S',32,0);
		requestPacker->AddField("combi_no"  ,        'S',8,0); 
		requestPacker->AddField("market_no" ,        'S',3,0);
		requestPacker->AddField("stock_code",        'S',16,0);
		requestPacker->AddField("entrust_direction", 'S',1,0);
		requestPacker->AddField("price_type",        'S',1,0);
		requestPacker->AddField("entrust_price",     'F',9,3);
		requestPacker->AddField("entrust_amount",    'F',16,2);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gBatchNo);
		requestPacker->AddStr("106000");
		requestPacker->AddStr("106A1");
		requestPacker->AddStr("1");
		requestPacker->AddStr("600000");
		requestPacker->AddStr("1");
		requestPacker->AddStr("0");                  //限价
		requestPacker->AddDouble(13.92);
		requestPacker->AddDouble(100);
		requestPacker->EndPack();
		return requestPacker;
	}

	int SingleEntrust(CConnectionInterface* connection)
	{
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		//委托功能号：91001，接口功能号及其输入输出参数定义可以参考《恒生投资管理系统O3.2_周边接口规范_x.x.x.x.xls》
		lpBizMessage->SetFunction(91001);
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		IF2Packer* lpPacker = MakeSingleEntrustPacker();
		lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
		int ret = CallService(connection,lpBizMessage);
		lpPacker->FreeMem(lpPacker->GetPackBuf());
		lpPacker->Release();
		lpBizMessage->Release();
		return ret;
	}

	IF2Packer* MakeBatchEntrustPacker()
	{
		IF2Packer* requestPacker = NewPacker(2);
		requestPacker->AddRef();
		requestPacker->BeginPack();
		requestPacker->AddField("user_token",        'S',512,0);
		requestPacker->AddField("batch_no",          'I',8,0);
		requestPacker->AddField("account_code",      'S',32,0);
		requestPacker->AddField("combi_no"  ,        'S',8,0); 
		requestPacker->AddField("market_no" ,        'S',3,0);
		requestPacker->AddField("stock_code",        'S',16,0);
		requestPacker->AddField("entrust_direction", 'S',1,0);
		requestPacker->AddField("futures_direction", 'S',1,0);
		requestPacker->AddField("price_type",        'S',1,0);
		requestPacker->AddField("entrust_price",     'F',9,3);
		requestPacker->AddField("entrust_amount",    'F',16,2);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gBatchNo);
		requestPacker->AddStr("106000");
		requestPacker->AddStr("106A1");
		requestPacker->AddStr("1");
		requestPacker->AddStr("600000");
		requestPacker->AddStr("1");
		requestPacker->AddStr("");                   //股票没有开平方向,所以传空
		requestPacker->AddStr("0");                  //限价
		requestPacker->AddDouble(13.92);
		requestPacker->AddDouble(100);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gBatchNo);
		requestPacker->AddStr("106000");
		requestPacker->AddStr("106A1");
		requestPacker->AddStr("2");
		requestPacker->AddStr("000001");
		requestPacker->AddStr("1");
		requestPacker->AddStr("");                   //股票没有开平方向,所以传空
		requestPacker->AddStr("0");                  //限价
		requestPacker->AddDouble(14.00);
		requestPacker->AddDouble(100);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gBatchNo);
		requestPacker->AddStr("106000");
		requestPacker->AddStr("106FA1");
		requestPacker->AddStr("7");
		requestPacker->AddStr("IF1503");
		requestPacker->AddStr("1");                   //买入
		requestPacker->AddStr("1");                   //开仓
		requestPacker->AddStr("0");                   //限价
		requestPacker->AddDouble(3622.8);
		requestPacker->AddDouble(1);
		requestPacker->EndPack();
		return requestPacker;
	}

	int BasketEntrust(CConnectionInterface* connection)
	{
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		//委托功能号：91090，接口功能号及其输入输出参数定义可以参考《恒生投资管理系统O3.2_周边接口规范_x.x.x.x.xls》
		lpBizMessage->SetFunction(91090);
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		IF2Packer* lpPacker = MakeBatchEntrustPacker();
		lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
		int ret = CallService(connection,lpBizMessage);
		lpPacker->FreeMem(lpPacker->GetPackBuf());
		lpPacker->Release();
		lpBizMessage->Release();
		return ret;
	}

	IF2Packer* MakeWithdrawPacker()
	{
		IF2Packer* requestPacker = NewPacker(2);
		requestPacker->AddRef();
		requestPacker->BeginPack();
		requestPacker->AddField("user_token",'S',512,0);
		requestPacker->AddField("entrust_no",'I');
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gEntrustNo);
		requestPacker->EndPack();
		return requestPacker;
	}

	int Withdraw(CConnectionInterface* connection)
	{
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		//撤单功能号：91101
		lpBizMessage->SetFunction(91101);
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		IF2Packer* lpPacker = MakeWithdrawPacker();
		lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
		int ret = CallService(connection,lpBizMessage);
		lpPacker->FreeMem(lpPacker->GetPackBuf());
		lpPacker->Release();
		lpBizMessage->Release();
		return ret;
	}

	IF2Packer* MakeHeartbeatPacker()
	{
		IF2Packer* requestPacker = NewPacker(2);
		requestPacker->AddRef();
		requestPacker->BeginPack();
		requestPacker->AddField("user_token",'S',512,0);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->EndPack();
		return requestPacker;
	}

	void runHeartbeat(CConnectionInterface* connection)
	{
		while (gbIsRun)
		{
			if (!gUserToken.empty())
			{
				IBizMessage* lpBizMessage = NewBizMessage();
				lpBizMessage->AddRef();
				//登录功能号：10000
				lpBizMessage->SetFunction(10000);
				lpBizMessage->SetPacketType(REQUEST_PACKET);
				IF2Packer* lpPacker = MakeHeartbeatPacker();
				lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
				CallService(connection,lpBizMessage);
				lpPacker->FreeMem(lpPacker->GetPackBuf());
				lpPacker->Release();
				lpBizMessage->Release();
			}

			Sleep(500000); //发心跳的时间间隔,根据需要设置
		}
	}

};