#include "mainwindow.h"
#include <QtGui>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
	, close_close_(NumVector, 0)
	, BidA_AskB_(NumVector, 0)
	, AskA_BidB_(NumVector, 0)
{	
	memset(&tick0, 0, sizeof(FutureTick));
	memset(&tick1, 0, sizeof(FutureTick));

	createGroupBox();
	createButtonBox();

	QVBoxLayout *mdLayout = new QVBoxLayout;
	mdLayout->addWidget(groupBox_);
	mdLayout->addWidget(btnBox_);
	show_logs_ = new QTextEdit;
	mdLayout->addWidget(show_logs_);

	createChart();

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(mdLayout);
	mainLayout->addStretch(1);
	//mainLayout->addWidget(chart_);
	drawWgt_ = new DrawWgt;
	mainLayout->addWidget(drawWgt_, 1);
	setLayout(mainLayout);

	api_ = new CtpFutureMarketDataApi;
	
}

void MainWindow::Subscribe() {
	string err;
	if (api_) {
		if (!api_->Subscribe(lineEdits_[0]->text().toStdString().c_str(), err)) 
			show_logs_->append(err.c_str());
		
		if (!api_->Subscribe(lineEdits_[1]->text().toStdString().c_str(), err))
			show_logs_->append(err.c_str());
	}

	show_logs_->append("Subscribe SUCCESS");
}

void MainWindow::UnSubscribe() {
	string err;
	if (api_) {
		if (!api_->UnSubscribe(lineEdits_[0]->text().toStdString().c_str(), err)) 
			show_logs_->append(err.c_str());

		if (!api_->UnSubscribe(lineEdits_[1]->text().toStdString().c_str(), err))
			show_logs_->append(err.c_str());
	}

	show_logs_->append("UnSubscribe SUCCESS");
}

bool MainWindow::Init() {
	string err;
	if (!api_->Init("tcp://222.66.97.241:41213", this, err)) {
		show_logs_->append(err.c_str());
		return false;
	}
	if (!api_->Login("7070", "", "", err))  {
		show_logs_->append(err.c_str());
		return false;
	}
	show_logs_->append("LOGIN SUCCESS");
	return true;
}

MainWindow::~MainWindow()
{

}

void MainWindow::createButtonBox() {
	btnBox_ = new QGroupBox(tr("Controller"));
	QHBoxLayout *layout = new QHBoxLayout(btnBox_);

	for (int i = 0; i < NumButtons; ++i) {
		buttons_[i] = new QPushButton;
		layout->addWidget(buttons_[i]);
		layout->addSpacing(10);
	}
	buttons_[0]->setText(tr("Subscribe"));
	connect(buttons_[0], SIGNAL(clicked()), this, SLOT(Subscribe()));
	buttons_[1]->setText(tr("UnSubscribe"));
	connect(buttons_[1], SIGNAL(clicked()), this, SLOT(UnSubscribe()));

	btnBox_->setLayout(layout);
}

void MainWindow::createGroupBox() {
	groupBox_ = new QGroupBox(tr("MarketData Shows"));
	QGridLayout *layout = new QGridLayout(groupBox_);
	layout->setSpacing(10);
	for (int i = 0; i < NumLabels; ++i) {
		labels_[i] = new QLabel(tr("0"), this);
		labels_[i]->setAlignment(Qt::AlignCenter);
		layout->addWidget(labels_[i], i/2, i%2);
	}

	for (int i = 0; i < 2; ++i) {
		spinBoxs_[i] = new QSpinBox(this);
		spinBoxs_[i]->setRange(1, 500);
		spinBoxs_[i]->setValue(1);
		layout->addWidget(spinBoxs_[i], 3, i);
	}
	
	for (int i = 0; i < 2; ++i) {
		lineEdits_[i] = new QLineEdit(this);
		layout->addWidget(lineEdits_[i], 4, i);
	}
	lineEdits_[0]->setText("IF1510");
	lineEdits_[1]->setText("IF1512");
}

