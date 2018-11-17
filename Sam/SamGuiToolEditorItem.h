#ifndef SAMGUITOOLEDITORITEM_H
#define SAMGUITOOLEDITORITEM_H

#include <QWidget>
//#include "ui_SamGuiToolEditorItem.h"

#include "SamGuiToolEditor.h"
#include "SamGuiToolEditorToolbar.h"

class SamGuiToolEditorItem : public QWidget
{
	Q_OBJECT

public:
	SamGuiToolEditorItem(QWidget *parent = 0);
	~SamGuiToolEditorItem();

	void setEditor(SamGuiToolEditor *editor);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiToolEditorItemClass ui;
	SamGuiToolEditorToolbar *m_toolbar;
	SamGuiToolEditor *m_editor;
};

#endif // SAMGUITOOLEDITORITEM_H
