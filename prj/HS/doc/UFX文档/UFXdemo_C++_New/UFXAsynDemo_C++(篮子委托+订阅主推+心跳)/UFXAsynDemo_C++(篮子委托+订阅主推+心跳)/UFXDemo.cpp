/*
 *   UFX接口异步使用示例(C++版)
*
*    UFX的使用过程其实就是使用t2sdk开发包和UFX服务器建立连接、发送并接收业务消息的过程。
*    其中，T2SDK开发包是业务无关的，其使用可以参考《T2SDK 外部版开发指南.docx》
*               业务消息的定义则是业务相关的，每个接口都有自己的定义，可以参考《恒生投资管理系统O3.2_周边接口规范_x.x.x.x.xls》
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
    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
    // Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
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

string gOperatorNo  = "1000";  //操作员
string gPassword    = "0";         //操作员密码
string gUserToken;
int    gBatchNo     = 500000;     //委托批号(如果要自己指定请注意: O32的有效委托批号必须在1到5000000之间, UFT的委托批号必须大于5000000)
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
	/****************************** 创建功能连接 ******************************/
	string serverAddr  = "192.168.50.167:9113"; //UFX服务器地址
	CCallback             callback1;
	CConnectionInterface* connection1 = NULL;

	//功能连接
	int ret = Connect(serverAddr.c_str(), &connection1, &callback1);
	if (ret != 0)
	{
		cout << "功能连接失败。错误信息为：" << connection1->GetErrorMsg(ret) << endl;
		connection1->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "功能连接成功。" << endl;
	}
	/****************************** 创建连接完成 ******************************/

	//创建心跳线程
	ghThreadReportProcess = CreateThread(NULL,
		0,
		startHeartbeatThread,
		(LPVOID)connection1,
		0,
		NULL
		);
	if (ghThreadReportProcess == NULL)
	{
		cout << "创建心跳线程失败。" << endl;
		getchar();
		return -1;
	}

	/****************************** 创建订阅连接 ******************************/
	CCallback             callback2;
	CSubCallback          subCallback;
	CSubscribeInterface*  subscribe = NULL;
	CConnectionInterface* connection2 = NULL;

	//订阅连接
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
		cout << "订阅连接成功。" << endl;
	}
	/****************************** 创建连接完成 ******************************/


	int iOrderID = 0;
	while (1)
	{
		cout << endl;
		cout << "1：登录 2：证券单笔委托 3：篮子委托 4：撤单 0：退出" << endl;
		cout << "请输入指令号：";
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
				//登录
				int handler = Login(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "登录失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 2:
			{
				//证券单笔委托
				int handler = SingleEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "委托失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 3:
			{
				//篮子委托
				int handler = BasketEntrust(connection1);
				if (handler > 0)
				{
					Sleep(2000);
				}
				else
				{
					cout << "委托失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		case 4:
			{
				//撤单
				int handler = Withdraw(connection1);
				if (handler > 0)
				{
					Sleep(1000);
				}
				else
				{
					cout << "撤单失败。错误信息为：" << connection1->GetErrorMsg(handler) << endl;
				}
				break;
			}
		default:
			{
				cout << "输入的指令号不正确！" << endl;
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

int CallService(CConnectionInterface* connection, IBizMessage* requestBizMessage)
{
	return connection->SendBizMsg(requestBizMessage,1);
}

int Connect(const char* serverAddr, CConnectionInterface** connection, CCallback* callback)
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

int SubConnect(CConnectionInterface** connection, CCallback* callback, CSubscribeInterface** subscribe, CSubCallback* subcallback)
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


unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    //未使用
    return 0;
}

unsigned long CCallback::AddRef()
{
    //未使用
    return 0;
}

unsigned long CCallback::Release()
{
    //未使用
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
    //未使用
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
    //未使用
}

int  CCallback::Reserved3()
{
    //未使用
    return 0;
}

void CCallback::Reserved4()
{
    //未使用
}

void CCallback::Reserved5()
{
    //未使用
}

void CCallback::Reserved6()
{
    //未使用
}

void CCallback::Reserved7()
{
    //未使用
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    //未使用
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
    //未使用
}

void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	if (lpMsg == NULL)
	{
		cout << "消息包指针为空" << endl;
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
			cout << "业务操作成功" << endl;
		}
		else
		{
			cout << "业务操作失败" << endl;
		}

		int iLen = 0;
		const void* responseBuffer = lpMsg->GetContent(iLen);
		IF2UnPacker* responseUnPacker = NewUnPacker((void *)responseBuffer,iLen);
		//ShowPacket(responseUnPacker);

		responseUnPacker->SetCurrentDatasetByIndex(0);  //第1个结果集是消息头,肯定存在
		responseUnPacker->First();
		int iErrorCode = responseUnPacker->GetInt("ErrorCode");     //接口调用结果,0表示成功,非0表示失败
		string sErrorMsg = responseUnPacker->GetStr("ErrorMsg");    //失败时的错误信息,成功时为空
		string sMsgDetail = responseUnPacker->GetStr("MsgDetail");  //详细错误信息,建议取这个字段,方便排查问题
		int iDataCount = responseUnPacker->GetInt("DataCount");     //表示第2个结果集的记录数
		cout << "ErrorCode=" << iErrorCode << " ErrorMsg:" << sErrorMsg.c_str() << " MsgDetail:" << sMsgDetail.c_str() << " DataCount=" << iDataCount << endl;

		if (responseUnPacker->GetDatasetCount() > 1)
		{
			responseUnPacker->SetCurrentDatasetByIndex(1);
			responseUnPacker->First();

			if (iFuncNo == 10001)  //登录
			{
				gUserToken = responseUnPacker->GetStr("user_token");
				string strVersionNo = responseUnPacker->GetStr("version_no");
				cout << "login response: user_token=" << gUserToken.c_str() << " version_no=" << strVersionNo.c_str() << endl;
			}
			else if (iFuncNo == 91001)  //证券单笔委托
			{
				int iBatchNo = responseUnPacker->GetInt("batch_no"); //gBatchNo = iBatchNo + 1;
				gEntrustNo = responseUnPacker->GetInt("entrust_no");
				int iExtsystemId = responseUnPacker->GetInt("extsystem_id");
				cout << "Single Entrust Response: batch_no=" << iBatchNo << " entrust_no=" << gEntrustNo << " extsystem_id=" << iExtsystemId << endl;
			}
			else if (iFuncNo == 91090)  //篮子委托
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
				//解析第3个结果集,即风控信息
			}
			else if (iFuncNo == 91101)  //委托撤单
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
    //未使用
    return 0;
}

unsigned long CSubCallback::AddRef()
{
    //未使用
    return 0;
}

unsigned long CSubCallback::Release()
{
    //未使用
    return 0;
}

void CSubCallback::OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData)
{
	if (lpData == NULL)
	{
		cout << "主推数据指针为空" << endl;
		return;
	}

	IF2UnPacker* lpUnPack = NewUnPacker((void*)lpData,nLength);
	cout << endl;
	cout << endl;
	cout << "CSubCallback::OnReceived: SubscribeIndex(" << subscribeIndex << "), MsgType(" << lpUnPack->GetStr("msgtype") << "), DataLength(" << nLength << ")." << endl;
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

void CSubCallback::OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo)
{
}
