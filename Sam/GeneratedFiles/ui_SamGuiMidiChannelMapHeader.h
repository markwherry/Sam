/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiChannelMapHeader.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDICHANNELMAPHEADER_H
#define UI_SAMGUIMIDICHANNELMAPHEADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiChannelMapHeader
{
public:

    void setupUi(QWidget *SamGuiMidiChannelMapHeader)
    {
        if (SamGuiMidiChannelMapHeader->objectName().isEmpty())
            SamGuiMidiChannelMapHeader->setObjectName(QString::fromUtf8("SamGuiMidiChannelMapHeader"));
        SamGuiMidiChannelMapHeader->resize(400, 300);

        retranslateUi(SamGuiMidiChannelMapHeader);

        QMetaObject::connectSlotsByName(SamGuiMidiChannelMapHeader);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiChannelMapHeader)
    {
        SamGuiMidiChannelMapHeader->setWindowTitle(QApplication::translate("SamGuiMidiChannelMapHeader", "SamGuiMidiChannelMapHeader", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiChannelMapHeader: public Ui_SamGuiMidiChannelMapHeader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDICHANNELMAPHEADER_H
