#ifndef SEGUIZONEEDITOR_H
#define SEGUIZONEEDITOR_H

#include <QWidget>
//#include "ui_SeGuiZoneEditor.h"

#include <QScrollArea>
#include <QScrollBar>

#include "UiPianoKeyboard.h"
#include "SeGuiZoneDisplay.h"
#include "SeGuiZoneVelocityRuler.h"

#include "SamLayer.h"
#include "SamChannel.h"

class SeGuiZoneEditor : public QWidget
{
	Q_OBJECT

public:
	SeGuiZoneEditor(QWidget *parent = 0);
	~SeGuiZoneEditor();

	void setLayer(SamLayer *layer, SamChannel *channel);

	SeGuiZoneDisplay* zoneDisplay(void) { return m_zoneDisplay; }
	SeGuiZoneVelocityRuler* velocityRuler(void) { return m_velocityRuler; }

public slots:
	void updateZoneDisplay(void);
	void updateScrollBars(void);
	void setHorizontalScroll(int value);
	void selectFromPianoKeyboard(int pitch, int velocity, int modifiers);

protected:
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SeGuiZoneEditorClass ui;
	UiPianoKeyboard *m_pianoKeyboard;
	SeGuiZoneDisplay *m_zoneDisplay;
	QScrollArea *scrollZoneDisplay;
	QScrollArea *scrollPianoKeyboard;
	QScrollArea *scrollVelocityRuler;
	QScrollBar *hScrollBar;
	QScrollBar *vScrollBar;
	SamLayer *m_layer;
	SamChannel::Ptr m_channel;
	SeGuiZoneVelocityRuler *m_velocityRuler;

};

#endif // SEGUIZONEEDITOR_H
