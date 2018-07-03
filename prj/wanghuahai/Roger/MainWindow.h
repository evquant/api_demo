#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTextEdit>
#include <QTableView>
#include <QTimer>
#include "MarketDataTable.h"
#include "MsgDisplay.h"
#include "WindApi.h"
#include "qcustomplot.h"

class BaseUi;

class MainWindow : public QMainWindow, public WindDataSpi, public MsgDisplaySpi
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);

private:
	QWidget *central_wgt_;
	QTextEdit *log_output_;
	QTableView *market_data_table_;
	MarketDataTable *md_model_;

	virtual void OnTick(const Tick& tick);
	virtual void PrintImp(const std::string &msg);

	QTimer dataTimer_;
	QVector<BaseUi *> uis_;

private slots:
	void realtimeDataSlot(){
		for (int i = 0; i< uis_.size(); ++i){
			uis_[i]->Update();
		}
	}
};

#endif // MAINWINDOW_H
