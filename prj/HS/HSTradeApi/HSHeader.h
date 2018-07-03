#pragma once
#include "t2sdk_interface.h"
#include "MarketDefine.h"
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <Windows.h>


#ifdef HS_EXPORTS
#define HS_API  __declspec(dllexport)
#else 
#define HS_API  __declspec(dllimport)
#endif 



namespace kiiik {

class HSTradeApi;
class HSSubApi;

extern string gOperatorNo;//����Ա
extern string gPassword  ;      //����Ա����
extern string gUserToken;
extern int    gBatchNo ;   //ί������(���Ҫ�Լ�ָ����ע��: O32����Чί�����ű�����1��5000000֮��, UFT��ί�����ű������5000000)
extern int    gEntrustNo ;
extern int    gEntrustNo2 ;
HS_API extern bool   gbIsRun ;
HS_API extern map<int,CSubscribeParamInterface*> gAllSubscribeParam;
HS_API extern HANDLE ghThreadReportProcess;

int HS_API Connect(const char* serverAddr, CConnectionInterface** connection, HSTradeApi* callback);
int HS_API SubConnect(CConnectionInterface** connection, HSTradeApi* callback, CSubscribeInterface** subscribe, HSSubApi* subcallback);
int HS_API Login(CConnectionInterface* connection);
int HS_API  SingleEntrust(CConnectionInterface* connection);
int HS_API BasketEntrust(CConnectionInterface* connection);
int HS_API Withdraw(CConnectionInterface* connection);
void HS_API runHeartbeat(CConnectionInterface* connection);
void HS_API ShowPacket(IF2UnPacker* unPacker);
void HS_API ShowSubscribe(int subIndex,LPSUBSCRIBE_RECVDATA lpRecvData);
int HS_API  CallService(CConnectionInterface* connection, IBizMessage* requestBizMessage);

};