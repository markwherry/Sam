/********************************************************************************
** Form generated from reading UI file 'SamGuiChannelList.ui'
**
** Created: Thu Feb 12 17:05:53 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUICHANNELLIST_H
#define UI_SAMGUICHANNELLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiChannelListClass
{
public:

    void setupUi(QWidget *SamGuiChannelListClass)
    {
        if (SamGuiChannelListClass->objectName().isEmpty())
            SamGuiChannelListClass->setObjectName(QString::fromUtf8("SamGuiChannelListClass"));
        SamGuiChannelListClass->resize(400, 300);

        retranslateUi(SamGuiChannelListClass);

        QMetaObject::connectSlotsByName(SamGuiChannelListClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiChannelListClass)
    {
        SamGuiChannelListClass->setWindowTitle(QApplication::translate("SamGuiChannelListClass", "SamGuiChannelList", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiChannelListClass: public Ui_SamGuiChannelListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUICHANNELLIST_H
