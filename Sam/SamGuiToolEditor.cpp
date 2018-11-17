#include "SamGuiToolEditor.h"

SamGuiToolEditor::SamGuiToolEditor(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);
}

SamGuiToolEditor::~SamGuiToolEditor()
{

}

void SamGuiToolEditor::setEditorChannel(SamChannel *channel)
{
	m_channel = channel;
}