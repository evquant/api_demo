#ifndef GTA_STOCK_MARKET_TRADE_API_H
#define GTA_STOCK_MARKET_TRADE_API_H

#include <map>
#include "kcbpcli.h"
#include "MarketDefine.h"

#ifdef JZ_TD_EXPORTS
#define JZ_TD_API  __declspec(dllexport)
#else 
#define JZ_TD_API  __declspec(dllimport)
#endif 

namespace kiiik {

enum GtaQueryEnum {
	kQueryAccount,
	kQueryOrder,
	kQueryTrade,
	kQueryPosition,
	kQryInstrument,
	kQueryOther
};

class JZ_TD_API JZTradeApi {
public:
	JZTradeApi(void);
	virtual ~JZTradeApi(void);

	bool Init(const std::string& ip, int port, 
		const std::string& szServerName,
		const std::string& szUserName,
		const std::string& szPassword,
		const std::string& szSendQName,
		const std::string& szReceiveQName,
		TradeSpi* spi, std::string& err);
	bool Login(const std::string& broker_id, 
		const std::string& user_id, const std::string& password, 
		const std::string& szLoginNetaddr,
		std::string& err);
	bool Logout(std::string& err);
	bool InitPreTrade(std::string& err) { return true; }

	int QueryAccount(std::string& err);
	int QueryPosition(std::string& err);
	int QueryOrder(std::string& err);
	int QueryTrade(std::string& err);
	bool QryInstrument(std::string& err);

	// ��[410336]ʵ�� ����ί��: ֻ�ṩ������ͨ��Ʊ�����𡢺�ծȯ������ҵ�񣬲��������¹��깺�ȷǽ��׻��������⽻��ҵ��������ί�б�������Ϊ50��
	int SubmitOrder(const OrderParamData& param, std::string& err);
	// ��[410411]ʵ�� ί������ҵ��: 
	// �������ڵ�ת�ɻ��ۣ�����BSFLAG������G��H��ǰ̨��Ҫ�����˵�����Ҳ������������������깺����أ����������3��4
	// �������ڵ�Ȩ֤��Ȩ������BSFLAG������7��ǰ̨��Ҫ�����˵�
	int SubmitOrder(const OrderParamData *param, int num, std::string& err);
	// ��ί����Ż�ί�����ų���
	int CancelOrder(const OrderData& param, std::string& err);

private:
	KCBPCLIHANDLE m_hKCBPHandle;

	bool m_succed_connect;	/**< ���ӳɹ� */
	bool m_succed_login;	/**< ��½�ɹ� */
	std::string ip_;
	int port_;
	std::string m_user_id;
	std::string m_password;
	typedef struct
	{
		char szCustid[30];					//�ͻ�����
		char szLoginOrgid[12];				//��������
		char szTradePassword[40];			//���ܵĽ�������
		char szLoginNetaddr[40];			//������ַ��绰
		char szOperateOrgid[12];			//��������
		char szOperway[4];					//������ʽ
		char szServerid[4];					//���������
	}CustInfo;
	CustInfo m_cust_info;

	std::map<ExchangeIdType, std::string> exe_secuid_; // ������ => �ɶ�����
	std::string bankcode_; // �ⲿ����
	bool m_iCreditflag; // �Ƿ��������˻�

	TradeSpi *spi_;

private:
	int Stock_ADoTrade(const char *szFuncID, const char *szParameter, std::string &err);
	int query(tagCallCtrl &stControl, std::string &szRetMsg);

	void OnQueryAccount(std::string& szRetMsg);
	void OnQueryPosition(std::string& szRetMsg);
	void OnQueryOrder(std::string& szRetMsg);
	void OnQueryTrade(std::string& szRetMsg);
	void OnQueryCancelAbledOrder(std::string& szRetMsg);
	void OnQryInstrument(std::string& szRetMsg);

	GtaQueryEnum GetQryType(const char *szFuncID) {
		if (!strcmp(szFuncID, "410502")) return kQueryAccount;
		else if (!strcmp(szFuncID, "410510")) return kQueryOrder;
		else if (!strcmp(szFuncID, "410512")) return kQueryTrade;
		else if (!strcmp(szFuncID, "410503")) return kQueryPosition;
		else if (!strcmp(szFuncID, "410203")) return kQryInstrument;
		else return kQueryOther;
	}
};

}

#endif
