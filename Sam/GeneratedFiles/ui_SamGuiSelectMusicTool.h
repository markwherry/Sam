/********************************************************************************
** Form generated from reading UI file 'SamGuiSelectMusicTool.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUISELECTMUSICTOOL_H
#define UI_SAMGUISELECTMUSICTOOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <uiselectmenu.h>

QT_BEGIN_NAMESPACE

class Ui_SamGuiSelectMusicToolClass
{
public:

    void setupUi(UiSelectMenu *SamGuiSelectMusicToolClass)
    {
        if (SamGuiSelectMusicToolClass->objectName().isEmpty())
            SamGuiSelectMusicToolClass->setObjectName(QString::fromUtf8("SamGuiSelectMusicToolClass"));
        SamGuiSelectMusicToolClass->resize(400, 300);

        retranslateUi(SamGuiSelectMusicToolClass);

        QMetaObject::connectSlotsByName(SamGuiSelectMusicToolClass);
    } // setupUi

    void retranslateUi(UiSelectMenu *SamGuiSelectMusicToolClass)
    {
        SamGuiSelectMusicToolClass->setWindowTitle(QApplication::translate("SamGuiSelectMusicToolClass", "SamGuiSelectMusicTool", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiSelectMusicToolClass: public Ui_SamGuiSelectMusicToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUISELECTMUSICTOOL_H
