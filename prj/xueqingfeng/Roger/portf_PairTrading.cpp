#include "portf_PairTrading.h"
#include <QCoreApplication>
#include <QBoxLayout>
#include <QSizePolicy>
#include <QDebug>
#include "RealTimeTick.h"
#include "configInfo.h"
#include "SimpleMath.hpp"

extern configInfo *g_config;
extern WindApi *g_windApi;


portf_PairTrading::portf_PairTrading(int idx, QWidget *parent)
	: QWidget(parent), delta_chart_(NULL), delta_ma_chart_(NULL)
{
	//读入参数
	ConfigNode node = g_config->getConfigNode(idx);
	assert(node.instruments.size() == 2);
	inst_a_ = node.instruments[0];
	inst_b_ = node.instruments[1];
	a_ = atof(node.others[0].c_str());
	b_ = atof(node.others[1].c_str());
	opera_ = node.others[2][0];
	delta_period_ = atoi(node.others[3].c_str());
	std_dev_period_ = atoi(node.others[4].c_str());
	std_dev_param_ = atof(node.others[5].c_str());

	CreateExpressionLine();
	CreateTimeSharingplan();

	QVBoxLayout *main_layout = new QVBoxLayout(this);
	main_layout->addLayout(title_layout_);
	main_layout->addWidget(delta_chart_);
	main_layout->addLayout(period_layout_);
	main_layout->addWidget(delta_ma_chart_);

	g_windApi->addStrategy(this);

	assert(connect(this, SIGNAL(SignalUpdateUi()), this, SLOT(SlotUpdateUi()), Qt::QueuedConnection));
}

#define CALCU_CASE(i, op) \
	case i: \
	{ del = a_*a_lasts_[0] op b_*b_lasts_[0]; } \
	break;

#define  CALCU_CASE_2(j, op) \
	case j: \
	{ del = a_*(*a_lasts_.Data(i)) op b_*(*b_lasts_.Data(i)); } \
	break;


