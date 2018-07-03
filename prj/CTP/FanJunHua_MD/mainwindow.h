#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include "../CTPMarketDataApi/CtpFutureMarketDataApi.h"
#include "zoomwidget.h"
#include "DrawWgt.h"

using namespace std;
using namespace kiiik;

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class QSpinBox;
class QSplitter;
class QFrame;
class QStandardItemModel;
QT_END_NAMESPACE

namespace KDChart {
	class Chart;
	class LineDiagram;
}

class MainWindow : public QWidget, public MarketDataSpi
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

	bool Init();

public slots:
	void Subscribe();
	void UnSubscribe();

private:
	virtual void OnMarketPrice(BaseTick* market_price);

	void createGroupBox();
	void createButtonBox();
	void createChart();

private:
	CtpFutureMarketDataApi *api_;
	FutureTick tick0;
	FutureTick tick1;


	enum { NumButtons = 2, NumLabels = 6, NumVector = 50 };

	QGroupBox *groupBox_;
	QTextEdit *show_logs_;
	QLabel *labels_[NumLabels];
	QSpinBox *spinBoxs_[2];
	QLineEdit *lineEdits_[2];
	QDialogButtonBox *buttonBox_;

	QGroupBox *btnBox_;
	QPushButton *buttons_[NumButtons];

	QVector<double> close_close_;
	QVector<double> BidA_AskB_;
	QVector<double> AskA_BidB_;
	ZoomWidget		*chart_;
	//KDChart::LineDiagram       * m_lines;
	//QStandardItemModel         * m_model;

	DrawWgt *drawWgt_;
};

#endif // MAINWINDOW_H
