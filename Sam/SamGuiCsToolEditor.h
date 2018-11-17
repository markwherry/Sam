#ifndef SAMGUICSTOOLEDITOR_H
#define SAMGUICSTOOLEDITOR_H

#include <QWidget>
//#include "ui_SamGuiCsToolEditor.h"

#include "AStateManager.h"

#include "SamAudioToolInsert.h"
#include "SamMusicToolInsert.h"

#include "SamGuiBypassButton.h"
#include "UiStateButton.h"

#include "UiPianoKeyboard.h"
#include "SamGuiPerVoiceFxGroupEditor.h"

#include "SamAudioBus.h"

#include "ACriticalSection.h"

class SamGuiCsToolEditor : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiCsToolEditor(QWidget *parent = 0);
	~SamGuiCsToolEditor();

	void onState(AStateEvent *ev);

	void setToolInsert(SamToolInsert *insert, SamAudioBus *bus);

	void updateLink();

public slots:
	void updateToolActive(void);
	void bypassButtonPressed(void);
	void linkButtonPressed(void);
	void updateGroup(int id);
	void setSpeakerButton(int id);
	void resizeEditor(int width, int height);

signals:
	void toolActiveUpdated(void);
	void toolEditorSizeUpdate(int width, int height);

protected:
	void closeEvent(QCloseEvent *ev);
	void hideEvent(QHideEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void timerEvent(QTimerEvent *ev);

private:
	//Ui::SamGuiCsToolEditorClass ui;
	void flushEditor(void);
	SamToolInsert *m_insert;
	SamAudioBus *m_audioBus;
	QWidget *m_editor[8];
	int m_numEditors;
	SamGuiBypassButton *m_bypass;
	UiStateButton *m_groups[8];
	UiStateButton *m_link;
	QList<UiStateButton*> m_speakerButtons;

	SamTool *m_tool;
	CsVstPlugin *m_vstPlugin;
	UiPianoKeyboard *m_pianoKeyboard;
	//SamGuiPerVoiceFxGroupEditor *m_groupEditor;

	ACriticalSection m_lock;


};

#endif // SAMGUICSTOOLEDITOR_H
