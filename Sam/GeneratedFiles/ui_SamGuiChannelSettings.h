/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelSettings.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELSETTINGS_H
#define UI_SAMGUICHANNELSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelSettingsClass
{
public:

    void setupUi(QWidget *SamGuiChannelSettingsClass)
    {
        if (SamGuiChannelSettingsClass->objectName().isEmpty())
            SamGuiChannelSettingsClass->setObjectName(QString::fromUtf8("SamGuiChannelSettingsClass"));
        SamGuiChannelSettingsClass->resize(400, 300);

        retranslateUi(SamGuiChannelSettingsClass);

        QMetaObject::connectSlotsByName(SamGuiChannelSettingsClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelSettingsClass)
    {
        SamGuiChannelSettingsClass->setWindowTitle(QApplication::translate("SamGuiChannelSettingsClass", "SamGuiChannelSettings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelSettingsClass: public Ui_SamGuiChannelSettingsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELSETTINGS_H
