/********************************************************************************
** Form generated from reading UI file 'SamGuiParameterControl.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIPARAMETERCONTROL_H
#define UI_SAMGUIPARAMETERCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiParameterControlClass
{
public:

    void setupUi(QWidget *SamGuiParameterControlClass)
    {
        if (SamGuiParameterControlClass->objectName().isEmpty())
            SamGuiParameterControlClass->setObjectName(QString::fromUtf8("SamGuiParameterControlClass"));
        SamGuiParameterControlClass->resize(400, 300);

        retranslateUi(SamGuiParameterControlClass);

        QMetaObject::connectSlotsByName(SamGuiParameterControlClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiParameterControlClass)
    {
        SamGuiParameterControlClass->setWindowTitle(QApplication::translate("SamGuiParameterControlClass", "SamGuiParameterControl", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiParameterControlClass: public Ui_SamGuiParameterControlClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIPARAMETERCONTROL_H
