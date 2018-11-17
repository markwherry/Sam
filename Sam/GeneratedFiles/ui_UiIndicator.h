/********************************************************************************
** Form generated from reading UI file 'UiIndicator.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIINDICATOR_H
#define UI_UIINDICATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiIndicatorClass
{
public:

    void setupUi(QWidget *UiIndicatorClass)
    {
        if (UiIndicatorClass->objectName().isEmpty())
            UiIndicatorClass->setObjectName(QString::fromUtf8("UiIndicatorClass"));
        UiIndicatorClass->resize(400, 300);

        retranslateUi(UiIndicatorClass);

        QMetaObject::connectSlotsByName(UiIndicatorClass);
    } // setupUi

    void retranslateUi(QWidget *UiIndicatorClass)
    {
        UiIndicatorClass->setWindowTitle(QApplication::translate("UiIndicatorClass", "UiIndicator", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiIndicatorClass: public Ui_UiIndicatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIINDICATOR_H
