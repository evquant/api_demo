#ifndef _UFXBUSINESS_H
#define _UFXBUSINESS_H
#include "t2sdk_interface.h"
#include "ufxcallback.h"
#include <iostream>
#include <string>
#include <pthread.h>
using namespace std;

struct HeartConn
{
		CConnectionInterface* ufxConn;
		string strToken;
		bool isRun;
};

class UFXBusiness
{
private:
	CConnectionInterface* BusinessConn;
	CConnectionInterface* SubscribeConn;
	CConfigInterface* SubConfig;
	CSubCallback* SubCallBack;
	CSubscribeInterface* lpSub;
	int iSubIndex;
	string strToken;
public:
	UFXBusiness();
	~UFXBusiness();
  pthread_t pthreadID;
	HeartConn* HtConn;
	int iEntrustNo;
	int CallService(int iFunc,IF2Packer* requestPack);
	void PrintUnPack(int iFunc,IF2UnPacker* Unpacker);
	IF2Packer* BuildEntrustPack();
	IF2Packer* BuildQryEntrustPack();
	IF2Packer* BuildQryHoldPack();
	IF2Packer* BuildQryDealPack();
	IF2Packer* BuildWithdrawPack();
	IF2Packer* BuildLoginPack();
	IF2Packer* BuildLoginOffPack();
	void HeartBeat();
	void Login();
	void Logoff();
	void Entrust();
	void QryEntrust();
	void QryHold();
	void QryDeal();
	void WithDraw();
	int SubTopic();
};





#endif
