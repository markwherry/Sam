#ifndef UIFADER_H
#define UIFADER_H

#include <QWidget>
//#include "ui_UiFader.h"

class UiFader : public QWidget
{
	Q_OBJECT

public:
	UiFader(QWidget *parent, bool vertical, int length, int capLength, int capDepth, int id = 0);
	~UiFader();

	void setPower(bool power) { m_power = power; }
	void setTrackSize(int offset, int depth);
	void setValueNormalised(float value);
	void setOpacity(float opacity) { m_opacity = opacity; }
	void setLevelColor(QColor color) { m_levelColor = color; update(); }

signals:
	void valueChanged(float value, int flags);
	void valueChanged(int id, float value, int flags);

protected:
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	void updateFaderPositionFromValue(void);
	void updateFaderValueFromPosition(void);

	//Ui::UiFaderClass ui;
	int m_id;
	bool m_vertical;
	int m_length;
	int m_capLength;
	int m_capDepth;

	int m_faderPosition;
	int m_value;
	//int m_maxValue;
	//int m_minValue;

	//int m_numSteps;
	bool m_power;

	int m_trackOffset;
	int m_trackDepth;

	float m_opacity;

	QColor m_levelColor;
};

#endif // UIFADER_H
