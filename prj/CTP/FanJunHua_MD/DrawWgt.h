#ifndef DRAWWGT_H
#define DRAWWGT_H

#include <QWidget>

class DrawWgt : public QWidget
{
	Q_OBJECT

public:
	DrawWgt(QWidget *parent = 0);
	~DrawWgt();

protected:
	void paintEvent(QPaintEvent *);	
};

#endif // DRAWWGT_H
