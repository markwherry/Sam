#ifndef SAMGUIMAINWIDGET_H
#define SAMGUIMAINWIDGET_H

#include <QWidget>
//#include "ui_SamGuiMainWidget.h"

//#include "Core/StateManager.h"

#include "AStateManager.h"
using namespace Atom;

#include <QStackedWidget>
#include <QShortcut>

#include "UiEditor.h"
#include "UiToolbar.h"
#include "UiTabBar.h"
#include "UiToolPanelContainer.h"

#include "SamGuiStatusBar.h"
#include "SamGuiChannelList.h"
#include "SamGuiChannelProperties.h"
#include "SamGuiToolContainer.h"
#include "SamGuiPianoController.h"
#include "SamGuiMixer.h"
#include "SamGuiAudioBusProperties.h"
#include "SamGuiStreamMixer.h"
#include "SamGuiMixerPresetSection.h"
#include "SamGuiOutputConfigSection.h"
#include "SamGuiInstrumentList.h"
#include "SamGuiArticulationModeSection.h"
#include "SamGuiArticulationEditor.h"
#include "SamGuiArticulationSection.h"

#include "SamGuiChannelSettings.h"
#include "SamGuiChannelInput.h"
#include "SamGuiChannelCrossfade.h"
#include "SamGuiChannelTools.h"
#include "SamGuiChannelInserts.h"
#include "SamGuiChannelSends.h"
#include "SamGuiChannelOutputs.h"

//#define USE_LIBRARY
#define USE_MIXER

class SamGuiMainWidget : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiMainWidget(QWidget *parent = 0);
	~SamGuiMainWidget();

	void onState(AStateEvent *ev);

	SamGuiPianoController* pianoController(void) { return m_pianoController; }

	QMenu *popupChannelIndicators(void) { return m_popupChannelIndicators; }

	inline SamGuiChannelList* channelList(void) const { return m_channelList; }

public slots:
	void updateSelectedChannel(void);
	void updateArticulation(void);
	void selectNextChannel(void);
	void selectPrevChannel(void);
	void editInstrument(void);
	void properties(void);
	void mainMixer(void);
	void mixer(void);
	void articulation(void);
	void keyboard(void);
	void findChannel(void);
	void setChannelIndicator(QAction *action);

signals:
	void channelSelected(void);
	void articulationUpdated(void);
	//void channelConfigUpdated(void);
	void timerTick(void);

protected:
	void showEvent(QShowEvent *ev);
	void hideEvent(QHideEvent *ev);
	void timerEvent(QTimerEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SamGuiMainWidgetClass ui;

	int m_timerId; 

	UiToolbar *m_toolbar;
	UiToolbarObject *m_tbAdd;
	UiToolbarObject *m_tbRemove;
	UiToolbarObject *m_tbReplace;
	UiToolbarObject *m_tbProperties;
	UiToolbarObject *m_tbMainMixer;
	UiToolbarObject *m_tbMixer;
	UiToolbarObject *m_tbArticulation;
	UiToolbarObject *m_tbKeyboard;
	UiToolbarObject *m_tbFindChannel;
	//UiToolbarObject *m_tbCellAnalyser;
	UiToolbarObject *m_tbMidiMonitor;
	UiToolbarObject *m_tbMidiChannelMap;

#ifdef USE_LIBRARY
	UiTabBar *m_tabBar;
#endif

	SamGuiStatusBar *m_statusBar;
	UiEditor *m_editorChannelList;
	SamGuiChannelList *m_channelList;

	QStackedWidget *m_stackedWidget;
	UiEditor *m_editorProperties;
#ifdef USE_LIBRARY
	UiEditor *m_editorBrowser;
#endif
	SamGuiChannelProperties *m_channelProperties;
	SamGuiInstrumentList *m_instrumentList;

	UiToolPanelContainer *m_tpcChannelProperties;
	
#ifdef USE_MIXER
	UiEditor *m_editorMainMixer;
	SamGuiMixer *m_mixer;
	//SamGuiAudioBusProperties *m_audioBusProperties;
#endif
	
	UiEditor *m_editorMixer;
	SamGuiToolContainer *m_toolContainer;
	SamGuiStreamMixer *m_streamMixer;
	SamGuiMixerPresetSection *m_mixerPresetSection;
	SamGuiOutputConfigSection *m_outputConfigSection;

	UiEditor *m_editorArticulation;
	SamGuiArticulationEditor *m_articulationEditor;
	SamGuiArticulationSection *m_articulationSection;

	UiEditor *m_editorPiano;
	SamGuiPianoController *m_pianoController;
	SamGuiArticulationModeSection *m_articulationModeSection;
	
	QShortcut *m_scNewProject;
	QShortcut *m_scLoadRecentInstrument;
	QShortcut *m_scLoadRecentProject;
	QShortcut *m_scSelectNextChannel;
	QShortcut *m_scSelectPrevChannel;
	QShortcut *m_scEditInstrument;
	QShortcut *m_scRemoveChannels;
	QShortcut *m_scMixer;
	QShortcut *m_scKeyboard;
	QShortcut *m_scProperties;

	QMenu *m_popupChannelIndicators;
	QAction *m_pciInfoActions[13];

	SamGuiChannelSettings  *m_channelSettings;
	SamGuiChannelInput     *m_channelInput;
	//SamGuiChannelCrossfade *m_channelCrossfade;
	SamGuiChannelTools     *m_channelTools;
	SamGuiChannelInserts   *m_channelInserts;
	SamGuiChannelSends     *m_channelSends;
	SamGuiChannelOutputs   *m_channelOutputs;
};

#endif // SAMGUIMAINWIDGET_H
