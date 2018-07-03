#include "ufxbusiness.h"

int main()
{
	UFXBusiness* myUFX = new UFXBusiness();
	string choose="";
	bool isRun=true;
	while(isRun)
	{
		printf("\n\n\n*********************************��ӭʹ��UFX LINUX��demo*********************************\n");
		printf("*|                                                                                     |*\n");
		printf("***********************************��ǰdemo֧�����¹���**********************************\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    1.����                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    2.��¼                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    3.ί��                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                  4.ί�в�ѯ                                         |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                  5.�ֲֲ�ѯ                                         |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                  6.�ɽ���ѯ                                         |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    7.����                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    8.����                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*****************************************************************************************\n");
		printf("\t\tִ�в���:");
		cin>>choose;
		while(atoi(choose.c_str())<=0 || atoi(choose.c_str())>=9)
		{
			printf("\t������������������:");
			cin>>choose;
		}
		switch(atoi(choose.c_str()))
		{
			case 1:
				myUFX->SubTopic();
				break;
			case 2:
				myUFX->Login();
				break;
			case 3:
				myUFX->Entrust();
				break;
			case 4:
				myUFX->QryEntrust();
				break;
			case 5:
				myUFX->QryHold();
				break;
			case 6:
				myUFX->QryDeal();
				break;
			case 7:
				myUFX->WithDraw();
				break;
			case 8:
				myUFX->Logoff();
				isRun=false;
				break;
			default:
				printf("��������,��ȷ�Ϻ�����!\n");
		}
		printf("\n");
	}
	delete myUFX;
	return 0;
}
