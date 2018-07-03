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
	central_wgt_ = new QWidget;
	QGridLayout *central_layout_ = new QGridLayout(central_wgt_);
	int num = g_config->getNums();

	portf_PairTrading *pair_trade = new portf_PairTrading(0, this);
	central_layout_->addWidget(pair_trade, 0, 0);
	uis_.append(pair_trade);

	/*for (int i=0; i<3; ++i) {
	for (int j=0; j <3; ++j) {
	int idx = i*3 + j;
	if (idx < num) {
	ConfigNode node = g_config->getConfigNode(idx);
	switch(node.trade_type) {
	case E_ARBITRAGE:
	central_layout_->addWidget(new portf_Arbitrage(this), i, j);
	break;
	case E_PAIR_TRADE:
	central_layout_->addWidget(new portf_PairTrading(idx, this), i, j);
	break;
	default:
	central_layout_->addWidget(new QLabel(tr(""),this), i, j);
	break;
	}
	}
	else {
	central_layout_->addWidget(new QLabel(tr(""),this), i, j);
	}
	}
	}*/
	//central_layout_->setColumnStretch(2, 1);

	md_model_ = new MarketDataTable(this);
	g_windApi->addStrategy(md_model_);
	market_data_table_ = new QTableView(this);
	market_data_table_->setModel(md_model_);
	market_data_table_->setSortingEnabled(true);
	assert(connect(market_data_table_->horizontalHeader(), SIGNAL(sectionClicked(int)),
		md_model_, SLOT(SlotSort(int))), Qt::QueuedConnection);
	market_data_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
	market_data_table_->verticalHeader()->setDefaultSectionSize(22);
	market_data_table_->verticalHeader()->setVisible(false);
	market_data_table_->horizontalHeader()->setFixedHeight(25);
	market_data_table_->horizontalHeader()->setDefaultSectionSize(70);

	QFont font1;
	font1.setBold(true);
	market_data_table_->horizontalHeader()->setFont(font1);


	QSplitter *splite = new QSplitter(Qt::Horizontal, this);
	splite->setHandleWidth(10);
	splite->addWidget(central_wgt_);
	splite->addWidget(market_data_table_);
	//setCentralWidget(splite);
#if 0
	setCentralWidget(market_data_table_);
	central_wgt_->hide();
#else
	setCentralWidget(central_wgt_);
	market_data_table_->hide();
#endif
	

	QDockWidget *log_dock = new QDockWidget(tr("log"), this);
	log_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
	log_output_ = new QTextEdit(this);
	log_dock->setWidget(log_output_);
	addDockWidget(Qt::BottomDockWidgetArea, log_dock);

	//每隔100ms刷新一次分时图
	connect(&dataTimer_, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	//dataTimer_.start(100);
}

void MainWindow::OnTick(const Tick& tick){
	//log_output_->append(tick.Str().c_str());
}

void MainWindow::PrintImp(const std::string &msg){
	QDateTime now= QDateTime::currentDateTime();
	QString out_put = QString("%1 : %2").arg(now.toString("hh:mm:ss")).arg(msg.c_str());
	log_output_->append(out_put);
}