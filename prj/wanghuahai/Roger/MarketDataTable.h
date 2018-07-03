#pragma once
#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include "SpinLock.h"
#include "WindApi.h"
#include "MsgDisplay.h"

struct TickLine{
	string pro_name; // "IH" "IF"
	Tick* A;
	Tick* B;
	MuJiJin MU; 
	Tick* GEN; // 跟踪指数
};
/*
template<int column>
bool compareTickLine(const TickLine &left, const TickLine &rhs){
	switch(column){
	case 0:return left.A->instrument < rhs.A->instrument;
	case 1:return left.A->rt_last < rhs.A->rt_last;
	case 2:return left.A->rt_pct_chg < rhs.A->rt_pct_chg;
	case 3:return left.A->rt_amt < rhs.A->rt_amt;
	case 4:return left.B->instrument < rhs.B->instrument;
	case 5:return left.B->rt_last < rhs.B->rt_last;
	case 6:return left.B->rt_pct_chg < rhs.B->rt_pct_chg;
	case 7:return left.B->rt_amt < rhs.B->rt_amt;
	case 8:return left.MU->instrument < rhs.MU->instrument;
	case 9:return left.MU->rt_last < rhs.MU->rt_last;
	default:return true;
	}
}*/

//显示行情的表格
class MarketDataTable : public QAbstractTableModel, public WindDataSpi, public MsgDisplayApi
{
	Q_OBJECT
public:
	MarketDataTable(QObject *parent=0);
	~MarketDataTable(void);

	int rowCount(const QModelIndex&) const;
	int columnCount(const QModelIndex &parent /* = QModelIndex() */)const;
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */)const;
	QVariant headerData(int section, Qt::Orientation, int role= Qt::DisplayRole)const;
	//设置某一列为可选角色，绘画出QCheckBox
	Qt::ItemFlags flags(const QModelIndex &index) const;  
	//根据界面选择QCheckbox，修改Model中的数据
	bool setData(const QModelIndex &index, const QVariant &value, int role);  

public slots:
	//void SlotSort(int );
	

private:
	virtual void OnTick(const Tick& tick);

	QMap<int, int> sym_line_;
	QMap<int, Tick> datas_;//行情
	QList<TickLine> data_table_;

	int select_line_;
	QMap<int, Qt::CheckState> rowCheckStateMap;
};

