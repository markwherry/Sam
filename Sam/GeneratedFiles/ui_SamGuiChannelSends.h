/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelSends.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELSENDS_H
#define UI_SAMGUICHANNELSENDS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelSendsClass
{
public:

    void setupUi(QWidget *SamGuiChannelSendsClass)
    {
        if (SamGuiChannelSendsClass->objectName().isEmpty())
            SamGuiChannelSendsClass->setObjectName(QString::fromUtf8("SamGuiChannelSendsClass"));
        SamGuiChannelSendsClass->resize(400, 300);

        retranslateUi(SamGuiChannelSendsClass);

        QMetaObject::connectSlotsByName(SamGuiChannelSendsClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelSendsClass)
    {
        SamGuiChannelSendsClass->setWindowTitle(QApplication::translate("SamGuiChannelSendsClass", "SamGuiChannelSends", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelSendsClass: public Ui_SamGuiChannelSendsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELSENDS_H
