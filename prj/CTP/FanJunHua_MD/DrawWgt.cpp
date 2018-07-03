#include "DrawWgt.h"
#include <QtGui/QtGui>

DrawWgt::DrawWgt(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(500, 800);
}

DrawWgt::~DrawWgt()
{

}

void DrawWgt::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawLine(QPoint(70,40), QPoint(150,100));
	QPen pen(Qt::green);
	painter.setPen(pen);
	QRectF rectangle(70.0,40.0,80.0,60.0); 
	painter.drawRect(rectangle);
	painter.setPen(Qt::red);
	//painter.drawArc(rectangle, 30*16, 180*16);
	painter.drawEllipse(rectangle);

	pen.setWidth(1); 
	pen.setStyle(Qt::SolidLine); 
	painter.setPen(pen); 
	QBrush brush(Qt::blue, Qt::CrossPattern);
	painter.setBrush(brush);
	//绘制一个矩形 
	painter.drawRect(160,20,50,40); 


	//定义四个点 
	static const QPointF points[4]={ 
		QPointF(270.0,80.0), 
		QPointF(290.0,10), 
		QPointF(400.0,30), 
		QPointF(390.0,50) 
	}; 		
	painter.drawPolygon(points, 4);

	painter.fillRect(QRect(10,100,150,20),QBrush(Qt::darkYellow)); 
	painter.eraseRect(QRect(50,0,50,120)); 


	QLinearGradient linearGradient(QPointF(40,190),QPointF(70,190)); 
	//插入颜色 
	linearGradient.setColorAt(0,Qt::yellow); 
	linearGradient.setColorAt(0.5,Qt::red); 
	linearGradient.setColorAt(1.0,Qt::green); 
	linearGradient.setSpread(QGradient::ReflectSpread); 
	//使用渐变作为画刷 
	painter.setBrush(linearGradient); 
	painter.drawRect(10,170,90,40); 


	//辐射渐变 
	QRadialGradient radialGradient(QPointF(200,190),50,QPointF(275,200)); 
	radialGradient.setColorAt(0,QColor(255,255,100,150)); 
	radialGradient.setColorAt(1,QColor(0,0,0,50)); 
	painter.setBrush(radialGradient); 
	painter.drawEllipse(QPointF(200,190),50,50); 
}