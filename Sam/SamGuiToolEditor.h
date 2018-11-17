#ifndef SAMGUITOOLEDITOR_H
#define SAMGUITOOLEDITOR_H

#include <QWidget>
#include "ui_SamGuiToolEditor.h"

#include "SamChannel.h"

class SamGuiToolEditor : public QWidget
{
	Q_OBJECT

public:
	SamGuiToolEditor(QWidget *parent = 0);
	~SamGuiToolEditor();

	virtual void setEditorChannel(SamChannel *channel);
	SamChannel* editorChannel(void) { return m_channel; }

private:
	//Ui::SamGuiToolEditorClass ui;
	SamChannel::Ptr m_channel;
};

#endif // SAMGUITOOLEDITOR_H
