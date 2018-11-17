/********************************************************************************
** Form generated from reading UI file 'SamGuiOutputEditor.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIOUTPUTEDITOR_H
#define UI_SAMGUIOUTPUTEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiOutputEditor
{
public:

    void setupUi(QWidget *SamGuiOutputEditor)
    {
        if (SamGuiOutputEditor->objectName().isEmpty())
            SamGuiOutputEditor->setObjectName(QString::fromUtf8("SamGuiOutputEditor"));
        SamGuiOutputEditor->resize(400, 300);

        retranslateUi(SamGuiOutputEditor);

        QMetaObject::connectSlotsByName(SamGuiOutputEditor);
    } // setupUi

    void retranslateUi(QWidget *SamGuiOutputEditor)
    {
        SamGuiOutputEditor->setWindowTitle(QApplication::translate("SamGuiOutputEditor", "SamGuiOutputEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiOutputEditor: public Ui_SamGuiOutputEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIOUTPUTEDITOR_H