void MainWindow::createChart() {
	chart_ = new ZoomWidget(this);

	chart_->setDataset(0, close_close_, "close-close");
	chart_->setDataset(1, BidA_AskB_, "BidA-AskB");
	chart_->setDataset(2, AskA_BidB_, "AskA-BidB_");
	/*m_model = new QStandardItemModel(this);
	m_model->insertRows(0, 50);
	m_model->insertColumns(0, 3);
	for (int row = 0; row < m_model->rowCount(); ++row)
	for (int column = 0; column < m_model->rowCount(); ++column) {
	QModelIndex index = m_model->index(row, column);
	m_model->setData(index, qrand()%30);
	}

	m_lines = new KDChart::LineDiagram(this);
	m_lines->setModel(m_model);*/

	QPen pen;
	QBrush brush;
	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::black);
	chart_->lineDiagram()->setPen(0, pen);
	pen.setColor(Qt::green);
	chart_->lineDiagram()->setPen(1, pen);
	pen.setColor(Qt::red);
	chart_->lineDiagram()->setPen(2, pen);

	KDChart::Legend *legend = new KDChart::Legend(chart_);
	legend->setPosition(KDChart::Position::East);
	legend->setAlignment(Qt::AlignCenter);
	legend->setOrientation(Qt::Vertical);
	legend->setLegendStyle(KDChart::Legend::LinesOnly);
	legend->setShowLines(true);
	chart_->addLegend(legend);

	KDChart::CartesianAxis *xAxis = new KDChart::CartesianAxis(chart_->lineDiagram());
	KDChart::CartesianAxis *yAxis = new KDChart::CartesianAxis(chart_->lineDiagram());
	xAxis->setPosition(KDChart::CartesianAxis::Bottom);
	yAxis->setPosition(KDChart::CartesianAxis::Left);
	xAxis->setTitleText("Time");
	yAxis->setTitleText("Value");
	chart_->lineDiagram()->addAxis(xAxis);
	chart_->lineDiagram()->addAxis(yAxis);
	chart_->lineDiagram()->adjustSize();

	KDChart::LineAttributes la(chart_->lineDiagram()->lineAttributes());
	la.setDisplayArea(true);
	la.setTransparency(25);
	chart_->lineDiagram()->setLineAttributes(la);
	
}

void MainWindow::OnMarketPrice(BaseTick* market_price) {
	FutureTick *tick = (FutureTick*)market_price;

	if (lineEdits_[0]->text() == market_price->symbol.instrument) {
		tick0 = *tick;
		labels_[0]->setText(QString::number(tick0.sell_price*spinBoxs_[0]->text().toInt(), 'f', 3));
		labels_[2]->setText(QString::number(tick0.buy_price*spinBoxs_[0]->text().toInt(), 'f', 3));
		labels_[4]->setText(QString::number(tick0.buy_price-tick1.sell_price, 'f', 3));
	}
	else if (lineEdits_[1]->text() == market_price->symbol.instrument) {
		tick1 = *tick;
		labels_[1]->setText(QString::number(tick1.buy_price*spinBoxs_[1]->text().toInt(), 'f', 3));
		labels_[3]->setText(QString::number(tick1.sell_price*spinBoxs_[1]->text().toInt(), 'f', 3));
		labels_[5]->setText(QString::number(tick0.sell_price-tick1.buy_price, 'f', 3));
	}

	double close_close = tick0.last_price*spinBoxs_[0]->text().toInt() - tick1.last_price*spinBoxs_[1]->text().toInt();
	double BidA_AskB = tick0.buy_price*spinBoxs_[0]->text().toInt() - tick1.sell_price*spinBoxs_[1]->text().toInt();
	double AskA_BidB = tick0.sell_price*spinBoxs_[0]->text().toInt() - tick1.buy_price*spinBoxs_[1]->text().toInt();

	if (close_close > 500) return;
	/*if (close_close_.size() >= NumVector) {
	close_close_.pop_front();
	BidA_AskB_.pop_front();
	AskA_BidB_.pop_front();
	}
	close_close_.push_back(close_close);
	BidA_AskB_.push_back(BidA_AskB);
	AskA_BidB_.push_back(AskA_BidB);*/
	
	static int i = 0;
	chart_->setDataCell(i%NumVector, 0, close_close);
	chart_->setDataCell(i%NumVector, 1, BidA_AskB);
	chart_->setDataCell(i%NumVector, 2, AskA_BidB);
	chart_->update();
	++i;

	show_logs_->append(QString("%1 : LastPrice %2").arg(tick->symbol.instrument).arg(QString::number(tick->last_price,'f',3)));
	
	
}
