/********************************************************************************
** Form generated from reading UI file 'SeGuiZoneEditor.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUIZONEEDITOR_H
#define UI_SEGUIZONEEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiZoneEditorClass
{
public:

    void setupUi(QWidget *SeGuiZoneEditorClass)
    {
        if (SeGuiZoneEditorClass->objectName().isEmpty())
            SeGuiZoneEditorClass->setObjectName(QString::fromUtf8("SeGuiZoneEditorClass"));
        SeGuiZoneEditorClass->resize(400, 300);

        retranslateUi(SeGuiZoneEditorClass);

        QMetaObject::connectSlotsByName(SeGuiZoneEditorClass);
    } // setupUi

    void retranslateUi(QWidget *SeGuiZoneEditorClass)
    {
        SeGuiZoneEditorClass->setWindowTitle(QApplication::translate("SeGuiZoneEditorClass", "SeGuiZoneEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiZoneEditorClass: public Ui_SeGuiZoneEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUIZONEEDITOR_H
