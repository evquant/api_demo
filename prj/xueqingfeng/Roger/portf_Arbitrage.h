#ifndef PORTF_ARBITRAGE_H
#define PORTF_ARBITRAGE_H

#include <QWidget>
#include <string>
#include "WindApi.h"
#include "qcustomplot.h"
class QLabel;
class QLineEdit;
class QPushButton;
//期限套利
class portf_Arbitrage : public QWidget, public WindDataSpi, public BaseUi
{
	Q_OBJECT

public:
	portf_Arbitrage(QWidget *parent = 0);

	
private:
	enum { NumLabels = 3, NumButtons = 2 };
	QLabel *title_;
	QLabel *labels_[NumLabels];
	QLineEdit *lineEdits_[NumLabels];
	QPushButton *buttons_[NumButtons];

	//青峰： 这里定义策略需要的变量，并且在OnTick回调中进行计算
	// ...
	virtual void OnTick(const Tick& tick){}

	virtual void Update() {}
};


#endif PORTF_ARBITRAGE_H