/********************************************************************************
** Form generated from reading UI file 'SamGuiOutputHeader.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIOUTPUTHEADER_H
#define UI_SAMGUIOUTPUTHEADER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiOutputHeader
{
public:

    void setupUi(QWidget *SamGuiOutputHeader)
    {
        if (SamGuiOutputHeader->objectName().isEmpty())
            SamGuiOutputHeader->setObjectName(QString::fromUtf8("SamGuiOutputHeader"));
        SamGuiOutputHeader->resize(400, 300);

        retranslateUi(SamGuiOutputHeader);

        QMetaObject::connectSlotsByName(SamGuiOutputHeader);
    } // setupUi

    void retranslateUi(QWidget *SamGuiOutputHeader)
    {
        SamGuiOutputHeader->setWindowTitle(QApplication::translate("SamGuiOutputHeader", "SamGuiOutputHeader", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiOutputHeader: public Ui_SamGuiOutputHeader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIOUTPUTHEADER_H
