#ifndef REAL_TIME_TICK_H
#define REAL_TIME_TICK_H

#include <string>
#include <sstream>
#include "WAPIWrapperCpp.h"
using namespace std;

enum ProductType {
	E_STOCK,
	E_FUTURE,
	E_INDEX
};

class Tick
{
public:
	Tick(){ memset(this, 0, sizeof(Tick)); }

	ProductType type;
	std::string instrument;

	double rt_date; // 20151109.0000
	double rt_time; // 113004.0000

	double rt_pre_close;
	double rt_open;
	double rt_high;
	double rt_low;

	double rt_last; // �ּ�
	double rt_last_vol; // �ɽ���
	double rt_amt; // �ɽ���

	double rt_vol;

	double rt_ask1;
	double rt_asize1;

	double rt_bid1;
	double rt_bsize1;

	double rt_pct_chg; // �ǵ���
	
	static int GetSecsSinceEpoch(double date, double time);

	string Str() const{
		int date = (int)rt_date;
		int time = (int)rt_time;
		char date_time[30]={0};
		sprintf(date_time, "%04d-%02d-%02d %02d:%02d:%02d", 
			date/10000, date%10000/100, date%100, time/10000, time%10000/100, time%100);

		stringstream ss;
		ss << "ʱ��"<< date_time <<" : ��Լ��"<< instrument<<" ���¼�"<< rt_last;
		if(type == E_INDEX){
			ss <<" �ǵ���"<<rt_pct_chg<<" �ɽ���"<<rt_amt;
		}
		
		return ss.str();
	}
};

std::string toString( LPCWSTR p_wstr );

struct HisData {
	/*
	WCHAR* tim = WindData::DateToString(t, L"%Y-%m-%d %H:%M:%S");
	string date_time = toString(tim);
	WindData::FreeString(tim);
	*/
	double date_time;
	double close;

	static int GetSecsSinceEpoch(double date_time);
	string Str() const{
		stringstream ss;
		WCHAR* tim = WindData::DateToString(date_time, L"%Y-%m-%d %H:%M:%S");
		string dt = toString(tim);
		WindData::FreeString(tim);
		ss<<"ʱ��"<<dt<<"�ռ�"<<close<<"���"<<GetSecsSinceEpoch(date_time)<<"��";
		return ss.str();
	}
};




#endif REAL_TIME_TICK_H