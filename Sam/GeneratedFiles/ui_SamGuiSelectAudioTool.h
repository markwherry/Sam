/********************************************************************************
** Form generated from reading UI file 'SamGuiSelectAudioTool.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISELECTAUDIOTOOL_H
#define UI_SAMGUISELECTAUDIOTOOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSelectAudioTool
{
public:

    void setupUi(QWidget *SamGuiSelectAudioTool)
    {
        if (SamGuiSelectAudioTool->objectName().isEmpty())
            SamGuiSelectAudioTool->setObjectName(QString::fromUtf8("SamGuiSelectAudioTool"));
        SamGuiSelectAudioTool->resize(400, 300);

        retranslateUi(SamGuiSelectAudioTool);

        QMetaObject::connectSlotsByName(SamGuiSelectAudioTool);
    } // setupUi

    void retranslateUi(QWidget *SamGuiSelectAudioTool)
    {
        SamGuiSelectAudioTool->setWindowTitle(QApplication::translate("SamGuiSelectAudioTool", "SamGuiSelectAudioTool", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSelectAudioTool: public Ui_SamGuiSelectAudioTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISELECTAUDIOTOOL_H
