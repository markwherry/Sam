#include "SamGuiMusicToolInsert.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"
#include "Sam.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMusicToolInsert::SamGuiMusicToolInsert(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
	, m_index(0)
{
	//ui.setupUi(this);
	setFixedSize(296-4, 20);

	int y = 0;

	buttonMusicTool = new UiStateButton(this, 2000, 2);
	buttonMusicTool->move(2,y+2);
	buttonMusicTool->setFixedSize(16, 16);
	buttonMusicTool->setText("Tool");
	buttonMusicTool->setStateImage(0, ":/Resources/tool16.png");
	buttonMusicTool->setStateImage(1, ":/Resources/tool16.png");
	buttonMusicTool->setStateFillColor(0,QColor(0, 75, 75));
	buttonMusicTool->setStateFillColor(1,QColor(0, 255, 255));
	buttonMusicTool->setStateLineColor(1,QColor(0, 75, 75));

	connect(buttonMusicTool, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setMusicToolActive(int, int)));

	buttonMusicToolEdit = new UiStateButton(this, 2000, 2);
	buttonMusicToolEdit->move(254,y+2);
	buttonMusicToolEdit->setFixedSize(16, 16);
	buttonMusicToolEdit->setStateFillColor(0,QColor(0, 75, 75));
	buttonMusicToolEdit->setStateFillColor(1,QColor(0, 255, 255));
	buttonMusicToolEdit->setStateImage(0, ":/Resources/edit16.png");
	buttonMusicToolEdit->setStateImage(1, ":/Resources/edit16.png");

	connect(buttonMusicToolEdit, SIGNAL(clickedWithFlags(int, int)), this, SLOT(showMusicToolEditor(int, int)));

	selectMusicTool = new SamGuiSelectMusicTool(this);
	selectMusicTool->setDrawFill(false);
	selectMusicTool->setDrawTint(false);
	selectMusicTool->setDrawLine(false);
	selectMusicTool->move(110, y); //22
	selectMusicTool->setDoubleClick(true);

	connect(selectMusicTool, SIGNAL(editorOpened(int, int)), this, SLOT(showMusicToolEditor(int, int)));

	connect(this, SIGNAL(musicToolActiveUpdated()), this, SLOT(updateMusicToolActive()));

	samEngine->registerListener(this, SamEngine::ContextMusicTool);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMusicToolInsert::~SamGuiMusicToolInsert()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMusicToolInsert::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextMusicTool) {
		switch(ev->item()) {
			case SamMusicToolInsert::ItemActive:
				emit musicToolActiveUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMusicToolInsert::setChannel(SamChannel *channel, int index)
{
	m_channel = channel;
	m_index = index;
	selectMusicTool->setChannel(m_channel);

	if(m_channel) {
		buttonMusicTool->setState(m_channel->musicToolInsert(m_index)->isActive());
		
		selectMusicTool->setInsertIndex(m_index);
		buttonMusicTool->setFixedSize(16, 16);
		buttonMusicTool->setText("Tool "+QString::number(m_index+1));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMusicToolInsert::setMusicToolActive(int id, int flags)
{
	if(m_channel) {
		id = m_index; //-= 2000;
		if(id >= 0 && id < SamChannel::maxMusicTools) {
			samEngine->executeRealTimeCommand(new RtcSetMusicToolActive(m_channel, id, false, !m_channel->musicToolInsert(id)->isActive()));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMusicToolInsert::updateMusicToolActive(void)
{
	if(m_channel) {
		buttonMusicTool->setState(m_channel->musicToolInsert(m_index)->isActive());
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMusicToolInsert::showMusicToolEditor(int id, int flags)
{
	if(m_channel) {
		id = m_index; //-= 2000;
		if(id >= 0 && id < SamChannel::maxMusicTools) {
			if(m_channel->musicToolInsert(id)->isAssigned()) {
				samGui->toolEditor()->setToolInsert(m_channel->musicToolInsert(id), 0);
				samGui->toolEditor()->show();
				
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMusicToolInsert::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(m_index%2 == 0) {
		p.fillRect(0,0,width(),height(),QColor(0,0,0,20));
	} else {
		p.fillRect(0,0,width(),height(),QColor(255,255,255,10));
	}

	if(m_channel) {
		if(m_channel->musicToolInsert(m_index)->isActive()) {
			p.fillRect(0,0,width(),height(),QColor(0, 255, 255,30));
		}
	}
}