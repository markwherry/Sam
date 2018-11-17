/********************************************************************************
** Form generated from reading UI file 'SamGuiInstrumentList.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIINSTRUMENTLIST_H
#define UI_SAMGUIINSTRUMENTLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiInstrumentListClass
{
public:

    void setupUi(QWidget *SamGuiInstrumentListClass)
    {
        if (SamGuiInstrumentListClass->objectName().isEmpty())
            SamGuiInstrumentListClass->setObjectName(QString::fromUtf8("SamGuiInstrumentListClass"));
        SamGuiInstrumentListClass->resize(400, 300);

        retranslateUi(SamGuiInstrumentListClass);

        QMetaObject::connectSlotsByName(SamGuiInstrumentListClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiInstrumentListClass)
    {
        SamGuiInstrumentListClass->setWindowTitle(QApplication::translate("SamGuiInstrumentListClass", "SamGuiInstrumentList", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiInstrumentListClass: public Ui_SamGuiInstrumentListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIINSTRUMENTLIST_H
