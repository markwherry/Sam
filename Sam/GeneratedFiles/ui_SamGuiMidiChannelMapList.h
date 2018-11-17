/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiChannelMapList.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDICHANNELMAPLIST_H
#define UI_SAMGUIMIDICHANNELMAPLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiChannelMapList
{
public:

    void setupUi(QWidget *SamGuiMidiChannelMapList)
    {
        if (SamGuiMidiChannelMapList->objectName().isEmpty())
            SamGuiMidiChannelMapList->setObjectName(QString::fromUtf8("SamGuiMidiChannelMapList"));
        SamGuiMidiChannelMapList->resize(400, 300);

        retranslateUi(SamGuiMidiChannelMapList);

        QMetaObject::connectSlotsByName(SamGuiMidiChannelMapList);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiChannelMapList)
    {
        SamGuiMidiChannelMapList->setWindowTitle(QApplication::translate("SamGuiMidiChannelMapList", "SamGuiMidiChannelMapList", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiChannelMapList: public Ui_SamGuiMidiChannelMapList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDICHANNELMAPLIST_H
