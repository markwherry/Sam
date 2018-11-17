/********************************************************************************
** Form generated from reading UI file 'SamGuiAudioBusProperties.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIAUDIOBUSPROPERTIES_H
#define UI_SAMGUIAUDIOBUSPROPERTIES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiAudioBusPropertiesClass
{
public:

    void setupUi(QWidget *SamGuiAudioBusPropertiesClass)
    {
        if (SamGuiAudioBusPropertiesClass->objectName().isEmpty())
            SamGuiAudioBusPropertiesClass->setObjectName(QString::fromUtf8("SamGuiAudioBusPropertiesClass"));
        SamGuiAudioBusPropertiesClass->resize(400, 300);

        retranslateUi(SamGuiAudioBusPropertiesClass);

        QMetaObject::connectSlotsByName(SamGuiAudioBusPropertiesClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiAudioBusPropertiesClass)
    {
        SamGuiAudioBusPropertiesClass->setWindowTitle(QApplication::translate("SamGuiAudioBusPropertiesClass", "SamGuiAudioBusProperties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiAudioBusPropertiesClass: public Ui_SamGuiAudioBusPropertiesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIAUDIOBUSPROPERTIES_H
