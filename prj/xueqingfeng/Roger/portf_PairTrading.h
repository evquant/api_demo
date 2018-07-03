#ifndef PORTF_PAIRTRADING_H
#define PORTF_PAIRTRADING_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QToolButton>
#include <QDateTime>
#include <set>
#include "SpinLock.h"
#include "WindApi.h"
#include "qcustomplot.h"
#include "Array.h"

typedef Array<int> TimeSeries; // int值为距现在的秒数

class QHBoxLayout;
//配对交易
class portf_PairTrading : public QWidget, public WindDataSpi, public BaseUi
{
	Q_OBJECT

public:
	portf_PairTrading(int idx, QWidget *parent = 0);


protected:
	bool eventFilter(QObject *obj, QEvent *event);

Q_SIGNALS:
	void SignalUpdateUi();

private slots:
	void SlotMouseMove(QMouseEvent *event);
	void SlotUpdateUi(){
		delta_chart_->replot();
		delta_ma_chart_->replot();
	}

private:
	void CreateExpressionLine();
	void CreateTimeSharingplan();
	

	QToolButton *delta_toolbtn_;
	QToolButton *ma_toolbtn_;
	QHBoxLayout *title_layout_;

	QCustomPlot *delta_chart_;// delta 和 MA 图形
	QCPGraph *delta_line_;
	QCPGraph *ma_line_;

	QLineEdit *LineEdit_period;
	QToolButton *del_ma_toolbtn_;
	QToolButton *std_dev_up_toolbtn_;
	QToolButton *std_dev_toolbtn_;
	QToolButton *std_dev_down_toolbtn_;
	QHBoxLayout *period_layout_;

	QCustomPlot *delta_ma_chart_;
	QCPGraph *delta_ma_line_;
	QCPGraph *std_dev_up_line_; // 标准差的上轨
	QCPGraph *std_dev_down_line_; // 标准差的下轨

	SpinLock spin_lock_;
	TimeSeries a_times_; // 时间序列
	NumericSeries a_lasts_; //最新价序列
	//TimeSeries b_times_;
	NumericSeries b_lasts_;

	NumericSeries deltas_; // 放delta的序列
	NumericSeries mas_; // 放ma的序列
	NumericSeries deltas_ma_; // delta - ma

	NumericSeries avgs_; // delta-ma的均线
	NumericSeries std_dev_; // delta-ma的标准差

	string inst_a_;
	string inst_b_;
	double a_; // 系数
	double b_;
	char opera_; // 加减乘除
	int delta_period_; // delta均线的周期 240
	int std_dev_period_; // 标准差周期 720
	double std_dev_param_; // 标准差系数 1.5
	

	virtual void OnTick(const Tick& tick);

	virtual void Update() { 
		//让Y轴自适应
		/*if(delta_line_)delta_line_->rescaleValueAxis(false);
		if(delta_ma_line_)delta_ma_line_->rescaleValueAxis(false);	*/

		if (delta_chart_) delta_chart_->replot();
		if (delta_ma_chart_) delta_ma_chart_->replot();
	}
};


#endif PORTF_PAIRTRADING_H