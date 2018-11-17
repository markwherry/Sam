/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelTools.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELTOOLS_H
#define UI_SAMGUICHANNELTOOLS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelToolsClass
{
public:

    void setupUi(QWidget *SamGuiChannelToolsClass)
    {
        if (SamGuiChannelToolsClass->objectName().isEmpty())
            SamGuiChannelToolsClass->setObjectName(QString::fromUtf8("SamGuiChannelToolsClass"));
        SamGuiChannelToolsClass->resize(400, 300);

        retranslateUi(SamGuiChannelToolsClass);

        QMetaObject::connectSlotsByName(SamGuiChannelToolsClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelToolsClass)
    {
        SamGuiChannelToolsClass->setWindowTitle(QApplication::translate("SamGuiChannelToolsClass", "SamGuiChannelTools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelToolsClass: public Ui_SamGuiChannelToolsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELTOOLS_H
