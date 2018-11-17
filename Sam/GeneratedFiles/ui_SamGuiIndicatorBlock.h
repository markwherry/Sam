/********************************************************************************
** Form generated from reading UI file 'SamGuiIndicatorBlock.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIINDICATORBLOCK_H
#define UI_SAMGUIINDICATORBLOCK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiIndicatorBlockClass
{
public:

    void setupUi(QWidget *SamGuiIndicatorBlockClass)
    {
        if (SamGuiIndicatorBlockClass->objectName().isEmpty())
            SamGuiIndicatorBlockClass->setObjectName(QString::fromUtf8("SamGuiIndicatorBlockClass"));
        SamGuiIndicatorBlockClass->resize(400, 300);

        retranslateUi(SamGuiIndicatorBlockClass);

        QMetaObject::connectSlotsByName(SamGuiIndicatorBlockClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiIndicatorBlockClass)
    {
        SamGuiIndicatorBlockClass->setWindowTitle(QApplication::translate("SamGuiIndicatorBlockClass", "SamGuiIndicatorBlock", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiIndicatorBlockClass: public Ui_SamGuiIndicatorBlockClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIINDICATORBLOCK_H
