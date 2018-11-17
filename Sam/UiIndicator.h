#ifndef UIINDICATOR_H
#define UIINDICATOR_H

#include <QWidget>
//#include "ui_UiIndicator.h"

class UiIndicator : public QWidget
{
	Q_OBJECT

public:
	UiIndicator(QWidget *parent, int width, int height, bool held = false, bool vertical = true);
	~UiIndicator();

	void setColor(QColor color) { m_color = color; }
	void setBackgroundColor(QColor color) { m_backgroundColor = color; }
	void setHeld(bool held) { m_held = held; }
	void setValue(int value, bool falling);

public slots:
	void updateTimer(void);

protected:
	void paintEvent (QPaintEvent *event);
	//void timerEvent(QTimerEvent *event);
	

private:
	//Ui::UiIndicatorClass ui;
	bool m_vertical;
	QColor m_color;
	QColor m_backgroundColor;
    int m_value;
	int m_maximumValue;
	int m_minimumValue;
	void timerUpdate (void);
	int m_timerId;
	bool m_held;
};

#endif // UIINDICATOR_H
	