/********************************************************************************
** Form generated from reading UI file 'SamGuiMixer.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIXER_H
#define UI_SAMGUIMIXER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMixerClass
{
public:

    void setupUi(QWidget *SamGuiMixerClass)
    {
        if (SamGuiMixerClass->objectName().isEmpty())
            SamGuiMixerClass->setObjectName(QString::fromUtf8("SamGuiMixerClass"));
        SamGuiMixerClass->resize(400, 300);

        retranslateUi(SamGuiMixerClass);

        QMetaObject::connectSlotsByName(SamGuiMixerClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMixerClass)
    {
        SamGuiMixerClass->setWindowTitle(QApplication::translate("SamGuiMixerClass", "SamGuiMixer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMixerClass: public Ui_SamGuiMixerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIXER_H
