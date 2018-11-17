/********************************************************************************
** Form generated from reading UI file 'SamGuiMixerStrip.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIXERSTRIP_H
#define UI_SAMGUIMIXERSTRIP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMixerStripClass
{
public:

    void setupUi(QWidget *SamGuiMixerStripClass)
    {
        if (SamGuiMixerStripClass->objectName().isEmpty())
            SamGuiMixerStripClass->setObjectName(QString::fromUtf8("SamGuiMixerStripClass"));
        SamGuiMixerStripClass->resize(400, 300);

        retranslateUi(SamGuiMixerStripClass);

        QMetaObject::connectSlotsByName(SamGuiMixerStripClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMixerStripClass)
    {
        SamGuiMixerStripClass->setWindowTitle(QApplication::translate("SamGuiMixerStripClass", "SamGuiMixerStrip", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMixerStripClass: public Ui_SamGuiMixerStripClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIXERSTRIP_H
