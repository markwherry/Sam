/********************************************************************************
** Form generated from reading UI file 'SamGuiMixerInsert.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIMIXERINSERT_H
#define UI_SAMGUIMIXERINSERT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiMixerInsert
{
public:

    void setupUi(QWidget *SamGuiMixerInsert)
    {
        if (SamGuiMixerInsert->objectName().isEmpty())
            SamGuiMixerInsert->setObjectName(QString::fromUtf8("SamGuiMixerInsert"));
        SamGuiMixerInsert->resize(400, 300);

        retranslateUi(SamGuiMixerInsert);

        QMetaObject::connectSlotsByName(SamGuiMixerInsert);
    } // setupUi

    void retranslateUi(QWidget *SamGuiMixerInsert)
    {
        SamGuiMixerInsert->setWindowTitle(QApplication::translate("SamGuiMixerInsert", "SamGuiMixerInsert", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiMixerInsert: public Ui_SamGuiMixerInsert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIMIXERINSERT_H
