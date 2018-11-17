/********************************************************************************
** Form generated from reading UI file 'SamGuiCellAnalyser.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICELLANALYSER_H
#define UI_SAMGUICELLANALYSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiCellAnalyserClass
{
public:

    void setupUi(QWidget *SamGuiCellAnalyserClass)
    {
        if (SamGuiCellAnalyserClass->objectName().isEmpty())
            SamGuiCellAnalyserClass->setObjectName(QString::fromUtf8("SamGuiCellAnalyserClass"));
        SamGuiCellAnalyserClass->resize(400, 300);

        retranslateUi(SamGuiCellAnalyserClass);

        QMetaObject::connectSlotsByName(SamGuiCellAnalyserClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiCellAnalyserClass)
    {
        SamGuiCellAnalyserClass->setWindowTitle(QApplication::translate("SamGuiCellAnalyserClass", "SamGuiCellAnalyser", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiCellAnalyserClass: public Ui_SamGuiCellAnalyserClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICELLANALYSER_H
