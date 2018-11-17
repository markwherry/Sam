/********************************************************************************
** Form generated from reading UI file 'SeGuiParameterProperties.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIPARAMETERPROPERTIES_H
#define UI_SEGUIPARAMETERPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiParameterPropertiesClass
{
public:

    void setupUi(QWidget *SeGuiParameterPropertiesClass)
    {
        if (SeGuiParameterPropertiesClass->objectName().isEmpty())
            SeGuiParameterPropertiesClass->setObjectName(QString::fromUtf8("SeGuiParameterPropertiesClass"));
        SeGuiParameterPropertiesClass->resize(400, 300);

        retranslateUi(SeGuiParameterPropertiesClass);

        QMetaObject::connectSlotsByName(SeGuiParameterPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiParameterPropertiesClass)
    {
        SeGuiParameterPropertiesClass->setWindowTitle(QApplication::translate("SeGuiParameterPropertiesClass", "SeGuiParameterProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiParameterPropertiesClass: public Ui_SeGuiParameterPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIPARAMETERPROPERTIES_H
