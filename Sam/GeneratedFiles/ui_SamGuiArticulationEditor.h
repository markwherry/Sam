/********************************************************************************
** Form generated from reading UI file 'SamGuiArticulationEditor.ui'
**
** Created: Thu Feb 12 17:05:56 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIARTICULATIONEDITOR_H
#define UI_SAMGUIARTICULATIONEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiArticulationEditor
{
public:

    void setupUi(QWidget *SamGuiArticulationEditor)
    {
        if (SamGuiArticulationEditor->objectName().isEmpty())
            SamGuiArticulationEditor->setObjectName(QString::fromUtf8("SamGuiArticulationEditor"));
        SamGuiArticulationEditor->resize(400, 300);

        retranslateUi(SamGuiArticulationEditor);

        QMetaObject::connectSlotsByName(SamGuiArticulationEditor);
    } // setupUi

    void retranslateUi(QWidget *SamGuiArticulationEditor)
    {
        SamGuiArticulationEditor->setWindowTitle(QApplication::translate("SamGuiArticulationEditor", "SamGuiArticulationEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiArticulationEditor: public Ui_SamGuiArticulationEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIARTICULATIONEDITOR_H
