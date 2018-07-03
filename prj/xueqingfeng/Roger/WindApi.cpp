#include "WindApi.h"
#include <sstream >
#include <QDateTime>

extern configInfo *g_config;
extern WindApi *g_windApi;


#define FIELDS_STOCK L"rt_date,rt_time,rt_last,rt_last_vol,rt_ask1,rt_asize1,rt_bid1,rt_bsize1"
#define FIELDS_FUTURE L"rt_date,rt_time,rt_last,rt_last_vol,rt_ask1,rt_asize1,rt_bid1,rt_bsize1"
//最新价、涨跌幅、成交额
#define FIELDS_INDEX L"rt_date,rt_time,rt_last,rt_pct_chg,rt_amt"

#define FIELDS_HIS_DATA L"open,high,low,close,volume"

std::string toString( LPCWSTR p_wstr )
{
	size_t len = wcslen(p_wstr) + 1;
	size_t converted = 0;
	char * p_str;
	p_str = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, p_str, len, p_wstr, _TRUNCATE);
	string ret = p_str;
	free(p_str);
	return ret;
}

int Tick::GetSecsSinceEpoch(double rt_date, double rt_time) {
	int date = (int)rt_date;
	int time = (int)rt_time;
	char dt[30]={0};
	sprintf(dt, "%04d-%02d-%02d %02d:%02d:%02d", 
		date/10000, date%10000/100, date%100, time/10000, time%10000/100, time%100);
	QDateTime qdt = QDateTime::fromString(dt, "yyyy-MM-dd HH:mm:ss");
	return qdt.toMSecsSinceEpoch()/1000.0;
}

int HisData::GetSecsSinceEpoch(double date_time){
	WCHAR* tim = WindData::DateToString(date_time, L"%Y-%m-%d %H:%M:%S");
	string dt = toString(tim);
	WindData::FreeString(tim);
	QDateTime qdt = QDateTime::fromString(dt.c_str(), "yyyy-MM-dd HH:mm:ss");
	return qdt.toMSecsSinceEpoch()/1000.0;
}

WindApi::WindApi()
{
	Start();
}


WindApi::~WindApi(void)
{
}



void getStockTick(const WindData &wd, int timeIdx, Tick &pStock){
	pStock.type = E_STOCK;
	pStock.instrument = toString( wd.GetCodeByIndex(0) );

	VARIANT var;
	wd.GetDataItem(timeIdx,0, 0,var); pStock.rt_date = var.dblVal;
	wd.GetDataItem(timeIdx,0, 1,var); pStock.rt_time = var.dblVal;
	wd.GetDataItem(timeIdx,0, 2,var); pStock.rt_last = var.dblVal;
	wd.GetDataItem(timeIdx,0, 3,var); pStock.rt_last_vol = var.dblVal;
	wd.GetDataItem(timeIdx,0, 4,var); pStock.rt_ask1 = var.dblVal;
	wd.GetDataItem(timeIdx,0, 5,var); pStock.rt_asize1 = var.dblVal;
	wd.GetDataItem(timeIdx,0, 6,var); pStock.rt_bid1 = var.dblVal;
	wd.GetDataItem(timeIdx,0, 7,var); pStock.rt_bsize1 = var.dblVal;
}

LONG WINAPI WindCallBack_Stock(ULONGLONG reqId, const WindData &wd)
{
	Tick pStock;
	getStockTick(wd, 0, pStock);
	if (g_windApi) g_windApi->Push(pStock);
	return 0;
}

void getFutureTick(const WindData &wd, int timeIdx, Tick &pFuture){
	pFuture.type = E_FUTURE;
	pFuture.instrument = toString( wd.GetCodeByIndex(0) );

	VARIANT var;
	wd.GetDataItem(timeIdx,0, 0,var); pFuture.rt_date = var.dblVal;
	wd.GetDataItem(timeIdx,0, 1,var); pFuture.rt_time = var.dblVal;
	wd.GetDataItem(timeIdx,0, 2,var); pFuture.rt_last = var.dblVal;
	wd.GetDataItem(timeIdx,0, 3,var); pFuture.rt_last_vol = var.dblVal;
	wd.GetDataItem(timeIdx,0, 4,var); pFuture.rt_ask1 = var.dblVal;
	wd.GetDataItem(timeIdx,0, 5,var); pFuture.rt_asize1 = var.dblVal;
	wd.GetDataItem(timeIdx,0, 6,var); pFuture.rt_bid1 = var.dblVal;
	wd.GetDataItem(timeIdx,0, 7,var); pFuture.rt_bsize1 = var.dblVal;
}

LONG WINAPI WindCallBack_Future(ULONGLONG reqId, const WindData &wd)
{
	Tick pFuture;
	getFutureTick(wd, 0, pFuture);
	if (g_windApi) g_windApi->Push(pFuture);
	return 0;
}

