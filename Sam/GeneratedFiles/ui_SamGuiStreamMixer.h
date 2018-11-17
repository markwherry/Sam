/********************************************************************************
** Form generated from reading UI file 'SamGuiStreamMixer.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISTREAMMIXER_H
#define UI_SAMGUISTREAMMIXER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiStreamMixerClass
{
public:

    void setupUi(QWidget *SamGuiStreamMixerClass)
    {
        if (SamGuiStreamMixerClass->objectName().isEmpty())
            SamGuiStreamMixerClass->setObjectName(QString::fromUtf8("SamGuiStreamMixerClass"));
        SamGuiStreamMixerClass->resize(400, 300);

        retranslateUi(SamGuiStreamMixerClass);

        QMetaObject::connectSlotsByName(SamGuiStreamMixerClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiStreamMixerClass)
    {
        SamGuiStreamMixerClass->setWindowTitle(QApplication::translate("SamGuiStreamMixerClass", "SamGuiStreamMixer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiStreamMixerClass: public Ui_SamGuiStreamMixerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISTREAMMIXER_H
