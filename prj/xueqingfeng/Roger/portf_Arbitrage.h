#ifndef PORTF_ARBITRAGE_H
#define PORTF_ARBITRAGE_H

#include <QWidget>
#include <string>
#include "WindApi.h"
#include "qcustomplot.h"
class QLabel;
class QLineEdit;
class QPushButton;
//��������
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

	//��壺 ���ﶨ�������Ҫ�ı�����������OnTick�ص��н��м���
	// ...
	virtual void OnTick(const Tick& tick){}

	virtual void Update() {}
};


#endif PORTF_ARBITRAGE_H