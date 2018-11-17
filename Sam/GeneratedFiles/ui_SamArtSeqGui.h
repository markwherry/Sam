/********************************************************************************
** Form generated from reading UI file 'SamArtSeqGui.ui'
**
** Created: Thu Feb 12 17:05:56 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMARTSEQGUI_H
#define UI_SAMARTSEQGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SamArtSeqGui
{
public:

    void setupUi(QWidget *SamArtSeqGui)
    {
        if (SamArtSeqGui->objectName().isEmpty())
            SamArtSeqGui->setObjectName(QString::fromUtf8("SamArtSeqGui"));
        SamArtSeqGui->resize(400, 300);

        retranslateUi(SamArtSeqGui);

        QMetaObject::connectSlotsByName(SamArtSeqGui);
    } // setupUi

    void retranslateUi(QWidget *SamArtSeqGui)
    {
        SamArtSeqGui->setWindowTitle(QApplication::translate("SamArtSeqGui", "SamArtSeqGui", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SamArtSeqGui: public Ui_SamArtSeqGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMARTSEQGUI_H
