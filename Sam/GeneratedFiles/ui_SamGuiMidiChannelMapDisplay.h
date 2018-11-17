/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiChannelMapDisplay.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDICHANNELMAPDISPLAY_H
#define UI_SAMGUIMIDICHANNELMAPDISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiChannelMapDisplay
{
public:

    void setupUi(QWidget *SamGuiMidiChannelMapDisplay)
    {
        if (SamGuiMidiChannelMapDisplay->objectName().isEmpty())
            SamGuiMidiChannelMapDisplay->setObjectName(QString::fromUtf8("SamGuiMidiChannelMapDisplay"));
        SamGuiMidiChannelMapDisplay->resize(400, 300);

        retranslateUi(SamGuiMidiChannelMapDisplay);

        QMetaObject::connectSlotsByName(SamGuiMidiChannelMapDisplay);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiChannelMapDisplay)
    {
        SamGuiMidiChannelMapDisplay->setWindowTitle(QApplication::translate("SamGuiMidiChannelMapDisplay", "SamGuiMidiChannelMapDisplay", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiChannelMapDisplay: public Ui_SamGuiMidiChannelMapDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDICHANNELMAPDISPLAY_H
