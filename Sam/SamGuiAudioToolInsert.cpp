#include "SamGuiAudioToolInsert.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"
#include "Sam.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiAudioToolInsert::SamGuiAudioToolInsert(QWidget *parent, int insertIndex)
	: QWidget(parent)
	, m_insertIndex(insertIndex)
{
	//ui.setupUi(this);

	setFixedSize(296-4, 20);

	m_buttonAudioTool = new UiStateButton(this, 2000, 2);
	m_buttonAudioTool->move(2,2);
	m_buttonAudioTool->setFixedSize(16, 16);
	m_buttonAudioTool->setText("Per Voice FX");
	m_buttonAudioTool->setStateImage(0, ":/Resources/tool16.png");
	m_buttonAudioTool->setStateImage(1, ":/Resources/tool16.png");
	if(m_insertIndex < 0) {
		m_buttonAudioTool->setStateFillColor(0,QColor(100,75,150));
		m_buttonAudioTool->setStateFillColor(1,QColor(170,70,255));
		m_buttonAudioTool->setStateLineColor(1,QColor(100,75,150));
	} else {
		m_buttonAudioTool->setStateFillColor(0,QColor(0, 75, 75));
		m_buttonAudioTool->setStateFillColor(1,QColor(0, 255, 255));
		m_buttonAudioTool->setStateLineColor(1,QColor(0, 75, 75));
	}

	connect(m_buttonAudioTool, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setToolActive(int, int)));

	m_buttonAudioToolEdit = new UiStateButton(this, 2000, 2);
	m_buttonAudioToolEdit->move(254,2);
	m_buttonAudioToolEdit->setFixedSize(16, 16);
	if(m_insertIndex < 0) {
		m_buttonAudioToolEdit->setStateFillColor(0,QColor(100,75,150));
		m_buttonAudioToolEdit->setStateFillColor(1,QColor(170,70,255));
	} else {
		m_buttonAudioToolEdit->setStateFillColor(0,QColor(0, 75, 75));
		m_buttonAudioToolEdit->setStateFillColor(1,QColor(0, 255, 255));
	}
	m_buttonAudioToolEdit->setStateImage(0, ":/Resources/edit16.png");
	m_buttonAudioToolEdit->setStateImage(1, ":/Resources/edit16.png");

	connect(m_buttonAudioToolEdit, SIGNAL(clickedWithFlags(int, int)), this, SLOT(showToolEditor(int, int)));

	m_selectAudioTool = new SamGuiSelectAudioTool(this, m_insertIndex);
	//m_selectAudioTool->setDrawFill(false);
	//m_selectAudioTool->setDrawTint(false);
	//m_selectAudioTool->setDrawLine(false);
	m_selectAudioTool->move(110, 0); //22
	//m_selectAudioTool->setDoubleClick(true);

	//connect(selectMusicTool, SIGNAL(editorOpened(int, int)), this, SLOT(showMusicToolEditor(int, int)));

	connect(this, SIGNAL(toolActiveUpdated()), this, SLOT(updateToolActive()));

	samEngine->registerListener(this, SamEngine::ContextAudioTool);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiAudioToolInsert::~SamGuiAudioToolInsert()
{
	samEngine->unregisterListener(this, SamEngine::ContextAudioTool);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioToolInsert::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextAudioTool) {
		switch(ev->item()) {
			case SamAudioToolInsert::ItemActive:
				emit toolActiveUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioToolInsert::setChannel(SamChannel *channel, int index)
{
	m_channel = channel;
	m_index = index;
	m_selectAudioTool->setChannel(m_channel, 0, m_insertIndex);

	if(m_channel) {
		//if(m_channel->numVoices() > 0) {
		//	if(m_channel->voice(0)->audioToolInsert()) {
		//		m_buttonAudioTool->setState(m_channel->voice(0)->audioToolInsert()->isActive());
		//		return;
		//	}
		//}
	//	m_buttonAudioTool->setState(m_channel->musicToolInsert(m_index)->isActive());
		
	//	selectMusicTool->setInsertIndex(m_index);
	//	buttonMusicTool->setFixedSize(16, 16);
	} else {
		//m_selectAudioTool->set
	}

	m_buttonAudioTool->setState(false);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioToolInsert::setToolActive(int id, int flags)
{
	if(m_channel) {
		//if(m_insertIndex < 0) {
		//	samEngine->executeRealTimeCommand(new RtcSetPerVoiceFxToolActive(m_channel, 0, !m_channel->voice(0)->audioToolInsert()->isActive()));
		//} else {
		//}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioToolInsert::updateToolActive(void)
{
	if(m_channel) {
		//if(m_insertIndex < 0) {
		//	if(m_channel->numVoices() > 0 && m_channel->voice(0)->audioToolInsert()) {
		//		m_buttonAudioTool->setState(m_channel->voice(0)->audioToolInsert()->isActive());
		//	}
		//}
		////m_buttonAudioTool->setState(m_channel->musicToolInsert(m_index)->isActive());
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioToolInsert::showToolEditor(int id, int flags)
{
	if(m_channel) {
		id = m_index; //-= 2000;

		//if(m_channel->numVoices() > 0) {
		//	if(m_channel->voice(0)->audioToolInsert()) {
		//		if(m_channel->voice(0)->audioToolInsert()->isActive()) {
		//			samGui->toolEditor()->setToolInsert(m_channel->voice(0)->audioToolInsert(), 0);
		//			samGui->toolEditor()->show();
		//		}
		//	}
		//}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioToolInsert::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(m_index%2 == 0) {
		if(m_insertIndex > -1) {
			p.fillRect(0,0,width(),height(),QColor(0,0,0,20));
		}
	} else {
		p.fillRect(0,0,width(),height(),QColor(255,255,255,10));
	}

	if(m_channel) {
		//if(m_channel->musicToolInsert(m_index)->isActive()) {
		//	p.fillRect(0,0,width(),height(),QColor(0, 255, 255,30));
		//}
	}
}