#include "SamGuiSelectAudioTool.h"

#include "SamEngine.h"
#include "SamRealTimeCommands.h"

#include "Sam.h"
#include "SamProjectCommands.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSelectAudioTool::SamGuiSelectAudioTool(QWidget *parent, bool includeVst)
	: QWidget(parent)
	, m_insertIndex(0)
	, m_channel(0)
	, m_bus(0)
{
	//ui.setupUi(this);

	m_menu = new QMenu();
	
	m_actionNone = m_menu->addAction("None");

	m_menu->addSeparator();

	m_menuInternal = m_menu->addMenu("Internal");

	for(int i=0; i<samEngine->audioToolManager()->numTools(); i++) {
		QAction *action = m_menuInternal->addAction(samEngine->audioToolManager()->toolInfo(i).name.toChar());
		m_actionsInternal.append(action);
	}

	if(includeVst) {
		m_menuVst = m_menu->addMenu("VST");

		if(samEngine->vstPluginManager()->numPlugins() > 0) {
			for(int i=0; i<samEngine->vstPluginManager()->numPlugins(); i++) {
				QAction *action = m_menuVst->addAction(samEngine->vstPluginManager()->pluginProductName(i));
				m_actionsVst.append(action);
			}
		} else {
			QAction *action = m_menuVst->addAction("No VST Plug-ins");
			action->setEnabled(false);
		}
	}

	connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(selectAction(QAction*)));

	//m_timer = new QTimer(this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSelectAudioTool::~SamGuiSelectAudioTool()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectAudioTool::setChannel(SamChannel *channel, int outputIndex, int insertIndex)
{
	m_channel = channel;
	m_outputIndex = outputIndex;
	m_insertIndex = insertIndex;

	//updateTool();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiSelectAudioTool::setInsertIndex(int insertIndex)
{
	m_insertIndex = insertIndex;

	//updateTool();
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectAudioTool::selectAction(QAction *action)
{
	qDebug("SamGuiSelectAudioTool::selectAction()");

	if(action == m_actionNone) {
		qDebug("action == m_actionNone");
		/*if(m_insertIndex < 0) {
			samEngine->executeRealTimeCommand(new RtcSetPerVoiceFxToolActive(m_channel, 0, false));
			samEngine->executeRealTimeCommand(new RtcSetPerVoiceFxTool(m_channel, ""));
			return;
		} else */ {
			//samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(m_channel->audioOutputBus(m_outputIndex), m_insertIndex, false));
			//samEngine->executeRealTimeCommand(new RtcSetAudioTool(m_channel->audioOutputBus(m_outputIndex), 0, m_insertIndex, ""));
			
			if(m_channel) {
				samGui->commandManager()->execute(new SetAudioInsert(m_channel->audioOutputBus(m_outputIndex), 0, m_insertIndex, AString("")));
			} else {
				samGui->commandManager()->execute(new SetAudioInsert(samEngine->project()->audioBus(m_outputIndex), 0, m_insertIndex, AString("")));
			}
			
			return;
		}
	}

	for(int i=0; i<m_actionsInternal.size(); i++) {
		if(action == m_actionsInternal[i]) {
			qDebug("Found tool");
			/*if(m_insertIndex < 0) {
				samEngine->executeRealTimeCommand(new RtcSetPerVoiceFxTool(m_channel, samEngine->audioToolManager()->toolInfo(i).uuid));
			} else*/ {
				//aLog("m_channel->numAudioOutputs() = %d", m_channel->numAudioOutputs());
				if(m_channel) {
					if(m_channel->numAudioOutputs() > 0) {
						//samEngine->executeRealTimeCommand(new RtcSetAudioTool(m_channel->audioOutputBus(m_outputIndex), 0, m_insertIndex, samEngine->audioToolManager()->toolInfo(i).uuid));
						samGui->commandManager()->execute(new SetAudioInsert(m_channel->audioOutputBus(m_outputIndex), 0, m_insertIndex, samEngine->audioToolManager()->toolInfo(i).uuid));
					}
				} else {
					samGui->commandManager()->execute(new SetAudioInsert(samEngine->project()->audioBus(m_outputIndex), 0, m_insertIndex, samEngine->audioToolManager()->toolInfo(i).uuid));
				}
			}
			return;
		}
	}

	for(int i=0; i<m_actionsVst.size(); i++) {
		if(action == m_actionsVst[i]) {
			qDebug("action == VST");
			if(m_insertIndex < 0) {
			} else {
				//samEngine->executeRealTimeCommand(new RtcSetVstPlugin(m_channel->audioOutputBus(m_outputIndex), 0, m_insertIndex, samEngine->vstPluginManager()->pluginFilename(i)));
				if(m_channel) {
					samGui->commandManager()->execute(new SetAudioInsert(m_channel->audioOutputBus(m_outputIndex), 0, m_insertIndex, samEngine->vstPluginManager()->pluginFilename(i)));
				} else {
					samGui->commandManager()->execute(new SetAudioInsert(samEngine->project()->audioBus(m_outputIndex), 0, m_insertIndex, samEngine->vstPluginManager()->pluginFilename(i)));
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectAudioTool::timerElapsed(void)
{
	if(!m_doubleClicked) {
		m_menu->popup(mapToGlobal(QPoint(0,height())));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiSelectAudioTool::updateTool(void)
{
	//qDebug("SamGuiSelectMusicTool::updateTool()");

	if(!m_channel) return;
	if(m_channel->numVoices() < 1) return;

	SamAudioToolInsert *insert = m_channel->voice(0)->audioToolInsert();


	/*if(!insert) {
		setSelectedAction(m_actionNoTool);
	} else {
		for(int i=0; i<m_toolActions.size(); i++) { //qDebug("5");
			if(insert->audioTool()->uuid() == samEngine->audioToolManager()->toolInfo(i).uuid) { //qDebug("6");
				setSelectedAction(m_toolActions[i]); //qDebug("7");
			}
		}
	}*/
//}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectAudioTool::mousePressEvent(QMouseEvent *ev)
{
	if(m_insertIndex < 0) {

	} else {
		SamAudioBus *atb = 0;
		if(m_channel) {
			atb = m_channel->audioOutputBus(m_outputIndex);
		} else {
			atb = samEngine->project()->audioBus(m_outputIndex);
		}
		if(!atb->audioInsert(m_insertIndex)->isAssigned()) {
			m_menu->popup(mapToGlobal(QPoint(0,height())));
		} else {
			m_doubleClicked = false;
			//m_timerId = startTimer(300);
			QTimer::singleShot(300, this, SLOT(timerElapsed()));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSelectAudioTool::mouseDoubleClickEvent(QMouseEvent *ev)
{
	m_doubleClicked = true;
	//emit doubleClicked();

	if(m_insertIndex < 0) {

	} else {
		SamAudioBus *atb = 0;
		if(m_channel) {
			atb = m_channel->audioOutputBus(m_outputIndex);
		} else {
			atb = samEngine->project()->audioBus(m_outputIndex);
		}
		if(atb->audioInsert(m_insertIndex)->isAssigned()) {
			samGui->toolEditor()->setToolInsert(m_channel->audioOutputBus(m_outputIndex)->audioInsert(m_insertIndex), m_channel->audioOutputBus(m_outputIndex));
			samGui->toolEditor()->show();
		}
	}

	//killTimer(m_timerId); // Just incase
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiSelectAudioTool::timerEvent(QTimerEvent *ev)
{
	//killTimer(m_timerId);

	if(!m_doubleClicked) {
		m_menu->popup(mapToGlobal(QPoint(0,height())));
	}
}*/