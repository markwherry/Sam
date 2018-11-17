/********************************************************************************
** Form generated from reading UI file 'SeGuiSampleEditor.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUISAMPLEEDITOR_H
#define UI_SEGUISAMPLEEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiSampleEditor
{
public:

    void setupUi(QWidget *SeGuiSampleEditor)
    {
        if (SeGuiSampleEditor->objectName().isEmpty())
            SeGuiSampleEditor->setObjectName(QString::fromUtf8("SeGuiSampleEditor"));
        SeGuiSampleEditor->resize(400, 300);

        retranslateUi(SeGuiSampleEditor);

        QMetaObject::connectSlotsByName(SeGuiSampleEditor);
    } // setupUi

    void retranslateUi(QWidget *SeGuiSampleEditor)
    {
        SeGuiSampleEditor->setWindowTitle(QApplication::translate("SeGuiSampleEditor", "SeGuiSampleEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiSampleEditor: public Ui_SeGuiSampleEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUISAMPLEEDITOR_H
