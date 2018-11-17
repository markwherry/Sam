#ifndef SAMGUITOOLCONTAINER_H
#define SAMGUITOOLCONTAINER_H

#include <QWidget>
//#include "ui_SamGuiToolContainer.h"

#include "SamGuiStreamMixer.h"
#include "SamGuiToolContainer.h"
#include "SamGuiToolEditorItem.h"

class SamGuiToolContainer : public QWidget
{
	Q_OBJECT

public:
	SamGuiToolContainer(QWidget *parent = 0);
	~SamGuiToolContainer();

	void setChannel(SamChannel *channel);

private:
	//Ui::SamGuiToolContainerClass ui;
	SamGuiStreamMixer *m_streamMixer;
	QList<SamGuiToolEditorItem*> m_editorItems;
};

#endif // SAMGUITOOLCONTAINER_H
