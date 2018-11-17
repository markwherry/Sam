/********************************************************************************
** Form generated from reading UI file 'SamGuiOutputList.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIOUTPUTLIST_H
#define UI_SAMGUIOUTPUTLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiOutputList
{
public:

    void setupUi(QWidget *SamGuiOutputList)
    {
        if (SamGuiOutputList->objectName().isEmpty())
            SamGuiOutputList->setObjectName(QString::fromUtf8("SamGuiOutputList"));
        SamGuiOutputList->resize(400, 300);

        retranslateUi(SamGuiOutputList);

        QMetaObject::connectSlotsByName(SamGuiOutputList);
    } // setupUi

    void retranslateUi(QWidget *SamGuiOutputList)
    {
        SamGuiOutputList->setWindowTitle(QApplication::translate("SamGuiOutputList", "SamGuiOutputList", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiOutputList: public Ui_SamGuiOutputList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIOUTPUTLIST_H
