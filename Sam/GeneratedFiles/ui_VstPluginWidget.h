/********************************************************************************
** Form generated from reading UI file 'VstPluginWidget.ui'
**
** Created: Thu Feb 12 17:05:56 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSTPLUGINWIDGET_H
#define UI_VSTPLUGINWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VstPluginWidget
{
public:

    void setupUi(QWidget *VstPluginWidget)
    {
        if (VstPluginWidget->objectName().isEmpty())
            VstPluginWidget->setObjectName(QString::fromUtf8("VstPluginWidget"));
        VstPluginWidget->resize(400, 300);

        retranslateUi(VstPluginWidget);

        QMetaObject::connectSlotsByName(VstPluginWidget);
    } // setupUi

    void retranslateUi(QWidget *VstPluginWidget)
    {
        VstPluginWidget->setWindowTitle(QApplication::translate("VstPluginWidget", "VstPluginWidget", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VstPluginWidget: public Ui_VstPluginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSTPLUGINWIDGET_H
