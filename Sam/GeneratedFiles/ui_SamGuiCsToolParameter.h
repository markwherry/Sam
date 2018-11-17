/********************************************************************************
** Form generated from reading UI file 'SamGuiCsToolParameter.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICSTOOLPARAMETER_H
#define UI_SAMGUICSTOOLPARAMETER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiCsToolParameterClass
{
public:

    void setupUi(QWidget *SamGuiCsToolParameterClass)
    {
        if (SamGuiCsToolParameterClass->objectName().isEmpty())
            SamGuiCsToolParameterClass->setObjectName(QString::fromUtf8("SamGuiCsToolParameterClass"));
        SamGuiCsToolParameterClass->resize(400, 300);

        retranslateUi(SamGuiCsToolParameterClass);

        QMetaObject::connectSlotsByName(SamGuiCsToolParameterClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiCsToolParameterClass)
    {
        SamGuiCsToolParameterClass->setWindowTitle(QApplication::translate("SamGuiCsToolParameterClass", "SamGuiCsToolParameter", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiCsToolParameterClass: public Ui_SamGuiCsToolParameterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICSTOOLPARAMETER_H
