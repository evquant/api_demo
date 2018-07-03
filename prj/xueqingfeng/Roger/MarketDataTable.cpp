#include "MarketDataTable.h"
#include <QFont>
extern configInfo *g_config;
static SpinLock lock_;

MarketDataTable::MarketDataTable(QObject *parent) : QAbstractTableModel(parent)
{//std::vector<string>::iterator iter=indexes.begin(); iter!=indexes.end();++iter
	if(g_config){
		std::vector<string> indexes = g_config->getIndexs();
		TickLine line;
		int li = 0;
		for(int i=0; i<indexes.size(); ++i){
			int pos = configInfo::GetPos(indexes[i]);
			datas_[pos] = Tick();
			if(i%3 == 0){
				line.A = &datas_[pos];
				sym_line_[pos] = li;
			}
			else if(i%3 == 1){
				line.B = &datas_[pos];
				sym_line_[pos] = li;
			}
			else{
				line.MU = &datas_[pos];
				data_table_.push_back(line);
				sym_line_[pos] = li++;
			}
		}
	}
}


MarketDataTable::~MarketDataTable(void)
{
}

int MarketDataTable::rowCount(const QModelIndex&) const{
	return data_table_.size();
}
int MarketDataTable::columnCount(const QModelIndex &parent /* = QModelIndex() */)const{
	return 10;
}

QVariant MarketDataTable::data(const QModelIndex &index, int role /* = Qt::DisplayRole */)const
try
{
	Locker locker(&lock_);
	if (!index.isValid())
		return QVariant();

	if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}
	else if (role == Qt::FontRole)
	{
		return QFont("\345\256\213\344\275\223", 10, 60);
	}
	else if (role == Qt::DisplayRole) {
		switch(index.column()){
		case 0:return data_table_.at(index.row()).A->instrument.substr(0,6).c_str();
		case 1:return QString::number(data_table_.at(index.row()).A->rt_last,'f',3);
		case 2:return QString("%1%").arg(QString::number(data_table_.at(index.row()).A->rt_pct_chg*100,'f',2));
		case 3:return QString::number(data_table_.at(index.row()).A->rt_amt/10000,'f',2);
		case 4:return data_table_.at(index.row()).B->instrument.substr(0,6).c_str();
		case 5:return QString::number(data_table_.at(index.row()).B->rt_last,'f',3);
		case 6:return QString("%1%").arg(QString::number(data_table_.at(index.row()).B->rt_pct_chg*100,'f',2));
		case 7:return QString::number(data_table_.at(index.row()).B->rt_amt/10000,'f',2);
		case 8:return data_table_.at(index.row()).MU->instrument.substr(0,6).c_str();
		case 9:return QString::number(data_table_.at(index.row()).MU->rt_last,'f',3);
		default:return QVariant();
		}
	}

	return QVariant();
}
catch(...){
	return QVariant();
}

QVariant MarketDataTable::headerData(int section, Qt::Orientation orientation, int role)const{
	if(role!=Qt::DisplayRole || orientation!=Qt::Horizontal)return QVariant();
	switch(section){
	case 0:return tr("A代码");
	case 1:return tr("A价格");
	case 2:return tr("A涨幅");
	case 3:return tr("A成交(万)");
	case 4:return tr("B代码");
	case 5:return tr("B价格");
	case 6:return tr("B涨幅");
	case 7:return tr("B成交(万)");
	case 8:return tr("跟踪代码");
	case 9:return tr("跟踪涨幅");
	default:return QVariant();
	}
}

void MarketDataTable::OnTick(const Tick& tick){
	int pos = configInfo::GetPos(tick.instrument);
	if (pos == -1) return;

	Locker locker(&lock_);
	datas_[pos] = tick;

	int row = sym_line_[pos];
	emit dataChanged(createIndex(row, 0), createIndex(row, 9));

}

#define SORT_CASE(column) \
	case column: \
		qSort(data_table_.begin(), data_table_.end(), compareTickLine<column>); \
		break;

void MarketDataTable::SlotSort(int column){
	Locker locker(&lock_);
	switch(column){
	SORT_CASE(0)
	SORT_CASE(1)
	SORT_CASE(2)
	SORT_CASE(3)
	SORT_CASE(4)
	SORT_CASE(5)
	SORT_CASE(6)
	SORT_CASE(7)
	SORT_CASE(8)
	SORT_CASE(9)
	default:break;
	}

	int row = 0;
	for (QList<TickLine>::iterator iter = data_table_.begin(); iter != data_table_.end(); ++iter) {
		int pos = configInfo::GetPos(iter->A->instrument);
		sym_line_[pos] = row;
		pos = configInfo::GetPos(iter->B->instrument);
		sym_line_[pos] = row;
		pos = configInfo::GetPos(iter->MU->instrument);
		sym_line_[pos] = row;
		++row;
	}

}