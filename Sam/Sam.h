#ifndef SAM_H
#define SAM_H

#include <QtGui/QMainWindow>
//#include "ui_Sam.h"

#include "ACommandManager.h"
#include "AStateManager.h"
using namespace Atom;

#include "UiAudioHardware.h"
#include "UiCommandProgress.h"
#include "SamGuiArticulationVector.h"

#include "SamGuiView.h"

#include "SamPresetManager.h"

#include "SamGuiCopyLayerFromInstrument.h"
#include "SamGuiCellAnalyser.h"
#include "SamGuiTextFieldDialog.h"
#include "SamGuiBounceInstrument.h"
#include "SeGuiMoveCopyZones.h"
#include "SeGuiCopyPasteZoneSettings.h"
#include "SamGuiCsToolEditor.h"
#include "SamGuiMidiMonitor.h"
#include "SamGuiMidiChannelMap.h"
#include "SamGuiChannelPitchBendRemoteSetup.h"
#include "SamGuiVstPluginManager.h"
//#define USE_OUTPUT_MANAGER
#ifdef USE_OUTPUT_MANAGER
#include "SamGuiOutputManager.h"
#endif

#define MARGIN 8
#define VERSION_STRING "v2.1.0 beta (Build 91)"
#define BUILD_NUMBER 71

