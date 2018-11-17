/********************************************************************************
** Form generated from reading UI file 'SamGuiPianoController.ui'
**
** Created: Thu Feb 12 17:05:54 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIPIANOCONTROLLER_H
#define UI_SAMGUIPIANOCONTROLLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiPianoControllerClass
{
public:

    void setupUi(QWidget *SamGuiPianoControllerClass)
    {
        if (SamGuiPianoControllerClass->objectName().isEmpty())
            SamGuiPianoControllerClass->setObjectName(QString::fromUtf8("SamGuiPianoControllerClass"));
        SamGuiPianoControllerClass->resize(400, 300);

        retranslateUi(SamGuiPianoControllerClass);

        QMetaObject::connectSlotsByName(SamGuiPianoControllerClass);
    } // setupUi

    void retranslateUi(QWidget *SamGuiPianoControllerClass)
    {
        SamGuiPianoControllerClass->setWindowTitle(QApplication::translate("SamGuiPianoControllerClass", "SamGuiPianoController", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiPianoControllerClass: public Ui_SamGuiPianoControllerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIPIANOCONTROLLER_H
