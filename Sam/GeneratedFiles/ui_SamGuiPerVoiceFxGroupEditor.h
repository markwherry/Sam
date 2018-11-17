/********************************************************************************
** Form generated from reading UI file 'SamGuiPerVoiceFxGroupEditor.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIPERVOICEFXGROUPEDITOR_H
#define UI_SAMGUIPERVOICEFXGROUPEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiPerVoiceFxGroupEditor
{
public:

    void setupUi(QWidget *SamGuiPerVoiceFxGroupEditor)
    {
        if (SamGuiPerVoiceFxGroupEditor->objectName().isEmpty())
            SamGuiPerVoiceFxGroupEditor->setObjectName(QString::fromUtf8("SamGuiPerVoiceFxGroupEditor"));
        SamGuiPerVoiceFxGroupEditor->resize(400, 300);

        retranslateUi(SamGuiPerVoiceFxGroupEditor);

        QMetaObject::connectSlotsByName(SamGuiPerVoiceFxGroupEditor);
    } // setupUi

    void retranslateUi(QWidget *SamGuiPerVoiceFxGroupEditor)
    {
        SamGuiPerVoiceFxGroupEditor->setWindowTitle(QApplication::translate("SamGuiPerVoiceFxGroupEditor", "SamGuiPerVoiceFxGroupEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiPerVoiceFxGroupEditor: public Ui_SamGuiPerVoiceFxGroupEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIPERVOICEFXGROUPEDITOR_H
