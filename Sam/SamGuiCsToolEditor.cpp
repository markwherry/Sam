#include "SamGuiCsToolEditor.h"

#include "SamGuiCsToolGenericEditor.h"

#include <QPainter>
#include <QCloseEvent>

#include "SamEngine.h"
#include "SamRealTimeCommands.h"

#include "VstPluginWidget.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiCsToolEditor::SamGuiCsToolEditor(QWidget *parent)
	: QWidget(parent)
	, m_insert(0)
	, m_tool(0)
	, m_vstPlugin(0)
	, m_audioBus(0)
	, m_numEditors(0)
{
	//ui.setupUi(this);
	setWindowTitle("Tool Editor");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	for(int i=0; i<8; i++) {
		m_editor[i] = 0;
	}

	m_bypass = new SamGuiBypassButton(this);
	m_bypass->move(10,10);
	connect(m_bypass, SIGNAL(pressed()), this, SLOT(bypassButtonPressed()));

	m_link = new UiStateButton(this, -1, 2);
	m_link->setTextDisplaysInButton(true);
	m_link->setText("Link");
	m_link->move(90, 10);
	m_link->setFixedSize(40,20);
	m_link->setStateLineColor(0, QColor(0,0,0));
	//m_link->setStateFillColor(0, QColor(24, 87, 82));
	m_link->setStateFillColor(1, QColor(56, 218, 205));
	m_link->setTextColor(QColor(0,0,0));
	m_link->setFont(QFont("Calibri", 10, QFont::Bold));
	m_link->setDrawTint(false);
	connect(m_link, SIGNAL(clicked()), this, SLOT(linkButtonPressed()));

	QColor groupColors[15] = { QColor(143,26,26), QColor(143,26,143), QColor(84,26,143),
		                       QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
						    	QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	for(int i=0; i<8; i++) {
		m_groups[i] = new UiStateButton(this, 200+i, 2);
		m_groups[i]->setTextDisplaysInButton(true);
		m_groups[i]->setText(QString::number(i+1));
		m_groups[i]->setFixedSize(20,20);
		m_groups[i]->setStateLineColor(0, QColor(0,0,0));
		m_groups[i]->setStateFillColor(1, groupColors[i].darker(50));
		groupColors[i].setAlpha(110);
		m_groups[i]->setStateFillColor(0, groupColors[i]);
		
		m_groups[i]->setTextColor(QColor(0,0,0));
		m_groups[i]->setFont(QFont("Calibri", 10, QFont::Bold));
		m_groups[i]->setDrawTint(false);
		m_groups[i]->move(90, 10);

		connect(m_groups[i], SIGNAL(clicked(int)), this, SLOT(updateGroup(int)));
	}

	setFixedSize(400,200);

	m_pianoKeyboard = new UiPianoKeyboard(this, 10, 40);
	m_pianoKeyboard->move(0,42);

	//m_groupEditor = new SamGuiPerVoiceFxGroupEditor(this);
	//m_groupEditor->move(0, 42 + m_pianoKeyboard->height());

	samEngine->registerListener(this, SamEngine::ContextMusicTool);
	samEngine->registerListener(this, SamEngine::ContextAudioTool);

	connect(this, SIGNAL(toolActiveUpdated()), this, SLOT(updateToolActive()));
	connect(this, SIGNAL(toolEditorSizeUpdate(int, int)), this, SLOT(resizeEditor(int, int)));

	startTimer(80);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiCsToolEditor::~SamGuiCsToolEditor()
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextMusicTool || ev->context() == SamEngine::ContextAudioTool) {
		switch(ev->item()) {
			case SamMusicToolInsert::ItemActive: // SamAudioToolInsert::ItemActive:
				emit toolActiveUpdated();
				break;
			case SamAudioToolInsert::ItemEditorSizeUpdate:
				emit toolEditorSizeUpdate((int)ev->updatedObject(), (int)ev->fromObject());
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::resizeEditor(int width, int height)
{
	qDebug("SamGuiCsToolEditor::resizeEditor()");

	for(int i=0; i<m_numEditors; i++) {
		m_editor[i]->resize(width, height);
	}

	setFixedSize(m_editor[0]->width(), m_editor[0]->height()+42+1+30);
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::setToolInsert(SamToolInsert *insert, SamAudioBus *bus)
{
	qDebug("SamGuiCsToolEditor::setMusicToolInsert()");

	flushEditor();

	m_audioBus = bus;

	m_pianoKeyboard->hide();
	//m_groupEditor->hide();
	m_link->hide();

	for(int i=0; i<8; i++) {
		m_groups[i]->hide();
	}

	m_insert = insert;


	/*if(m_editor) {
		m_editor->hide();
		m_editor->setParent(0);
		delete m_editor;
		m_editor = 0;
	}*/

	if(!m_insert) {
		hide();
		return;
	}

	m_tool = 0;
	m_vstPlugin = 0;
	if(insert->type() == 0) {
		m_tool = ((SamAudioToolInsert*)insert)->audioTool();
		m_vstPlugin = ((SamAudioToolInsert*)insert)->vstPlugin();
	} else if(insert->type() == 1) {
		m_tool = ((SamMusicToolInsert*)insert)->musicTool();
	}

	if(!m_tool && !m_vstPlugin) return;

	if(m_tool) {
		m_numEditors = 1;
		m_editor[0] = m_tool->createEditor();
		if(!m_editor[0]) { // Create generic
			m_editor[0] = new SamGuiCsToolGenericEditor(0, (SamTool*)m_tool);
		}
	} else if(m_vstPlugin) {
		m_numEditors = ((SamAudioToolInsert*)insert)->numInstances();

		for(int i=0; i<m_numEditors; i++) {
			VstPluginWidget *vpw = new VstPluginWidget(this);
			((SamAudioToolInsert*)insert)->vstPlugin(i)->editorOpen(vpw->hWnd());
			RECT rcPlug;
			m_vstPlugin->editorGetRect(&rcPlug);
			vpw->resize(rcPlug.right - rcPlug.left,rcPlug.bottom - rcPlug.top);
			m_editor[i] = (QWidget*)vpw;
			m_editor[i]->hide();
		}
		
	}

	if(m_editor) { // Attach editor
		for(int i=0; i<m_numEditors; i++) {
			qDebug("Attaching editor");
			m_editor[i]->setParent(this);
			setFixedSize(m_editor[i]->width(), m_editor[0]->height()+42+1+30);
			m_editor[i]->move(0, 42);
			if(((SamAudioToolInsert*)insert)->currentSpeaker() == i) {
				m_editor[i]->show();
			}

		}
	}

	m_bypass->setToolInsert(m_insert);

	if(m_speakerButtons.size() > 0) {
		foreach(UiStateButton *speakerButton, m_speakerButtons) {
			delete speakerButton;
		}
		m_speakerButtons.clear();
	}

	int xx = 90;

	if(insert->type() == 0) {

		SamAudioToolInsert *ati = (SamAudioToolInsert*)insert;

		// If we're dealing with a per-voice Fx insert, there's other stuff that needs to be set up.
		/*if(ati->isPerVoiceFx()) {
			m_groupEditor->setChannel((SamChannel*)ati->parent());
			m_pianoKeyboard->show();
			m_groupEditor->show();

			int ew = m_editor[0]->width();
			if(ew < 880) ew = 880;
			setFixedSize(ew, m_editor[0]->height()+42+1+30 + 80);
			m_editor[0]->move((width()/2)-(m_editor[0]->width()/2), m_editor[0]->y() + 80);

			// Group mode nonsense
			SamChannel *channel = (SamChannel*)ati->parent();
			for(int i=0; i<8; i++) {
				m_groups[i]->move(xx, m_link->y());
				m_groups[i]->show();
				xx += (m_groups[i]->width() + 2);
				
				/*if(channel->currentPerVoiceGroup() == i) {
					m_groups[i]->setState(1);
				} else {
					m_groups[i]->setState(0);
				}*/

				
			/*}
			/*for(int i=0; i<ati->audioTool()->numParameters(); i++) {
					SamToolParameter *pr = channel->perVoiceParameter(channel->currentPerVoiceGroup(), i);
					m_tool->setParameterValueNormalised(pr->id(), pr->value());
						//aLog("[%d] %f", pr->id(), pr->value());
			
				}*/
			/*updateGroup(channel->currentPerVoiceGroup()+200);*/
		//} else {

			if(ati->numInstances() > 1) {
				//xx+=18;
				// Next, let's sort out Link mode
				m_link->move(xx, m_link->y()); xx += (m_link->width()+10);
				m_link->show();

				CsSpeakerConfig sc = ati->speakerConfig();
				int ns = ati->numInstances();

				//if(sc.numSpeakers() > 1) { What was I thinking here?! This is either a mistake, or something is very wrong...
				//	ns = sc.numSpeakers();
				//}

				QString speakerName[4] = { "L", "R", "Ls", "Rs" };
				QString speakerNameStereo[2] = { "LR", "LsRs" };

				//int x = 140;
				for(int i=0; i<ns; i++) {
					UiStateButton * button = new UiStateButton(this, i, 2);
					button->setTextDisplaysInButton(true);
					if(ati->mode() == 2 && ati->numInstances() == 2) {
						button->setText(speakerNameStereo[i]);
					} else {
						button->setText(speakerName[i]);
					}
					button->move(xx,10);
					button->setFixedSize(30,20);
					xx+=button->width()+2;
					button->setStateLineColor(0, QColor(0,0,0));
					button->setStateFillColor(0, QColor(118,115,88));
					button->setStateFillColor(1, QColor(255,230,0));
					button->setTextColor(QColor(0,0,0));
					button->setFont(QFont("Calibri", 10, QFont::Bold));
					button->setDrawTint(false);
					button->show(); // For some reason, this is needed or the buttons won't show again when recreated if the window is open
					m_speakerButtons.push_back(button);
					connect(button, SIGNAL(clicked(int)), this, SLOT(setSpeakerButton(int)));
				}

				updateLink();
			//}

		}

	} else if(insert->type() == 1) {
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::updateToolActive(void)
{
	update();

	if(m_insert) {
		if(!m_insert->isAssigned()) {
			hide();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::setSpeakerButton(int id)
{
	if(!m_insert) return;
	
	SamAudioToolInsert *ati = (SamAudioToolInsert*)m_insert;
	
	if(!ati->isLink()) {
		ati->setCurrentSpeaker(id);
		updateLink();

		if(m_vstPlugin && m_numEditors > 1) {
			for(int i=0; i<m_numEditors; i++) {
				m_editor[i]->hide();
			}
			m_editor[ati->currentSpeaker()]->show();
		}

		//m_speakerButtons[id]->setState(1);
		//emit speakerChannelSelected(id);
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::bypassButtonPressed(void)
{
	if(!m_insert) return;
	
	if(m_insert->type() == 0) {
		SamAudioToolInsert *ati = (SamAudioToolInsert*)m_insert;
		SamChannel *channel = (SamChannel*)ati->parent();
		//if(ati->isPerVoiceFx()) {
		//	samEngine->executeRealTimeCommand(new RtcSetPerVoiceFxToolActive(channel, m_insert->index(), !m_insert->isActive()));
		//} else {
			if(m_audioBus) {
				samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(m_audioBus, m_insert->index(), !ati->isActive()));
			}
		//}
	} else {
		m_insert->setActive(!m_insert->isActive());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::linkButtonPressed(void)
{
	//qDebug("SamGuiCsToolEditor::linkButtonPressed()");

	SamAudioToolInsert *ati = (SamAudioToolInsert*)m_insert;

	ati->setLink(!ati->isLink());

	updateLink();

	if(m_vstPlugin && m_numEditors > 1) {
		for(int i=0; i<m_numEditors; i++) {
			m_editor[i]->hide();
		}
		m_editor[ati->currentSpeaker()]->show();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::updateLink(void)
{
	SamAudioToolInsert *ati = (SamAudioToolInsert*)m_insert;
	if(ati->numInstances() < 2) return;

	m_link->setState((int)ati->isLink());

	if(ati->isLink()) {
		for(int i=0; i<m_speakerButtons.size(); i++) {
			m_speakerButtons[i]->setState(1);
		}
	} else {
		for(int i=0; i<m_speakerButtons.size(); i++) {
			m_speakerButtons[i]->setState(0);
		}
		m_speakerButtons[ati->currentSpeaker()]->setState(1);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::updateGroup(int id)
{
	/*if(m_insert && m_insert->type() == 0) {
		SamAudioToolInsert *ati = (SamAudioToolInsert*)m_insert;
		SamChannel *channel = (SamChannel*)ati->parent();
		channel->setCurrentPerVoiceGroup(id-200);

		for(int i=0; i<ati->audioTool()->numParameters(); i++) {
			SamToolParameter *pr = channel->perVoiceParameter(channel->currentPerVoiceGroup(), i);
			m_tool->setParameterValueNormalised(pr->id(), pr->value());
				//aLog("[%d] %f", pr->id(), pr->value());
			
		}

		((SamGuiCsToolGenericEditor*)m_editor)->updateParameters();


		for(int i=0; i<8; i++) {		
			if(((SamChannel*)ati->parent())->currentPerVoiceGroup() == i) {
				m_groups[i]->setState(1);
			} else {
				m_groups[i]->setState(0);
			}
		}
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::closeEvent(QCloseEvent *ev)
{
	qDebug("SamGuiCsToolEditor::closeEvent(QCloseEvent *ev)");

	ev->accept();

	flushEditor();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::hideEvent(QHideEvent *ev)
{
	qDebug("SamGuiCsToolEditor::hideEvent()");

	// Why do we need this? Presumably it's to handle this if not-the-user hides the window?
	if(m_editor) {
		/*for(int i=0; i<m_numEditors; i++) {
			m_editor->hide();
			m_editor->setParent(0);
			delete m_editor;
			m_editor = 0;
		}*/
		flushEditor();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0,0,width(),height(),QColor(50,50,50));

	int th = 40;

	//p.setPen(QColor(72,72,72));
	//p.setBrush(QColor(72,72,72));
	//p.drawRect(0,0,width()-1,th);
	p.fillRect(0,0,width(), th, QColor(70,70,70));

	//EStyle::paintBackground(&p,this);
	p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	//QPen pen(QColor(63,63,63));
	//pen.setWidth(2);
	//p.setPen(pen);
	//for (int i=0; i<height(); i+=4) {
	//	p.drawLine(0,i,width()-1,i);
	//}

	p.setPen(QColor(0,0,0,70));
	p.setBrush(QColor(0,0,0,70));
	p.drawRect(0,0,width()-1,th);
	//*/

	p.setPen(QColor(50,50,50));
	p.drawLine(0,th, width()-1, th);

	p.setPen(QColor(30,30,30));
	p.drawLine(0,th+1, width()-1, th+1);

	p.fillRect(0,height()-30,width(),30,QColor(30,30,30));
	p.setFont(QFont("Calibri", 14, QFont::Bold));
	p.setPen(QColor(200,200,200));
	if(m_insert->isAssigned()) p.drawText(10,height()-30,width()-20,30,Qt::AlignLeft|Qt::AlignVCenter, m_insert->name().toChar());

	if(m_insert->isAssigned() && m_insert->name() == "Doubler") {
		p.setPen(QColor(200,200,200,40));
		if(m_tool) p.drawText(75,height()-30,width()-20,30,Qt::AlignLeft|Qt::AlignVCenter, m_insert->name().toChar());
	}

	/*if(m_insert->activeController() > -1) {
		p.setPen(QColor(0,255,255));
		QFont font("Verdana",7,QFont::Bold);
		font.setStyleStrategy(QFont::PreferAntialias);
		p.setFont(font);
		p.drawText(m_bypass->x() + m_bypass->width() + 10, m_bypass->y(), 50, m_bypass->height(), Qt::AlignLeft | Qt::AlignVCenter, QString("CC ")+QString::number(m_insert->activeController()));
	}//*/

	p.setPen(QColor(0,0,0,50));
	p.drawLine(80,0,80,th-1);

	p.setPen(QColor(255,255,255,25));
	p.drawLine(81,0,81,th-1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::timerEvent(QTimerEvent *ev)
{
	if(m_editor && m_vstPlugin) {
		m_lock.enter();
		m_vstPlugin->editorIdle();
		m_lock.leave();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolEditor::flushEditor(void)
{
	m_lock.enter();

	if(m_tool) {
		
	} else if(m_vstPlugin) {
		for(int i=0; i<m_numEditors; i++) {
			((SamAudioToolInsert*)m_insert)->vstPlugin(i)->editorClose();
		}
	}

	if(m_editor) {
		for(int i=0; i<m_numEditors; i++) {
			m_editor[i]->hide();
			m_editor[i]->setParent(0);
			delete m_editor[i];
			m_editor[i] = 0;
		}
	}

	m_insert = 0;
	m_tool = 0;
	m_vstPlugin = 0;
	m_audioBus = 0;
	m_numEditors = 0;

	m_lock.leave();
}