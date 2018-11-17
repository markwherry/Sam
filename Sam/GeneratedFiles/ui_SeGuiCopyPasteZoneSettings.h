/********************************************************************************
** Form generated from reading UI file 'SeGuiCopyPasteZoneSettings.ui'
**
** Created: Thu Feb 12 17:05:55 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGUICOPYPASTEZONESETTINGS_H
#define UI_SEGUICOPYPASTEZONESETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeGuiCopyPasteZoneSettings
{
public:

    void setupUi(QWidget *SeGuiCopyPasteZoneSettings)
    {
        if (SeGuiCopyPasteZoneSettings->objectName().isEmpty())
            SeGuiCopyPasteZoneSettings->setObjectName(QString::fromUtf8("SeGuiCopyPasteZoneSettings"));
        SeGuiCopyPasteZoneSettings->resize(400, 300);

        retranslateUi(SeGuiCopyPasteZoneSettings);

        QMetaObject::connectSlotsByName(SeGuiCopyPasteZoneSettings);
    } // setupUi

    void retranslateUi(QWidget *SeGuiCopyPasteZoneSettings)
    {
        SeGuiCopyPasteZoneSettings->setWindowTitle(QApplication::translate("SeGuiCopyPasteZoneSettings", "SeGuiCopyPasteZoneSettings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SeGuiCopyPasteZoneSettings: public Ui_SeGuiCopyPasteZoneSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGUICOPYPASTEZONESETTINGS_H
