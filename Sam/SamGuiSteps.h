#ifndef SAMGUISTEPS_H
#define SAMGUISTEPS_H

#include <QWidget>
//#include "ui_SamGuiSteps.h"

class SamGuiSteps : public QWidget
{
	Q_OBJECT

public:
	SamGuiSteps(QWidget *parent = 0);
	~SamGuiSteps();

	void setNumSteps(int numSteps);
	void setValue(int step, float value);
	void setMute(int step, bool mute);
	void setAccent(int step, bool accent);

	void setMidZero(bool midZero) { m_midZero = midZero; }
	void setShowMutes(bool show) { m_showMutes = show; }
	void setShowAccents(bool show) { m_showAccents = show; }

signals:
	void valueUpdated(int step, float value);
	void muteUpdated(int step, bool mute);
	void accentUpdated(int step, bool accent);

protected:
	void paintEvent(QPaintEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);

private:
	int xStep(int x);
	float yRange(int y);
	//Ui::SamGuiStepsClass ui;

	class Step {
	public:
		Step() : value(-0.5f), mute(false), accent(false) {}
		float value;
		bool mute;
		bool accent;
	};
	Step m_steps[128];

	int m_numSteps;
	//SamStepSequencer *m_stepSequencer;
	QPixmap *pixMute;
	QPixmap *pixAccent;

	bool m_showMutes;
	bool m_showAccents;
	bool m_midZero;
};

#endif // SAMGUISTEPS_H
