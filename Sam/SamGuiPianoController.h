#ifndef SAMGUIPIANOCONTROLLER_H
#define SAMGUIPIANOCONTROLLER_H

//#include <QWidget>
//#include "ui_SamGuiPianoController.h"

#include "UiPianoKeyboard.h"
#include "UiCurve.h"
#include "SamChannel.h"

#include "AStateManager.h"

class SamGuiPianoController : public UiPianoKeyboard, public AStateListener
{
	Q_OBJECT

public:
	SamGuiPianoController(QWidget *parent = 0);
	~SamGuiPianoController();

	void onState(AStateEvent *ev);

	bool isAttenuationMode(void) { return m_attenuationMode; }
	void setAttenuationMode(bool enabled);

signals:
	void midiReceived(int ev, int port);

public slots:
	void setChannel(SamChannel *channel);
	void makeInQAdjustment(float q);
	void makeOutQAdjustment(float q);
	void updateActivePitches(void);
	void midiEvent(int ev, int port);
	void createNoteEvent(int pitch, int velocity, int modifiers);
	void createPolyAftEvent(int pitch, int aftertouch);

protected:
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SamGuiPianoControllerClass ui;
	//UiPianoKeyboard *m_pianoKeyboard;
	void updateCurves(void);

	SamChannel::Ptr m_channel;
	bool leftButtonPressed;
	int clickedOnNote;
	bool m_attenuationMode;
	int m_attenuationStart;
	int m_attenuationEnd;
	int m_attenuationFadeIn;
	int m_attenuationFadeOut;
	int amAdjust;
	QPixmap *pixFadeIn;
	QPixmap *pixFadeOut;
	QPixmap *pixArrowLeft;
	QPixmap *pixArrowRight;
	QPixmap *pixArrowLeftRight;
	UiCurve *guiCurveIn;
	UiCurve *guiCurveOut;
	int m_velocities[128];
};

#endif // SAMGUIPIANOCONTROLLER_H
