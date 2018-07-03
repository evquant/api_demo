
#include "HSHeader.h"
#include "HSSubApi.h"
#include "HSTradeApi.h"



namespace kiiik {

	

	string gOperatorNo  = "00300000";  //����Ա
	string gPassword    = "pb88888";         //����Ա����
	string gUserToken;
	int    gBatchNo     = 500000;     //ί������(���Ҫ�Լ�ָ����ע��: O32����Чί�����ű�����1��5000000֮��, UFT��ί�����ű������5000000)
	int    gEntrustNo   = 0;
	int    gEntrustNo2  = 0;
	bool   gbIsRun = false;
	map<int,CSubscribeParamInterface*> gAllSubscribeParam;
	HANDLE ghThreadReportProcess = NULL;

	int Connect(const char* serverAddr, CConnectionInterface** connection, HSTradeApi* callback)
	{
		//CConfigInterface��CConnectionInterface��ʹ�ÿ��Բο���T2SDK �ⲿ�濪��ָ��.docx��
		//����T2SDK���ö��󣬲�����UFX��������ַ�Լ�License�ļ�
		CConfigInterface * lpConfig = NewConfig();
		lpConfig->AddRef();
		lpConfig->SetString("t2sdk", "servers", serverAddr);
		lpConfig->SetString("t2sdk", "license_file", "license.dat");

		//�������Ӷ��󣬲�ע��ص�
		(*connection) = NewConnection(lpConfig);
		(*connection)->AddRef();
		int ret = (*connection)->Create2BizMsg(callback);
		if (ret != 0)
		{
			lpConfig->Release();
			return ret;
		}

		//����UFX������������3000Ϊ��ʱ��������λ����
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
			// ���õ�ǰ�����
			unPacker->SetCurrentDatasetByIndex(i);

			// ��ӡ�ֶ�
			for (t = 0; t < unPacker->GetColCount(); ++t)
			{
				printf("%20s", unPacker->GetColName(t));
			}

			putchar('\n');

			// ��ӡ���м�¼
			for (j = 0; j < (int)unPacker->GetRowCount(); ++j)
			{
				// ��ӡÿ����¼
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

							// ��2�������ݽ��д���
							break;
						}

					default:
						// δ֪��������
						printf("δ֪�������͡�\n");
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
			printf("û�����������\n");
			return;
		}
		CSubscribeParamInterface* lpSubParam = (*it).second;

		printf("----------�������-------\n");
		printf("�������֣�           %s\n",lpSubParam->GetTopicName());
		printf("�������ݳ��ȣ�       %d\n",lpRecvData->iAppDataLen);
		if (lpRecvData->iAppDataLen > 0)
		{
			printf("�������ݣ�           %s\n",lpRecvData->lpAppData);
		}
		printf("�����ֶβ��֣�\n");
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
		//ͨ��T2SDK����������������ȡһ���µ�CConfig����ָ��
		//�˶����ڴ������Ӷ���ʱ�����ݣ��������������������Ӷ���ĸ������ԣ����������IP��ַ����ȫģʽ�ȣ�
		//ֵ��ע����ǣ��������ö�������������Ϣʱ��������Ϣ�ȿ��Դ�ini�ļ������룬
		//Ҳ�����ڳ���������趨��������2�ߵĻ�ϣ������ͬһ���������費ͬ��ֵ���������һ������Ϊ׼
		CConfigInterface * lpConfig = NewConfig();
		lpConfig->AddRef();
		lpConfig->Load("subscriber.ini");
		//�����Ҫʹ�÷������Ĺ��ܣ�������������mc��ǩ�����client_name������ļ��������ˣ�����Ҫ�������������
		//lpConfig->SetString("mc","client_name","xuxp");

		//�������Ӷ��󣬲�ע��ص�(���ӵ�ַ�Ͷ˿���subscriber.ini�ļ�������)
		(*connection) = NewConnection(lpConfig);
		(*connection)->AddRef();
		int ret = (*connection)->Create2BizMsg(callback);
		if (ret != 0)
		{
			cout << "Create2BizMsg Error: " << (*connection)->GetErrorMsg(ret) << endl;
			lpConfig->Release();
			return ret;
		}

		//����UFX������������3000Ϊ��ʱ��������λ����
		ret = (*connection)->Connect(3000);
		if (ret != 0)
		{
			cout << "Connect Error: " << (*connection)->GetErrorMsg(ret) << endl;
			lpConfig->Release();
			return ret;
		}

		//�������Ķ���
		char* bizName = (char*)lpConfig->GetString("subcribe","biz_name","");
		(*subscribe) = (*connection)->NewSubscriber(subcallback,bizName,5000);
		if ((*subscribe) == NULL)
		{
			cout << "NewSubscribe Error: " << (*connection)->GetMCLastError() << endl;
			lpConfig->Release();
			return -1;
		}
		(*subscribe)->AddRef();

