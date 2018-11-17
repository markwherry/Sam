#ifndef UISTATEBUTTON_H
#define UISTATEBUTTON_H

#include "UiObject.h"
//#include "ui_UiStateButton.h"

class UiStateButton : public UiObject
{
	Q_OBJECT

public:
	UiStateButton(QWidget *parent, int id, int numStates);
	~UiStateButton();

	void setState(int stateId);
	void setText(QString text);

	void setStateLineColor(int stateIndex, QColor color);
	void setStateFillColor(int stateIndex, QColor color);
	void setStateImage(int stateIndex, QString file);
	void setImageHorizontalCenter(bool horizontalCenter) { m_imageHorizontalCenter = horizontalCenter; }
	void setImageVerticalCenter(bool verticalCenter) { m_imageVerticalCenter = verticalCenter; }
	void setTextDisplaysInButton(bool tdib) { m_textDisplaysInButton = tdib; }

signals:
	void clicked(void);
	void clicked(int id);
	void clickedWithFlags(int id, int flags);

protected:
	void mousePressEvent(QMouseEvent *event);
	void paint(QPainter &p);

private:
	bool isStateIndexValid(int stateIndex);
	
	struct UiButtonState {
		QColor fillColor;
		QColor lineColor;
		QPixmap *pixmap;
	};

	//Ui::UiStateButtonClass ui;

	int m_id;
	
	UiButtonState **stateInfo;
	int m_numStates;
	int m_state;
	bool m_imageHorizontalCenter;
	bool m_imageVerticalCenter;
	bool m_textDisplaysInButton;
	
};

#endif // UISTATEBUTTON_H
