#ifndef UIPIANOKEYBOARD_H
#define UIPIANOKEYBOARD_H

#include <QWidget>
//#include "ui_UiPianoKeyboard.h"

class UiPianoKeyboard : public QWidget
{
	Q_OBJECT

public:
	enum KeyboardOrientation {
		Horizontal = 0,
		Vertical = 1
	};
	UiPianoKeyboard(QWidget *parent, float keyWidth, int depth, int numPitches = 88, int firstMidiNote = 21, KeyboardOrientation orientation = Horizontal);
	~UiPianoKeyboard();

	bool isHorizontal(void) { return (bool)!m_orientation; }

	float pitchWidth(void) { return m_pitchWidth; }
	void setPitchWidth(float pixels);
	int firstMidiNote(void) { return m_firstMidiNote; }
	int numPitches(void) { return m_numPitches; }

	void setNoteDownColor(QColor color) { m_noteDownColor = color; }

	//void setNoteVelocities(int* velocities);

public slots:
	void setNoteDown(int pitch, int velocity);

signals:
	void pitchPressed(int pitch, int velocity, int modifiers);
	void pitchReleased(int pitch, int velocity, int modifiers);
	void pitchAftertouch(int pitch, int aftertouch);

protected:
	virtual void mouseMoveEvent(QMouseEvent *ev);
	virtual void mousePressEvent(QMouseEvent *ev);
	virtual void mouseReleaseEvent(QMouseEvent *ev);
	virtual void paintEvent(QPaintEvent *paintEvent);

	void paintWhiteNote(QPainter &p, int midiNote, float x, float y, float w, float h);
	void paintBlackNote(QPainter &p, int midiNote, float x, float y, float w, float h);
	float whiteNotePos(int key, int noteInOctave);
	int nextWhiteNote(int noteInOctave);
	int pitchFromPoint(int px, int py);
	
	int m_pitchDown[128];
	int m_aftertouch[128];
	bool m_pitchActive[128];
private:
	
	//Ui::UiPianoKeyboardClass ui;
	float m_pitchWidth;
	KeyboardOrientation m_orientation;
	int m_numPitches;
	int m_numWhiteNotes;
	int m_firstMidiNote;
	
	int m_style;
	int m_depth;
	int m_keyboardLength;
	QColor m_noteDownColor;

	int m_clickedOnPitch;
	bool m_leftButtonPressed;
};

#endif // UIPIANOKEYBOARD_H
