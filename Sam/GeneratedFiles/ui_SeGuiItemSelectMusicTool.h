/********************************************************************************
** Form generated from reading UI file 'SeGuiItemSelectMusicTool.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIITEMSELECTMUSICTOOL_H
#define UI_SEGUIITEMSELECTMUSICTOOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiItemSelectMusicToolClass
{
public:

    void setupUi(QWidget *SeGuiItemSelectMusicToolClass)
    {
        if (SeGuiItemSelectMusicToolClass->objectName().isEmpty())
            SeGuiItemSelectMusicToolClass->setObjectName(QString::fromUtf8("SeGuiItemSelectMusicToolClass"));
        SeGuiItemSelectMusicToolClass->resize(400, 300);

        retranslateUi(SeGuiItemSelectMusicToolClass);

        QMetaObject::connectSlotsByName(SeGuiItemSelectMusicToolClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiItemSelectMusicToolClass)
    {
        SeGuiItemSelectMusicToolClass->setWindowTitle(QApplication::translate("SeGuiItemSelectMusicToolClass", "SeGuiItemSelectMusicTool", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiItemSelectMusicToolClass: public Ui_SeGuiItemSelectMusicToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIITEMSELECTMUSICTOOL_H
