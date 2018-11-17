#ifndef SAMGUISTREAMMIXER_H
#define SAMGUISTREAMMIXER_H

#include "SamGuiToolEditor.h"
//#include "ui_SamGuiStreamMixer.h"

#include "AStateManager.h"
using namespace Atom;

#include "SamGuiStreamMixStrip.h"

class SamGuiStreamMixer : public SamGuiToolEditor, public AStateListener
{
	Q_OBJECT

public:
	SamGuiStreamMixer(QWidget *parent = 0);
	~SamGuiStreamMixer();

	void onState(AStateEvent *ev);

	void setEditorChannel(SamChannel *channel);
	SamStreamMix* currentStreamMix(void);

public slots:
	void updateStreamGain(int streamIndex);
	void updateStreamPanX(int streamIndex);
	void updateStreamPanY(int streamIndex);
	void updateStreamMute(int streamIndex);
	void updateStreamSolo(int streamIndex);
	void updateSetActive(void);

signals:
	void streamGainUpdated(int streamIndex);
	void streamPanXUpdated(int streamIndex);
	void streamPanYUpdated(int streamIndex);
	void streamMuteUpdated(int streamIndex);
	void streamSoloUpdated(int streamIndex);
	void setActiveUpdated(void);

private:
	//Ui::SamGuiStreamMixerClass ui;
	
	QList<SamGuiStreamMixStrip*> m_strips;
};

#endif // SAMGUISTREAMMIXER_H
