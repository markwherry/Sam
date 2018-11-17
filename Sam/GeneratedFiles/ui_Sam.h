/********************************************************************************
** Form generated from reading UI file 'Sam.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAM_H
#define UI_SAM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SamClass)
    {
        if (SamClass->objectName().isEmpty())
            SamClass->setObjectName(QString::fromUtf8("SamClass"));
        SamClass->resize(600, 400);
        menuBar = new QMenuBar(SamClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        SamClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SamClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        SamClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SamClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        SamClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SamClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SamClass->setStatusBar(statusBar);

        retranslateUi(SamClass);

        QMetaObject::connectSlotsByName(SamClass);
    } // setupUi

    void retranslateUi(QMainWindow *SamClass)
    {
        SamClass->setWindowTitle(QApplication::translate("SamClass", "Sam", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamClass: public Ui_SamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAM_H
