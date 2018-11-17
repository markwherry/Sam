#include "SamGuiMainWidget.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <QPainter>

#include "Sam.h"
#include "SamGuiChannel.h"
#include "SamPreferences.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMainWidget::SamGuiMainWidget(QWidget *parent)
	: QWidget(parent)
	, m_timerId(0)
{
	//ui.setupUi(this);

	m_toolbar = new UiToolbar(this, 38);
	m_toolbar->move(MARGIN,4);

	m_tbAdd = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbAdd->setImage(":/Resources/tmuAdd.png");
	m_tbAdd->setLabel("Add", "Instrument");
	connect(m_tbAdd, SIGNAL(clicked()), samGui, SLOT(openInstrument()));
	m_toolbar->addObject(m_tbAdd);

	m_tbRemove = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbRemove->setImage(":/Resources/tmuRemove.png");
	m_tbRemove->setLabel("Remove", "Channel(s)");
	connect(m_tbRemove, SIGNAL(clicked()), samGui, SLOT(deleteChannels()));
	m_toolbar->addObject(m_tbRemove);//*/

	m_tbReplace = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbReplace->setImage(":/Resources/update.png");
	m_tbReplace->setLabel("Replace", "Instrument");
	connect(m_tbReplace, SIGNAL(clicked()), samGui, SLOT(replaceInstrument()));
	m_toolbar->addObject(m_tbReplace);//*/

	m_toolbar->addSeparator();
	/*
#ifdef USE_MIXER
	m_tbMainMixer = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbMainMixer->setImage(":/Resources/mixer25b.png");
	m_tbMainMixer->setLabel("Main", "Mixer");
	connect(m_tbMainMixer, SIGNAL(clicked()), this, SLOT(mainMixer()));
	m_toolbar->addObject(m_tbMainMixer);
#endif
	*/

	m_tbMixer = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbMixer->setImage(":/Resources/mixer25b.png");
	m_tbMixer->setLabel("Channel", "Mixers");
	connect(m_tbMixer, SIGNAL(clicked()), this, SLOT(mixer()));
	m_toolbar->addObject(m_tbMixer);

	m_tbArticulation = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbArticulation->setImage(":/Resources/articulation25.png");
	m_tbArticulation->setLabel("Articulation", "Viewer");
	connect(m_tbArticulation, SIGNAL(clicked()), this, SLOT(articulation()));
	m_toolbar->addObject(m_tbArticulation);

	m_tbKeyboard = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbKeyboard->setImage(":/Resources/keyboard25.png");
	m_tbKeyboard->setLabel("Virtual", "Keyboard");
	connect(m_tbKeyboard, SIGNAL(clicked()), this, SLOT(keyboard()));
	m_toolbar->addObject(m_tbKeyboard);

	m_tbProperties = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbProperties->setImage(":/Resources/toggleRight25.png");
	m_tbProperties->setLabel("Channel", "Properties");
	connect(m_tbProperties, SIGNAL(clicked()), this, SLOT(properties()));
	m_toolbar->addObject(m_tbProperties);

	m_toolbar->addSeparator();

	//m_tbCellAnalyser = new UiToolbarObject(UiToolbarObject::Button, 100);
	//m_tbCellAnalyser->setImage(":/Resources/cellanalyser25.png");
	//m_tbCellAnalyser->setLabel("Cell", "Analyser");
	//connect(m_tbCellAnalyser, SIGNAL(clicked()), samGui->cellAnalyser(), SLOT(show()));
	//m_toolbar->addObject(m_tbCellAnalyser);

	m_tbMidiMonitor = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbMidiMonitor->setImage(":/Resources/midi25.png");
	m_tbMidiMonitor->setLabel("MIDI Input", "Monitor");
	connect(m_tbMidiMonitor, SIGNAL(clicked()), samGui, SLOT(openMidiMonitor()));
	m_toolbar->addObject(m_tbMidiMonitor);

	m_tbMidiChannelMap = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbMidiChannelMap->setImage(":/Resources/usedchannels25.png");
	m_tbMidiChannelMap->setLabel("MIDI Channel", "Map");
	connect(m_tbMidiChannelMap, SIGNAL(clicked()), samGui->midiChannelMap(), SLOT(show()));
	m_toolbar->addObject(m_tbMidiChannelMap);

	/*m_tbFindChannel = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbFindChannel->setImage(":/Resources/midiInput25.png");
	m_tbFindChannel->setImage(":/Resources/midiInputOn25.png",1);
	m_tbFindChannel->setLabel("Select Channel", "by MIDI Input");
	connect(m_tbFindChannel, SIGNAL(clicked()), this, SLOT(findChannel()));
	m_toolbar->addObject(m_tbFindChannel);//*/

	m_statusBar = new SamGuiStatusBar(this);

#ifdef USE_LIBRARY
	m_tabBar = new UiTabBar(this);
	m_tabBar->addTab("Channel ");
	m_tabBar->addTab("Library");
	m_tabBar->selectTab(0);
	m_tabBar->setCeiling(10);
	m_tabBar->setFont(QFont("Calibri", 12));
#endif

	m_channelList = new SamGuiChannelList(0);
	//m_channelList->setEmptyTitle("\nWelcome to Sam");
	//m_channelList->setEmptyText("To load an instrument, click the Add Instrument button\nor open a Project or Instrument from the File menu.\n\n");
	m_editorChannelList = new UiEditor(this, true);
	m_editorChannelList->setBackgroundText("To load an instrument, click the Add Instrument button\nor open a Project or Instrument from the File menu.");
	m_editorChannelList->setBackgroundTextVisible(true);
	//m_editorChannelList->setTitle("Channels");
	m_editorChannelList->setWidget(m_channelList);
	m_channelProperties = new SamGuiChannelProperties(0);


	m_tpcChannelProperties = new UiToolPanelContainer(this, UiToolPanelContainer::Right);
	//m_tpcChannelProperties->toolPanel()->addSection("No Channel Selected", m_channelProperties);

	m_channelSettings  = new SamGuiChannelSettings(0);
	m_channelInput     = new SamGuiChannelInput(0);
	//m_channelCrossfade = new SamGuiChannelCrossfade(0);
	m_channelTools     = new SamGuiChannelTools(0);
	m_channelInserts   = new SamGuiChannelInserts(0);
	m_channelSends     = new SamGuiChannelSends(0);
	m_channelOutputs   = new SamGuiChannelOutputs(0);

	m_tpcChannelProperties->toolPanel()->addSection("Additional Settings", m_channelSettings);
	m_tpcChannelProperties->toolPanel()->addSection("MIDI Input", m_channelInput);
	m_tpcChannelProperties->toolPanel()->addSection("Music Tools", m_channelTools);
	//m_tpcChannelProperties->toolPanel()->addSection("Articulation Crossfade", m_channelCrossfade);
	//m_tpcChannelProperties->toolPanel()->addSection("Audio Inserts", m_channelInserts);
	m_tpcChannelProperties->toolPanel()->addSection("Audio Sends", m_channelSends);
	m_tpcChannelProperties->toolPanel()->addSection("Audio Outputs", m_channelOutputs);
	

	m_editorProperties = new UiEditor(this, true);
	//m_editorProperties->setTitle("No Channel Selected");
	//m_editorProperties->setWidget(m_channelProperties);
	m_editorProperties->hide();

	
#ifdef USE_LIBRARY
	m_editorBrowser = new UiEditor(this, true);
	m_instrumentList = new SamGuiInstrumentList(0);
	m_editorBrowser->setWidget(m_instrumentList);
#endif
	

	m_stackedWidget = new QStackedWidget(this);
	//m_stackedWidget->addWidget(m_editorProperties);
	m_stackedWidget->addWidget(m_tpcChannelProperties);
#ifdef USE_LIBRARY
	m_stackedWidget->addWidget(m_editorBrowser);
#endif
	m_stackedWidget->setCurrentIndex(0);
	m_stackedWidget->setFixedWidth(m_tpcChannelProperties->width());
	m_stackedWidget->move(width()-MARGIN-m_stackedWidget->width(), m_toolbar->height()+MARGIN); // was 10
	if(!samPreferences->app()->preference("propertiesVisible", "0").toInt()) {
		m_stackedWidget->hide();
#ifdef USE_LIBRARY
		m_tabBar->hide();
#endif
	}

#ifdef USE_MIXER
	m_editorMainMixer = new UiEditor(this, false, true);
	//m_editorMainMixer->setSideBarWidth(UiEditor::Right, 308);
	m_mixer = new SamGuiMixer(0);
	m_editorMainMixer->setWidget(m_mixer);
	if(!samPreferences->app()->preference("mixerVisible", "0").toInt()) {
		m_editorMainMixer->hide();
	}
	//m_audioBusProperties = new SamGuiAudioBusProperties(0);
	//m_editorMainMixer->addWidgetToSideBar(UiEditor::Right, m_audioBusProperties);
	//connect(m_mixer, SIGNAL(audioBusSelected(SamChannel*, int)), m_audioBusProperties, SLOT(setAudioBus(SamChannel*, int)));
#endif

	m_editorMixer = new UiEditor(this, false, true);
	m_editorMixer->setSideBarWidth(UiEditor::Right, 160);
	//m_editorMixer->setTitle("Mixer");
	//m_toolContainer = new SamGuiToolContainer(0);
	m_streamMixer = new SamGuiStreamMixer(0);
	m_editorMixer->setWidget(m_streamMixer);
	if(!samPreferences->app()->preference("mixerVisible", "0").toInt()) {
		m_editorMixer->hide();
	}

	m_editorArticulation = new UiEditor(this, false, true);
	m_editorArticulation->setSideBarWidth(UiEditor::Right, 160);
	m_articulationEditor = new SamGuiArticulationEditor(0);
	m_editorArticulation->setWidget(m_articulationEditor);
	if(!samPreferences->app()->preference("articulationVisible", "0").toInt()) {
		m_editorArticulation->hide();
	}
	m_articulationSection = new SamGuiArticulationSection(0);
	m_editorArticulation->addWidgetToSideBar(UiEditor::Right, m_articulationSection);

	m_outputConfigSection = new SamGuiOutputConfigSection(0);
	m_mixerPresetSection = new SamGuiMixerPresetSection(0);
	m_editorMixer->addWidgetToSideBar(UiEditor::Right, m_mixerPresetSection);
	m_editorMixer->addWidgetToSideBar(UiEditor::Right, m_outputConfigSection);
	m_editorMixer->setSideBarWidgetsVisible(false);
	


	m_pianoController = new SamGuiPianoController(0);
	m_editorPiano = new UiEditor(this);
	m_editorPiano->setSideBarWidth(UiEditor::Right, 160);
	m_editorPiano->setWidget(m_pianoController);
	if(!samPreferences->app()->preference("pianoVisible", "1").toInt()) {
		m_editorPiano->hide();
	}

	m_articulationModeSection = new SamGuiArticulationModeSection(0);
	m_editorPiano->addWidgetToSideBar(UiEditor::Right, m_articulationModeSection);
	m_editorPiano->setSideBarWidgetsVisible(false);

	//m_editorPiano->hide();

	/*m_editorKeyboard = new UiEditor(this);
	m_editorKeyboard->setTitle("Keyboard: No Channel Selected");
	m_editorKeyboard->setWidget(m_pianoKeyboard, -2);
	m_editorKeyboard->setFixedHeight(91);

	m_streamMixer = new SamGuiStreamMixer(0);
	m_editorStreamMixer = new UiEditor(this, false, true);
	m_editorStreamMixer->setTitle("Instrument Mixer: No Channel Selected");
	m_editorStreamMixer->setWidget(m_streamMixer);
	m_editorStreamMixer->setFixedHeight(348);*/

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);

	connect(this, SIGNAL(channelSelected()), this, SLOT(updateSelectedChannel()));
	connect(this, SIGNAL(articulationUpdated()), this, SLOT(updateArticulation()));

