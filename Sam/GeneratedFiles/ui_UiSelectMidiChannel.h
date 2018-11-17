/********************************************************************************
** Form generated from reading UI file 'UiSelectMidiChannel.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UISELECTMIDICHANNEL_H
#define UI_UISELECTMIDICHANNEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiSelectMidiChannelClass
{
public:

    void setupUi(QWidget *UiSelectMidiChannelClass)
    {
        if (UiSelectMidiChannelClass->objectName().isEmpty())
            UiSelectMidiChannelClass->setObjectName(QString::fromUtf8("UiSelectMidiChannelClass"));
        UiSelectMidiChannelClass->resize(400, 300);

        retranslateUi(UiSelectMidiChannelClass);

        QMetaObject::connectSlotsByName(UiSelectMidiChannelClass);
    } // setupUi

    void retranslateUi(QWidget *UiSelectMidiChannelClass)
    {
        UiSelectMidiChannelClass->setWindowTitle(QApplication::translate("UiSelectMidiChannelClass", "UiSelectMidiChannel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiSelectMidiChannelClass: public Ui_UiSelectMidiChannelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UISELECTMIDICHANNEL_H
