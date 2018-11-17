#ifndef SAMGUIMIDIMONITOR_H
#define SAMGUIMIDIMONITOR_H

#include <QWidget>
//#include "ui_SamGuiMidiMonitor.h"

#include "AStateManager.h"
using namespace Atom;

#include <QScrollArea>
#include <QScrollBar>

#include "SamGuiMidiMonitorList.h"

#include "EButton.h"
#include "EClickMenu.h"

class SamGuiMidiMonitor : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiMidiMonitor(QWidget *parent = 0);
	~SamGuiMidiMonitor();

	class Properties
	{
	public:
		Properties();
		void reset(void);

		bool scrollToNewEvent;
	};

	void onState(AStateEvent *ev);

public slots:
	void midiEvent(CsMidiMessage midiMessage);
	void doVerticalScroll(int value);
	void filterAll(void);
	void filterShowAll(void);
	void updateFilterMenuButton(void);

signals:
	void midiReceived(CsMidiMessage midiMessage);

protected:
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	
	void updateScrollBars(void);

	Properties m_properties;

	SamGuiMidiMonitorList *midiMonitorList;
	QScrollArea *scrollArea;
	QScrollBar *scrollBar;

	EButton *buttonClear;
	EClickMenu *menuFilter;
	EClickMenu *menuTools;

	QAction *actionFilterNoteOn;
	QAction *actionFilterNoteOff;
	QAction *actionFilterAftertouch;
	QAction *actionFilterController;
	QAction *actionFilterProgramChange;
	QAction *actionFilterChannelPressure;
	QAction *actionFilterPitchWheel;
	QAction *actionFilterSysEx;
	QAction *actionFilterShowAll;
	QAction *actionFilterAll;

	QAction *actionScrollToNewEvent;
	QAction *actionShowNoteControllerNumbers;
	QAction *actionDeleteFilteredEvents;
	QAction *actionSaveEventsToLogFile;

	bool scrollBarUpdateFlag;

private:
	//Ui::SamGuiMidiMonitorClass ui;
};

#endif // SAMGUIMIDIMONITOR_H
