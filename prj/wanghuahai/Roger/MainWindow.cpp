#include "mainwindow.h"
#include <QBoxLayout>
#include <QDockWidget>
#include <qlabel.h>
#include <QSplitter>
#include <QHeaderView>
#include <QDateTime>
#include "portf_Arbitrage.h"
#include "portf_PairTrading.h"
#include "qcustomplot.h"

extern WindApi *g_windApi;
extern configInfo *g_config;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

	md_model_ = new MarketDataTable(this);
	md_model_->SetMsgHandler(this);
	g_windApi->addStrategy(md_model_);
	market_data_table_ = new QTableView(this);
	market_data_table_->setModel(md_model_);
	market_data_table_->setSortingEnabled(true);
	/*assert(connect(market_data_table_->horizontalHeader(), SIGNAL(sectionClicked(int)),
		md_model_, SLOT(SlotSort(int))), Qt::QueuedConnection);*/
	
	//market_data_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
	market_data_table_->verticalHeader()->setDefaultSectionSize(22);
	market_data_table_->verticalHeader()->setVisible(false);
	market_data_table_->horizontalHeader()->setFixedHeight(25);
	market_data_table_->horizontalHeader()->setDefaultSectionSize(70);

	QFont font1;
	font1.setBold(true);
	market_data_table_->horizontalHeader()->setFont(font1);

	central_wgt_ = new QWidget(this);
	QHBoxLayout *main_layout = new QHBoxLayout(central_wgt_);
	main_layout->addWidget(market_data_table_);
	setCentralWidget(central_wgt_);

	

	QDockWidget *log_dock = new QDockWidget(tr("log"), this);
	log_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
	log_output_ = new QTextEdit(this);
	log_dock->setWidget(log_output_);
	addDockWidget(Qt::BottomDockWidgetArea, log_dock);

	//每隔100ms刷新一次分时图
	connect(&dataTimer_, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	dataTimer_.start(100);

	resize(800,600);
}

void MainWindow::OnTick(const Tick& tick){
	//log_output_->append(tick.Str().c_str());
}

void MainWindow::PrintImp(const std::string &msg){
	QDateTime now= QDateTime::currentDateTime();
	QString out_put = QString("%1")/*.arg(now.toString("hh:mm:ss"))*/.arg(msg.c_str());
	log_output_->append(out_put);
}