class Sam : public QMainWindow,  public ACommandManagerProgress
{
	Q_OBJECT

public:
	Sam(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Sam();

	static Sam* instance(void);

	SamPresetManager* presetManager(void) { return m_presetManager; }

	ACommandManager* commandManager(void) { return m_commandManager; }

	// CommandManagerProgress
	void startCommand(int commandId);
	void stopCommand(int commandId, AString message, int messageType);
	void setProgress(int commandId, float progress);
	void setProgressLabel(int commandId, int labelId, AString label);

	// Recent Projects
	void setCurrentProjectFilename(AString filename);
	void addRecentProject(AString filename);
	AString recentProject(int index) { return m_recentProjects[index]; }
	int numRecentProjects(void) { return m_recentProjects.size(); }
	void updateRecentProjectsMenu(void);

	// Recent Instruments
	void addRecentInstrument(AString filename);
	AString recentInstrument(int index) { return m_recentInstruments[index]; }
	int numRecentInstruments(void) { return m_recentInstruments.size(); }
	void updateRecentInstrumentsMenu(void);

	void signalEditorRefresh(void);

	SamGuiMainWidget*       mainWidget(void) { return m_view->m_mainWidget; }
	SamGuiCellAnalyser*     cellAnalyser(void) { return m_cellAnalyser; }
	SamGuiBounceInstrument* bnceInstrument(void) { return m_bounceInstrument; }
	SamGuiTextFieldDialog*  textFieldDialog(void) { return m_textFieldDialog; }
	SamGuiCsToolEditor*     toolEditor(void) { return m_toolEditor; }
	SamGuiMidiChannelMap*   midiChannelMap(void) { return m_midiChannelMap; }
	UiAudioHardware*        audioHardware(void) { return m_audioHardware; }

	inline bool isShowLabels(void) const { return m_showLabels; }
	void setShowLabels(bool show);

	inline bool isChannelProgramDisplay(void) const { return m_channelProgramDisplay; }

	QPixmap* articulationPixmap(int index, bool inverted = false) { return m_articulationPixmaps[(int)inverted][index]; }

public slots:
	void newProject(void);
	void openProject(void);
	void openRecentProject(void);
	void openRecentInstrument(void);
	void saveProject(void);
	void saveProjectAs(void);
	void openCollection(void);
	void saveCollectionAs(void);
	void newInstrument(void);
	void newStereoInstrument(void);
	void newQuadInstrument(void);
	void newThreeStereoInstrument(void);
	void openInstrument(void);
	void saveInstrument(void);
	void saveInstrumentAs(void);
	void saveAcceleratedAs(void);
	void replaceInstrument(void);
	void bounceInstrument(void);
	void manageOutputs(void);
	void createMonoOutput(void);
	void createStereoOutput(void);
	void createQuadOutput(void);
	void create50Output(void);
	void createStereoBus(void);
	void createQuadBus(void);
	void settingsMidiControllerFloodgate(void);
	void settingsLoadSamplesWithInstrument(void);
	void showChannelLabels(void);
	void settingsChannelProgramDisplay(void);
	void deleteChannels(void);
	void debugToggleEngineTestTone(void);
	void debugLoadShortCelli(void);
	void debugLoadTest(void);
	void debugSetPianoValues(void);
	void debugSetFilter(void);
	void debugPerformanceControls(void);
	void debugPluginEntries(void);

	void setCommandStart(int commandId);
	void setCommandStop(int commandId, QString message, int messageType);
	void setCommandProgress(int commandId, float progress);
	void setCommandProgressLabel(int commandId, int labelId, QString label);

	void openEditor(SamChannel *channel);
	void closeEditor(void);
	void refreshEditor(void);

	void copyLayerFromInstrument(void);
	void mergeLoadedInstruments(void);
	void exportArticulationAsInstrument(void);
	void exportElementPool(void);
	void moveCopyZones(void);
	void copyPasteZoneSettings(void);
	void importSampleFolder(void);
	void assignSourceToRelease(void);
	void clearReleases(void);
	//void fixReleaseSets(void);
	void enableLoops(void);
	void disableLoops(void);
	void updateLoops(void);

	void openMidiMonitor(void);

signals:
	void commandStarted(int commandId);
	void commandStopped(int commandId, QString message, int messageType);
	void commandProgressUpdated(int commandId, float progress);
	void commandProgressLabelUpdated(int commandId, int labelId, QString label);
	void editorRefreshed(void);

protected:
	void closeEvent(QCloseEvent *ev);
	void dragEnterEvent(QDragEnterEvent *ev);
	void dropEvent(QDropEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	void initMenus(void);
	//Ui::SamClass ui;
	static Sam *m_instance;
	SamGuiView *m_view;

	SamPresetManager *m_presetManager;

	ACommandManager *m_commandManager;

	QString m_currentProjectFilename;

	AList<AString> m_recentProjects;
	AList<AString> m_recentInstruments;

	// Other windows
	UiAudioHardware                   *m_audioHardware;
	UiCommandProgress                 *m_commandProgress;
	SamGuiArticulationVector          *m_articulationVector;
	SamGuiCopyLayerFromInstrument     *m_copyLayerFromInstrument;
	SamGuiCellAnalyser                *m_cellAnalyser;
	SamGuiBounceInstrument            *m_bounceInstrument;
	SeGuiMoveCopyZones                *m_moveCopyZones;
	SeGuiCopyPasteZoneSettings        *m_copyPasteZoneSettings;
	SamGuiCsToolEditor                *m_toolEditor;
	SamGuiMidiChannelMap              *m_midiChannelMap;
	SamGuiChannelPitchBendRemoteSetup *m_channelPitchBendRemoteSetup;
	SamGuiVstPluginManager            *m_vstPluginManager;
#ifdef USE_OUTPUT_MANAGER
	SamGuiOutputManager               *m_outputManager;
#endif

	// Menus
	QMenu *m_menuFile;
	QAction *m_actionNewProject;
	QAction *m_actionOpenProject;
	QMenu *m_menuRecentProjects;
	QMenu *m_menuRecentInstruments;
	QAction *m_actionRecentProjects[5];
	QAction *m_actionRecentInstruments[5];
	QAction *m_actionSaveProject;
	QAction *m_actionSaveProjectAs;
	//QAction *m_actionOpenCollection;
	//QAction *m_actionSaveCollectionAs;
	QAction *m_actionOpenInstrument;
	QAction *m_actionReplaceInstrument;
	//QAction *m_actionNewInstrument;
	QMenu *m_menuNewInstrument;
	QAction *m_actionNewStereoInstrument;
	QAction *m_actionNewQuadInstrument;
	QAction *m_actionNewThreeStereoInstrument;
	QAction *m_actionSaveInstrument;
	QAction *m_actionSaveInstrumentAs;
	//QAction *m_actionSaveAcceleratedAs;
	QAction *m_actionBounceInstrument;
	QAction *m_actionExit;

	QMenu *m_menuProject;
	QAction *m_actionManageAudioOutputs;
	QAction *m_actionCreateMonoOutput;
	QAction *m_actionCreateStereoOutput;
	QAction *m_actionCreateQuadOutput;
	QAction *m_actionCreate50Output;
	QAction *m_actionCreateStereoBus;
	QAction *m_actionCreateQuadBus;

	QMenu *m_menuSettings;
	QAction *m_actionConfigureAudioHardware;
	QAction *m_actionVstPluginManager;
	QAction *m_actionMidiControllerFloodgate;
	//QAction *m_actionLoadSamplesWithInstrument;
	QAction *m_actionShowChannelLabels;
	QAction *m_actionProgramChangeScale;
	QAction *m_actionChannelPitchBendRemoteSetup;

	QMenu *m_menuEdit;
	//QAction *actionEditUndo;
	//QAction *actionEditRedo;
	//QAction *actionEditDelete;
	QAction *m_actionCopyLayerFromInstrument;
	QAction *m_actionMergeLoadedInstruments;
	QAction *m_actionExportArticulationAsInstrument;
	QAction *m_actionExportElementPool;
	QAction *m_actionMoveCopyZones;
	QAction *m_actionCopyPasteZoneSettings;
	QAction *m_actionImportSampleFolder;
	QAction *m_actionAssignSourceToRelease;
	QAction *m_clearReleases;
	//QAction *m_fixReleaseSets;
	QAction *m_actionEnableLoops;
	QAction *m_actionDisableLoops;
	QAction *m_actionUpdateLoops;

	QMenu *m_menuTools;
	QAction *m_actionCellAnalyser;
	QAction *m_actionMidiMonitor;
	QAction *m_actionMidiChannelMap;

	QMenu *m_menuDebug;
	QAction *m_actionToggleEngineTestTone;
	QAction *m_actionLoadShortCelli;
	QAction *m_actionLoadTest;
	QAction *m_actionSetPianoValues;
	QAction *m_actionSetFilter;
	QAction *m_actionDebugPerformanceControls;
	QAction *m_actionDebugPluginEntries;

	UiEditor *m_editorAudioOutputs;
	SamGuiTextFieldDialog *m_textFieldDialog;
	SamGuiMidiMonitor *m_midiMonitor;

	bool m_showLabels;
	bool m_channelProgramDisplay;

	QList<QPixmap*> m_articulationPixmaps[2];
};

#define samGui Sam::instance()

#endif // SAM_H
