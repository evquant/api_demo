#include "ufxbusiness.h"

int main()
{
	UFXBusiness* myUFX = new UFXBusiness();
	string choose="";
	bool isRun=true;
	while(isRun)
	{
		printf("\n\n\n*********************************欢迎使用UFX LINUX版demo*********************************\n");
		printf("*|                                                                                     |*\n");
		printf("***********************************当前demo支持如下功能**********************************\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    1.订阅                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    2.登录                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    3.委托                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                  4.委托查询                                         |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                  5.持仓查询                                         |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                  6.成交查询                                         |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    7.撤单                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*|                                    8.结束                                           |*\n");
		printf("*|                                                                                     |*\n");
		printf("*****************************************************************************************\n");
		printf("\t\t执行操作:");
		cin>>choose;
		while(atoi(choose.c_str())<=0 || atoi(choose.c_str())>=9)
		{
			printf("\t操作有误，请重新输入:");
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
				printf("操作不符,请确认后输入!\n");
		}
		printf("\n");
	}
	delete myUFX;
	return 0;
}
