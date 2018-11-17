#include "UiStateButton.h"

#include <QPainter>
#include <QMouseEvent>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
UiStateButton::UiStateButton(QWidget *parent, int id, int numStates)
	: UiObject(parent)
	, m_id(id)
	, m_numStates(numStates)
	, m_state(0)
	, m_imageHorizontalCenter(true)
	, m_imageVerticalCenter(true)
	, m_textDisplaysInButton(false)
{
	//ui.setupUi(this);

	stateInfo = new UiButtonState*[m_numStates];
	for(int i=0; i<m_numStates; i++) {
		stateInfo[i] = new UiButtonState;
		stateInfo[i]->fillColor = QColor(120,120,120);
		stateInfo[i]->lineColor = QColor(40,40,40);
		stateInfo[i]->pixmap = 0;
	}
	//setDrawTint(false);
	//setFont(QFont("Calibri", 8));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
UiStateButton::~UiStateButton()
{
	for(int i=0; i<m_numStates; i++) {
		delete stateInfo[i];
	}

	delete[] stateInfo;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::setState(int stateIndex)
{
	if(!isStateIndexValid(stateIndex)) {
		return;
	}

	m_state = stateIndex;

	setLineColor(stateInfo[stateIndex]->lineColor);
	setFillColor(stateInfo[stateIndex]->fillColor);

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::setText(QString text)
{
	UiObject::setText(text);

	if(!m_textDisplaysInButton) {
		QFontMetrics fm(m_font);
		m_textWidth = fm.width(m_text) + 5;
		setFixedWidth(width()+m_textWidth);
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::setStateLineColor(int stateIndex, QColor color)
{
	if(!isStateIndexValid(stateIndex)) {
		return;
	}

	stateInfo[stateIndex]->lineColor = color;

	if(stateIndex == m_state) {
		setLineColor(stateInfo[stateIndex]->lineColor);	
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::setStateFillColor(int stateIndex, QColor color)
{
	if(!isStateIndexValid(stateIndex)) {
		return;
	}

	stateInfo[stateIndex]->fillColor = color;

	if(stateIndex == m_state) {
		setFillColor(stateInfo[stateIndex]->fillColor);	
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::setStateImage(int stateIndex, QString file)
{
	if(!isStateIndexValid(stateIndex)) {
		return;
	}

	stateInfo[stateIndex]->pixmap = new QPixmap(file);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::mousePressEvent(QMouseEvent *ev)
{
	qDebug("UiStateButton::mousePressEvent()");

	if(m_id > -1) { qDebug("1");
		emit clicked(m_id);
	} else { qDebug("2");
		emit clicked();
	}
	int flags = 0;
	if(ev->modifiers() & Qt::ControlModifier) {
		flags = 1;
	}
	emit clickedWithFlags(m_id, flags);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void UiStateButton::paint(QPainter &p)
{
	//qDebug("");
	//InGuiObject::paintEvent(event);

	//QPainter p(this);

	if(m_textDisplaysInButton) {
		p.setPen(QColor(0,0,0));
		p.setFont (m_font);
		p.drawText(0, 0, width(), height()-1, Qt::AlignCenter|Qt::AlignVCenter, m_text);
	} else {


		if(stateInfo[m_state]->pixmap) {
			int xPos = 0;
			int yPos = 0;
			if(m_imageHorizontalCenter) {
				xPos = ((width()-m_textWidth)/2)-(stateInfo[m_state]->pixmap->width()/2);
			}
			if(m_imageVerticalCenter) {
				yPos = (height()/2)-(stateInfo[m_state]->pixmap->height()/2);
			}
			p.drawPixmap(xPos,yPos, *stateInfo[m_state]->pixmap);
		}

		if(m_text.size()>0) {
			int tc = 200;
			if(m_mouseOver) {
				tc = 255;
			}
			p.setPen(QColor(tc,tc,tc));
			p.setFont (m_font);

			p.drawText(width()-m_textWidth+5, 0, m_textWidth, height()-1, Qt::AlignLeft|Qt::AlignVCenter, m_text);
		}

	}
	//if(isDrawTint()) {
	//	drawTint(p, this, 10);
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool UiStateButton::isStateIndexValid(int stateIndex)
{
	if(stateIndex < 0 || stateIndex >= m_numStates) {
		return false;
	}

	return true;
}