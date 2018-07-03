#include "MarketDataTable.h"
#include <QFont>
#include <QString>
#include <QDebug>
extern configInfo *g_config;
extern WindApi *g_windApi;
static SpinLock lock_;


static std::string s_MuJiJin[10] = {
	"国寿养老","银河增强","证券分级"
};

void MarketDataTable::OnTick(const Tick& tick){
	if(data_table_[0].MU.yest_value < 0.001) {
		int num = g_config->getNums();
		for (int i=0; i<num; ++i) {
			MuJiJin mu = g_windApi->getHisData(data_table_[i].MU.code);
			data_table_[i].MU = mu;
			data_table_[i].MU.name = s_MuJiJin[i];
		}
	}

	int pos = configInfo::GetPos(tick.instrument);
	if (pos == -1) return;

	qDebug()<<tick.Str().c_str();

	Locker locker(&lock_);
	datas_[pos] = tick;

	int row = sym_line_[pos];
	emit dataChanged(createIndex(row, 0), createIndex(row, 16));

	if(select_line_ == row){
		double yest_jin = data_table_.at(row).MU.yest_value;
		//跟踪指数涨幅
		double pct_chg = data_table_.at(row).GEN->rt_pct_chg;
		double jinzhi = yest_jin*(1+2*pct_chg);
		double a = data_table_.at(row).A->rt_bid1;
		double b = data_table_.at(row).B->rt_bid1;
		double yijia = a+b - 2*jinzhi;

		a = data_table_.at(row).A->rt_ask1;
		b = data_table_.at(row).B->rt_ask1;
		double zhejia = 2*jinzhi-(a+b);

		const TickLine &line = data_table_.at(row);
		QString left, right, out;
		if(yijia>0.1){
			left = tr("买入%1手 %2 %3 卖出%4手 %5\t\t")
				.arg(line.MU.hands).arg(line.A->instrument.c_str()).arg(line.B->instrument.c_str())
				.arg(line.MU.hands*2).arg(line.MU.code.c_str());	
		}

		if(zhejia<-0.1){
			right = tr("卖出%1手 %2 %3 买入%4手 %5")
				.arg(line.MU.hands).arg(line.A->instrument.c_str()).arg(line.B->instrument.c_str())
				.arg(line.MU.hands*2).arg(line.MU.code.c_str());
		}
		
		if(left=="") left = "\t\t\t\t";
		out = left + right;
		Print(out.toStdString());
	}
}

MarketDataTable::MarketDataTable(QObject *parent) : QAbstractTableModel(parent),select_line_(-1)
{
	/*s_MuJiJin.push_back(MuJiJin("168001.OF","国寿养老",0.));
	s_MuJiJin.push_back(MuJiJin("161507.OF","银河增强",0.));
	s_MuJiJin.push_back(MuJiJin("502010.OF","证券分级",0.));*/

	int num = g_config->getNums();
	for (int i=0; i<num; ++i) {
		TickLine line;
		ConfigNode node = g_config->getConfigNode(i);

		line.pro_name = node.pro_name;

		int pos = configInfo::GetPos(node.instruments[0]);
		datas_[pos] = Tick();
		line.A = &datas_[pos];
		sym_line_[pos] = i;

		pos = configInfo::GetPos(node.instruments[1]);
		datas_[pos] = Tick();
		line.B = &datas_[pos];
		sym_line_[pos] = i;

		pos = configInfo::GetPos(node.instruments[2]);
		datas_[pos] = Tick();
		line.GEN = &datas_[pos];
		sym_line_[pos] = i;

		line.MU = node.mu;
		line.MU.name = s_MuJiJin[i];

		data_table_.push_back(line);
	}
}


MarketDataTable::~MarketDataTable(void)
{
}

void SlotSelectLine(const QModelIndex &) {

}

