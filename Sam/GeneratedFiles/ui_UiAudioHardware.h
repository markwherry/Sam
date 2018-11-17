/********************************************************************************
** Form generated from reading UI file 'UiAudioHardware.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIAUDIOHARDWARE_H
#define UI_UIAUDIOHARDWARE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiAudioHardwareClass
{
public:

    void setupUi(QWidget *UiAudioHardwareClass)
    {
        if (UiAudioHardwareClass->objectName().isEmpty())
            UiAudioHardwareClass->setObjectName(QString::fromUtf8("UiAudioHardwareClass"));
        UiAudioHardwareClass->resize(400, 300);

        retranslateUi(UiAudioHardwareClass);

        QMetaObject::connectSlotsByName(UiAudioHardwareClass);
    } // setupUi

    void retranslateUi(QWidget *UiAudioHardwareClass)
    {
        UiAudioHardwareClass->setWindowTitle(QApplication::translate("UiAudioHardwareClass", "UiAudioHardware", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiAudioHardwareClass: public Ui_UiAudioHardwareClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIAUDIOHARDWARE_H
