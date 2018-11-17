#ifndef SAMGUIEDITOR_H
#define SAMGUIEDITOR_H

#include <QWidget>
//#include "ui_SamGuiEditor.h"

#include "ACommandManager.h"
#include "AStateManager.h"
using namespace Atom;

#include "UiToolbar.h"
#include "UiEditor.h"
#include "UiTabBar.h"

#include "SamChannel.h"

#include "UiToolPanelContainer.h"

#include "SeGuiVersionProperties.h"
#include "SeGuiArticulationProperties.h"
#include "SeGuiParameters.h"
#include "SeGuiDescriptionProperties.h"

#include "SeGuiZoneEditor.h"
#include "SeGuiZoneProperties.h"
#include "SeGuiLayerProperties.h"
#include "SeGuiCellProperties.h"

//#define USE_EXTENDED_EDITOR
#ifdef USE_EXTENDED_EDITOR
#include "SeGuiSampleEditor.h"
#endif

#include <QShortcut>
#include <qstackedwidget.h>

class SamGuiEditor : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiEditor(QWidget *parent = 0);
	~SamGuiEditor();

	static SamGuiEditor* instance(void);

	ACommandManager* commandManager(void) { return m_commandManager; }

	void onState(AStateEvent *ev);

	void setChannel(SamChannel *channel);
	

	SamInstrument* instrument(void) { return m_instrument; }
	SamLayer* layer(void) { return m_layer; }

public slots:
	void updateZoneSelection(SamZone *zone);
	void setLayer(int id);
	void setPropertiesTab(int id);
	void updateLayerTabs(void);
	void removeZones(void);
	void refresh(void);
	void toggleLeftPanel(void);
	void toggleRightPanel(void);
	void toggleBottomPanel(void);

signals:
	void zoneSelected(SamZone *zone);
	void layerNameChanged(void);
	void layerScopeChanged(void);
	void layerZonesChanged(void);
	void zoneVelocityPitchChanged(void);
	void channelLayerChanged(void);
	void articulationChanged(void);

protected:
	void resizeEvent(QResizeEvent *ev);
	void showEvent(QShowEvent *ev);
	void hideEvent(QHideEvent *ev);
	void timerEvent(QTimerEvent *ev);

private:
	void initToolbar(void);
	//Ui::SamGuiEditorClass ui;
	static SamGuiEditor *m_instance;
	ACommandManager *m_commandManager;
	SamChannel::Ptr m_channel;
	SamInstrument *m_instrument;
	SamVersion *m_version;
	SamArticulation *m_articulation;
	SamLayer *m_layer;

	UiToolbar *m_toolbar;
	UiToolbarObject *m_tbLeft;
	UiToolbarObject *m_tbRight;
#ifdef USE_EXTENDED_EDITOR
	UiToolbarObject *m_tbBottom;
#endif
	UiToolbarObject *m_tbCopyMove;
	UiToolbarObject *m_tbCopyPaste;

	UiToolPanelContainer *m_tpcInstrument;
	UiToolPanelContainer *m_tpcZoneProperties;
	UiToolPanelContainer *m_tpcLayerProperties;
	UiEditor *m_layerEditor;
	UiEditor *m_cellEditor;
	SeGuiZoneEditor *m_zoneEditor;
#ifdef USE_EXTENDED_EDITOR
	QStackedWidget *m_stackedWidget;
	UiEditor *m_extendedEditor;
	SeGuiSampleEditor *m_sampleEditor;
#endif

	SeGuiVersionProperties *m_versionProperties;
	SeGuiArticulationProperties *m_articulationProperties;
	SeGuiParameters* m_parameters;
	SeGuiDescriptionProperties *m_descriptionProperties;
	SeGuiZoneProperties *m_zoneProperties[11];
	SeGuiLayerProperties *m_layerProperties[5];
	SeGuiCellProperties *m_cellProperties;

	UiTabBar *m_tabBar;

	QShortcut *m_scCloseEditor;
	QShortcut *m_scCloseEditorEsc;
	QShortcut *m_scRemoveZones;

	int m_timerId;

	QAction *actionParametersAdd;
	QAction *actionParametersRemove;
	QAction *actionCreateFilterCutOffParameter;
	QAction *actionCreateMinimumVelocityParameter;
	QAction *actionCreateSampleStartParameter;
	QAction *actionCreateAttackParameter;
	QAction *actionCreateReleaseEnvelopeParameters;
	QAction *actionCreateCellSampleStartAdjustParameter;
	QAction *actionCreateLayerSwitchParameters;
};

#define samEditor SamGuiEditor::instance()

#endif // SAMGUIEDITOR_H
