#include "TimeSharingplan.h"

#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_point_data.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot_canvas.h>
#include <qmath.h>
#include <qwt_plot_magnifier.h>  
#include <qwt_plot_panner.h>  


TimeSharingplan::TimeSharingplan( QWidget *parent ):
    QwtPlot( parent ), x_(0)
{
    setAutoReplot( false );

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    setCanvas( canvas );
    setCanvasBackground( QColor(0,0,0) );//黑色背景
	//使用滚轮放大/缩小  
	(void) new QwtPlotMagnifier( canvas );
	//使用鼠标左键平移  
	(void) new QwtPlotPanner( canvas );  

    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    //grid->enableXMin( true );
    grid->setMajorPen( Qt::white, 0, Qt::DotLine );
    //grid->setMinorPen( Qt::white, 0 , Qt::DotLine );
    grid->attach( this );

    // axes
    //enableAxis( QwtPlot::yRight );
	setAxisScale(QwtPlot::xBottom, 0, 500);
	setAxisScale(QwtPlot::yLeft, -500, 500);
	/*setAxisScale(QwtPlot::yRight, 0, 30);*/
    
    //setAxisScaleEngine( QwtPlot::xBottom, new QwtLogScaleEngine );

    // curves
    curve_delta = new QwtPlotCurve( "曲线a" );
	curve_delta->setStyle(QwtPlotCurve::Lines);
	curve_delta->setCurveAttribute(QwtPlotCurve::Fitted, true);
    curve_delta->setPen( Qt::yellow );
    curve_delta->setYAxis( QwtPlot::yLeft );
    curve_delta->attach( this );

    setAutoReplot( true );
}

void TimeSharingplan::OnDelta(double delta){
	Locker locker(&spin_lock_);
	xs.append(++x_);
	ys.append(delta);
	QwtPointArrayData * const data = new QwtPointArrayData(xs, ys);  
	curve_delta->setData(data);
}