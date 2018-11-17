#ifndef UICURVE_H
#define UICURVE_H

#include <QWidget>
//#include "ui_UiCurve.h"

#include "UiFader.h"

#include "CsCurve.h"
using namespace Cs;

class UiCurve : public QWidget
{
	Q_OBJECT

public:
	UiCurve(QWidget *parent = 0);
	~UiCurve();

	float depth(void) { return m_curve->depth(); }

	void setShape(int shape) { m_curve->setShape(shape); update(); }

	void setStart(float start) { m_curve->setStart(start); update(); }

	void setEnd(float end) { m_curve->setEnd(end); update(); }

	void setDrawBackground(bool drawBackground = true) { m_drawBackground = drawBackground; }
	void setColor(QColor color) { m_color = color; }

	void setMargin(int margin) { m_margin = margin; }

public slots:
	void setDepth(int q);
	void setDepth(float q);
	void setDepthFromFader(int, float q, int);

signals:
	void depthAdjusted(float);

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	//Ui::UiCurveClass ui;
	CsCurve *m_curve;
	bool m_drawBackground;
	int m_margin;
	UiFader *m_fader;
	bool m_mouseOver;
	QColor m_color;
};

#endif // UICURVE_H
