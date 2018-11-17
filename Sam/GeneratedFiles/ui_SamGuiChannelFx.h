/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelFx.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELFX_H
#define UI_SAMGUICHANNELFX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelFx
{
public:

    void setupUi(QWidget *SamGuiChannelFx)
    {
        if (SamGuiChannelFx->objectName().isEmpty())
            SamGuiChannelFx->setObjectName(QString::fromUtf8("SamGuiChannelFx"));
        SamGuiChannelFx->resize(400, 300);

        retranslateUi(SamGuiChannelFx);

        QMetaObject::connectSlotsByName(SamGuiChannelFx);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelFx)
    {
        SamGuiChannelFx->setWindowTitle(QApplication::translate("SamGuiChannelFx", "SamGuiChannelFx", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelFx: public Ui_SamGuiChannelFx {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELFX_H
