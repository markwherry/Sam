/********************************************************************************
** Form generated from reading UI file 'SamGuiMidiChannelMap.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIDICHANNELMAP_H
#define UI_SAMGUIMIDICHANNELMAP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMidiChannelMap
{
public:

    void setupUi(QWidget *SamGuiMidiChannelMap)
    {
        if (SamGuiMidiChannelMap->objectName().isEmpty())
            SamGuiMidiChannelMap->setObjectName(QString::fromUtf8("SamGuiMidiChannelMap"));
        SamGuiMidiChannelMap->resize(400, 300);

        retranslateUi(SamGuiMidiChannelMap);

        QMetaObject::connectSlotsByName(SamGuiMidiChannelMap);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMidiChannelMap)
    {
        SamGuiMidiChannelMap->setWindowTitle(QApplication::translate("SamGuiMidiChannelMap", "SamGuiMidiChannelMap", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMidiChannelMap: public Ui_SamGuiMidiChannelMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIDICHANNELMAP_H
