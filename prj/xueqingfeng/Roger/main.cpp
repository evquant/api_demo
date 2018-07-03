#include <QtGui/QApplication>
#include <QTextCodec>
#include <QDateTime>
#include <vector>
#include <QtCore>

#include "configInfo.h"
#include "RealTimeTick.h"
#include "MainWindow.h"
#include "WindApi.h"

configInfo *g_config;
WindApi *g_windApi;

void RunInThread(){
	if(g_config && g_windApi){
		
		g_windApi->loginWind();
	}
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

	g_config = new configInfo();
	g_config->Init("subscribe.txt");
	g_windApi = new WindApi;

	MainWindow w;
	g_config->SetMsgHandler(&w);
	g_windApi->SetMsgHandler(&w);
	g_windApi->addStrategy(&w);
	w.show();

	QtConcurrent::run(RunInThread);

	return app.exec();
}

