#include "SamGuiToolContainer.h"

SamGuiToolContainer::SamGuiToolContainer(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	m_streamMixer = new SamGuiStreamMixer(0);

	SamGuiToolEditorItem *editorItem = new SamGuiToolEditorItem(this);
	editorItem->setEditor(m_streamMixer);
	m_editorItems.append(editorItem);

	setFixedSize(300,300);
}

SamGuiToolContainer::~SamGuiToolContainer()
{

}

void SamGuiToolContainer::setChannel(SamChannel *channel)
{
	m_streamMixer->setEditorChannel(channel);
}