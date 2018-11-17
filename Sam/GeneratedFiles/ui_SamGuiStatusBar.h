/********************************************************************************
** Form generated from reading UI file 'SamGuiStatusBar.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISTATUSBAR_H
#define UI_SAMGUISTATUSBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiStatusBarClass
{
public:

    void setupUi(QWidget *SamGuiStatusBarClass)
    {
        if (SamGuiStatusBarClass->objectName().isEmpty())
            SamGuiStatusBarClass->setObjectName(QString::fromUtf8("SamGuiStatusBarClass"));
        SamGuiStatusBarClass->resize(400, 300);

        retranslateUi(SamGuiStatusBarClass);

        QMetaObject::connectSlotsByName(SamGuiStatusBarClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiStatusBarClass)
    {
        SamGuiStatusBarClass->setWindowTitle(QApplication::translate("SamGuiStatusBarClass", "SamGuiStatusBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiStatusBarClass: public Ui_SamGuiStatusBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISTATUSBAR_H
