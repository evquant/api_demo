/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "../../qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionInsert_Plot;
    QAction *actionSave_Document;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout;
    QCustomPlot *plot;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *sbWidth;
    QLabel *label_2;
    QSpinBox *sbHeight;
    QCheckBox *cbUseCurrentSize;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(786, 440);
        actionInsert_Plot = new QAction(MainWindow);
        actionInsert_Plot->setObjectName(QString::fromUtf8("actionInsert_Plot"));
        actionSave_Document = new QAction(MainWindow);
        actionSave_Document->setObjectName(QString::fromUtf8("actionSave_Document"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setMinimumSize(QSize(400, 0));

        horizontalLayout_2->addWidget(textEdit);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        plot = new QCustomPlot(centralWidget);
        plot->setObjectName(QString::fromUtf8("plot"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(plot);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        sbWidth = new QSpinBox(centralWidget);
        sbWidth->setObjectName(QString::fromUtf8("sbWidth"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sbWidth->sizePolicy().hasHeightForWidth());
        sbWidth->setSizePolicy(sizePolicy1);
        sbWidth->setMinimum(1);
        sbWidth->setMaximum(9999);
        sbWidth->setValue(480);

        horizontalLayout->addWidget(sbWidth);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        sbHeight = new QSpinBox(centralWidget);
        sbHeight->setObjectName(QString::fromUtf8("sbHeight"));
        sizePolicy1.setHeightForWidth(sbHeight->sizePolicy().hasHeightForWidth());
        sbHeight->setSizePolicy(sizePolicy1);
        sbHeight->setMinimum(1);
        sbHeight->setMaximum(9999);
        sbHeight->setValue(340);

        horizontalLayout->addWidget(sbHeight);

        cbUseCurrentSize = new QCheckBox(centralWidget);
        cbUseCurrentSize->setObjectName(QString::fromUtf8("cbUseCurrentSize"));

        horizontalLayout->addWidget(cbUseCurrentSize);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionInsert_Plot);
        mainToolBar->addAction(actionSave_Document);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QCustomPlot Document Insertion Demo", 0, QApplication::UnicodeUTF8));
        actionInsert_Plot->setText(QApplication::translate("MainWindow", "Insert Plot", 0, QApplication::UnicodeUTF8));
        actionSave_Document->setText(QApplication::translate("MainWindow", "Save Document...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Size of plot<br/>in document:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "x", 0, QApplication::UnicodeUTF8));
        cbUseCurrentSize->setText(QApplication::translate("MainWindow", "Use current", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
