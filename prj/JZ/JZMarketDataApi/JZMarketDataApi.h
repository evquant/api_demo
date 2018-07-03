#ifndef EYEGLE_MARKETAPI_GTASTOCK_MARKETDATAAPI_H_
#define EYEGLE_MARKETAPI_GTASTOCK_MARKETDATAAPI_H_

#include <vector>
#include "tdr.h"
#include "MarketDefine.h"

#ifdef JZ_MD_EXPORTS
#define JZ_MD_API  __declspec(dllexport)
#else 
#define JZ_MD_API  __declspec(dllimport)
#endif 

namespace kiiik {

/*
* @brief JZMarketDataApi �г����ݶ���
*/
class JZ_MD_API JZMarketDataApi {
public:
	JZMarketDataApi();
	virtual ~JZMarketDataApi();

	bool Init(const std::string& ip, int port, MarketDataSpi* spi, std::string& err); 
	bool Login(const std::string& broker_id, const std::string& user_id, const std::string& password, std::string& err);
	bool Logout(std::string& err);
	///��Լ����淶����֤".SH"��β����֤".SZ"��������Ч
	///һ��ֻ�ܶ���һ����Լ
	bool Subscribe(const std::string& symbol, std::string& err);

private:
	std::string GetMarketByStockCode(const std::string &strStockCode) {
		if(strStockCode.empty()){
			printf("invalid symbol"); return "";
		}		
		if (strStockCode.back() == 'H') return "SH";
		else if (strStockCode.back() == 'Z') return "SZ";
		else return "";		
	}
	int GetServiceIdByStockCode(const std::string &strStockCode) {
		if (!strStockCode.empty() && strStockCode.back() == 'Z') return ID_SZ_MARKETDATA;
		else return ID_SH_MARKETDATA;		
	}

	std::vector<T_HANDLE> hTDR_;
	int num_syms_;

	std::string ip_;
	int port_;
	std::string user_id_;
	std::string password_;

	bool m_succed_connect;	/**< ��������Ƿ����ӳɹ� */
	bool m_succed_login;	/**< �Ƿ��½�ɹ� */

	MarketDataSpi* spi_;
};

}

#endif // 