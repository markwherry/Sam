/********************************************************************************
** Form generated from reading UI file 'SamGuiInsert.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIINSERT_H
#define UI_SAMGUIINSERT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiInsertClass
{
public:

    void setupUi(QWidget *SamGuiInsertClass)
    {
        if (SamGuiInsertClass->objectName().isEmpty())
            SamGuiInsertClass->setObjectName(QString::fromUtf8("SamGuiInsertClass"));
        SamGuiInsertClass->resize(400, 300);

        retranslateUi(SamGuiInsertClass);

        QMetaObject::connectSlotsByName(SamGuiInsertClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiInsertClass)
    {
        SamGuiInsertClass->setWindowTitle(QApplication::translate("SamGuiInsertClass", "SamGuiInsert", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiInsertClass: public Ui_SamGuiInsertClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIINSERT_H
