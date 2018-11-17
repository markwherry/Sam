/********************************************************************************
** Form generated from reading UI file 'SamGuiVstPluginManager.ui'
**
** Created: Thu Feb 12 17:05:56 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMGUIVSTPLUGINMANAGER_H
#define UI_SAMGUIVSTPLUGINMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamGuiVstPluginManager
{
public:

    void setupUi(QWidget *SamGuiVstPluginManager)
    {
        if (SamGuiVstPluginManager->objectName().isEmpty())
            SamGuiVstPluginManager->setObjectName(QString::fromUtf8("SamGuiVstPluginManager"));
        SamGuiVstPluginManager->resize(400, 300);

        retranslateUi(SamGuiVstPluginManager);

        QMetaObject::connectSlotsByName(SamGuiVstPluginManager);
    } // setupUi

    void retranslateUi(QWidget *SamGuiVstPluginManager)
    {
        SamGuiVstPluginManager->setWindowTitle(QApplication::translate("SamGuiVstPluginManager", "SamGuiVstPluginManager", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamGuiVstPluginManager: public Ui_SamGuiVstPluginManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMGUIVSTPLUGINMANAGER_H
