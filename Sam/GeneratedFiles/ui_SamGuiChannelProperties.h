/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelProperties.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELPROPERTIES_H
#define UI_SAMGUICHANNELPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelPropertiesClass
{
public:

    void setupUi(QWidget *SamGuiChannelPropertiesClass)
    {
        if (SamGuiChannelPropertiesClass->objectName().isEmpty())
            SamGuiChannelPropertiesClass->setObjectName(QString::fromUtf8("SamGuiChannelPropertiesClass"));
        SamGuiChannelPropertiesClass->resize(400, 300);

        retranslateUi(SamGuiChannelPropertiesClass);

        QMetaObject::connectSlotsByName(SamGuiChannelPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelPropertiesClass)
    {
        SamGuiChannelPropertiesClass->setWindowTitle(QApplication::translate("SamGuiChannelPropertiesClass", "SamGuiChannelProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelPropertiesClass: public Ui_SamGuiChannelPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELPROPERTIES_H
