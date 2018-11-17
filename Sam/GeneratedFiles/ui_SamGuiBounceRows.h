/********************************************************************************
** Form generated from reading UI file 'SamGuiBounceRows.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIBOUNCEROWS_H
#define UI_SAMGUIBOUNCEROWS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiBounceRowsClass
{
public:

    void setupUi(QWidget *SamGuiBounceRowsClass)
    {
        if (SamGuiBounceRowsClass->objectName().isEmpty())
            SamGuiBounceRowsClass->setObjectName(QString::fromUtf8("SamGuiBounceRowsClass"));
        SamGuiBounceRowsClass->resize(400, 300);

        retranslateUi(SamGuiBounceRowsClass);

        QMetaObject::connectSlotsByName(SamGuiBounceRowsClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiBounceRowsClass)
    {
        SamGuiBounceRowsClass->setWindowTitle(QApplication::translate("SamGuiBounceRowsClass", "SamGuiBounceRows", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiBounceRowsClass: public Ui_SamGuiBounceRowsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIBOUNCEROWS_H
