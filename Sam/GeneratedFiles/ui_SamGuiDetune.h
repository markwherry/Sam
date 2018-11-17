/********************************************************************************
** Form generated from reading UI file 'SamGuiDetune.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIDETUNE_H
#define UI_SAMGUIDETUNE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiDetuneClass
{
public:

    void setupUi(QWidget *SamGuiDetuneClass)
    {
        if (SamGuiDetuneClass->objectName().isEmpty())
            SamGuiDetuneClass->setObjectName(QString::fromUtf8("SamGuiDetuneClass"));
        SamGuiDetuneClass->resize(400, 300);

        retranslateUi(SamGuiDetuneClass);

        QMetaObject::connectSlotsByName(SamGuiDetuneClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiDetuneClass)
    {
        SamGuiDetuneClass->setWindowTitle(QApplication::translate("SamGuiDetuneClass", "SamGuiDetune", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiDetuneClass: public Ui_SamGuiDetuneClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIDETUNE_H
