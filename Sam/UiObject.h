#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <QWidget>
//#include "ui_UiObject.h"

class UiObject : public QWidget
{
	Q_OBJECT

public:
	UiObject(QWidget *parent = 0);
	~UiObject();

	void setDrawLine(bool drawLine) { m_drawLine = drawLine; }
	void setDrawFill(bool drawFill) { m_drawFill = drawFill; }
	void setDrawTint(bool drawTint) { m_drawTint = drawTint; }
	
	void setLineColor(QColor color) { m_lineColor = color; }
	void setFillColor(QColor color) { m_fillColor = color; }
	void setTextColor(QColor color) { m_textColor = color; }

	void setLineWidth(int pixels) { m_lineWidth = pixels; }

	void setDrawMouseOverFill(bool drawMouseOverFill) { m_drawMouseOverFill = drawMouseOverFill; }

	virtual void setText(QString text) { m_text = text; update(); }
	void setTextAlignment(int textAlignment) { m_textAlignment = textAlignment; }
	virtual void setTextBold(bool bold) { m_bold = bold; }
	virtual void setTextSize(int size) { m_textSize = size; }
	void setFont(QFont font) { m_font = font; }

	static void drawTint(QPainter &p, QWidget *widget, int alphaAdjustment, int width = -1, int xOffset = 0);

	QString text(void) { return m_text; }

	bool isEditable(void) { return m_editable; }
	void setEditable(bool editable) { m_editable = editable; }

protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);
	virtual void paint(QPainter &p);
	
	//QString text(void) { return m_text; }
	int textAlignment(void) { return m_textAlignment; }
	QColor textColor(void) { return m_textColor; }
	bool isTextBold(void) { return m_bold; }

	bool m_mouseOver;
	QString m_text;
	QFont m_font;
	int m_textWidth;
	QColor m_textColor;
	int m_textSize;

private:
	//Ui::UiObjectClass ui;
	
	bool m_drawLine;
	bool m_drawFill;
	bool m_drawTint;

	int m_lineWidth;
	bool m_bold;
	int m_textAlignment;
	

	QColor m_lineColor;
	QColor m_fillColor;
	

	bool m_drawMouseOverFill;

	bool m_editable;
};

#endif // UIOBJECT_H
