/*
 *   UFX�ӿ��첽ʹ��ʾ��(C++��)
*
*    UFX��ʹ�ù�����ʵ����ʹ��t2sdk��������UFX�������������ӡ����Ͳ�����ҵ����Ϣ�Ĺ��̡�
*    ���У�T2SDK��������ҵ���޹صģ���ʹ�ÿ��Բο���T2SDK �ⲿ�濪��ָ��.docx��
*               ҵ����Ϣ�Ķ�������ҵ����صģ�ÿ���ӿڶ����Լ��Ķ��壬���Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
*/

#include "t2sdk_interface.h"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <Windows.h>

using namespace std;


class CCallback : public CCallbackInterface
{
public:
    // ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
    // Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);
};

class CSubCallback : public CSubCallbackInterface
{
	unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
	unsigned long  FUNCTION_CALL_MODE AddRef();
	unsigned long  FUNCTION_CALL_MODE Release();

	void FUNCTION_CALL_MODE OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData);
	void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo);
};

string gOperatorNo  = "1000";  //����Ա
string gPassword    = "0";         //����Ա����
string gUserToken;
int    gBatchNo     = 500000;     //ί������(���Ҫ�Լ�ָ����ע��: O32����Чί�����ű�����1��5000000֮��, UFT��ί�����ű������5000000)
int    gEntrustNo   = 0;
int    gEntrustNo2  = 0;
bool   gbIsRun = false;
map<int,CSubscribeParamInterface*> gAllSubscribeParam;
HANDLE ghThreadReportProcess = NULL;

int Connect(const char* serverAddr, CConnectionInterface** connection, CCallback* callback);
int SubConnect(CConnectionInterface** connection, CCallback* callback, CSubscribeInterface** subscribe, CSubCallback* subcallback);
int Login(CConnectionInterface* connection);
int SingleEntrust(CConnectionInterface* connection);
int BasketEntrust(CConnectionInterface* connection);
int Withdraw(CConnectionInterface* connection);
void runHeartbeat(CConnectionInterface* connection);
void ShowPacket(IF2UnPacker* unPacker);
void ShowSubscribe(int subIndex,LPSUBSCRIBE_RECVDATA lpRecvData);

static DWORD WINAPI startHeartbeatThread (LPVOID lpParam)
{
	gbIsRun = true;
	runHeartbeat((CConnectionInterface*)lpParam);
	return 0;
}

