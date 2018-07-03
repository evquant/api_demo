#pragma once

#include "t2sdk_interface.h"
#include "MarketDefine.h"
#include "HSHeader.h"

namespace kiiik {

class HS_API HSSubApi : public CSubCallbackInterface
{

	unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv){return 0;}
	unsigned long  FUNCTION_CALL_MODE AddRef(){return 0;}
	unsigned long  FUNCTION_CALL_MODE Release(){return 0;}

	void FUNCTION_CALL_MODE OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData);
	void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo);
};

};