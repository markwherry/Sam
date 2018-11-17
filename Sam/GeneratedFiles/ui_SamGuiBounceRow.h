/********************************************************************************
** Form generated from reading UI file 'SamGuiBounceRow.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIBOUNCEROW_H
#define UI_SAMGUIBOUNCEROW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiBounceRowClass
{
public:

    void setupUi(QWidget *SamGuiBounceRowClass)
    {
        if (SamGuiBounceRowClass->objectName().isEmpty())
            SamGuiBounceRowClass->setObjectName(QString::fromUtf8("SamGuiBounceRowClass"));
        SamGuiBounceRowClass->resize(400, 300);

        retranslateUi(SamGuiBounceRowClass);

        QMetaObject::connectSlotsByName(SamGuiBounceRowClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiBounceRowClass)
    {
        SamGuiBounceRowClass->setWindowTitle(QApplication::translate("SamGuiBounceRowClass", "SamGuiBounceRow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiBounceRowClass: public Ui_SamGuiBounceRowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIBOUNCEROW_H
