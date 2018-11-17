#include "SamGuiEditor.h"

#include "Sam.h"
#include "SamEngine.h"

#include <QMessageBox>

SamGuiEditor* SamGuiEditor::m_instance = 0;

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiEditor::SamGuiEditor(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
	, m_instrument(0)
	, m_version(0)
	, m_articulation(0)
	, m_layer(0)
	, m_timerId(0)
{
	//ui.setupUi(this);

	m_instance = this;

	m_commandManager = new ACommandManager();

	initToolbar();

	m_layerEditor = new UiEditor(this);
	m_zoneEditor = new SeGuiZoneEditor(0);
	m_layerEditor->setWidget(m_zoneEditor);

#ifdef USE_EXTENDED_EDITOR
	m_extendedEditor = new UiEditor(this);
	m_stackedWidget = new QStackedWidget(0);
	m_extendedEditor->setWidget(m_stackedWidget);
	m_sampleEditor = new SeGuiSampleEditor(0);
	m_stackedWidget->addWidget(m_sampleEditor);
#endif

	m_tpcInstrument = new UiToolPanelContainer(this, UiToolPanelContainer::Left);

	// Instrument / Version
	// ------------------------------------------------------------------------------------------------------------------------------------------------------
	m_versionProperties = new SeGuiVersionProperties(0);
	m_tpcInstrument->toolPanel()->addSection("Instrument / Version", m_versionProperties);
	

	// Articulations
	// ------------------------------------------------------------------------------------------------------------------------------------------------------
	m_articulationProperties = new SeGuiArticulationProperties(0);
	m_tpcInstrument->toolPanel()->addSection("Articulation", m_articulationProperties);
	

	// Parameters
	// ------------------------------------------------------------------------------------------------------------------------------------------------------
	m_parameters = new SeGuiParameters(0);
	m_tpcInstrument->toolPanel()->addSection("Parameters", m_parameters);
	
	actionParametersAdd = new QAction("Add New Parameter", this);
	connect(actionParametersAdd, SIGNAL(triggered()), m_parameters, SLOT(addParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionParametersAdd);

	actionParametersRemove = new QAction("Remove Selected Parameter", this);
	connect(actionParametersRemove, SIGNAL(triggered()), m_parameters, SLOT(removeParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionParametersRemove);

	m_tpcInstrument->toolPanel()->addSeparatorToMenu(2);

	actionCreateFilterCutOffParameter = new QAction("Create Filter Cut-Off Parameter in Current Articulation", this);
	connect(actionCreateFilterCutOffParameter, SIGNAL(triggered()), m_parameters, SLOT(createFilterCutOffParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateFilterCutOffParameter);

	actionCreateMinimumVelocityParameter = new QAction("Create Minimum Velocity Parameter in Current Articulation", this);
	connect(actionCreateMinimumVelocityParameter, SIGNAL(triggered()), m_parameters, SLOT(createMinimumVelocityParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateMinimumVelocityParameter);

	actionCreateSampleStartParameter = new QAction("Create Sample Start Parameter in Current Articulation", this);
	connect(actionCreateSampleStartParameter, SIGNAL(triggered()), m_parameters, SLOT(createSampleStartParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateSampleStartParameter);

	actionCreateAttackParameter = new QAction("Create Envelope Parameters in Current Articulation", this);
	connect(actionCreateAttackParameter, SIGNAL(triggered()), m_parameters, SLOT(createAttackParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateAttackParameter);

	actionCreateReleaseEnvelopeParameters = new QAction("Create Release Envelope Parameters in Current Articulation", this);
	connect(actionCreateReleaseEnvelopeParameters, SIGNAL(triggered()), m_parameters, SLOT(createReleaseEnvelopeParameters()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateReleaseEnvelopeParameters);

	actionCreateCellSampleStartAdjustParameter = new QAction("Create Cell Sample Start Adjust Parameter", this);
	connect(actionCreateCellSampleStartAdjustParameter, SIGNAL(triggered()), m_parameters, SLOT(createCellSampleStartAdjustParameter()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateCellSampleStartAdjustParameter);

	actionCreateLayerSwitchParameters = new QAction("Create Layer Switch Parameters", this);
	connect(actionCreateLayerSwitchParameters, SIGNAL(triggered()), m_parameters, SLOT(createLayerSwitchParameters()));
	m_tpcInstrument->toolPanel()->addActionToMenu(2, actionCreateLayerSwitchParameters);//*/

	// Description
	// ------------------------------------------------------------------------------------------------------------------------------------------------------
	m_descriptionProperties = new SeGuiDescriptionProperties(0);
	m_tpcInstrument->toolPanel()->addSection("Description", m_descriptionProperties);

	/*tpContainer->toolPanel()->addSection("Version", versionEditor);
	tpContainer->toolPanel()->addSection("Outputs", outputsEditor);
	tpContainer->toolPanel()->addSection("Articulation", articulationEditor);
	tpContainer->toolPanel()->addSection("Parameters", parametersEditor);
	tpContainer->toolPanel()->addSection("Description", descriptionEditor);*/
	//m_tpcInstrument->hide();

	m_cellEditor = new UiEditor(this);
	m_cellProperties = new SeGuiCellProperties(0);
	m_cellEditor->setWidget(m_cellProperties);

	m_tpcLayerProperties = new UiToolPanelContainer(this, UiToolPanelContainer::Right);
	QString layerPropertiesName[5] = { "General", "Scope", "Downbeat Offset", "Interval Selection", "Time from Last Note Off" };
	for(int i=0; i<5; i++) {
		m_layerProperties[i] = new SeGuiLayerProperties(0, i);//, m_editorFaderController);
		m_tpcLayerProperties->toolPanel()->addSection(layerPropertiesName[i], m_layerProperties[i]);
	}

	m_tpcZoneProperties = new UiToolPanelContainer(this, UiToolPanelContainer::Right);
	//m_tpcProperties->hide();
	QString zoneEditorName[11] = { "0 Zones Selected", "Levels and Outputs", "Pitch", "Velocity", "Triggering", "Filter", "Envelope 1 (Amplifier)", "Envelope 2 (Release)", "Envelope 3 (Filter)", "Tempo Release Modulation", "Crossfade" };
	for(int i=0; i<11; i++) {
		m_zoneProperties[i] = new SeGuiZoneProperties(0, i);//m_editorFaderController, i);
		m_tpcZoneProperties->toolPanel()->addSection(zoneEditorName[i], m_zoneProperties[i]);

		//connect(zoneEditor[i], SIGNAL(updateZoneDisplay(SamZone*)), m_zoneViewer->zoneDisplay(), SLOT(setZoneXywh(SamZone*)));
		//connect(m_zoneViewer->zoneDisplay(), SIGNAL(zoneSelected(SamZone*,int)), zoneEditor[i], SLOT(setZone(SamZone*,int)));
	}

	m_tabBar = new UiTabBar(this);
	m_tabBar->addTab("LAYER");
	m_tabBar->addTab("ZONE");
	m_tabBar->addTab("CELL");
	m_tabBar->selectTab(1);
	m_tabBar->setCeiling(8);
	m_tabBar->setFont(QFont("Calibri", 12));
	m_tabBar->setCeiling(10);
	setPropertiesTab(1);
	connect(m_tabBar, SIGNAL(tabSelected(int)), this, SLOT(setPropertiesTab(int)));

	samEngine->registerListener(this, SamEngine::ContextZone);
	samEngine->registerListener(this, SamEngine::ContextLayer);
	samEngine->registerListener(this, SamEngine::ContextChannel);

	connect(this, SIGNAL(zoneSelected(SamZone*)), this, SLOT(updateZoneSelection(SamZone*)));

	connect(m_layerEditor, SIGNAL(tabSelected(int)), this, SLOT(setLayer(int)));
	connect(this, SIGNAL(layerNameChanged()), this, SLOT(updateLayerTabs()));
	connect(this, SIGNAL(layerScopeChanged()), m_zoneEditor, SLOT(updateZoneDisplay()));
	connect(this, SIGNAL(layerZonesChanged()), m_zoneEditor->zoneDisplay(), SLOT(rebuildZones()));

	connect(this, SIGNAL(zoneVelocityPitchChanged()), m_zoneEditor->zoneDisplay(), SLOT(updateZoneSizes()));

	connect(this, SIGNAL(channelLayerChanged()), this, SLOT(updateLayerTabs()));

	m_scCloseEditor = new QShortcut(QKeySequence(Qt::Key_E), this);
	connect(m_scCloseEditor, SIGNAL(activated()), samGui, SLOT(closeEditor()));

	m_scCloseEditorEsc = new QShortcut(QKeySequence(Qt::Key_Escape), this);
	connect(m_scCloseEditorEsc, SIGNAL(activated()), samGui, SLOT(closeEditor()));

	m_scRemoveZones = new QShortcut(QKeySequence(Qt::Key_Backspace), this);
	connect(m_scRemoveZones, SIGNAL(activated()), this, SLOT(removeZones()));

	connect(this, SIGNAL(articulationChanged()), this, SLOT(refresh()));
}

SamGuiEditor::~SamGuiEditor()
{
	delete m_commandManager;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiEditor* SamGuiEditor::instance(void)
{
	return m_instance;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::onState(AStateEvent *ev)
{
	//qDebug("SamGuiEditor::onState()");

	if(ev->context() == SamEngine::ContextZone) {
		switch(ev->item()) {
			case SamZone::ItemSelected:
				//qDebug("SamZone::ItemSelected:");
				emit zoneSelected((SamZone*)ev->updatedObject());
				break;
			case SamZone::ItemVelocity:
				//qDebug("SamZone::ItemVelocity:");
				emit zoneVelocityPitchChanged();
				break;
			case SamZone::ItemPitch:
				//qDebug("SamZone::ItemVelocity:");
				emit zoneVelocityPitchChanged();
				break;
		}
	} else if(ev->context() == SamEngine::ContextLayer) {
		switch(ev->item()) {
			case SamLayer::ItemName:
				emit layerNameChanged();
				break;
			case SamLayer::ItemScope:
				emit layerScopeChanged();
				break;
			case SamLayer::ItemZones:
				emit layerZonesChanged();
				break;

		}
	} else if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemLayer:
				if(ev->updatedObject() == m_channel) {
					emit channelLayerChanged();
				}
				break;
			case SamChannel::ItemArticulation:
				qDebug("SamChannel::ItemArticulation:");
				if(ev->updatedObject() == m_channel) {
					qDebug("->HERE");
					emit articulationChanged();
				}
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::setChannel(SamChannel *channel)
{
	qDebug("SamGuiEditor::setChannel()");

	if(channel) {
		m_channel = channel; qDebug("1");
		m_instrument = m_channel->instrument(); qDebug("2");
		m_version = m_channel->version(); qDebug("3");
		m_articulation = m_version->articulation(m_channel->currentArticulationIndex()); qDebug("4");
		m_layer = m_articulation->layer(0); qDebug("5");

		updateLayerTabs();

		m_versionProperties->setVersion(m_version, m_channel);
		m_articulationProperties->setVersion(m_version, m_articulation);
		m_parameters->setArticulation(m_articulation);
		m_descriptionProperties->setInstrument(m_instrument);


		m_zoneEditor->setLayer(m_layer, m_channel);
		for(int i=0; i<5; i++) {
			m_layerProperties[i]->setLayer(m_layer);
		}

		m_tpcInstrument->toolPanel()->setSectionTitle(0, m_version->name().toChar());

	} else {
		qDebug("channel is NULL");
		
		updateZoneSelection(0);
		m_zoneEditor->setLayer(0, 0);

		m_channel = 0;
		m_instrument = 0;
		m_version = 0;
		m_articulation = 0;
		m_layer = 0;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::refresh(void)
{
	qDebug("SamGuiEditor::refresh()");

	setChannel(m_channel);
}

void SamGuiEditor::toggleLeftPanel(void)
{
	m_tpcInstrument->setVisible(!m_tpcInstrument->isVisible());
	resizeEvent(0);
}

void SamGuiEditor::toggleRightPanel(void)
{
	m_tpcZoneProperties->setVisible(!m_tpcZoneProperties->isVisible());
	m_cellEditor->setVisible(m_tpcZoneProperties->isVisible());
	m_tpcLayerProperties->setVisible(m_tpcZoneProperties->isVisible());
	m_tabBar->setVisible(m_tpcZoneProperties->isVisible());
	resizeEvent(0);
}

void SamGuiEditor::toggleBottomPanel(void)
{
#ifdef USE_EXTENDED_EDITOR

#endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::updateZoneSelection(SamZone *zone)
{
	m_zoneEditor->zoneDisplay()->updateSelection();
	
	int numZonesSelected = 0;
	if(m_layer) {
		numZonesSelected = m_layer->numSelectedZones();
	}

	
	for(int i=0; i<11; i++) {
		m_zoneProperties[i]->setZone(zone, numZonesSelected);
	}

	m_cellProperties->setZone(zone, numZonesSelected);

	if(numZonesSelected != 1) {
		m_tpcZoneProperties->toolPanel()->setSectionTitle(0, QString(QString::number(numZonesSelected)+" Zones Selected"));
	} else {
		m_tpcZoneProperties->toolPanel()->setSectionTitle(0, QString(QString::number(numZonesSelected)+" Zone Selected"));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::setLayer(int id)
{
	qDebug("SamGuiEditor::setLayer(%d)", id);

	if(id > -1) { qDebug("1");
		if(m_channel) {
			SamLayer *layer = m_articulation->layer(id);

			m_zoneEditor->zoneDisplay()->clearSelection();

			if(layer) {
				//m_layerEditor->setArticulation(articulation, 0, m_channel);
				//m_zoneViewer->setLayer(layer, m_channel);
				for(int i=0; i<5; i++) {
					m_layerProperties[i]->setLayer(layer);
				}
				m_zoneEditor->setLayer(layer, m_channel);
				m_layer = layer;

				
			}
				
			
		}
	} qDebug("10");
}

void SamGuiEditor::setPropertiesTab(int id)
{
	m_tpcZoneProperties->hide();
	m_cellEditor->hide();
	m_tpcLayerProperties->hide();

	if(id == 0) {
		m_tpcLayerProperties->show();
	} else if(id == 1) {
		m_tpcZoneProperties->show();
	} else {
		m_cellEditor->show();
		//m_cellEditor->resizeEvent(0);
	}
}

void SamGuiEditor::updateLayerTabs(void)
{
	int selectedTab = m_layerEditor->selectedTab(); // What if this runs for the first time?

	m_layerEditor->clearTabs();
	for(int i=0; i<m_articulation->numLayers(); i++) {
		m_layerEditor->addTab(m_articulation->layer(i)->name().toChar());
	}

	if(m_articulation->numLayers() > 1) {
		for(int i=0; i<m_articulation->numLayers(); i++) {
			switch(m_channel->layerSelectMode(m_channel->currentArticulationIndex())) {
				case SamArticulation::SingleLayer:
					if(m_channel->currentLayerIndex(m_channel->currentArticulationIndex()) == i) {
						m_layerEditor->tab(i)->setUnderline(true);
						m_layerEditor->tab(i)->setUnderlineColor(QColor(0,255,0,125));
					}
					break;
				case SamArticulation::CycleLayers:
					m_layerEditor->tab(i)->setUnderline(true);
					m_layerEditor->tab(i)->setUnderlineColor(QColor(255,255,0,125));
					break;
				case SamArticulation::AllLayers:
				case SamArticulation::Legato:
					m_layerEditor->tab(i)->setUnderline(true);
					m_layerEditor->tab(i)->setUnderlineColor(QColor(0,255,0,125));
					break;
			}
		}
	}

	m_layerEditor->setSelectedTab(selectedTab);

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::removeZones(void)
{
	if(QMessageBox::question(this, "Sam", "<b>Do you really want to remove the selected Zones?</b><br><br>This operation cannot be undone.","Yes","No") == 1) {
		return;
	}
	if(m_layer) {

		int numZonesSelected = m_layer->numSelectedZones();

		for(int i = 0; i < numZonesSelected; i++) {
			for(int j = 0; j < m_layer->numZones(); j++) {
				if(m_layer->zone(j)->isSelected()) {
					m_layer->removeZone(m_layer->zone(j), true);
					break;
				}
			}
		}

		/*for(int i=0; i<m_layer->numZones(); i++) {
			if(m_layer->zone(i)->isSelected()) {
				m_layer->removeZone(m_layer->zone(i), true);
			}
		}*/
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::resizeEvent(QResizeEvent *ev)
{
	int y = m_toolbar->height()+MARGIN;

	m_tpcInstrument->move(MARGIN, y);
	m_tpcInstrument->setFixedHeight(height()-1-m_toolbar->height()-(MARGIN*2));

	m_tpcZoneProperties->move(width()-1-m_tpcZoneProperties->width()-MARGIN, y);
	m_tpcZoneProperties->setFixedHeight(height()-1-m_toolbar->height()-(MARGIN*2));

	m_tpcLayerProperties->move(width()-1-m_tpcZoneProperties->width()-MARGIN, y);
	m_tpcLayerProperties->setFixedHeight(height()-1-m_toolbar->height()-(MARGIN*2));

	m_cellEditor->move(width()-1-MARGIN-m_tpcZoneProperties->width(), MARGIN + m_toolbar->height());
	m_cellEditor->setFixedSize(300+12+8+4, height()-(MARGIN*2)-m_toolbar->height());

	int layerX = MARGIN;
	int layerW = width()-1-(MARGIN*2);
	if(m_tpcInstrument->isVisible()) {
		layerX += (MARGIN + m_tpcInstrument->width());
		layerW -= (MARGIN + m_tpcInstrument->width());
	}

	if(m_tpcZoneProperties->isVisible()) {
		layerW -= (MARGIN+m_tpcZoneProperties->width());
	}

#ifdef USE_EXTENDED_EDITOR
	m_extendedEditor->move(layerX, height()-MARGIN-200-1);
	m_extendedEditor->setFixedSize(layerW, 200);
#endif

	m_layerEditor->move(layerX, y);
#ifdef USE_EXTENDED_EDITOR
	m_layerEditor->setFixedSize(layerW, height()-1-m_toolbar->height()-(MARGIN*2)-(m_extendedEditor->height()+MARGIN));
#else
	m_layerEditor->setFixedSize(layerW, height()-1-m_toolbar->height()-(MARGIN*2));
#endif

	m_tabBar->move(width() - m_tpcZoneProperties->width() - 1 -MARGIN +3+60, MARGIN + m_toolbar->height() + 3 - 42);
	m_tabBar->setFixedWidth(304-(60*2));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::showEvent(QShowEvent *ev)
{
	//qDebug("SamGuiEditor::showEvent()");

	m_timerId = startTimer(100);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::hideEvent(QHideEvent *ev)
{
	//qDebug("SamGuiEditor::hideEvent()");

	killTimer(m_timerId);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::timerEvent(QTimerEvent *ev)
{
	//qDebug("SamGuiEditor::timerEvent()");

	m_zoneEditor->zoneDisplay()->updateActivePitch();
	m_zoneEditor->velocityRuler()->update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiEditor::initToolbar(void)
{
	m_toolbar = new UiToolbar(this, 38);
	m_toolbar->move(10,4);

	m_tbLeft = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbLeft->setImage(":/Resources/toggleLeft25.png");
	m_tbLeft->setLabel("Instrument", "Panel");
	connect(m_tbLeft, SIGNAL(clicked()), this, SLOT(toggleLeftPanel()));
	m_toolbar->addObject(m_tbLeft);

	m_tbRight = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbRight->setImage(":/Resources/toggleRight25.png");
	m_tbRight->setLabel("Layer", "Panel");
	connect(m_tbRight, SIGNAL(clicked()), this, SLOT(toggleRightPanel()));
	m_toolbar->addObject(m_tbRight);

#ifdef USE_EXTENDED_EDITOR
	m_tbBottom = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbBottom->setImage(":/Resources/toggleBottom25.png");
	m_tbBottom->setLabel("Extended", "Panel");
	connect(m_tbBottom, SIGNAL(clicked()), this, SLOT(toggleBottomPanel()));
	m_toolbar->addObject(m_tbBottom);
#endif

	m_toolbar->addSeparator();

	m_tbCopyMove = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbCopyMove->setImage(":/Resources/copymove25.png");
	m_tbCopyMove->setLabel("Move / Copy", "Zones");
	connect(m_tbCopyMove, SIGNAL(clicked()), samGui, SLOT(moveCopyZones()));
	m_toolbar->addObject(m_tbCopyMove);

	m_tbCopyPaste = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbCopyPaste->setImage(":/Resources/copypaste25.png");
	m_tbCopyPaste->setLabel("Copy / Paste", "Zone Settings");
	connect(m_tbCopyPaste, SIGNAL(clicked()), samGui, SLOT(copyPasteZoneSettings()));
	m_toolbar->addObject(m_tbCopyPaste);

	/*m_tbReturn = new UiToolbarObject(UiToolbarObject::Button, 100);
	m_tbReturn->setImage(":/Resources/arrowLeft25.png");
	m_tbReturn->setLabel("Exit Editor");
	connect(m_tbReturn, SIGNAL(clicked()), samGui, SLOT(closeEditor()));
	m_toolbar->addObject(m_tbReturn);*/

	//m_toolbar->addSeparator();
}

