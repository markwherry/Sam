#include "Sam.h"

#include "SamEngine.h"
#include "SamProject.h"
#include "SamProjectCommands.h"
#include "SamRealTimeCommands.h"

#include "SamCommands.h"
#include "SamPreferences.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QPainter>
#include <QUrl>

Sam* Sam::m_instance = 0;

// ------------------------------------------------------------------------------------------------------------------------------------------------------
Sam::Sam(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, m_midiMonitor(0)
#ifdef USE_OUTPUT_MANAGER
	, m_outputManager(0)
#endif
	, m_showLabels(true)
	, m_channelProgramDisplay(false)
{
	//ui.setupUi(this);

	m_instance = this;

	setMinimumSize(1200, 900);

	setAcceptDrops(true);

	m_commandManager = new ACommandManager();

	m_presetManager = new SamPresetManager();

#ifdef USE_LIBRARY
	commandManager()->executeInBackground(new BuildDatabase());
#endif

	/*AList<AString> *files = new AList<AString>;
	files->append(AString("S:\\Sam Instruments\\AIR3 Celli 14\\AIR3 Celli 14 All\\AIR3 Cel14 short 07.sam"));
	commandManager()->executeInBackground(new SamCommandLoadInstruments(files, 1, true));//*/

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(40,40,40)); //30
	setPalette(plt);


	// Init Other Windows
	// ------------------
	m_audioHardware = new UiAudioHardware(0, samEngine->audioManager());
	m_audioHardware->setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	m_audioHardware->setWindowTitle("Configure Audio/MIDI");
	m_audioHardware->setPalette(plt);
	//m_audioHardware->show();

	m_vstPluginManager = new SamGuiVstPluginManager();
	//m_vstPluginManager->show();

	m_copyLayerFromInstrument = new SamGuiCopyLayerFromInstrument(this);

	m_cellAnalyser = new SamGuiCellAnalyser(0);
	//m_cellAnalyser->show();

	m_bounceInstrument = new SamGuiBounceInstrument(0);
	//m_bounceInstrument->show();

	m_moveCopyZones = new SeGuiMoveCopyZones(0);
	//m_moveCopyZones->show();

	m_copyPasteZoneSettings = new SeGuiCopyPasteZoneSettings(0);

	m_toolEditor = new SamGuiCsToolEditor(0);

	m_midiMonitor = new SamGuiMidiMonitor(0);

	m_midiChannelMap = new SamGuiMidiChannelMap(0);

	m_channelPitchBendRemoteSetup = new SamGuiChannelPitchBendRemoteSetup(0);
	//m_channelPitchBendRemoteSetup->show();

	m_commandProgress = new UiCommandProgress();
	connect(this, SIGNAL(commandStarted(int)), this, SLOT(setCommandStart(int)));
	connect(this, SIGNAL(commandStopped(int, QString, int)), this, SLOT(setCommandStop(int, QString, int)));
	connect(this, SIGNAL(commandProgressUpdated(int, float)), this, SLOT(setCommandProgress(int, float)));
	connect(this, SIGNAL(commandProgressLabelUpdated(int, int, QString)), this, SLOT(setCommandProgressLabel(int, int, QString)));

	m_commandManager->setProgressReceiver(this);

	m_articulationVector = new SamGuiArticulationVector(0);
	//m_articulationVector->show();

	// Init recent Projects and Instruments
	for(int i=0; i<5; i++) {
		//if(samPreferences->recentFiles()->hasPreference(AString("project"+AString::number(i))) {
		AString project = samPreferences->recentFiles()->preference(AString("project")+AString::number(i));
		if(project.length() > 0) {
			m_recentProjects.append(project);//insertAt(0, project);
		}
	}

	for(int i=0; i<5; i++) {
		//if(samPreferences->recentFiles()->hasPreference(AString("project"+AString::number(i))) {
		AString instrument = samPreferences->recentFiles()->preference(AString("instrument")+AString::number(i));
		if(instrument.length() > 0) {
			m_recentInstruments.append(instrument);//insertAt(0, instrument);
		}
	}

	m_editorAudioOutputs = new UiEditor(0);
	m_editorAudioOutputs->setSideBarWidth(UiEditor::Right, 150);
	m_editorAudioOutputs->setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	m_editorAudioOutputs->setWindowTitle("Configure Audio Outputs");
	m_editorAudioOutputs->setFixedSize(750,300);
	//m_editorAudioOutputs->show();

	// Init Menus
	// ----------
	initMenus();


	// Init the UI for the main window
	// -------------------------------
	m_view = new SamGuiView(this);
	setCentralWidget(m_view);

	m_textFieldDialog = new SamGuiTextFieldDialog(this);

	


	// Start Animation Timer
	//startTimer(20);

	setWindowTitle("Unsaved Project - Sam");

	connect(this, SIGNAL(editorRefreshed()), this, SLOT(refreshEditor()));


	const int numArts = 3;
	QString artPixFile[numArts] = {
		":/Resources/artQuarterNote36.png",
		":/Resources/art1Note36.png",
		":/Resources/art16Note36.png"
	};

	for(int i = 0; i < numArts; i++) {
		QPixmap *pixmap = new QPixmap(artPixFile[i]);
		m_articulationPixmaps[0].append(pixmap);

		QString invFile = artPixFile[i];
		for(int j=0; j<invFile.length(); j++) {
			if(invFile.at(j) == '.') {
				invFile.insert(j, "i");
				break;
			}
		}

		QPixmap *pixmapInv = new QPixmap(invFile);
		m_articulationPixmaps[1].append(pixmapInv);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
Sam::~Sam()
{
	aLog("Sam::~Sam()");
	delete m_commandManager;
	aLog("EXIT: Sam::~Sam()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
Sam* Sam::instance(void)
{
	return m_instance;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::startCommand(int commandId)
{
	qDebug("Sam::startCommand()");

	emit commandStarted(commandId);	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::stopCommand(int commandId, AString message, int messageType)
{
	qDebug("Sam::stopCommand()");

	emit commandStopped(commandId, QString(message.toChar()), messageType);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setProgress(int commandId, float progress)
{
	//qDebug("Sam::setProgress()");

	emit commandProgressUpdated(commandId, progress);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setProgressLabel(int commandId, int labelId, AString label)
{
	//qDebug("Sam::setProgressLabel(%d, %d, %s)", commandId, labelId, label.toChar());
	QString l = label.toChar();

	emit commandProgressLabelUpdated(commandId, labelId, l);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::openEditor(SamChannel *channel)
{
	qDebug("Sam::openEditor(%d)", channel);

	m_view->m_tabBar->selectTab(1, false); qDebug("oe1");

	m_view->m_stackedWidget->setCurrentWidget(m_view->m_editor); qDebug("oe2");
	m_view->m_editor->resize(m_view->m_stackedWidget->width()-1, m_view->m_stackedWidget->height()-1); qDebug("oe3");
	m_view->m_editor->setChannel(channel); qDebug("oe4");

	m_actionMoveCopyZones->setEnabled(true);
	m_actionCopyPasteZoneSettings->setEnabled(true);
	//m_actionImportSampleFolder->setEnabled(true);
	m_actionAssignSourceToRelease->setEnabled(true);
	m_clearReleases->setEnabled(true);
	m_actionEnableLoops->setEnabled(true);
	m_actionDisableLoops->setEnabled(true);
	m_actionUpdateLoops->setEnabled(true);

	qDebug("oe5");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::closeEditor(void)
{
	m_view->m_tabBar->selectTab(0, false);

	m_view->m_stackedWidget->setCurrentWidget(m_view->m_mainWidget);

	m_view->m_editor->setChannel(0);

	m_copyPasteZoneSettings->hide();

	m_actionMoveCopyZones->setEnabled(false);
	m_actionCopyPasteZoneSettings->setEnabled(false);
	//m_actionImportSampleFolder->setEnabled(false);
	m_actionAssignSourceToRelease->setEnabled(false);
	m_clearReleases->setEnabled(false);
	m_actionEnableLoops->setEnabled(false);
	m_actionDisableLoops->setEnabled(false);
	m_actionUpdateLoops->setEnabled(false);

	m_view->m_mainWidget->updateSelectedChannel();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::refreshEditor(void)
{
	m_view->m_editor->refresh();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::signalEditorRefresh(void)
{
	emit editorRefreshed();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setShowLabels(bool show)
{
	m_showLabels = show;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setCurrentProjectFilename(AString filename)
{
	m_currentProjectFilename = filename.toChar();

	QFileInfo fi(m_currentProjectFilename);
	setWindowTitle(fi.fileName()+" - Sam"); 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::addRecentProject(AString filename)
{
	if(m_recentProjects.size() > 0) {
		for(int i=0; i<m_recentProjects.size(); i++) {
			if(filename == m_recentProjects[i]) {
				if(i==0) {
					// Return if the filename is already the most recent Project
					return;
				} else {
					m_recentProjects.removeAt(i);
				}
			}
		}
	}

	m_recentProjects.insertAt(0, filename);

	if(m_recentProjects.size() > 5) {
		m_recentProjects.removeAt(5);
	}

	updateRecentProjectsMenu();

	for(int i=0; i<5; i++) {
		samPreferences->recentFiles()->setPreference(AString("project")+AString::number(i), m_recentProjects[i]);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::updateRecentProjectsMenu(void)
{
	m_actionRecentProjects[0]->setEnabled(false);
	m_actionRecentProjects[0]->setText("(No Recent Projects)");

	for(int i=0; i<5; i++) {
		if(i < m_recentProjects.size()) {
			m_actionRecentProjects[i]->setVisible(true);
			m_actionRecentProjects[i]->setEnabled(true);
			m_actionRecentProjects[i]->setText(m_recentProjects[i].toChar());
		} else {
			if(i!=0) {
				m_actionRecentProjects[i]->setVisible(false);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::addRecentInstrument(AString filename)
{
	aLog("Sam::addRecentInstrument()");

	if(m_recentInstruments.size() > 0) {
		for(int i=0; i<m_recentInstruments.size(); i++) {
			if(filename == m_recentInstruments[i]) {
				if(i==0) {
					// Return if the filename is already the most recent Instrument
					return;
				} else {
					m_recentInstruments.removeAt(i);
				}
			}
		}
	}

	m_recentInstruments.insertAt(0, filename);

	if(m_recentInstruments.size() > 5) {
		m_recentInstruments.removeAt(5);
	}

	updateRecentInstrumentsMenu();

	for(int i=0; i<5; i++) {
		samPreferences->recentFiles()->setPreference(AString("instrument")+AString::number(i), m_recentInstruments[i]);
	}

	aLog("EXIT: Sam::addRecentInstrument()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::updateRecentInstrumentsMenu(void)
{
	aLog("Sam::updateRecentInstrumentsMenu()");

	m_actionRecentInstruments[0]->setEnabled(false);
	m_actionRecentInstruments[0]->setText("(No Recent Instruments)");

	for(int i=0; i<5; i++) {
		if(i < m_recentInstruments.size()) {
			m_actionRecentInstruments[i]->setVisible(true);
			m_actionRecentInstruments[i]->setEnabled(true);
			m_actionRecentInstruments[i]->setText(m_recentInstruments[i].toChar());
		} else {
			if(i!=0) {
				m_actionRecentInstruments[i]->setVisible(false);
			}
		}
	}

	aLog("EXIT: Sam::updateRecentInstrumentsMenu()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::newProject(void)
{
	if(QMessageBox::question(this, "Sam", "<b>Do you really want to start a new Project?</b><br><br>All settings for the current Project will be cleared.","Yes","No") == 0) {
		commandManager()->executeInBackground(new NewProject());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::openProject(void)
{
	SamProject *project = samEngine->project();
	if(!project) {
		return;
	}

	bool doOpen = false;
	
	if(project->isEmpty()) {
		doOpen = true;
	} else {
		switch(QMessageBox::question(this, "Sam", "<b>Do you really want to open a new Project?</b><br><br>All settings for the current Project will be cleared.","Yes","No")){
			case 0:
				doOpen = true;
				break;
			case 1:
				break;
		}
	}

	if(doOpen) {
		QString file = QFileDialog::getOpenFileName(this,"Choose a Sam Project File (.spr) to open:","","Sam Project Files (*.spr)");
		if(file.size()>0) {
			QByteArray baFile = file.toLatin1();
			commandManager()->executeInBackground(new OpenProject(baFile.data()));
			setCurrentProjectFilename(baFile.data());
			addRecentProject(baFile.data());
		}
	}


}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::openRecentProject(void)
{
	qDebug("Sam::openRecentProject()");

	AString prj = "";

	QAction *action = qobject_cast<QAction*>(sender());
	for(int i=0; i<5; i++) {
		if(action == m_actionRecentProjects[i]) {
			prj = m_recentProjects[i];
			break;
		}
	}

	if(prj.length() < 1 && m_recentProjects.size() > 0) {
		prj = m_recentProjects[0];
	}

	if(prj.length() > 1) {
		commandManager()->executeInBackground(new OpenProject(prj));
		setCurrentProjectFilename(prj);
		addRecentProject(prj);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::openRecentInstrument(void)
{
	qDebug("Sam::openRecentInstrument()");

	AList<AString> *files = new AList<AString>;

	QAction *action = qobject_cast<QAction*>(sender());
	for(int i=0; i<5; i++) {
		if(action == m_actionRecentInstruments[i]) {
			files->append(m_recentInstruments[i]);
			addRecentInstrument(m_recentInstruments[i]);
			break;
		}
	}

	if(files->size() < 1 && m_recentInstruments.size() > 0) {
		files->append(m_recentInstruments[0]);
		addRecentInstrument(m_recentInstruments[0]);
	}
	if(files->size() > 0) {
		commandManager()->executeInBackground(new LoadInstruments(files, 1, true));
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::saveProject(void)
{
	if(m_currentProjectFilename.size() > 0) {
		QByteArray baFile = m_currentProjectFilename.toLatin1();
		commandManager()->executeInBackground(new SaveProject(baFile.data()));
		setCurrentProjectFilename(baFile.data());
		addRecentProject(baFile.data());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::saveProjectAs(void)
{
	QString file = QFileDialog::getSaveFileName(this,"Type in a name for the Sam Project File:", "" ,"Sam Project Files (*.spr)"); //samEngine->project()->path()

	if(file.size()>0) {
		QByteArray baFile = file.toLatin1();
		commandManager()->executeInBackground(new SaveProject(baFile.data()));
		setCurrentProjectFilename(baFile.data());
		addRecentProject(baFile.data());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::openCollection(void)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::saveCollectionAs(void)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::newInstrument(void)
{
	qDebug("Sam::newInstrument()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::newStereoInstrument(void)
{
	qDebug("Sam::newStereoInstrument()");

	SamInstrument *instrument = new SamInstrument(samEngine);
	instrument->create(2, false, true, "");
	samEngine->addInstrument(instrument);
	samEngine->project()->commandManager()->executeInBackground(new CreateChannel(instrument));

	/*AList<SamSet*> setList;

	SamSet *set = new SamSet(0);

	set->setName("Stereo");
	set->setStreams(2);

	setList.append(set);

	commandManager()->execute(new NewInstrument(&setList));*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::newQuadInstrument(void)
{
	qDebug("Sam::newQuadInstrument()");

	SamInstrument *instrument = new SamInstrument(samEngine);
	instrument->create(4, false, true, "");
	samEngine->addInstrument(instrument);
	samEngine->project()->commandManager()->executeInBackground(new CreateChannel(instrument));

	/*AList<SamSet*> setList;

	AString names[2] = { "Front", "Back" };
	for(int i=0; i<2; i++) {
		SamSet *set = new SamSet(0);

		set->setName(names[i]);
		set->setStreams(2);

		setList.append(set);
	}

	commandManager()->execute(new NewInstrument(&setList));*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::newThreeStereoInstrument(void)
{
	qDebug("Sam::newThreeStereoInstrument()");

	SamInstrument *instrument = new SamInstrument(samEngine);
	instrument->create(6, false, true, "");
	samEngine->addInstrument(instrument);
	samEngine->project()->commandManager()->executeInBackground(new CreateChannel(instrument));

	/*AList<SamSet*> setList;

	AString names[3] = { "Close", "Room", "Far" };
	for(int i=0; i<3; i++) {
		SamSet *set = new SamSet(0);

		set->setName(names[i]);
		set->setStreams(2);

		setList.append(set);
	}

	commandManager()->execute(new NewInstrument(&setList));*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::openInstrument(void)
{
	qDebug("Sam::openInstrument()");

	QString path = samPreferences->recentFiles()->preference("instrumentPath").toChar();

	//"S:\\Sam Instruments\\AIR3 Celli 14\\AIR3 Celli 14 All"
	QString file = QFileDialog::getOpenFileName(this,"Choose a Sam Instrument file (.sam) to load:", path);//,"Sam Instrument Files (*.sam)"); //samPreferences->currentPath()

	if(file.size()>0) {
		QFileInfo fi(file);
		QByteArray baPath = fi.path().toLatin1();
		samPreferences->recentFiles()->setPreference("instrumentPath", baPath.data());

		AList<AString> *files = new AList<AString>;
		QByteArray baFile = file.toLatin1();
		files->append(baFile.data());

		commandManager()->executeInBackground(new LoadInstruments(files, 1, true));



		addRecentInstrument(baFile.data());

		//SamInstrumentLoader *loader = new SamInstrumentLoader();
		//loader->setInstrumentsToLoad(fileList, 1, true);
		//csEngine->currentProject()->commandStack()->executeInBackground(loader);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::saveInstrument(void)
{
	qDebug("Sam::saveInstrument()");

	SamInstrument *instrument = 0;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			instrument = samEngine->project()->channel(i)->instrument();
			break;
		}
	}

	if(!instrument) {
		return;
	}

	if(instrument->filename().length() > 0) {
		commandManager()->executeInBackground(new SaveInstrument(instrument, instrument->filename(), false));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::saveInstrumentAs(void)
{
	qDebug("Sam::saveInstrument()");

	SamInstrument *instrument = 0;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			instrument = samEngine->project()->channel(i)->instrument();
			break;
		}
	}

	if(!instrument) {
		return;
	}

	QString path; //samPreferences->recentFiles()->preference("instrumentPath").toChar();
	
	if(instrument->filename().length() > 0) {
		path = instrument->filename().toChar(); 
	}

	QString file = QFileDialog::getSaveFileName(this,"Type in a name for the Sam Instrument File:", path ,"Sam Instrument Files (*.sam)"); //samEngine->project()->path()

	if(file.size()>0) {
		QByteArray baFile = file.toLatin1();
		commandManager()->executeInBackground(new SaveInstrument(instrument, baFile.data(), true));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::saveAcceleratedAs(void)
{
	qDebug("Sam::saveAcceleratedAs()");

	QMessageBox::warning(0, "Sam", "<b>The 'Save Accerlerated Instrument As' command is experimental.</b><br><br>Only use this command for test purposes", "OK");

	SamInstrument *instrument = 0;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			instrument = samEngine->project()->channel(i)->instrument();
			break;
		}
	}

	if(!instrument) {
		return;
	}

	instrument->elementPool()->setAccelerated(true);

	QString path = samPreferences->recentFiles()->preference("instrumentPath").toChar();

	QString file = QFileDialog::getSaveFileName(this,"Type in a name for the Sam Instrument File:", path ,"Sam Instrument Files (*.sam)"); //samEngine->project()->path()

	if(file.size()>0) {
		QByteArray baFile = file.toLatin1();
		commandManager()->executeInBackground(new SaveInstrument(instrument, baFile.data(), true));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::bounceInstrument(void)
{
	qDebug("Sam::bounceInstrument()");

	// Get the instrument we're going to export
    // ----------------------------------------

	if(samEngine->project()->numSelectedChannels() < 1) {
		qDebug("No channel is selected.");
		QMessageBox::critical(this, "Sam", "<b>No Channel is selected.</b><br><br>Please select a Channel to determine which Instrument should be bounced.");
		return;
	}

	if(samEngine->project()->numSelectedChannels() > 1) {
		QMessageBox::critical(this, "Sam", "<b>More than one Channel is selected.</b><br><br>Please select a single Channel to determine which Instrument should be bounced.");
		qDebug(">1 Channel selected.");
		return;
	}

	SamChannel *channel = samEngine->project()->selectedChannel();
	if(channel) {
		SamInstrument *instrument = channel->instrument();

		//m_bounceInstrument->setChannel(channel);
		m_bounceInstrument->show();
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::manageOutputs(void)
{
#ifdef USE_OUTPUT_MANAGER
	if(!m_outputManager) {
		m_outputManager = new SamGuiOutputManager(0);
	}

	m_outputManager->show();
#endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::replaceInstrument(void)
{
	if(samEngine->project()->numSelectedChannels() < 1) {
		qDebug("No channel is selected.");
		QMessageBox::critical(this, "Sam", "<b>No Channel is selected.</b><br><br>Please select a Channel to determine which Instrument should be replaced.");
		return;
	}

	if(samEngine->project()->numSelectedChannels() > 1) {
		QMessageBox::critical(this, "Sam", "<b>More than one Channel is selected.</b><br><br>Please select a single Channel to determine which Instrument should be replaced.");
		//qDebug(">1 Channel selected.");
		return;
	}

	SamChannel *channel = samEngine->project()->selectedChannel();
	if(!channel) return;

	QString path = samPreferences->recentFiles()->preference("instrumentPath").toChar();

	QString file = QFileDialog::getOpenFileName(this,"Choose a Sam Instrument file (.sam) to load:", path);//,"Sam Instrument Files (*.sam)"); //samPreferences->currentPath()

	if(file.size()>0) {
		QFileInfo fi(file);
		QByteArray baPath = fi.path().toLatin1();
		samPreferences->recentFiles()->setPreference("instrumentPath", baPath.data());
		QByteArray baFile = file.toLatin1();
		samEngine->project()->deselectChannels();

		commandManager()->executeInBackground(new ReplaceInstrument(channel->instrument(), baFile.data()));

		addRecentInstrument(baFile.data());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::createMonoOutput(void)
{
	AString name = AString("Output ") + AString::number(samEngine->project()->numAudioHardwareBuses()+1) + AString(" (Mono)");
	samEngine->project()->commandManager()->executeInBackground(new CreateAudioHardwareBus(-1, name, CsSpeakerConfig(CsSpeakerConfig::Mono), -1));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::createStereoOutput(void)
{
	AString name = AString("Output ") + AString::number(samEngine->project()->numAudioHardwareBuses()+1) + AString(" (Stereo)");
	samEngine->project()->commandManager()->executeInBackground(new CreateAudioHardwareBus(-1, name, CsSpeakerConfig(CsSpeakerConfig::Stereo), -1));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::createQuadOutput(void)
{
	AString name = AString("Output ") + AString::number(samEngine->project()->numAudioHardwareBuses()+1) + AString(" (Quad)");
	samEngine->project()->commandManager()->executeInBackground(new CreateAudioHardwareBus(-1, name, CsSpeakerConfig(CsSpeakerConfig::Surround40), -1));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::create50Output(void)
{
	//AString name = AString("Output ") + AString::number(samEngine->project()->numAudioHardwareBuses()+1) + AString(" (5.0)");
	//samEngine->project()->commandManager()->executeInBackground(new CreateAudioHardwareBus(-1, name, CsSpeakerConfig(CsSpeakerConfig::Surround50), -1));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::createStereoBus(void)
{
	AString name = AString("Aux ") + AString::number(samEngine->project()->numAudioBuses()+1);
	samEngine->project()->commandManager()->executeInBackground(new CreateAudioBus(-1, name, CsSpeakerConfig(CsSpeakerConfig::Stereo)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::createQuadBus(void)
{
	AString name = AString("Aux ") + AString::number(samEngine->project()->numAudioBuses()+1);
	samEngine->project()->commandManager()->executeInBackground(new CreateAudioBus(-1, name, CsSpeakerConfig(CsSpeakerConfig::Surround40)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::settingsMidiControllerFloodgate(void)
{
	qDebug("Sam::settingsMidiControllerFloodgate()");
	samEngine->setMidiControllerFloodgate(!samEngine->isMidiControllerFloodgate());
	m_actionMidiControllerFloodgate->setChecked(samEngine->isMidiControllerFloodgate());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::settingsLoadSamplesWithInstrument(void)
{
	qDebug("Sam::settingsLoadSamplesWithInstument() %d", (int)samEngine->isLoadSampleDataWithInstrument());

	samEngine->setLoadSampleDataWithInstrument(!samEngine->isLoadSampleDataWithInstrument());
	//m_actionLoadSamplesWithInstrument->setChecked(samEngine->isLoadSampleDataWithInstrument());

	qDebug("AFTER %d", (int)samEngine->isLoadSampleDataWithInstrument());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::showChannelLabels(void)
{
	qDebug("Sam::showChannelLabels()");

	m_showLabels = !m_showLabels;
	m_actionShowChannelLabels->setChecked(m_showLabels);

	m_view->mainWidget()->channelList()->updateLabels();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::settingsChannelProgramDisplay(void)
{
	m_channelProgramDisplay = !m_channelProgramDisplay;

	m_actionProgramChangeScale->setChecked(m_channelProgramDisplay);

	samPreferences->app()->setPreference("channelProgramDisplay", AString::number((int)m_channelProgramDisplay));

	m_view->mainWidget()->channelList()->updateArticulations();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::deleteChannels(void)
{
	qDebug("Sam::deleteChannels()");

	QList<SamChannel*> channelsToDelete;

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			channelsToDelete.append(samEngine->project()->channel(i));
		}
	}

	for(int i=0; i<channelsToDelete.size(); i++) {
		samEngine->project()->commandManager()->executeInBackground(new DeleteChannel(channelsToDelete[i]));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugToggleEngineTestTone(void)
{
	samEngine->executeRealTimeCommand(new RtcToggleEngineTestTone());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugLoadShortCelli(void)
{
	AList<AString> *files = new AList<AString>;
	files->append("S://Sam Instruments//AIR3 Celli 14//AIR3 Celli 14 All//AIR3 Cel14 short 07.sam");
	commandManager()->executeInBackground(new LoadInstruments(files, 1, true));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugLoadTest(void)
{
	qDebug("Sam::debugLoadTest()");

	for(int i=0; i<1; i++) {
		AList<AString> *files = new AList<AString>;
		files->append("S://Sam Instruments//First Instrument//test.sam");
		commandManager()->executeInBackground(new LoadInstruments(files, 1, true));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugSetPianoValues(void)
{
	qDebug("Sam::debugSetPianoValues()");

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			aLog("Channel %d is selected", i);
			SamArticulation *art = samEngine->project()->channel(i)->version()->articulation(samEngine->project()->channel(i)->currentArticulationIndex());
			for(int j=0; j<art->numLayers(); j++) {
				for(int k=0; k<art->numLayers(); k++) {
					for(int l=0; l<art->layer(k)->numZones(); l++) {
						SamZone *z = art->layer(k)->zone(l);
						z->envelope(0)->setAttack(1);
						z->envelope(0)->setRelease(200);
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugSetFilter(void)
{
	qDebug("Sam::debugSetFilter()");

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			aLog("Channel %d is selected", i);
			SamArticulation *art = samEngine->project()->channel(i)->version()->articulation(samEngine->project()->channel(i)->currentArticulationIndex());
			for(int j=0; j<art->numLayers(); j++) {
				for(int k=0; k<art->numLayers(); k++) {
					for(int l=0; l<art->layer(k)->numZones(); l++) {
						SamZone *z = art->layer(k)->zone(l);
						z->setFilterEnabled(true);
						//z->setCutOffController(1);
						z->setCutOff(4000);
						z->setMinCutOff(100);
						z->setFilterVelocityScaling(true);
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugPerformanceControls()
{
	qDebug("Sam::debugPerformanceControls()");

	SamChannel *channel = samEngine->project()->selectedChannel();
	if(!channel) return;

	qDebug("Articulation Index: %d", channel->currentArticulationIndex());
	channel->performanceControls(channel->currentArticulationIndex())->debug();
	
	for(int i=0; i<channel->numAudioOutputs(); i++) {
		qDebug("Audio Output %d", i);
		for(int j=0; j<SamAudioBus::maxSends; j++) {
			qDebug("  Send #%d", j);
			channel->audioOutputBus(i)->audioSend(j)->debug();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::debugPluginEntries(void)
{
	samEngine->vstPluginManager()->debugPluginEntries();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::copyLayerFromInstrument(void)
{
	qDebug("Sam::copyLayerFromInstrument()");

	if(samEngine->project()->numSelectedChannels() == 1) {
		m_copyLayerFromInstrument->setInstrument(samEngine->project()->selectedChannel()->instrument());
		m_copyLayerFromInstrument->show();
	} else {
		QMessageBox::information(this, "Sam", "You need to select an Instrument (or make sure you only have one Instrument selected) to use this command.");
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::mergeLoadedInstruments(void)
{
	qDebug("Sam::mergeLoadedInstruments()");

	if(samEngine->numInstruments() > 1) {
		commandManager()->executeInBackground(new MergeLoadedInstruments());
	} else {
		QMessageBox::information(this, "Sam", "You need at least two different Instruments loaded for this command to work.");
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::exportArticulationAsInstrument(void)
{
	SamInstrument *instrument = 0;
	SamArticulation *articulation = 0;

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			instrument = samEngine->project()->channel(i)->instrument();
			int index = samEngine->project()->channel(i)->currentArticulationIndex();
			articulation = instrument->version()->articulation(index);
			break;
		}
	}

	if(!instrument) {
		return;
	}

	bool xmlOnly = true;
	int ret = QMessageBox::question(this, "Sam", "<b>Export Articulation</b><br><br>Would you like to export a full instrument (containing samples), or only the .sam file (which reference the existing samples)?", ".sam Only", "Full Instrument");
	if(ret == 1) {
		xmlOnly = false;
	}

	QString path; //samPreferences->recentFiles()->preference("instrumentPath").toChar();
	
	if(articulation->name().length() > 0) {
		path = instrument->filename().toChar(); 
		path = articulation->name().toChar() + QString(".sam");
	}

	QString file = QFileDialog::getSaveFileName(this,"Type in a name for the Sam Instrument File:", path ,"Sam Instrument Files (*.sam)"); //samEngine->project()->path()

	if(file.size()>0) {
		QByteArray baFile = file.toLatin1();
		commandManager()->executeInBackground(new ExportArticulationAsInstrument(instrument, articulation, baFile.data(), xmlOnly));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::exportElementPool(void)
{
	SamInstrument *instrument = 0;
	SamArticulation *articulation = 0;

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->isSelected()) {
			instrument = samEngine->project()->channel(i)->instrument();
			int index = samEngine->project()->channel(i)->currentArticulationIndex();
			articulation = instrument->version()->articulation(index);
			break;
		}
	}

	if(!instrument) {
		return;
	}

	QString path;
	
	if(articulation->name().length() > 0) {
		path = instrument->filename().toChar(); 
		path = articulation->name().toChar();
	}

	QString file = QFileDialog::getSaveFileName(this,"Type in a name for the directory:", path ,""); //samEngine->project()->path()

	if(file.size()>0) {
		QByteArray baFile = file.toLatin1();
		commandManager()->executeInBackground(new ExportElementPool(instrument, articulation, baFile.data()));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::moveCopyZones(void)
{
	m_moveCopyZones->setLayer(m_view->m_editor->layer());
	m_moveCopyZones->show();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::copyPasteZoneSettings(void)
{
	//m_copyPasteZoneSettings->setInstrument(m_view->m_editor->instrument());
	m_copyPasteZoneSettings->show();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::importSampleFolder(void)
{
	QString path = QFileDialog::getExistingDirectory(this, "Select the folder that contains the samples for the instrument:");

	int tokens[8] = { 
		ImportSampleFolder::TkIgnore,
		ImportSampleFolder::TkNoteRoot,
		ImportSampleFolder::TkNoteLo,
		ImportSampleFolder::TkNoteHi,
		ImportSampleFolder::TkVelLo,
		ImportSampleFolder::TkVelHi,
		ImportSampleFolder::TkVariation,
		ImportSampleFolder::TkSpeaker
	};

	commandManager()->executeInBackground(new ImportSampleFolder(m_view->m_editor->layer(), path, 8, tokens, 3));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::assignSourceToRelease(void)
{
	commandManager()->executeInBackground(new AssignSourceToRelease(m_view->m_editor->layer()));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::clearReleases(void)
{
	commandManager()->executeInBackground(new UnassignRelease(m_view->m_editor->layer()));
}

/*void Sam::fixReleaseSets(void)
{

}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::enableLoops(void)
{
	commandManager()->executeInBackground(new EnableLoops(m_view->m_editor->layer(), true));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::disableLoops(void)
{
	commandManager()->executeInBackground(new EnableLoops(m_view->m_editor->layer(), false));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::updateLoops(void)
{
	commandManager()->executeInBackground(new UpdateLoops(m_view->m_editor->layer()));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setCommandStart(int commandId)
{
	m_commandProgress->clear();
	m_commandProgress->show();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setCommandStop(int commandId, QString message, int messageType)
{
	qDebug("setCommandStop(%d, %s, %d)", commandId, qPrintable(message), messageType);
	m_commandProgress->hide();

	if(message.length() > 0) {
		if(messageType == 0) {
			QMessageBox::critical(this, "Sam", message);
		} else if(messageType == 1) {
			QMessageBox::information(this, "Sam", message);
		}
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setCommandProgress(int commandId, float progress)
{
	//qDebug("Sam::setCommandProgress(%d, %f)", commandId, progress);

	m_commandProgress->setProgress(commandId, progress);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::setCommandProgressLabel(int commandId, int labelId, QString label)
{
	//qDebug("Sam::setCommandProgressLabel(%d, %d, %s", commandId, labelId, qPrintable(label));

	m_commandProgress->setProgressLabel(commandId, labelId, label);
}

void Sam::openMidiMonitor(void)
{
	
	m_midiMonitor->show();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::closeEvent(QCloseEvent *ev)
{
	aLog("Sam::closeEvent() %d", windowState());

	if(isMaximized()) {
		aLog("isMaximized()");
		samPreferences->app()->setPreference("maximised", "1");
	} else {
		aLog("!isMaximized()");
		samPreferences->app()->setPreference("maximised", "0");
	}

	ev->accept();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::dragEnterEvent(QDragEnterEvent *ev)
{
	qDebug("Sam::dragEnterEvent()");
	
	qDebug("%s", qPrintable(ev->mimeData()->text()));

	if(ev->mimeData()->hasUrls()) {
		ev->acceptProposedAction();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::dropEvent(QDropEvent *ev)
{
	qDebug("Sam::dropEvent()");

	if (ev->mimeData()->hasUrls()) {
		int numUrls = ev->mimeData()->urls().size();
        qDebug("hasUrls() %d", numUrls); 
		AList<AString> *files = new AList<AString>;
		for(int i=0; i<numUrls; i++){
			QString path = ev->mimeData()->urls()[i].toString();
			path = path.right(path.length()-8);
			qDebug("%s", qPrintable(path));

			QByteArray baFile = path.toLatin1();
			files->append(baFile.data());
			
			//instrumentList.append(path.right(path.length()-8));
			addRecentInstrument(baFile.data());
		}
		/*if(samEngine->memoryUsed() >= 90.0) {
			QMessageBox::critical(this, "Sam", "<b>There isn't enough memory available to load another instrument.</b><br><br>You are currently using at least 90 percent of the computer's available memory. Remove an instrument you may no longer need before trying to load another.", "OK");
			return;
		}*/
        commandManager()->executeInBackground(new LoadInstruments(files, files->size(), true));
     }
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);
	return;
	/*int bgc = 30; //30
	p.setPen(QColor(bgc, bgc, bgc));
	p.setBrush(QColor(bgc, bgc, bgc));
	p.drawRect(0, 0, width()-1, height()-1);*/

	

	//EStyle::paintBackground(&p,this);
	//p.setPen(QColor(70,70,70));
	//p.setBrush(QColor(70,0,0));
	//p.drawRect(0,0,width()-1, height()-1);
	p.fillRect(0,0,width(),height(),QColor(70,70,70));

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

	/*QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, 60));
	linearGradient.setColorAt(0, QColor(255,255,255,80));
	linearGradient.setColorAt(1, QColor(255,255,255,0));

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	p.drawRect(0,0,width(),60);*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void Sam::initMenus(void)
{
	// File Menu
	// ---------
	m_menuFile = menuBar()->addMenu("File");

	//actionCreateInstrument = menuFile->addAction("Create Instrument...");
	//connect(actionCreateInstrument, SIGNAL(triggered()), this, SLOT(createInstrument()));

	m_actionNewProject = m_menuFile->addAction("New Project...");
	m_actionNewProject->setShortcut(QKeySequence("Ctrl+N"));
	connect(m_actionNewProject, SIGNAL(triggered()), this, SLOT(newProject()));

	m_menuFile->addSeparator();

	m_actionOpenProject = m_menuFile->addAction("Open Project...");
	m_actionOpenProject->setShortcut(QKeySequence("Ctrl+O"));
	connect(m_actionOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));

	m_menuRecentProjects = m_menuFile->addMenu("Recent Projects");
	for(int i=0; i<5; i++) {
		m_actionRecentProjects[i] = m_menuRecentProjects->addAction("");
		connect(m_actionRecentProjects[i], SIGNAL(triggered()), this, SLOT(openRecentProject()));
	}
	//m_actionRecentProjects[0]->setShortcut(QKeySequence("Ctrl+0"));
	//m_actionRecentProjects[0]->setShortcut(QKeySequence(Qt::ControlModifier, Qt::Key_0));
	updateRecentProjectsMenu();

	m_menuFile->addSeparator();

	m_actionSaveProject = m_menuFile->addAction("Save Project");
	m_actionSaveProject->setShortcut(QKeySequence("Ctrl+S"));
	connect(m_actionSaveProject, SIGNAL(triggered()), this, SLOT(saveProject()));

	m_actionSaveProjectAs = m_menuFile->addAction("Save Project As...");
	connect(m_actionSaveProjectAs, SIGNAL(triggered()), this, SLOT(saveProjectAs()));

	m_menuFile->addSeparator();

	/*m_actionOpenCollection = m_menuFile->addAction("Open Collection...");
	connect(m_actionOpenCollection, SIGNAL(triggered()), this, SLOT(openCollection()));

	m_actionSaveCollectionAs = m_menuFile->addAction("Save Collection As...");
	connect(m_actionSaveCollectionAs, SIGNAL(triggered()), this, SLOT(saveCollectionAs()));*/

	m_menuFile->addSeparator();

	m_menuNewInstrument = m_menuFile->addMenu("New Instrument");
	m_actionNewStereoInstrument = m_menuNewInstrument->addAction("New Stereo Instrument");
	connect(m_actionNewStereoInstrument, SIGNAL(triggered()), this, SLOT(newStereoInstrument()));
	m_actionNewQuadInstrument = m_menuNewInstrument->addAction("New Quad Instrument");
	connect(m_actionNewQuadInstrument, SIGNAL(triggered()), this, SLOT(newQuadInstrument()));
	m_actionNewThreeStereoInstrument = m_menuNewInstrument->addAction("New Three-Stereo Instrument");
	connect(m_actionNewThreeStereoInstrument, SIGNAL(triggered()), this, SLOT(newThreeStereoInstrument()));
	
	//m_actionNewInstrument->setShortcut(QKeySequence("Shift+N"));
	//connect(m_actionNewInstrument, SIGNAL(triggered()), this, SLOT(newInstrument()));

	m_menuFile->addSeparator();

	m_actionOpenInstrument = m_menuFile->addAction("Open Instrument...");
	m_actionOpenInstrument->setShortcut(QKeySequence("Shift+O"));
	connect(m_actionOpenInstrument, SIGNAL(triggered()), this, SLOT(openInstrument()));

	m_menuRecentInstruments = m_menuFile->addMenu("Recent Instruments");
	for(int i=0; i<5; i++) {
		m_actionRecentInstruments[i] = m_menuRecentInstruments->addAction("");
		connect(m_actionRecentInstruments[i], SIGNAL(triggered()), this, SLOT(openRecentInstrument()));

	}
	updateRecentInstrumentsMenu();

	m_actionReplaceInstrument = m_menuFile->addAction("Replace Instrument...");
	m_actionReplaceInstrument->setShortcut(QKeySequence("Shift+R"));
	connect(m_actionReplaceInstrument, SIGNAL(triggered()), this, SLOT(replaceInstrument()));

	m_menuFile->addSeparator();

	m_actionSaveInstrument = m_menuFile->addAction("Save Instrument");
	m_actionSaveInstrument->setShortcut(QKeySequence("Shift+S"));
	connect(m_actionSaveInstrument, SIGNAL(triggered()), this, SLOT(saveInstrument()));
	//m_actionSaveInstrument->setEnabled(false);

	m_actionSaveInstrumentAs = m_menuFile->addAction("Save Instrument As...");
	connect(m_actionSaveInstrumentAs, SIGNAL(triggered()), this, SLOT(saveInstrumentAs()));

//	m_actionSaveAcceleratedAs = m_menuFile->addAction("Save Accelerated Instrument As...");
//	connect(m_actionSaveAcceleratedAs, SIGNAL(triggered()), this, SLOT(saveAcceleratedAs()));

	m_menuFile->addSeparator();

	m_actionBounceInstrument = m_menuFile->addAction("Bounce Instrument...");
	m_actionBounceInstrument->setShortcut(QKeySequence("Ctrl+B"));
	connect(m_actionBounceInstrument, SIGNAL(triggered()), this, SLOT(bounceInstrument()));

	m_menuFile->addSeparator();

	m_actionExit = m_menuFile->addAction("Exit");
	connect(m_actionExit, SIGNAL(triggered()), this, SLOT(close()));


	// Edit Menu
	// ---------
	m_menuEdit = menuBar()->addMenu("Edit");

	//actionEditUndo = menuEdit->addAction("Undo");
	//connect(actionFileNew, SIGNAL(triggered()), this, SLOT(projectNew()));

	//actionEditRedo = menuEdit->addAction("Redo");

	//menuEdit->addSeparator();

	//actionEditDelete = menuEdit->addAction("Delete");

	m_actionCopyLayerFromInstrument = m_menuEdit->addAction("Copy Layer from Instrument...");
	connect(m_actionCopyLayerFromInstrument, SIGNAL(triggered()), this, SLOT(copyLayerFromInstrument()));

	m_actionMergeLoadedInstruments = m_menuEdit->addAction("Merge Loaded Instruments");
	connect(m_actionMergeLoadedInstruments, SIGNAL(triggered()), this, SLOT(mergeLoadedInstruments()));

	m_menuEdit->addSeparator();

	m_actionExportArticulationAsInstrument = m_menuEdit->addAction("Export Articulation as Instrument...");
	connect(m_actionExportArticulationAsInstrument, SIGNAL(triggered()), this, SLOT(exportArticulationAsInstrument()));

	m_actionExportElementPool = m_menuEdit->addAction("Export Element Pool...");
	connect(m_actionExportElementPool, SIGNAL(triggered()), this, SLOT(exportElementPool()));

	m_menuEdit->addSeparator();

	m_actionMoveCopyZones = m_menuEdit->addAction("Move / Copy Zones...");
	connect(m_actionMoveCopyZones, SIGNAL(triggered()), this, SLOT(moveCopyZones()));
	m_actionMoveCopyZones->setEnabled(false);

	m_actionCopyPasteZoneSettings = m_menuEdit->addAction("Copy / Paste Zone Settings...");
	connect(m_actionCopyPasteZoneSettings, SIGNAL(triggered()), this, SLOT(copyPasteZoneSettings()));
	m_actionCopyPasteZoneSettings->setEnabled(false);

	m_menuEdit->addSeparator();

	//m_actionImportSampleFolder = m_menuEdit->addAction("Import Sample Folder to Layer... (Three-Stereo, Round-Robin)");
	//connect(m_actionImportSampleFolder, SIGNAL(triggered()), this, SLOT(importSampleFolder()));
	//m_actionImportSampleFolder->setEnabled(false);
	//
	//m_menuEdit->addSeparator();

	m_actionAssignSourceToRelease = m_menuEdit->addAction("Assign Source Elements to Releases");
	connect(m_actionAssignSourceToRelease, SIGNAL(triggered()), this, SLOT(assignSourceToRelease()));
	
	m_clearReleases = m_menuEdit->addAction("Clear Releases");
	connect(m_clearReleases, SIGNAL(triggered()), this, SLOT(clearReleases()));

	//m_fixReleaseSets = m_menuEdit->addAction("Fix Release Sets");
	//connect(m_

	m_actionAssignSourceToRelease->setEnabled(false);
	m_clearReleases->setEnabled(false);

	m_menuEdit->addSeparator();

	m_actionEnableLoops = m_menuEdit->addAction("Enable Loops");
	connect(m_actionEnableLoops, SIGNAL(triggered()), this, SLOT(enableLoops()));

	m_actionDisableLoops = m_menuEdit->addAction("Disable Loops");
	connect(m_actionDisableLoops, SIGNAL(triggered()), this, SLOT(disableLoops()));

	m_actionUpdateLoops = m_menuEdit->addAction("Update Loops");
	connect(m_actionUpdateLoops, SIGNAL(triggered()), this, SLOT(updateLoops()));

	m_actionEnableLoops->setEnabled(false);
	m_actionDisableLoops->setEnabled(false);



	// Project Menu
	// ------------
	m_menuProject = menuBar()->addMenu("Project");

#ifdef USE_OUTPUT_MANAGER
	m_actionManageAudioOutputs = m_menuProject->addAction("Outputs Manager...");
	m_actionManageAudioOutputs->setShortcut(QKeySequence("Ctrl+M"));
	connect(m_actionManageAudioOutputs, SIGNAL(triggered()), this, SLOT(manageOutputs()));

	m_menuProject->addSeparator();
#endif
	//

	//m_actionConfigureAudioOutputs = m_menuProject->addAction("Configure Audio Outputs...");
	//connect(m_actionConfigureAudioOutputs, SIGNAL(triggered()), m_editorAudioOutputs, SLOT(show()));

	//m_menuProject->addSeparator();

	m_actionCreateMonoOutput = m_menuProject->addAction("Create Mono Output");
	connect(m_actionCreateMonoOutput, SIGNAL(triggered()), this, SLOT(createMonoOutput()));

	m_actionCreateStereoOutput = m_menuProject->addAction("Create Stereo Output");
	connect(m_actionCreateStereoOutput, SIGNAL(triggered()), this, SLOT(createStereoOutput()));

	m_actionCreateQuadOutput = m_menuProject->addAction("Create Quad Output");
	connect(m_actionCreateQuadOutput, SIGNAL(triggered()), this, SLOT(createQuadOutput()));

	m_actionCreate50Output = m_menuProject->addAction("Create 5.0 Output");
	connect(m_actionCreate50Output, SIGNAL(triggered()), this, SLOT(create50Output()));

	/*m_menuProject->addSeparator();

	m_actionCreateStereoBus = m_menuProject->addAction("Create Stereo Aux");
	connect(m_actionCreateStereoBus, SIGNAL(triggered()), this, SLOT(createStereoBus()));

	m_actionCreateQuadBus = m_menuProject->addAction("Create Quad Aux");
	connect(m_actionCreateQuadBus, SIGNAL(triggered()), this, SLOT(createQuadBus()));*/


	// Settings Menu
	// -------------
	m_menuSettings = menuBar()->addMenu("Settings");

	m_actionConfigureAudioHardware = m_menuSettings->addAction("Configure Audio/MIDI...");
	connect(m_actionConfigureAudioHardware, SIGNAL(triggered()), m_audioHardware, SLOT(show()));

	m_actionVstPluginManager = m_menuSettings->addAction("VST Plug-in Manager...");
	connect(m_actionVstPluginManager, SIGNAL(triggered()), m_vstPluginManager, SLOT(show()));

	m_menuSettings->addSeparator();

	m_actionMidiControllerFloodgate = m_menuSettings->addAction("MIDI Controller Floodgate");
	m_actionMidiControllerFloodgate->setCheckable(true);
	m_actionMidiControllerFloodgate->setChecked(samEngine->isMidiControllerFloodgate());
	connect(m_actionMidiControllerFloodgate, SIGNAL(triggered()), this, SLOT(settingsMidiControllerFloodgate()));

	/*m_menuSettings->addSeparator();

	m_actionLoadSamplesWithInstrument = m_menuSettings->addAction("Load Samples with Instrument");
	m_actionLoadSamplesWithInstrument->setCheckable(true);
	m_actionLoadSamplesWithInstrument->setChecked(samEngine->isLoadSampleDataWithInstrument());
	connect(m_actionLoadSamplesWithInstrument, SIGNAL(triggered()), this, SLOT(settingsLoadSamplesWithInstrument()));*/

	m_menuSettings->addSeparator();
	
	m_actionShowChannelLabels = m_menuSettings->addAction("Show Channel Labels");
	m_actionShowChannelLabels->setShortcut(QKeySequence("L"));
	m_actionShowChannelLabels->setCheckable(true);
	m_actionShowChannelLabels->setChecked(m_showLabels);
	connect(m_actionShowChannelLabels, SIGNAL(triggered()), this, SLOT(showChannelLabels()));

	m_actionProgramChangeScale = m_menuSettings->addAction("Channel Program Displays as 1-128");
	m_actionProgramChangeScale->setCheckable(true);
	m_channelProgramDisplay = (bool)samPreferences->app()->preference("channelProgramDisplay", "0").toInt();
	m_actionProgramChangeScale->setChecked(m_channelProgramDisplay);
	connect(m_actionProgramChangeScale, SIGNAL(triggered()), this, SLOT(settingsChannelProgramDisplay()));

	m_menuSettings->addSeparator();

	m_actionChannelPitchBendRemoteSetup = m_menuSettings->addAction("Channel Pitch Bend Remote Setup...");
	connect(m_actionChannelPitchBendRemoteSetup, SIGNAL(triggered()), m_channelPitchBendRemoteSetup, SLOT(show()));

	// Tools Menu
	// ----------
	m_menuTools = menuBar()->addMenu("Tools");

	m_actionCellAnalyser = m_menuTools->addAction("Cell Analyser...");
	connect(m_actionCellAnalyser, SIGNAL(triggered()), m_cellAnalyser, SLOT(show()));

	m_actionMidiMonitor = m_menuTools->addAction("MIDI Input Monitor...");
	connect(m_actionMidiMonitor, SIGNAL(triggered()), this, SLOT(openMidiMonitor()));

	m_actionMidiChannelMap = m_menuTools->addAction("MIDI Channel Usage Map...");
	connect(m_actionMidiChannelMap, SIGNAL(triggered()), m_midiChannelMap, SLOT(show()));


	// Debug Menu
	// ----------
	m_menuDebug = menuBar()->addMenu("Debug");

	m_actionToggleEngineTestTone = m_menuDebug->addAction("Toggle Engine Test Tone");
	connect(m_actionToggleEngineTestTone, SIGNAL(triggered()), this, SLOT(debugToggleEngineTestTone()));

	m_actionDebugPerformanceControls = m_menuDebug->addAction("Debug Performance Controls");
	connect(m_actionDebugPerformanceControls, SIGNAL(triggered()), this, SLOT(debugPerformanceControls()));

	m_actionDebugPluginEntries = m_menuDebug->addAction("Debug Plug-in Entries");
	connect(m_actionDebugPluginEntries, SIGNAL(triggered()), this, SLOT(debugPluginEntries()));

	/*m_actionLoadShortCelli = m_menuDebug->addAction("Load Short Celli");
	connect(m_actionLoadShortCelli, SIGNAL(triggered()), this, SLOT(debugLoadShortCelli()));

	m_actionLoadTest = m_menuDebug->addAction("Load Test");
	connect(m_actionLoadTest, SIGNAL(triggered()), this, SLOT(debugLoadTest()));

	m_actionSetPianoValues = m_menuDebug->addAction("Set Piano Values");
	connect(m_actionSetPianoValues, SIGNAL(triggered()), this, SLOT(debugSetPianoValues()));

	m_actionSetFilter = m_menuDebug->addAction("Set Filter");
	connect(m_actionSetFilter, SIGNAL(triggered()), this, SLOT(debugSetFilter())); //*/

	//actionDebugCreateChannel = menuDebug->addAction("Create Channel");
	//connect(actionDebugCreateChannel, SIGNAL(triggered()), this, SLOT(debugCreateChannel()));

	//menuDebug->addSeparator();


	
	
}