int main(int argc, char** argv)
{
	/****************************** ������������ ******************************/
	string serverAddr  = "192.168.50.167:9113"; //UFX��������ַ
	CCallback             callback1;
	CConnectionInterface* connection1 = NULL;

	//��������
	int ret = Connect(serverAddr.c_str(), &connection1, &callback1);
	if (ret != 0)
	{
		cout << "��������ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(ret) << endl;
		connection1->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "�������ӳɹ���" << endl;
	}
	/****************************** ����������� ******************************/

	//���������߳�
	ghThreadReportProcess = CreateThread(NULL,
		0,
		startHeartbeatThread,
		(LPVOID)connection1,
		0,
		NULL
		);
	if (ghThreadReportProcess == NULL)
	{
		cout << "���������߳�ʧ�ܡ�" << endl;
		getchar();
		return -1;
	}

	/****************************** ������������ ******************************/
	CCallback             callback2;
	CSubCallback          subCallback;
	CSubscribeInterface*  subscribe = NULL;
	CConnectionInterface* connection2 = NULL;

	//��������
	ret = SubConnect(&connection2, &callback2, &subscribe, &subCallback);
	if (ret != 0)
	{
		if (subscribe != NULL)
		{
			subscribe->Release();
		}
		connection2->Release();
		connection1->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "�������ӳɹ���" << endl;
	}
	/****************************** ����������� ******************************/


	int iOrderID = 0;
	while (1)
	{
		cout << endl;
		cout << "1����¼ 2��֤ȯ����ί�� 3������ί�� 4������ 0���˳�" << endl;
		cout << "������ָ��ţ�";
		scanf("%d",&iOrderID);
		switch(iOrderID)
		{
		case 0:
			{
				gbIsRun = false;
				Sleep(1000);
				subscribe->Release();
				connection2->Release();
				connection1->Release();
				return 0;
			}
		case 1:
			{
				//��¼
				int handler = Login(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "��¼ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 2:
			{
				//֤ȯ����ί��
				int handler = SingleEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "ί��ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 3:
			{
				//����ί��
				int handler = BasketEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "ί��ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 4:
			{
				//����
				int handler = Withdraw(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "����ʧ�ܡ�������ϢΪ��" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		default:
			{
				cout << "�����ָ��Ų���ȷ��" << endl;
				//continue;
			}
		}
	}

	map<int,CSubscribeParamInterface*>::iterator it = gAllSubscribeParam.begin();
	for (; it != gAllSubscribeParam.end(); it++)
	{
		if ((*it).second != NULL)
		{
			(*it).second->Release();
			(*it).second = NULL;
		}
	}
	gAllSubscribeParam.clear();
	subscribe->Release();
	connection2->Release();
	connection1->Release();
    getchar();    
    return 0;
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

int CallService(CConnectionInterface* connection, IBizMessage* requestBizMessage)
{
	return connection->SendBizMsg(requestBizMessage,1);
}

int Connect(const char* serverAddr, CConnectionInterface** connection, CCallback* callback)
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

int SubConnect(CConnectionInterface** connection, CCallback* callback, CSubscribeInterface** subscribe, CSubCallback* subcallback)
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


unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    //δʹ��
    return 0;
}

unsigned long CCallback::AddRef()
{
    //δʹ��
    return 0;
}

unsigned long CCallback::Release()
{
    //δʹ��
    return 0;
}

void CCallback::OnConnect(CConnectionInterface *lpConnection)
{
    cout << "OnConnect: successfully connected." << endl;
}

void CCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    cout << "OnSafeConnect: successfully connected." << endl;
}

void CCallback::OnRegister(CConnectionInterface *lpConnection)
{
    cout << "OnRegister: successfully registered." << endl;
}

void CCallback::OnClose(CConnectionInterface *lpConnection)
{
    cout << "OnClose: sdk connection closed." << endl;
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
    cout << "OnSend: hSend(" << hSend << ") send successed, queuingData(" << nQueuingData << ")." << endl;
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
    //δʹ��
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
    //δʹ��
}

int  CCallback::Reserved3()
{
    //δʹ��
    return 0;
}

void CCallback::Reserved4()
{
    //δʹ��
}

void CCallback::Reserved5()
{
    //δʹ��
}

void CCallback::Reserved6()
{
    //δʹ��
}

void CCallback::Reserved7()
{
    //δʹ��
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    //δʹ��
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
    //δʹ��
}

void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	if (lpMsg == NULL)
	{
		cout << "��Ϣ��ָ��Ϊ��" << endl;
		return;
	}

	int iFuncNo = lpMsg->GetFunction();
	int iPacketType = lpMsg->GetPacketType();
	cout << "CCallback::OnReceivedBizMsg: HanderID(" << hSend << "), FunctionNo(" << iFuncNo << "), PacketType(" << iPacketType << ")." << endl;

	int iReturnCode = lpMsg->GetReturnCode();
	if (iReturnCode == 1 || iReturnCode == -1)
	{
		cout << "errorNo:" << lpMsg->GetErrorNo() << ",errorInfo:" << lpMsg->GetErrorInfo() << endl;
	}
	else
	{
		if (iReturnCode == 0)
		{
			cout << "ҵ������ɹ�" << endl;
		}
		else
		{
			cout << "ҵ�����ʧ��" << endl;
		}

		int iLen = 0;
		const void* responseBuffer = lpMsg->GetContent(iLen);
		IF2UnPacker* responseUnPacker = NewUnPacker((void *)responseBuffer,iLen);
		//ShowPacket(responseUnPacker);

		responseUnPacker->SetCurrentDatasetByIndex(0);  //��1�����������Ϣͷ,�϶�����
		responseUnPacker->First();
		int iErrorCode = responseUnPacker->GetInt("ErrorCode");     //�ӿڵ��ý��,0��ʾ�ɹ�,��0��ʾʧ��
		string sErrorMsg = responseUnPacker->GetStr("ErrorMsg");    //ʧ��ʱ�Ĵ�����Ϣ,�ɹ�ʱΪ��
		string sMsgDetail = responseUnPacker->GetStr("MsgDetail");  //��ϸ������Ϣ,����ȡ����ֶ�,�����Ų�����
		int iDataCount = responseUnPacker->GetInt("DataCount");     //��ʾ��2��������ļ�¼��
		cout << "ErrorCode=" << iErrorCode << " ErrorMsg:" << sErrorMsg.c_str() << " MsgDetail:" << sMsgDetail.c_str() << " DataCount=" << iDataCount << endl;

		if (responseUnPacker->GetDatasetCount() > 1)
		{
			responseUnPacker->SetCurrentDatasetByIndex(1);
			responseUnPacker->First();

			if (iFuncNo == 10001)  //��¼
			{
				gUserToken = responseUnPacker->GetStr("user_token");
				string strVersionNo = responseUnPacker->GetStr("version_no");
				cout << "login response: user_token=" << gUserToken.c_str() << " version_no=" << strVersionNo.c_str() << endl;
			}
			else if (iFuncNo == 91001)  //֤ȯ����ί��
			{
				int iBatchNo = responseUnPacker->GetInt("batch_no"); //gBatchNo = iBatchNo + 1;
				gEntrustNo = responseUnPacker->GetInt("entrust_no");
				int iExtsystemId = responseUnPacker->GetInt("extsystem_id");
				cout << "Single Entrust Response: batch_no=" << iBatchNo << " entrust_no=" << gEntrustNo << " extsystem_id=" << iExtsystemId << endl;
			}
			else if (iFuncNo == 91090)  //����ί��
			{
				cout << "Basket Entrust Response:" << endl;

				int index = 0;
				while (!responseUnPacker->IsEOF())
				{
					int iBatchNo = responseUnPacker->GetInt("batch_no"); //gBatchNo = iBatchNo + 1;
					gEntrustNo = responseUnPacker->GetInt("entrust_no");
					int iExtsystemId = responseUnPacker->GetInt("extsystem_id");
					int iEntFailCode = responseUnPacker->GetInt("entrust_fail_code");
					string strFailCause = responseUnPacker->GetStr("fail_cause");
					int iRiskSerialNo = responseUnPacker->GetInt("risk_serial_no");
					cout << "Record" << index << ": batch_no=" << iBatchNo << " entrust_no=" << gEntrustNo << " extsystem_id=" << iExtsystemId << endl;
					cout << "         entrust_fail_code=" << iEntFailCode << " fail_cause:" << strFailCause << " risk_serial_no=" << iRiskSerialNo << endl;
					responseUnPacker->Next();
					index++;
				}

				//cout << "Risk Control Response:" << endl;
				//responseUnPacker->SetCurrentDatasetByIndex(2);
				//responseUnPacker->First();
				//������3�������,�������Ϣ
			}
			else if (iFuncNo == 91101)  //ί�г���
			{
				gEntrustNo2 = responseUnPacker->GetInt("entrust_no");
				string strMarketNo = responseUnPacker->GetStr("market_no");
				string strStockCode = responseUnPacker->GetStr("stock_code");
				string strSuccessFlag = responseUnPacker->GetStr("success_flag");
				string strFailCause = responseUnPacker->GetStr("fail_cause");
				cout << "Withdraw Response: entrust_no=" << gEntrustNo2 << " market_no=" << strMarketNo.c_str() << " stock_code=" << strStockCode.c_str() << endl;
				cout << "                   success_flag=" << strSuccessFlag.c_str() << " fail_cause=" << strFailCause.c_str() << endl;
			}
		}
	}
}

unsigned long CSubCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    //δʹ��
    return 0;
}

unsigned long CSubCallback::AddRef()
{
    //δʹ��
    return 0;
}

unsigned long CSubCallback::Release()
{
    //δʹ��
    return 0;
}

void CSubCallback::OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData)
{
	if (lpData == NULL)
	{
		cout << "��������ָ��Ϊ��" << endl;
		return;
	}

	IF2UnPacker* lpUnPack = NewUnPacker((void*)lpData,nLength);
	cout << endl;
	cout << endl;
	cout << "CSubCallback::OnReceived: SubscribeIndex(" << subscribeIndex << "), MsgType(" << lpUnPack->GetStr("msgtype") << "), DataLength(" << nLength << ")." << endl;
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

void CSubCallback::OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo)
{
}
