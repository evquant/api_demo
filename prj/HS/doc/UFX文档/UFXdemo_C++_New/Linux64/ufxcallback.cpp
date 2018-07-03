#include "ufxcallback.h"

void CSubCallback::OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,
			LPSUBSCRIBE_RECVDATA lpRecvData)
{
	printf("***************************\n");
	//PrintSub(subscribeIndex,lpRecvData);
	IF2UnPacker* lpUnPack = NewUnPacker((void*)lpData,nLength);
	if (lpUnPack)
	{
		lpUnPack->AddRef();
		PrintUnPack(lpUnPack);
		lpUnPack->Release();
	}
	printf("***************************\n");
}
void CSubCallback::PrintUnPack(IF2UnPacker* lpUnPack)
{
		printf("记录行数：           %d\n",lpUnPack->GetRowCount());
		printf("列行数：			 %d\n",lpUnPack->GetColCount());
		while (!lpUnPack->IsEOF())
		{	
			for (int i=0;i<lpUnPack->GetColCount();i++)
			{
				char* colName = (char*)lpUnPack->GetColName(i);
				char colType = lpUnPack->GetColType(i);
				if (colType!='R')
				{
					char* colValue = (char*)lpUnPack->GetStrByIndex(i);
					printf("%s:			[%s]\n",colName,colValue);
				}
				else
				{
					int colLength = 0;
					char* colValue = (char*)lpUnPack->GetRawByIndex(i,&colLength);
					printf("%s:			[%s](%d)\n",colName,colValue,colLength);
				}
			}
			lpUnPack->Next();
		}
}

void CSubCallback::OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo)
{
	return;
}