void portf_PairTrading::CreateTimeSharingplan() {
	double tmp = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

	// delta 和 MA 图形
	delta_chart_ = new QCustomPlot(this);
	delta_chart_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	delta_chart_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	delta_chart_->setBackground(QBrush(QColor(0,0,0)));
	delta_chart_->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	delta_chart_->xAxis->setDateTimeFormat("hh:mm");	
	delta_chart_->xAxis->setRange(tmp-60*1200, tmp+60*90);
	delta_chart_->xAxis->setTickLabelColor(QColor(255,255,255));
	delta_chart_->xAxis->grid()->setPen(QPen(QColor(128,0,0)));
	delta_chart_->xAxis->setLabelColor(QColor(255,255,255));
	delta_chart_->xAxis->setTickPen(QColor(Qt::white));
	delta_chart_->xAxis->setSubTickPen(QColor(Qt::white));

	delta_chart_->yAxis->setRange(-2, 1);
	delta_chart_->yAxis->setTickLabelColor(QColor(255,255,255));
	delta_chart_->yAxis->grid()->setPen(QPen(QColor(128,0,0)));
	delta_chart_->yAxis->setTickPen(QColor(255,255,255));
	

	delta_line_ = delta_chart_->addGraph();
	delta_line_->setPen(QPen(QColor(255,255,0)));
	ma_line_ = delta_chart_->addGraph();
	ma_line_->setPen(QPen(QColor(255,0,255)));

	//////////////////////////////////////////////////////////////////////////

	//输入delta的周期
	QLabel *label_delta_ma = new QLabel("Delta - MA  ==> ",this);

	del_ma_toolbtn_ = new QToolButton(this);
	del_ma_toolbtn_->setText("delta-ma");
	del_ma_toolbtn_->setFixedSize(60, 30);
	del_ma_toolbtn_->setAutoRaise(true);

	QLabel *label_stddev = new QLabel(tr("  |   标准差周期: %1 | 标准差系数: %2 |  ").arg(std_dev_period_).arg(std_dev_param_));
	std_dev_up_toolbtn_ = new QToolButton(this);
	std_dev_up_toolbtn_->setStyleSheet(tr("background-color:rgb(0,0,0);color:green"));
	std_dev_up_toolbtn_->setText("上轨");
	std_dev_up_toolbtn_->setAutoRaise(true);
	/*std_dev_toolbtn_ = new QToolButton(this);
	std_dev_toolbtn_->setText("标准差");
	std_dev_toolbtn_->setAutoRaise(true);*/
	std_dev_down_toolbtn_ = new QToolButton(this);
	std_dev_down_toolbtn_->setStyleSheet(tr("background-color:rgb(0,0,0);color:rgb(255,0,255)"));
	std_dev_down_toolbtn_->setText("下轨");
	std_dev_down_toolbtn_->setAutoRaise(true);


	period_layout_ = new QHBoxLayout;
	period_layout_->addWidget(label_delta_ma);
	period_layout_->addWidget(del_ma_toolbtn_);
	period_layout_->addWidget(label_stddev);
	period_layout_->addWidget(std_dev_up_toolbtn_);
	//period_layout_->addWidget(std_dev_toolbtn_);
	period_layout_->addWidget(std_dev_down_toolbtn_);
	period_layout_->addStretch();

	
	//////////////////////////////////////////////////////////////////////////

	// delta -均线 的分时图
	delta_ma_chart_ = new QCustomPlot(this);
	delta_ma_chart_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	delta_ma_chart_->setBackground(QBrush(QColor(0,0,0)));
	delta_ma_chart_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	delta_ma_chart_->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	delta_ma_chart_->xAxis->setDateTimeFormat("hh:mm");
	delta_ma_chart_->xAxis->grid()->setPen(QColor(128,0,0));
	delta_ma_chart_->xAxis->setTickLabelColor(QColor(255,255,255));
	delta_ma_chart_->xAxis->setRange(tmp-60*1200, tmp+60*90);
	delta_ma_chart_->xAxis->setTickPen(QColor(255,255,255));
	delta_ma_chart_->xAxis->setSubTickPen(QColor(255,255,255));

	delta_ma_chart_->yAxis->setTickLabelColor(QColor(255,255,255));
	delta_ma_chart_->yAxis->setRange(-1, 1);
	delta_ma_chart_->yAxis->grid()->setPen(QColor(128,0,0));
	delta_ma_chart_->yAxis->setTickPen(QColor(255,255,255));
	
	delta_chart_->installEventFilter(delta_ma_chart_);
	delta_ma_chart_->installEventFilter(delta_chart_);	
	/*assert(connect(delta_chart_, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(SlotMouseMove(QMouseEvent *))));
	connect(delta_ma_chart_, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(SlotMouseMove(QMouseEvent *)));*/

	delta_ma_line_ = delta_ma_chart_->addGraph();
	delta_ma_line_->setPen(QPen(Qt::yellow));
	std_dev_up_line_ = delta_ma_chart_->addGraph();
	std_dev_up_line_->setPen(QPen(Qt::green));
	std_dev_down_line_ = delta_ma_chart_->addGraph();
	std_dev_down_line_->setPen(QPen(QColor(255,0,255)));
}

void portf_PairTrading::SlotMouseMove(QMouseEvent *event){
	if ((QCustomPlot*)sender() == delta_chart_)
		QCoreApplication::sendEvent(delta_ma_chart_, event);
	else
		QCoreApplication::sendEvent(delta_chart_, event);
}

bool portf_PairTrading::eventFilter(QObject *obj, QEvent *event) {
	/*QCustomPlot *receiver = static_cast<QCustomPlot*>(obj);
	if (receiver == delta_ma_chart_) {
	QCoreApplication::postEvent(delta_chart_, event);
	return false;
	}
	else {
	QCoreApplication::postEvent(delta_ma_chart_, event);
	return false;
	}*/
	QCoreApplication::sendEvent(delta_chart_, event);
	return false;
}