void getIndexTick(const WindData &wd, int timeIdx, Tick &pIdx){
	pIdx.type = E_INDEX;
	pIdx.instrument = toString( wd.GetCodeByIndex(0) );

	VARIANT var;
	wd.GetDataItem(timeIdx,0, 0,var); pIdx.rt_date = var.dblVal;
	wd.GetDataItem(timeIdx,0, 1,var); pIdx.rt_time = var.dblVal;
	wd.GetDataItem(timeIdx,0, 2,var); pIdx.rt_last = var.dblVal;	
	wd.GetDataItem(timeIdx,0, 3,var); pIdx.rt_pct_chg = var.dblVal;
	wd.GetDataItem(timeIdx,0, 4,var); pIdx.rt_amt = var.dblVal;
}

LONG WINAPI WindCallBack_Index(ULONGLONG reqId, const WindData &wd)
{
	Tick pIdx;
	getIndexTick(wd, 0, pIdx);
	if (g_windApi) g_windApi->Push(pIdx);
	return 0;
}

vector<HisData> WindApi::getHisMinDatas(/*ProductType pro, */string sym){
	WindData datas;

	wstringstream windcode;
	windcode << sym.c_str();

	QDateTime now = QDateTime::currentDateTime();
	QDateTime yest = now.addDays(-7);
	QString startTime = QString("%1 09:00:00").arg(yest.toString("yyyy-MM-dd"));
	QString endTime = QString("%1 15:30:00").arg(now.toString("yyyy-MM-dd"));
	wstringstream wss_start, wss_end;
	wss_start << startTime.toStdString().c_str();	
	wss_end << endTime.toStdString().c_str();

	LONG errCode = windApi.wsi(datas, windcode.str().c_str(), FIELDS_HIS_DATA, wss_start.str().c_str(), wss_end.str().c_str());
	
	vector<HisData> ret;
	if(errCode != 0)
	{
		Print("订阅历史数据失败");
		return ret;
	}
	VARIANT var;
	int length = datas.GetTimesLength();
	for(int i=0; i<length; ++i){
		HisData his_data;
		datas.GetDataItem(i,0, 3,var);
		if(_isnan(var.dblVal)) continue;
		his_data.close = var.dblVal;
		his_data.date_time = datas.GetTimeByIndex(i);
		ret.push_back(his_data);		
	}
	return ret;
}


int WindApi::loginWind()
{
	long errCode = windApi.start();
	if(errCode != 0){
		Print("wind登陆失败");
		return -1;
	}

	ULONGLONG wsq_reqID;

	// @ subscribe Stocks
	set<string> subStocks = g_config->getSubInstruments(E_STOCK);
	set<string> subFutures = g_config->getSubInstruments(E_FUTURE);
	set<string> subIndexs = g_config->getSubInstruments(E_INDEX);
	//取历史先
	for (set<string>::iterator iter = subStocks.begin(); iter != subStocks.end(); ++iter) 
		hist_datas_[*iter] = getHisMinDatas(*iter);
	for (set<string>::iterator iter = subFutures.begin(); iter != subFutures.end(); ++iter)
		hist_datas_[*iter] = getHisMinDatas(*iter);
	for (set<string>::iterator iter = subIndexs.begin(); iter != subIndexs.end(); ++iter)
		hist_datas_[*iter] = getHisMinDatas(*iter);
	//取实时
	for (set<string>::iterator iter = subStocks.begin(); iter != subStocks.end(); ++iter) {
		wstringstream wss;
		wss << iter->c_str();
		errCode = windApi.wsq(wsq_reqID, wss.str().c_str(), FIELDS_STOCK, WindCallBack_Stock, L"", TRUE);
		if(errCode != 0)
		{
			Print((string)"订阅"+*iter+"失败");
			return -1;
		}
	}

	// @ subscribe Futures	
	for (set<string>::iterator iter = subFutures.begin(); iter != subFutures.end(); ++iter) {
		wstringstream wss;
		wss << iter->c_str();
		LPCWSTR windcode = wss.str().c_str();
		errCode = windApi.wsq(wsq_reqID, windcode, FIELDS_FUTURE, WindCallBack_Future, L"", TRUE);
		if(errCode != 0)
		{
			Print((string)"订阅"+*iter+"失败");
			return -1;
		}
	}

	// @ subscribe Index	
	for (set<string>::iterator iter = subIndexs.begin(); iter != subIndexs.end(); ++iter) {
		wstringstream wss;
		wss << iter->c_str();
		errCode = windApi.wsq(wsq_reqID, wss.str().c_str(), FIELDS_INDEX, WindCallBack_Index, L"", TRUE);
		if(errCode != 0)
		{
			Print((string)"订阅"+*iter+"失败");
			return -1;
		}
	}

	Print("wind登陆成功并且订阅行情成功");
	return 0;
}

bool WindApi::Comsume(const Tick& val){
	for (vector<WindDataSpi*>::iterator iter = strategys_.begin(); iter != strategys_.end(); ++iter)
	{
		if (*iter != NULL)
			(*iter)->OnTick(val);
	}
	return true;
}