int MarketDataTable::rowCount(const QModelIndex&) const{
	return data_table_.size();
}
int MarketDataTable::columnCount(const QModelIndex &parent /* = QModelIndex() */)const{
	return 18;
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
		case 0:return data_table_.at(index.row()).pro_name.c_str();
		case 1:return data_table_.at(index.row()).A->instrument.substr(0,6).c_str();
		case 2:return data_table_.at(index.row()).B->instrument.substr(0,6).c_str();
		case 3:return QString::number(data_table_.at(index.row()).A->rt_bid1,'f',3);
		case 4:return QString::number(data_table_.at(index.row()).B->rt_bid1,'f',3);
		case 5:
			{
				double yest_jin = data_table_.at(index.row()).MU.yest_value;
				//跟踪指数涨幅
				double pct_chg = data_table_.at(index.row()).GEN->rt_pct_chg;
				double jinzhi = yest_jin*(1+2*pct_chg);
				double a = data_table_.at(index.row()).A->rt_bid1;
				double b = data_table_.at(index.row()).B->rt_bid1;
				double yijia = a+b - 2*jinzhi;
				return QString::number(yijia,'f',2);			   
			}
		case 6:return QString::number(data_table_.at(index.row()).A->rt_ask1,'f',3);
		case 7:return QString::number(data_table_.at(index.row()).B->rt_ask1,'f',3);
		case 8:
			{
				double yest_jin = data_table_.at(index.row()).MU.yest_value;
				//跟踪指数涨幅
				double pct_chg = data_table_.at(index.row()).GEN->rt_pct_chg;
				double jinzhi = yest_jin*(1+2*pct_chg);
				double a = data_table_.at(index.row()).A->rt_ask1;
				double b = data_table_.at(index.row()).B->rt_ask1;
				double zhejia = 2*jinzhi-(a+b);
				return QString::number(zhejia,'f',2);	
			}
		case 9:
			{
				double yest_jin = data_table_.at(index.row()).MU.yest_value;
				//跟踪指数涨幅
				double pct_chg = data_table_.at(index.row()).GEN->rt_pct_chg;
				double jinzhi = yest_jin*(1+2*pct_chg);
				return QString::number(jinzhi,'f',2);	
			}
		case 10:return data_table_.at(index.row()).MU.name.c_str();
		case 11:return data_table_.at(index.row()).MU.code.c_str();
		case 12:return QString::number(data_table_.at(index.row()).MU.yest_value, 'f',3);	
		case 13:return data_table_.at(index.row()).GEN->instrument.c_str();
		case 14:return QString::number(data_table_.at(index.row()).GEN->rt_pct_chg,'f',2);
		case 15:return data_table_.at(index.row()).MU.sub_rate;
		case 16:return data_table_.at(index.row()).MU.ask_rate;
		default:return QVariant();
		}
	}
	else if (role == Qt::CheckStateRole) {
		if(index.column()==17){
			return rowCheckStateMap[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked; 
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
	case 0:return tr("类型");//"IH" "IF"
	case 1:return tr("A代码");
	case 2:return tr("B代码");
	case 3:return tr("A买一");
	case 4:return tr("B买一");
	case 5:return tr("溢价");
	case 6:return tr("A卖一");
	case 7:return tr("B卖一");
	case 8:return tr("折价");
	case 9:return tr("净值");
	case 10:return tr("母名称");
	case 11:return tr("母代码");
	case 12:return tr("母昨净");
	case 13:return tr("跟踪代码");
	case 14:return tr("涨跌幅");
	case 15:return tr("申购费");
	case 16:return tr("赎回费");
	default:return QVariant();
	}
}

Qt::ItemFlags MarketDataTable::flags(const QModelIndex &index) const{
	if(!index.isValid())return 0;
	if (index.column() == 17)  
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable; 
	return  Qt::ItemIsEnabled | Qt::ItemIsSelectable; 
}
//根据界面选择QCheckbox，修改Model中的数据
bool MarketDataTable::setData(const QModelIndex &index, const QVariant &value, int role){
	if(!index.isValid())  
		return false;  
	if (role == Qt::CheckStateRole && index.column() == 17)  
	{  
		if (value == Qt::Checked) //  
		{   
			for(QMap<int, Qt::CheckState>::iterator iter=rowCheckStateMap.begin(); iter!=rowCheckStateMap.end();++iter){
				if (iter.key()!=index.row() && iter.value()==Qt::Checked){
					iter.value() = Qt::Unchecked;  
					emit dataChanged(createIndex(iter.key(), 17),createIndex(iter.key(), 17));
				}
			}
			rowCheckStateMap[index.row()] = Qt::Checked;  
			if(data_table_.size() > index.row())  
				select_line_ = index.row();
		}  
		else  
		{  
			rowCheckStateMap[index.row()] = Qt::Unchecked;  
			emit dataChanged(createIndex(index.row(), 17),createIndex(index.row(), 17));
		}   
	}  
	return true;  
}


/*
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

}*/