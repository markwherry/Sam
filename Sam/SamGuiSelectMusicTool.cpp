#include "SamGuiSelectMusicTool.h"

#include "SamEngine.h"
#include "SamRealTimeCommands.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSelectMusicTool::SamGuiSelectMusicTool(QWidget *parent, bool instrument)
	: UiSelectMenu(parent, 140)
	, m_instrument(instrument)
	, m_channel(0)
	, m_insertIndex(-1)
{
	//ui.setupUi(this);

	refresh();

	connect(this, SIGNAL(itemSelected(QAction*)), this, SLOT(selectTool(QAction*)));
	connect(this, SIGNAL(doubleClicked()), this, SLOT(doubleClickMenu()));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSelectMusicTool::~SamGuiSelectMusicTool()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectMusicTool::setChannel(SamChannel *channel)
{
	m_channel = channel;

	updateTool();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectMusicTool::setInsertIndex(int insertIndex)
{
	m_insertIndex = insertIndex;

	updateTool();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectMusicTool::refresh(void)
{
	//qDebug("SamGuiSelectMusicTool::refresh()");

	clear();
	m_toolActions.clear();

	m_actionNoTool = addItem("None");

	addSeparator();

	for(int i=0; i<samEngine->musicToolManager()->numTools(); i++) {
		QAction *action = addItem(samEngine->musicToolManager()->toolInfo(i).name.toChar());
		m_toolActions.append(action);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectMusicTool::selectTool(QAction *action)
{
	qDebug("SamGuiSelectMusicTool::selectTool()");

	if(action == m_actionNoTool) {
		qDebug("action == m_actionNoTool");
		samEngine->executeRealTimeCommand(new RtcSetMusicTool(m_channel, m_insertIndex, m_instrument, 0));
	}

	for(int i=0; i<m_toolActions.size(); i++) {
		if(action == m_toolActions[i]) {
			qDebug("Found tool");
			SamMusicTool *tool = (SamMusicTool*)samEngine->musicToolManager()->createInstance(samEngine->musicToolManager()->toolInfo(i).uuid, m_channel->musicToolInsert(m_insertIndex));
			if(tool) {
				qDebug("Adding tool");
				samEngine->executeRealTimeCommand(new RtcSetMusicTool(m_channel, m_insertIndex, m_instrument, tool));
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectMusicTool::updateTool(void)
{
	//qDebug("SamGuiSelectMusicTool::updateTool()");

	if(!m_channel || !(m_insertIndex >= 0 && m_insertIndex < SamChannel::maxMusicTools)) {
		return;
	}

	if(m_channel->musicToolInsert(m_insertIndex)->musicTool() == 0) { //qDebug("2");
		setSelectedAction(m_actionNoTool); //qDebug("3");
	} else { //qDebug("4");
		for(int i=0; i<m_toolActions.size(); i++) { //qDebug("5");
			if(m_channel->musicToolInsert(m_insertIndex)->musicTool()->uuid() == samEngine->musicToolManager()->toolInfo(i).uuid) { //qDebug("6");
				setSelectedAction(m_toolActions[i]); //qDebug("7");
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectMusicTool::doubleClickMenu(void)
{
	//qDebug("SamGuiSelectMusicTool::doubleClickMenu() %d", m_insertIndex);
	
	if(m_insertIndex > -1) {
		emit editorOpened(m_insertIndex+2000, 0);
	}
}