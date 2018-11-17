/********************************************************************************
** Form generated from reading UI file 'UiProgressBar.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIPROGRESSBAR_H
#define UI_UIPROGRESSBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiProgressBarClass
{
public:

    void setupUi(QWidget *UiProgressBarClass)
    {
        if (UiProgressBarClass->objectName().isEmpty())
            UiProgressBarClass->setObjectName(QString::fromUtf8("UiProgressBarClass"));
        UiProgressBarClass->resize(400, 300);

        retranslateUi(UiProgressBarClass);

        QMetaObject::connectSlotsByName(UiProgressBarClass);
    } // setupUi

    void retranslateUi(QWidget *UiProgressBarClass)
    {
        UiProgressBarClass->setWindowTitle(QApplication::translate("UiProgressBarClass", "UiProgressBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiProgressBarClass: public Ui_UiProgressBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIPROGRESSBAR_H
