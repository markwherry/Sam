#include "UiListContainer.h"

UiListContainer::UiListContainer(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	QPalette p = palette();
    p.setColor(QPalette::Dark, QColor(100,100,100));

	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scrollArea->setPalette(p);
	m_scrollArea->setBackgroundRole(QPalette::Dark);
	m_scrollArea->setFrameShape(QFrame::NoFrame);

	m_list = new UiList(this);
	m_scrollArea->setWidget(m_list);
	//m_scrollArea->hide();
}

UiListContainer::~UiListContainer()
{

}

void UiListContainer::createSimpleItem(QString label)
{
	UiListItem *item = new UiListItem(this, 20);
	item->setLabel(label);
	m_list->addItem(item);
}

void UiListContainer::resizeEvent(QResizeEvent *ev)
{
	qDebug("*>*>*>*>******  UiListContainer::resizeEvent() %d, %d", width(), height());

	m_scrollArea->setFixedSize(width(), height());
	m_list->setFixedWidth(width()-18);
}