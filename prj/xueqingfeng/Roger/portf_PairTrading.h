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

typedef Array<int> TimeSeries; // intֵΪ�����ڵ�����

class QHBoxLayout;
//��Խ���
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

	QCustomPlot *delta_chart_;// delta �� MA ͼ��
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
	QCPGraph *std_dev_up_line_; // ��׼����Ϲ�
	QCPGraph *std_dev_down_line_; // ��׼����¹�

	SpinLock spin_lock_;
	TimeSeries a_times_; // ʱ������
	NumericSeries a_lasts_; //���¼�����
	//TimeSeries b_times_;
	NumericSeries b_lasts_;

	NumericSeries deltas_; // ��delta������
	NumericSeries mas_; // ��ma������
	NumericSeries deltas_ma_; // delta - ma

	NumericSeries avgs_; // delta-ma�ľ���
	NumericSeries std_dev_; // delta-ma�ı�׼��

	string inst_a_;
	string inst_b_;
	double a_; // ϵ��
	double b_;
	char opera_; // �Ӽ��˳�
	int delta_period_; // delta���ߵ����� 240
	int std_dev_period_; // ��׼������ 720
	double std_dev_param_; // ��׼��ϵ�� 1.5
	

	virtual void OnTick(const Tick& tick);

	virtual void Update() { 
		//��Y������Ӧ
		/*if(delta_line_)delta_line_->rescaleValueAxis(false);
		if(delta_ma_line_)delta_ma_line_->rescaleValueAxis(false);	*/

		if (delta_chart_) delta_chart_->replot();
		if (delta_ma_chart_) delta_ma_chart_->replot();
	}
};


#endif PORTF_PAIRTRADING_H