#ifdef USE_LIBRARY
	connect(m_tabBar, SIGNAL(tabSelected(int)), m_stackedWidget, SLOT(setCurrentIndex(int)));
#endif

	//m_scNewProject = new QShortcut(QKeySequence(Qt::ControlModifier+Qt::Key_N), this); // Now handled in Sam.cpp because it's a menu command
	//connect(m_scNewProject, SIGNAL(activated()), samGui, SLOT(newProject()));

	m_scLoadRecentInstrument = new QShortcut(QKeySequence(Qt::ShiftModifier+Qt::Key_0), this);
	connect(m_scLoadRecentInstrument, SIGNAL(activated()), samGui, SLOT(openRecentInstrument()));

	m_scLoadRecentProject = new QShortcut(QKeySequence(Qt::ControlModifier+Qt::Key_0), this);
	connect(m_scLoadRecentProject, SIGNAL(activated()), samGui, SLOT(openRecentProject()));

	m_scSelectNextChannel = new QShortcut(QKeySequence(Qt::Key_Down), this);
	connect(m_scSelectNextChannel, SIGNAL(activated()), this, SLOT(selectNextChannel()));

	m_scSelectPrevChannel = new QShortcut(QKeySequence(Qt::Key_Up), this);
	connect(m_scSelectPrevChannel, SIGNAL(activated()), this, SLOT(selectPrevChannel()));

	m_scEditInstrument = new QShortcut(QKeySequence(Qt::Key_E), this);
	connect(m_scEditInstrument, SIGNAL(activated()), this, SLOT(editInstrument()));

	m_scRemoveChannels = new QShortcut(QKeySequence(Qt::Key_Backspace), this);
	connect(m_scRemoveChannels, SIGNAL(activated()), samGui, SLOT(deleteChannels()));

	m_scMixer = new QShortcut(QKeySequence(Qt::Key_M), this);
	connect(m_scMixer, SIGNAL(activated()), this, SLOT(mixer()));

	m_scKeyboard = new QShortcut(QKeySequence(Qt::Key_K), this);
	connect(m_scKeyboard, SIGNAL(activated()), this, SLOT(keyboard())); 

	m_scProperties = new QShortcut(QKeySequence(Qt::Key_P), this);
	connect(m_scProperties, SIGNAL(activated()), this, SLOT(properties()));

	//m_timerId = startTimer(80);

	m_popupChannelIndicators = new QMenu(0);
	int actionCount = 0;
	QString pciInfos[15] = { "Volume", "Expression", "Crossfade", "Sustain", "Brightness", "Pitchbend", "SEPARATOR", "Send 1", "Send 2", "Send 3", "Send 4", "SEPARATOR", "Close", "Medium", "Far" };
	for(int i=0; i<15; i++) {
		if(pciInfos[i] == "SEPARATOR") {
			m_popupChannelIndicators->addSeparator();
		} else {
			m_pciInfoActions[actionCount] = m_popupChannelIndicators->addAction(pciInfos[i]);
			m_pciInfoActions[actionCount]->setCheckable(true);
			actionCount++;
		}
	}
	//m_pciInfoActions[7]->setEnabled(false);
	//m_pciInfoActions[m_zdiInfo]->setChecked(true);
	connect(m_popupChannelIndicators, SIGNAL(triggered(QAction*)), this, SLOT(setChannelIndicator(QAction*)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMainWidget::~SamGuiMainWidget()
{
	qDebug("SamGuiMainWidget::~SamGuiMainWidget()");
	delete m_popupChannelIndicators;
	qDebug("EXIT: SamGuiMainWidget::~SamGuiMainWidget()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::onState(AStateEvent *ev)
{
	//qDebug("SamGuiMainWidget::onState() ev=%d", ev);

	if(ev->context() == SamEngine::ContextChannel) {

		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit channelSelected();
				break;
			case SamChannel::ItemMidiInputChannel:
				emit channelSelected();
				break;
			case SamChannel::ItemConfig:
				//emit channelConfigUpdated();
				emit channelSelected();
				break;
			case SamChannel::ItemArticulation:
				aLog("SamChannel::ItemArticulation: %d, %d", ev->updatedObject(), samEngine->project()->selectedChannel());
				if(ev->updatedObject() == samEngine->project()->selectedChannel()) {
					aLog("Here!");
					emit articulationUpdated();
				}
				break;
		}

	} else if(ev->context() == SamEngine::ContextProject) {
		
		switch(ev->item()) {
			case SamProject::ItemChannelRemoved:
				emit channelSelected();
				break;
		}

	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::updateSelectedChannel(void)
{
	QColor midiChannelColor[17] = { QColor(160,160,160),
									QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
		                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	if(samEngine->numInstruments() > 0) {
		m_editorChannelList->setBackgroundTextVisible(false);
	} else {
		m_editorChannelList->setBackgroundTextVisible(true);
	}

	SamChannel *channel = samEngine->project()->selectedChannel();


	samGui->cellAnalyser()->setChannel(channel);
	samGui->bnceInstrument()->setChannel(channel);


	if(channel) {
		m_editorProperties->setTitle(channel->version()->name().toChar());

		int channelColorIndex = channel->midiInputChannel();
		m_editorProperties->setHeaderColor(midiChannelColor[channelColorIndex]);


	
		m_tpcChannelProperties->toolPanel()->setSectionTitle(0, channel->version()->name().toChar());



		//m_editorKeyboard->setTitle("Keyboard: "+QString(channel->version()->name().toChar()));
		//m_editorKeyboard->setHeaderColor(midiChannelColor[channelColorIndex]);

		//m_editorMixer->setTitle("Instrument Mixer: "+QString(channel->version()->name().toChar()));
		//m_editorMixer->setHeaderColor(midiChannelColor[channelColorIndex]);
		
		m_mixer->addChannel(channel, true);
		m_streamMixer->setEditorChannel(channel);
		m_mixerPresetSection->setChannel(channel);
		m_articulationEditor->setChannel(channel);
		m_outputConfigSection->setSpeakerConfig(channel, channel->streamMix()->speakerConfig());
		m_editorMixer->setSideBarWidgetsVisible();
		
		m_pianoController->setChannel(channel);
		m_editorPiano->setSideBarWidgetsVisible();

		
		

	} else {
		if(samEngine->project()->numSelectedChannels() > 1) {
			m_editorProperties->setTitle("Multiple Channels Selected");
			m_tpcChannelProperties->toolPanel()->setSectionTitle(0, "Multiple Channels Selected");
		} else {
			m_editorProperties->setTitle("No Channel Selected");
			m_tpcChannelProperties->toolPanel()->setSectionTitle(0, "No Channel Selected");
		}
		m_editorProperties->setHeaderColor(QColor(120,120,120));

		m_mixer->addChannel(0, true);
		m_streamMixer->setEditorChannel(0);
		m_mixerPresetSection->setChannel(0);
		m_articulationEditor->setChannel(0);
		m_outputConfigSection->setSpeakerConfig(0, CsSpeakerConfig::Undefined);
		m_editorMixer->setSideBarWidgetsVisible(false);

		m_pianoController->setChannel(0);
		m_editorPiano->setSideBarWidgetsVisible(false);

		//samGui->cellAnalyser()->setChannel(0);
		//samGui->bnceInstrument()->setChannel(0);
	}

	if(m_channelList->numItems() < 1) {
		m_editorChannelList->sizeWidgetHeightToFit();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::updateArticulation(void)
{
	aLog("SamGuiMainWidget::updateArticulation()");

	SamChannel *channel = samEngine->project()->selectedChannel();
	if(channel) {
		aLog("Here!");
		m_pianoController->setChannel(channel);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::selectNextChannel(void)
{
	qDebug("SamGuiMainWidget::selectNextChannel()");

	m_channelList->selectNextItem();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::selectPrevChannel(void)
{
	qDebug("SamGuiMainWidget::selectPreviousChannel()");

	m_channelList->selectPrevItem();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::editInstrument(void)
{
	qDebug("SamGuiMainWidget::editInstrument()");

	if(m_channelList->numItemsSelected() == 1) {
		int pos = m_channelList->firstSelectedItemPosition();
		SamGuiChannel *item = (SamGuiChannel*)m_channelList->itemFromPosition(pos);
		samGui->openEditor(item->channel());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::properties(void)
{
	m_stackedWidget->setVisible(!m_stackedWidget->isVisible());
#ifdef USE_LIBRARY
	m_tabBar->setVisible(!m_tabBar->isVisible());
#endif
	samPreferences->app()->setPreference("propertiesVisible", AString::number((int)m_stackedWidget->isVisible()));
	resizeEvent(0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::mainMixer(void)
{
	/*
#ifdef USE_MIXER
	if(m_editorMainMixer->isVisible()) {
		m_editorMainMixer->setVisible(false);
	} else {
		if(m_editorMixer->isVisible()) {
			m_editorMixer->setVisible(false);
		}
		m_editorMainMixer->setVisible(true);
	}
#endif

	//samPreferences->app()->setPreference("mainMixerVisible", AString::number((int)m_editorMainMixer->isVisible()));
	samPreferences->app()->setPreference("mixerVisible", AString::number((int)m_editorMixer->isVisible()));

	resizeEvent(0);
	*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::mixer(void)
{
	/*
#ifdef USE_MIXER
	if(m_editorMixer->isVisible()) {
		m_editorMixer->setVisible(false);
	} else {
		if(m_editorMainMixer->isVisible()) {
			m_editorMainMixer->setVisible(false);
		}
		m_editorMixer->setVisible(true);
	}
#else
	m_editorMixer->setVisible(!m_editorMixer->isVisible());
#endif
	*/

	m_editorMixer->setVisible(!m_editorMixer->isVisible());
#ifdef USE_MIXER
	m_editorMainMixer->setVisible(m_editorMixer->isVisible());
#endif

	//if(m_editorMixer->isVisible()) {
	//	m_editorArticulation->setVisible(false);
	//}

	//samPreferences->app()->setPreference("mainMixerVisible", AString::number((int)m_editorMainMixer->isVisible()));
	//samPreferences->app()->setPreference("articulationVisible", AString::number((int)m_editorArticulation->isVisible()));
	samPreferences->app()->setPreference("mixerVisible", AString::number((int)m_editorMixer->isVisible()));
	
	resizeEvent(0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::articulation(void)
{
	m_editorArticulation->setVisible(!m_editorArticulation->isVisible());

	//if(m_editorArticulation->isVisible()) {
	//	m_editorMixer->setVisible(false);
	//	m_editorMainMixer->setVisible(false);
	//}

	samPreferences->app()->setPreference("articulationVisible", AString::number((int)m_editorArticulation->isVisible()));
	//samPreferences->app()->setPreference("mixerVisible", AString::number((int)m_editorMixer->isVisible()));

	resizeEvent(0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::keyboard(void)
{
	m_editorPiano->setVisible(!m_editorPiano->isVisible());
	samPreferences->app()->setPreference("pianoVisible", AString::number((int)m_editorPiano->isVisible()));
	resizeEvent(0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::findChannel(void)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::setChannelIndicator(QAction *action)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::showEvent(QShowEvent *ev)
{
	if(!m_timerId) {
		m_timerId = startTimer(50);
	}
	resizeEvent(0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::hideEvent(QHideEvent *ev)
{
	killTimer(m_timerId);
	m_timerId = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::timerEvent(QTimerEvent *ev)
{
	//qDebug("SamGuiMainWidget::timerEvent()");
	//m_pianoController->updateActivePitches();

	emit timerTick();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMainWidget::resizeEvent(QResizeEvent *ev)
{
	
	//m_editorKeyboard->move(10, height()-m_editorKeyboard->height()-10);
	//m_editorKeyboard->setFixedWidth(width()-20);

	m_statusBar->move(0, height()-m_statusBar->height());
	m_statusBar->setFixedWidth(width());

	//m_tpcZoneProperties->move(width()-1-m_tpcZoneProperties->width()-MARGIN, y);
	//m_tpcZoneProperties->setFixedHeight(height()-1-m_toolbar->height()-(MARGIN*2));

	//m_stackedWidget->setFixedSize(300, height()-m_statusBar->height()-(MARGIN*2)-m_toolbar->height());
	m_stackedWidget->setFixedHeight(height()-m_statusBar->height()-(MARGIN*2)-m_toolbar->height());

	int x = MARGIN;
	int w = width() - (MARGIN*2);
	if(m_stackedWidget->isVisible()) {
		//x += (m_stackedWidget->width() + MARGIN);
		w -= (m_stackedWidget->width() + MARGIN);
	}
	m_stackedWidget->move(width()-MARGIN-m_stackedWidget->width(), m_toolbar->height()+MARGIN);

#ifdef USE_LIBRARY
	m_tabBar->move(m_stackedWidget->x()+40,MARGIN + m_toolbar->height() + 3 - 42); //MARGIN+40
	m_tabBar->setFixedWidth(m_stackedWidget->width()-80);
#endif
	m_toolbar->setFixedWidth(width());
	m_toolbar->move(MARGIN, 4);

#ifdef USE_MIXER
	m_editorMainMixer->setFixedSize((w/2)-(MARGIN/2), 404+20); // h=304
	m_editorMixer->setFixedSize((w/2)-(MARGIN/2), 404+20);
	m_editorArticulation->setFixedSize(w, 404+20);
#else
	m_editorMixer->setFixedSize(w, 404+20);
#endif
	m_editorPiano->setFixedSize(w, 66); // 66

	m_editorArticulation->setFixedSize(w, 180);
	

	int channelListHeight = height()-(MARGIN*2)-m_toolbar->height()-m_statusBar->height();
	if(m_editorPiano->isVisible()) { // Keyboard
		channelListHeight -= m_editorPiano->height() + MARGIN;
	}
	if(m_editorMixer->isVisible()) { // Mixer
		channelListHeight -= m_editorMixer->height() + MARGIN;
	}
	if(m_editorArticulation->isVisible()) { // Mixer
		channelListHeight -= m_editorArticulation->height() + MARGIN;
	}
	/*
#ifdef USE_MIXER
	if(m_editorMainMixer->isVisible()) { // Main Mixer
		channelListHeight -= m_editorMainMixer->height() + MARGIN;
	}
#endif
	*/
	if(1) { // Parameters
		//channelListHeight -= 80 + 10;
	}

	
	//m_stackedWidget->move(width()-m_stackedWidget->width()-10, 70);

	m_editorChannelList->setFixedSize(w, channelListHeight);
	m_editorChannelList->move(x, m_toolbar->height()+MARGIN);


	int mixerY = m_editorChannelList->y()+channelListHeight+MARGIN;
	if(m_editorArticulation->isVisible()) {
		mixerY += (m_editorArticulation->height()+MARGIN);
	}

	m_editorMixer->move(x, mixerY); // was 10
#ifdef USE_MIXER
	m_editorMainMixer->move(x+(w/2)+(MARGIN/2), mixerY);
#endif
	
	
	
	m_editorArticulation->move(x, m_editorChannelList->y()+channelListHeight+MARGIN);

	int pky = m_editorChannelList->y()+channelListHeight+MARGIN; // was 10
	if(m_editorMixer->isVisible()) { // mixer
		pky += m_editorMixer->height()+MARGIN; // was 10
	}
	if(m_editorArticulation->isVisible()) { // mixer
		pky += m_editorArticulation->height()+MARGIN; // was 10
	}
/*
#ifdef USE_MIXER
	if(m_editorMainMixer->isVisible()) { // mixer
		pky += m_editorMainMixer->height()+MARGIN; // was 10
	}
#endif
	*/
	m_editorPiano->move(x, pky);

	if(m_channelList->numItems() < 1) {
		m_editorChannelList->sizeWidgetHeightToFit();
	}
		


	//m_editorChannelList->setFixedSize(width()-20, channelListHeight);

	//m_editorStreamMixer->move(10, 70 + channelListHeight + 10);
	//m_editorStreamMixer->setFixedWidth(width()-30-m_stackedWidget->width());*/
}