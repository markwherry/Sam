/********************************************************************************
** Form generated from reading UI file 'SeGuiParameters.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIPARAMETERS_H
#define UI_SEGUIPARAMETERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiParametersClass
{
public:

    void setupUi(QWidget *SeGuiParametersClass)
    {
        if (SeGuiParametersClass->objectName().isEmpty())
            SeGuiParametersClass->setObjectName(QString::fromUtf8("SeGuiParametersClass"));
        SeGuiParametersClass->resize(400, 300);

        retranslateUi(SeGuiParametersClass);

        QMetaObject::connectSlotsByName(SeGuiParametersClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiParametersClass)
    {
        SeGuiParametersClass->setWindowTitle(QApplication::translate("SeGuiParametersClass", "SeGuiParameters", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiParametersClass: public Ui_SeGuiParametersClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIPARAMETERS_H
