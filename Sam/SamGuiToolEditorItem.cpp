#include "SamGuiToolEditorItem.h"

#include <QPainter>

SamGuiToolEditorItem::SamGuiToolEditorItem(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	m_toolbar = new SamGuiToolEditorToolbar(this);
	setFixedHeight(300);
}

SamGuiToolEditorItem::~SamGuiToolEditorItem()
{

}

void SamGuiToolEditorItem::setEditor(SamGuiToolEditor *editor)
{
	m_editor = editor;
	
	m_editor->setParent(this);
	m_editor->move(0,m_toolbar->height()+1);
	setFixedWidth(m_editor->width());
	m_toolbar->setFixedWidth(width());
}

void SamGuiToolEditorItem::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(30,30,30));
	p.setBrush(QColor(50,50,50));
	p.drawRect(0,41,width()-1,height()-1-41);
}