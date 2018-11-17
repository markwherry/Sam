#ifndef UIVALUE_H
#define UIVALUE_H

#include <QWidget>
//#include "ui_UiValue.h"

#include <QLineEdit>

class UiValue : public QWidget
{
	Q_OBJECT

public:
	UiValue(QWidget *parent, int w, int h, QString unit, float min, float max, float def, float step, int pixels);
	~UiValue();

	float value(void) { return m_value; }
	void setValue(float value);
	void setValueNormalised(float value);

	float minValue(void) { return m_min; }
	void setMinValue(float min) { m_min = min; }

	float maxValue(void) { return m_max; }
	void setMaxValue(float max) { m_max = max; }

	void setValueTextColor(QColor valueTextColor) { m_valueTextColor = valueTextColor; }
	void setUnitTextColor(QColor unitTextColor) { m_unitTextColor = unitTextColor; }
	void setValueTextFont(QFont font) { m_font = font; }

	void setLiveUpdate(bool liveUpdate = true) { m_liveUpdate = liveUpdate; }

	void setExtraInfo(int extraInfo) { m_extraInfo = extraInfo; }

	bool isEditable(void) { return m_editable; }
	void setEditable(bool editable) { m_editable = editable; }

public slots:
	void acceptEdit(void);

signals:
	void valueChanged(float value);
	void valueChangedNormalised(float value);

protected:
	//bool event(QEvent *ev);
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	void validateValue(void);
	//Ui::UiValueClass ui;
	QString m_unit;
	float m_value;
	float m_min;
	float m_max;
	float m_def;
	float m_step;
	int m_pixels;

	int yClick;
	int yLast;
	int yDirection;
	float initialValue;

	int xClick;
	int xLast;
	int xDirection;

	QColor m_valueTextColor;
	QColor m_unitTextColor;
	QFont m_font;
	bool m_drawArrows;
	QLineEdit *m_lineEdit;
	QWidget *previousKeyboardGrabber;
	QWidget *previousFocus;

	bool m_liveUpdate;
	int directionAdjustment;
	//float initialValue;
	int m_extraInfo;
	bool m_editable;
};

#endif // UIVALUE_H
