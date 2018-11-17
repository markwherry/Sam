#include "SamGuiMidiChannelMap.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMap::SamGuiMidiChannelMap(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setWindowTitle("MIDI Channel Usage Map");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	setFixedSize(589,400);

	QPalette p=palette();
    p.setColor(QPalette::Dark, QColor(45,45,45));
	p.setColor(QPalette::Foreground, QColor(150,150,150));
	
   
	m_areaHeader = new QScrollArea(this);
	m_areaHeader->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_areaHeader->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_areaHeader->setPalette(p);
	m_areaHeader->setBackgroundRole(QPalette::Dark);
	//m_areaHeader->setStyleSheet("color:red");
	m_areaHeader->setForegroundRole(QPalette::Foreground);
	m_areaHeader->setFrameStyle(QFrame::Box | QFrame::Plain);

	m_header = new SamGuiMidiChannelMapHeader(m_areaHeader->viewport());
	m_areaHeader->setWidget(m_header);

	m_areaList = new QScrollArea(this);
	m_areaList->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaList->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaList->setPalette(p);
	m_areaList->setBackgroundRole(QPalette::Dark);
	m_areaList->setForegroundRole(QPalette::Foreground);
	m_areaList->setFrameStyle(QFrame::Box | QFrame::Plain);

	m_list = new SamGuiMidiChannelMapList(m_areaList->viewport());
	m_list->move(0,0);
	m_areaList->setWidget(m_list);

	m_areaDisplay = new QScrollArea(this);
	m_areaDisplay->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaDisplay->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaDisplay->setPalette(p);
	m_areaDisplay->setBackgroundRole(QPalette::Dark);
	m_areaDisplay->setForegroundRole(QPalette::Foreground);
	//m_areaDisplay->setFrameShape (QFrame::NoFrame);
	m_areaDisplay->setFrameStyle (QFrame::Box | QFrame::Plain);

	m_display = new SamGuiMidiChannelMapDisplay(m_areaDisplay->viewport());
	m_display->move(0,0);
	m_areaDisplay->setWidget(m_display);

	//m_scrollH = new QScrollBar(Qt::Horizontal, this);
	m_scrollV = new QScrollBar(Qt::Vertical, this);
	//connect(m_areaDisplay->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(updateVScrollBarRange(int, int)));
	
	connect(m_areaList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doVerticalScroll(int)));
	connect(m_areaDisplay->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doVerticalScroll(int)));
	connect(m_scrollV, SIGNAL(valueChanged(int)), this, SLOT(doVerticalScroll(int)));

	samEngine->registerListener(this, SamEngine::ContextProject);
	samEngine->registerListener(this, SamEngine::ContextChannel);

	connect(this, SIGNAL(mapUpdated()), this, SLOT(updateMap()));

	connect(m_display, SIGNAL(highlighted(int, int)), this, SLOT(updateHighlight(int, int)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMap::~SamGuiMidiChannelMap()
{
	samEngine->unregisterListener(this, SamEngine::ContextProject);
	samEngine->unregisterListener(this, SamEngine::ContextChannel);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::onState(AStateEvent *ev)
{
	//qDebug("SamGuiSelectAudioBus::onState()");

	if(ev->context() == SamEngine::ContextProject) {
		//qDebug("SamEngine::ContextProject");
		switch(ev->item()) {
			case SamProject::ItemChannelAdded:
			case SamProject::ItemChannelRemoved:
				emit mapUpdated();
				break;
		}
	} else if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemMidiInputChannel:
			case SamChannel::ItemMidiInputPort:
				emit mapUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::updateMap(void)
{
	m_display->updateMap();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::updateHighlight(int port, int channel)
{
	m_header->setHighlight(channel);
	m_list->setHighlight(port);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::updateVScrollBarRange(void)
{
	//qDebug("UiToolPanelContainer::updateScrollBarRange(%d, %d)", min, max);
	m_scrollV->setMinimum(m_areaDisplay->verticalScrollBar()->minimum());
	m_scrollV->setMaximum(m_areaDisplay->verticalScrollBar()->maximum());
	m_scrollV->setSingleStep(m_areaDisplay->verticalScrollBar()->singleStep());
	m_scrollV->setPageStep(m_areaDisplay->verticalScrollBar()->pageStep());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::updateVScrollBarValue(int value)
{
	//qDebug("UiToolPanelContainer::updateScrollBarValue(%d)");
	m_scrollV->setValue(value);
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::doVerticalScroll(int value)
{
	m_areaList->verticalScrollBar()->setValue(value);
	m_areaDisplay->verticalScrollBar()->setValue(value);
	m_scrollV->setValue(value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	updateVScrollBarRange();

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));

	//EStyle::paintBackground(&p,this);
	//p.setPen(QColor(70,70,70));
	//p.setBrush(QColor(70,70,70));
	//p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}
	//
	p.setPen(QColor(0,0,0,130));
	p.setBrush(QColor(0,0,0,130));
	p.drawRect(0,0,width()-1,height()-1);

	 
	p.fillRect(width() - 0 - 1, 1, 0, height()-2, QColor(60, 60, 60));
	p.setPen(QColor(75,75,75));
	p.drawLine(width() - 0 - 1, 1, width() - 0 - 1, height()-2);
	p.setPen(QColor(0,0,0));
	p.drawLine(width() - 0 - 1 -1, 1, width() - 0 - 1 -1, height()-2);


	int x = m_areaDisplay->x();
	int y = height()-26;


	p.fillRect(x,y,16,16, QColor(0,255,0,200));

	p.setFont(QFont("Calibri",8));//,QFont::Bold));
	p.setPen(QColor(255,255,255));
	p.drawText(x+=24,y,200,16,Qt::AlignLeft|Qt::AlignVCenter,"Assigned to a Channel");

	p.fillRect(x+=160,y,16,16, QColor(255,255,0,200));

	p.drawText(x+=24,y,200,16,Qt::AlignLeft|Qt::AlignVCenter,"Assigned to more than one Channel");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::resizeEvent(QResizeEvent *ev)
{
	int margin = 10;

	m_areaHeader->move(margin+150,margin);
	m_areaHeader->setFixedSize(16 * 25, 35);

	m_areaList->move(margin,margin+m_areaHeader->height());
	m_areaList->setFixedSize(150, height()-32-m_areaHeader->height()-20);
	
	//m_scrollH->move(200+5, 5+m_areaHeader->height()+m_areaList->height());
	//m_scrollH->setFixedWidth(m_areaHeader->width());

	m_scrollV->move(margin+m_areaList->width()+m_areaHeader->width(), margin+m_areaHeader->height());
	m_scrollV->setFixedHeight(m_areaList->height());

	m_areaDisplay->move(margin+m_areaList->width(), margin+m_areaHeader->height());
	m_areaDisplay->setFixedSize(m_areaHeader->width(), m_areaList->height());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMap::showEvent(QShowEvent *ev)
{
	qDebug("SamGuiMidiChannelMap::showEvent()");
}