// 第一行显示
void portf_PairTrading::CreateExpressionLine() {

	QLabel *label_delta = new QLabel(tr("%1(%2) %3 %4(%5) ==> ").arg(a_).arg(inst_a_.c_str()).arg(opera_).arg(b_).arg(inst_b_.c_str()), this);
	label_delta->setFixedHeight(30);

	delta_toolbtn_ = new QToolButton(this);
	delta_toolbtn_->setText(tr("delta"));
	delta_toolbtn_->setFixedSize(60, 30);
	delta_toolbtn_->setStyleSheet(tr("background-color:rgb(0,0,0);color:rgb(255,255,0)"));
	delta_toolbtn_->setAutoRaise(true);

	QLabel *label_ma = new QLabel(tr("  |  均线周期 : %1   ===>").arg(delta_period_), this);

	ma_toolbtn_ = new QToolButton(this);
	ma_toolbtn_->setStyleSheet(tr("background-color:rgb(0,0,0);color:rgb(255,0,255)"));
	ma_toolbtn_->setText("ma");
	ma_toolbtn_->setFixedSize(60, 30);
	ma_toolbtn_->setAutoRaise(true);

	title_layout_ = new QHBoxLayout;
	title_layout_->addWidget(label_delta);
	title_layout_->addWidget(delta_toolbtn_);
	title_layout_->addWidget(label_ma);
	title_layout_->addWidget(ma_toolbtn_);
	title_layout_->addStretch();
}