		/****************************** ��ȡ���Ĳ��� ******************************/
		CSubscribeParamInterface* lpSubscribeParam = NewSubscribeParam();
		lpSubscribeParam->AddRef();

		char* topicName = (char*)lpConfig->GetString("subcribe","topic_name","");//��������
		lpSubscribeParam->SetTopicName(topicName);

		char* isFromNow = (char*)lpConfig->GetString("subcribe","is_rebulid","");//�Ƿ�ȱ
		if (strcmp(isFromNow,"true")==0)
		{
			lpSubscribeParam->SetFromNow(true);
		}
		else
		{
			lpSubscribeParam->SetFromNow(false);
		}

		char* isReplace = (char*)lpConfig->GetString("subcribe","is_replace","");//�Ƿ񸲸�
		if (strcmp(isReplace,"true")==0)
		{
			lpSubscribeParam->SetReplace(true);
		}
		else
		{
			lpSubscribeParam->SetReplace(false);
		}

		char* lpApp = "lixuebin";
		lpSubscribeParam->SetAppData(lpApp,8);//��Ӹ�������

		//��ӹ����ֶ�
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

		//��ӷ���Ƶ��
		lpSubscribeParam->SetSendInterval(lpConfig->GetInt("subcribe","send_interval",0));

		//��ӷ����ֶ�
		nCount = lpConfig->GetInt("subcribe","return_count",0);
		for (int k=1;k<=nCount;k++)
		{
			char lName[128]={0};
			sprintf(lName,"return_filed%d",k);
			char* filedName = (char*)lpConfig->GetString("subcribe",lName,"");
			lpSubscribeParam->SetReturnFiled(filedName);
		}

		//����һ��ҵ���
		IF2Packer* pack = NewPacker(2);
		pack->AddRef();
		pack->BeginPack();
		pack->AddField("login_operator_no");
		pack->AddField("password");
		pack->AddStr(gOperatorNo.c_str());//��������Ĳ���Ա
		pack->AddStr(gPassword.c_str());//��������Ĳ���Ա����
		pack->EndPack();
		IF2UnPacker* lpBack = NULL;

		cout << "��ʼ����" << endl;
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
		cout << "SubscribeTopic info:[" << iRet << "] �ɹ�" << endl;
		gAllSubscribeParam[subscribeIndex] = lpSubscribeParam;//���浽map�У������Ժ��ȡ������

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
		//ҵ��������Ϣ���ɽӿڹ淶����ģ����Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
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
		//��¼���ܺţ�10001
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
		requestPacker->AddStr("0");                  //�޼�
		requestPacker->AddDouble(13.92);
		requestPacker->AddDouble(100);
		requestPacker->EndPack();
		return requestPacker;
	}

	int SingleEntrust(CConnectionInterface* connection)
	{
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		//ί�й��ܺţ�91001���ӿڹ��ܺż��������������������Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
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
		requestPacker->AddStr("");                   //��Ʊû�п�ƽ����,���Դ���
		requestPacker->AddStr("0");                  //�޼�
		requestPacker->AddDouble(13.92);
		requestPacker->AddDouble(100);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gBatchNo);
		requestPacker->AddStr("106000");
		requestPacker->AddStr("106A1");
		requestPacker->AddStr("2");
		requestPacker->AddStr("000001");
		requestPacker->AddStr("1");
		requestPacker->AddStr("");                   //��Ʊû�п�ƽ����,���Դ���
		requestPacker->AddStr("0");                  //�޼�
		requestPacker->AddDouble(14.00);
		requestPacker->AddDouble(100);
		requestPacker->AddStr(gUserToken.c_str());
		requestPacker->AddInt(gBatchNo);
		requestPacker->AddStr("106000");
		requestPacker->AddStr("106FA1");
		requestPacker->AddStr("7");
		requestPacker->AddStr("IF1503");
		requestPacker->AddStr("1");                   //����
		requestPacker->AddStr("1");                   //����
		requestPacker->AddStr("0");                   //�޼�
		requestPacker->AddDouble(3622.8);
		requestPacker->AddDouble(1);
		requestPacker->EndPack();
		return requestPacker;
	}

	int BasketEntrust(CConnectionInterface* connection)
	{
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		//ί�й��ܺţ�91090���ӿڹ��ܺż��������������������Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
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
		//�������ܺţ�91101
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
				//��¼���ܺţ�10000
				lpBizMessage->SetFunction(10000);
				lpBizMessage->SetPacketType(REQUEST_PACKET);
				IF2Packer* lpPacker = MakeHeartbeatPacker();
				lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
				CallService(connection,lpBizMessage);
				lpPacker->FreeMem(lpPacker->GetPackBuf());
				lpPacker->Release();
				lpBizMessage->Release();
			}

			Sleep(500000); //��������ʱ����,������Ҫ����
		}
	}

};