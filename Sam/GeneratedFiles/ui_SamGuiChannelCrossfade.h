/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelCrossfade.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELCROSSFADE_H
#define UI_SAMGUICHANNELCROSSFADE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelCrossfade
{
public:

    void setupUi(QWidget *SamGuiChannelCrossfade)
    {
        if (SamGuiChannelCrossfade->objectName().isEmpty())
            SamGuiChannelCrossfade->setObjectName(QString::fromUtf8("SamGuiChannelCrossfade"));
        SamGuiChannelCrossfade->resize(400, 300);

        retranslateUi(SamGuiChannelCrossfade);

        QMetaObject::connectSlotsByName(SamGuiChannelCrossfade);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelCrossfade)
    {
        SamGuiChannelCrossfade->setWindowTitle(QApplication::translate("SamGuiChannelCrossfade", "SamGuiChannelCrossfade", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelCrossfade: public Ui_SamGuiChannelCrossfade {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELCROSSFADE_H