void portf_PairTrading::OnTick(const Tick& tick){
	Locker locker(&spin_lock_);

	//将历史数据读入
	if (a_lasts_.Size()==0) {
		double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0; 

		vector<HisData> a_lasts = g_windApi->getHisMin(inst_a_);
		for(int i=0; i<a_lasts.size(); ++i){
			a_times_.Append(key - 60*(a_lasts.size()-i));
			a_lasts_.Append(a_lasts[i].close);
		}
		vector<HisData> b_lasts = g_windApi->getHisMin(inst_b_);
		for(int i=0; i<b_lasts.size(); ++i){
			//b_times_.Append(HisData::GetSecsSinceEpoch(b_lasts[i].date_time));
			b_lasts_.Append(b_lasts[i].close);
		}
		//计算历史的delta ma等 
		for(int i=0; i<a_times_.Size(); ++i) { //先假设a和b的时间序列一致
			double del;
			switch(opera_){
				CALCU_CASE_2('+', +)
				CALCU_CASE_2('-', -)
				CALCU_CASE_2('*', *)
				CALCU_CASE_2('/', /)
				default: break;
			}
			deltas_.Append(del);
			double ma = Average(deltas_, delta_period_);				
			mas_.Append(ma);
			deltas_ma_.Append(del - ma);
			double avg = Average(deltas_ma_, std_dev_period_);
			avgs_.Append(avg);
			double std_dev = StandardDev(avgs_, deltas_ma_, std_dev_period_);
			std_dev_.Append(std_dev);

			delta_line_->addData(*a_times_.Data(i), del);
			ma_line_->addData(*a_times_.Data(i), ma);
			delta_ma_line_->addData(*a_times_.Data(i), del -ma);
			std_dev_up_line_->addData(*a_times_.Data(i), std_dev*std_dev_param_);
			std_dev_down_line_->addData(*a_times_.Data(i), -std_dev*std_dev_param_);
		}
	}

	//qDebug() << a_times_.Size()<<" "<<a_lasts_.Size()<<" "<<b_lasts_.Size();

	//时间序列先
	int temp = Tick::GetSecsSinceEpoch(tick.rt_date, tick.rt_time);
	if (temp - a_times_[0] >= 60) {
		a_times_.Append((temp - a_times_[0])/60*60 + a_times_[0]);
	}

	if (tick.instrument == inst_a_) {
		if(a_lasts_.Size() < a_times_.Size()) {			
			a_lasts_.Append(tick.rt_last);
		}
		else{
			a_lasts_[0] = tick.rt_last;
		}
	}
	else if (tick.instrument == inst_b_){		
		if(b_lasts_.Size() < a_times_.Size()) {			
			b_lasts_.Append(tick.rt_last);
		}
		else{
			b_lasts_[0] = tick.rt_last;
		}
	}
	/*qDebug()<<a_times_.Size()<<" "<<a_lasts_.Size()<<" "<<b_lasts_.Size();
	qDebug()<<a_times_[0]<<" "<<a_lasts_[0]<<" "<<b_lasts_[0];*/
	//
	if (a_lasts_.Size() == b_lasts_.Size()) {
		double del = 0.;
		double ma = 0.; // delta的240均线
		double avg = 0.; // delta-ma的720均线
		double std_dev = 0.;
		switch(opera_){
			CALCU_CASE('+', +)
			CALCU_CASE('-', -)
			CALCU_CASE('*', *)
			CALCU_CASE('/', /)
			default: break;
		}
		//delta计算值超出范围，用上一个值代替
		if(del<-2||del>2) del = deltas_[0];

		if (deltas_.Size() < a_times_.Size()) { // 添加之
			deltas_.Append(del);
			ma = Average(deltas_, delta_period_);
			mas_.Append(ma);
			deltas_ma_.Append(del - ma);
			avg = Average(deltas_ma_, std_dev_period_);
			avgs_.Append(avg);
			std_dev = StandardDev(avgs_, deltas_ma_, std_dev_period_);
			std_dev_.Append(std_dev);
			
			//更新图形
			delta_line_->addData(a_times_[0], del);
			ma_line_->addData(a_times_[0], ma);
			delta_ma_line_->addData(a_times_[0], del -ma);	
			std_dev_up_line_->addData(a_times_[0], std_dev*std_dev_param_);
			std_dev_down_line_->addData(a_times_[0], -std_dev*std_dev_param_);
		}
		else if (deltas_.Size() == a_times_.Size()) { // 更新之
			deltas_[0] = del;
			ma = Average(deltas_, delta_period_);
			mas_[0] = ma;
			deltas_ma_[0] = del - ma;
			avg = Average(deltas_ma_, std_dev_period_);
			avgs_[0] = avg;
			std_dev = StandardDev(avgs_, deltas_ma_, std_dev_period_);
			std_dev_[0] = std_dev;

			//更新图形
			delta_line_->removeData(a_times_[0]);
			delta_line_->addData(a_times_[0], del);
			ma_line_->removeData(a_times_[0]);
			ma_line_->addData(a_times_[0], ma);
			delta_ma_line_->removeData(a_times_[0]);	
			delta_ma_line_->addData(a_times_[0], del -ma);	
			std_dev_up_line_->removeData(a_times_[0]);
			std_dev_up_line_->addData(a_times_[0], std_dev*std_dev_param_);
			std_dev_down_line_->removeData(a_times_[0]);
			std_dev_down_line_->addData(a_times_[0], -std_dev*std_dev_param_);
		}
		else{
			assert(false);
		}
		//qDebug()<<del<<" "<<avg;
		delta_toolbtn_->setText(QString::number(del, 'f', 3));
		ma_toolbtn_->setText(QString::number(ma, 'f', 3));
		del_ma_toolbtn_->setText(QString::number(del - ma, 'f', 3));
		std_dev_up_toolbtn_->setText(QString::number(std_dev*std_dev_param_, 'f', 3));
		//std_dev_toolbtn_->setText(tr("上|下"));
		std_dev_down_toolbtn_->setText(QString::number(-std_dev*std_dev_param_, 'f', 3));
		//坐标轴的显示区间
		/*delta_chart_->xAxis->setRange(a_times_[0]-60*1200, a_times_[0]+60*90);		
		delta_ma_chart_->xAxis->setRange(a_times_[0]-60*1200, a_times_[0]+60*90);*/
		
	}

	emit SignalUpdateUi();
}