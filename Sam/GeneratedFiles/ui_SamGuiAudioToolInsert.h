/********************************************************************************
** Form generated from reading UI file 'SamGuiAudioToolInsert.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIAUDIOTOOLINSERT_H
#define UI_SAMGUIAUDIOTOOLINSERT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiAudioToolInsert
{
public:

    void setupUi(QWidget *SamGuiAudioToolInsert)
    {
        if (SamGuiAudioToolInsert->objectName().isEmpty())
            SamGuiAudioToolInsert->setObjectName(QString::fromUtf8("SamGuiAudioToolInsert"));
        SamGuiAudioToolInsert->resize(400, 300);

        retranslateUi(SamGuiAudioToolInsert);

        QMetaObject::connectSlotsByName(SamGuiAudioToolInsert);
    } // setupUi

    void retranslateUi(QWidget *SamGuiAudioToolInsert)
    {
        SamGuiAudioToolInsert->setWindowTitle(QApplication::translate("SamGuiAudioToolInsert", "SamGuiAudioToolInsert", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiAudioToolInsert: public Ui_SamGuiAudioToolInsert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIAUDIOTOOLINSERT_H
