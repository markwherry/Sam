#ifndef SAMGUIOUTPUTEDITORITEM_H
#define SAMGUIOUTPUTEDITORITEM_H

#include <QWidget>
//#include "ui_SamGuiOutputEditorItem.h"

class SamGuiOutputEditorItem : public QWidget
{
	Q_OBJECT

public:
	SamGuiOutputEditorItem(QWidget *parent = 0);
	~SamGuiOutputEditorItem();

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiOutputEditorItem ui;
};

#endif // SAMGUIOUTPUTEDITORITEM_H
