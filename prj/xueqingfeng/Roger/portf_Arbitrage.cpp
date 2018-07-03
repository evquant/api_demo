#include "portf_Arbitrage.h"
#include <QtGui/QtGui>


portf_Arbitrage::portf_Arbitrage(QWidget *parent):QWidget(parent)
{
	QHBoxLayout *titleLayout = new QHBoxLayout;
	title_ = new QLabel("", this);
	titleLayout->addStretch();
	titleLayout->addWidget(title_);
	titleLayout->addStretch();

	static QString s_Labels[NumLabels] = {
		tr("名义基差"),
		tr("实际基差"),
		tr("隐含收益率")
	};
	QGridLayout *gridLayout = new QGridLayout;
	for (int i = 0; i < NumLabels; ++i) {
		labels_[i] = new QLabel(s_Labels[i], this);
		lineEdits_[i] = new QLineEdit(this);
		gridLayout->addWidget(labels_[i], i + 1, 0);
		gridLayout->addWidget(lineEdits_[i], i + 1, 1);
	}

	static QString s_Btns[NumButtons] = {
		tr("Long"),
		tr("Short")
	};
	QHBoxLayout *btnLayout = new QHBoxLayout;
	for (int i = 0; i < NumButtons; ++i) { 
		buttons_[i] = new QPushButton(s_Btns[i], this);
		btnLayout->addWidget(buttons_[i]);
	}
	
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(titleLayout);
	mainLayout->addLayout(gridLayout);
	mainLayout->addLayout(btnLayout);
}



// _____________________________________________________________________________________________________________________________
//void portf_Arbitrage::update()
//{
//	// add strategy here ...
//	double IF_b = rtTick.f_IF1509.rt_last - rtTick.idx_000300.rt_last;
//	double IF_rb = rtTick.f_IF1509.rt_last - rtTick.s_510300.rt_last * 1000;
//	double IF_yield = 0;
//	/*IF_basis->setText( QString::number( IF_b));
//	IF_real_basis->setText( QString::number( IF_rb));
//	IF_y->setText( QString::number( IF_yield));*/
//	
//	/*IF_basis->setText( QString::number( IF_b,'f',2));
//	IF_real_basis->setText( QString::number( IF_rb,'f',2));
//	IF_y->setText( QString::number( IF_yield,'f',2));*/
//
//
//	double IH_b = rtTick.f_IH1509.rt_last - rtTick.idx_000016.rt_last;
//	double IH_rb = rtTick.f_IH1509.rt_last - rtTick.s_510050.rt_last * 1000;
//	double IH_yield = 0;
//	/*IH_basis->setText( QString::number( IH_b));
//	IH_real_basis->setText( QString::number( IH_rb));
//	IH_y->setText( QString::number( IH_yield));*/
//	
//	/*IH_basis->setText( QString::number( IH_b,'f',2));
//	IH_real_basis->setText( QString::number( IH_rb,'f',2));
//	IH_y->setText( QString::number( IH_yield,'f',2));*/
//}