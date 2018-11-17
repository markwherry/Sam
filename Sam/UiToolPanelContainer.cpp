#include "UiToolPanelContainer.h"

#include <QPainter>
#include <QScrollBar>

UiToolPanelContainer::UiToolPanelContainer(QWidget *parent, UiToolPanelPlacement placement)
	: QWidget(parent)
	, m_placement(placement)
	, m_toolPanel(0)
{
	//ui.setupUi(this);
	
	QPalette p = palette();
    p.setColor(QPalette::Dark, QColor(20,20,20));

	scrollArea = new QScrollArea(this);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setPalette(p);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setFrameShape(QFrame::NoFrame);
	m_toolPanel = new UiToolPanel(scrollArea);
	scrollArea->setWidget(m_toolPanel);

	scrollBar = new UiScrollBar(this);

	connect(scrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(updateScrollBarRange(int, int)));
	connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateScrollBarValue(int)));

	connect(scrollBar, SIGNAL(valueChanged(int)), scrollArea->verticalScrollBar(), SLOT(setValue(int)));

	setFixedWidth(300+12+8+4);
}

UiToolPanelContainer::~UiToolPanelContainer()
{

}

void UiToolPanelContainer::updateScrollBarRange(int min, int max)
{
	//qDebug("ToolPanelContainer::updateScrollBarRange(%d, %d)", min, max);
	scrollBar->setMinimum(min);
	scrollBar->setMaximum(max);
	scrollBar->setPageStep(scrollArea->verticalScrollBar()->pageStep());
}

void UiToolPanelContainer::updateScrollBarValue(int value)
{
	//qDebug("ToolPanelContainer::updateScrollBarValue(%d)");
	scrollBar->setValue(value);
	
}

void UiToolPanelContainer::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(60,60,60));
	p.setBrush(QColor(0,0,0));
	p.drawRect(0,0,width()-1,height()-1);
}

void UiToolPanelContainer::resizeEvent(QResizeEvent *ev)
{
	int margin = 3;

	scrollBar->setFixedSize(12, height()-(margin*2));
	if(m_placement == Left) {
		scrollBar->move(margin,margin);
		scrollArea->move((margin*2)+scrollBar->width()-1, margin);
	} else {
		scrollBar->move(width()-margin-scrollBar->width(),margin);
		scrollArea->move(margin, margin);
	}
	

	
	scrollArea->setFixedSize(width()-(margin*3)-scrollBar->width()+1, height()-(margin*2));

	updateScrollBarRange(scrollArea->verticalScrollBar()->minimum(),scrollArea->verticalScrollBar()->maximum());
}