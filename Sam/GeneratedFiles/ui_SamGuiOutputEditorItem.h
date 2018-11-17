/********************************************************************************
** Form generated from reading UI file 'SamGuiOutputEditorItem.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIOUTPUTEDITORITEM_H
#define UI_SAMGUIOUTPUTEDITORITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiOutputEditorItem
{
public:

    void setupUi(QWidget *SamGuiOutputEditorItem)
    {
        if (SamGuiOutputEditorItem->objectName().isEmpty())
            SamGuiOutputEditorItem->setObjectName(QString::fromUtf8("SamGuiOutputEditorItem"));
        SamGuiOutputEditorItem->resize(400, 300);

        retranslateUi(SamGuiOutputEditorItem);

        QMetaObject::connectSlotsByName(SamGuiOutputEditorItem);
    } // setupUi

    void retranslateUi(QWidget *SamGuiOutputEditorItem)
    {
        SamGuiOutputEditorItem->setWindowTitle(QApplication::translate("SamGuiOutputEditorItem", "SamGuiOutputEditorItem", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiOutputEditorItem: public Ui_SamGuiOutputEditorItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIOUTPUTEDITORITEM_H
