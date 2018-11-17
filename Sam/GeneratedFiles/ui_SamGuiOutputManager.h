/********************************************************************************
** Form generated from reading UI file 'SamGuiOutputManager.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIOUTPUTMANAGER_H
#define UI_SAMGUIOUTPUTMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiOutputManager
{
public:

    void setupUi(QWidget *SamGuiOutputManager)
    {
        if (SamGuiOutputManager->objectName().isEmpty())
            SamGuiOutputManager->setObjectName(QString::fromUtf8("SamGuiOutputManager"));
        SamGuiOutputManager->resize(400, 300);

        retranslateUi(SamGuiOutputManager);

        QMetaObject::connectSlotsByName(SamGuiOutputManager);
    } // setupUi

    void retranslateUi(QWidget *SamGuiOutputManager)
    {
        SamGuiOutputManager->setWindowTitle(QApplication::translate("SamGuiOutputManager", "SamGuiOutputManager", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiOutputManager: public Ui_SamGuiOutputManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIOUTPUTMANAGER_H
