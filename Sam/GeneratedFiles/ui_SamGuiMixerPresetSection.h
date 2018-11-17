/********************************************************************************
** Form generated from reading UI file 'SamGuiMixerPresetSection.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIXERPRESETSECTION_H
#define UI_SAMGUIMIXERPRESETSECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMixerPresetSectionClass
{
public:

    void setupUi(QWidget *SamGuiMixerPresetSectionClass)
    {
        if (SamGuiMixerPresetSectionClass->objectName().isEmpty())
            SamGuiMixerPresetSectionClass->setObjectName(QString::fromUtf8("SamGuiMixerPresetSectionClass"));
        SamGuiMixerPresetSectionClass->resize(400, 300);

        retranslateUi(SamGuiMixerPresetSectionClass);

        QMetaObject::connectSlotsByName(SamGuiMixerPresetSectionClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMixerPresetSectionClass)
    {
        SamGuiMixerPresetSectionClass->setWindowTitle(QApplication::translate("SamGuiMixerPresetSectionClass", "SamGuiMixerPresetSection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMixerPresetSectionClass: public Ui_SamGuiMixerPresetSectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIXERPRESETSECTION_H
