#ifndef UIPANNER_H
#define UIPANNER_H

#include <QWidget>
//#include "ui_UiPanner.h"

class UiPanner : public QWidget
{
	Q_OBJECT

public:
	UiPanner(QWidget *parent, int size);
	~UiPanner();

	void setXNormalised(float x);
	void setYNormalised(float y);
	float x(bool normalised = true);
	float y(bool normalised = true);

signals:
	void valueChanged(float x, float y);

protected:
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::UiPannerClass ui;
	void updatePositionFromValues(void);
	void updateValuesFromPosition(void);

	int m_x;
	int m_y;
	int m_xPos;
	int m_yPos;
	int m_border;
};

#endif // UIPANNER_H
