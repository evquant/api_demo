#ifndef _PLOT_H_
#define _PLOT_H_

#include<qwt_plot.h>
#include "SpinLock.h"

class QwtPlotCurve;
class QwtPlotMarker;

class TimeSharingplan: public QwtPlot
{
    Q_OBJECT

public:
    TimeSharingplan( QWidget *parent );
	void OnDelta(double delta);

private:
    QwtPlotCurve *curve_a;
	QwtPlotCurve *curve_b;

	QwtPlotCurve *curve_delta;
	int x_;
	QVector<double> xs;  
	QVector<double> ys;  
	SpinLock spin_lock_;
};

#endif
