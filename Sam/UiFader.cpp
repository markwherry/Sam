#include "UiFader.h"

#include <QPainter>
#include <QMouseEvent>

#include <math.h>

UiFader::UiFader(QWidget *parent, bool vertical, int length, int capLength, int capDepth, int id)
	: QWidget(parent)
	, m_vertical(vertical)
	, m_length(length)
	, m_capLength(capLength)
	, m_capDepth(capDepth)
	, m_id(id)
	, m_faderPosition(0)
	, m_value(0)
	//, m_minValue(0)
	//, m_maxValue(1)
	//, m_numSteps(0)
	, m_power(true)
	, m_trackOffset(0)
	, m_trackDepth(capDepth)
	, m_opacity(1.0f)
	, m_levelColor(QColor(70,175,70))
{
	//ui.setupUi(this);

	if(m_vertical) {
		setFixedSize(m_capDepth, m_length);
	} else {
		setFixedSize(m_length, m_capDepth);
	}

	updateFaderPositionFromValue();
}

UiFader::~UiFader()
{

}

void UiFader::setTrackSize(int offset, int depth)
{
	m_trackOffset = offset;
	m_trackDepth = depth;

	update();
}

void UiFader::setValueNormalised(float value)
{
	if(m_power) {
		value = pow(value,0.5f);
	}
	float v = value * 1023;
	m_value = (int)v;
	

	/*if(m_value<m_minValue)
		m_value = m_minValue;

	if(m_value>m_maxValue)
		m_value = m_maxValue;*/

	updateFaderPositionFromValue();

	update();
}

void UiFader::mouseDoubleClickEvent(QMouseEvent *ev)
{
	setValueNormalised(0.5f);

	int flags = 0;
	if(ev->modifiers() & Qt::AltModifier) {
		flags = 1;
	}

	float normalised = 1.0/(float)1023;
	normalised *= (float)m_value; 
	if(m_power) {
		normalised = pow(normalised, 2.0f);
	}
	emit valueChanged(normalised, flags);
	emit valueChanged(m_id, normalised, flags);

	update();
}

void UiFader::mouseMoveEvent(QMouseEvent *ev)
{
	if(ev->modifiers() & Qt::ControlModifier) {
		return;
	}

	if(m_vertical) {
		m_faderPosition = ev->y();
	} else {
		m_faderPosition = ev->x();
	}

	updateFaderValueFromPosition();

	int flags = 0;
	if(ev->modifiers() & Qt::AltModifier) {
		flags = 1;
	}

	float normalised = 1.0/(float)1023;
	normalised *= (float)m_value; 
	if(m_power) {
		normalised = pow(normalised, 2.0f);
	}
	emit valueChanged(normalised, flags);
	emit valueChanged(m_id, normalised, flags);

	update();
}

void UiFader::mousePressEvent(QMouseEvent *ev)
{
	if(ev->modifiers() & Qt::ControlModifier) {
		setValueNormalised(0.5f);
	} else {
		if(m_vertical) {
			m_faderPosition = ev->y();
		} else {
			m_faderPosition = ev->x();
		}
		updateFaderValueFromPosition();
	}

	int flags = 0;
	if(ev->modifiers() & Qt::AltModifier) {
		flags = 1;
	}
	float normalised = 1.0/(float)1023;
	normalised *= (float)m_value; 
	if(m_power) {
		normalised = pow(normalised, 2.0f);
	}
	emit valueChanged(normalised, flags);
	emit valueChanged(m_id, normalised, flags);

	update();
}

void UiFader::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);
	if(m_opacity<1.0) {
		p.setOpacity(m_opacity);
	}

	int trackX, trackY, trackW, trackH;
	
	if(m_vertical) {
		trackX = m_trackOffset;
		trackY = 0;
		trackW = m_trackDepth-1;
		trackH = height()-1;
	} else {
		trackX = 0;
		trackY = m_trackOffset;
		trackW = width()-1;
		trackH = m_trackDepth-1;
	}

	p.setPen(QColor(30,30,30));
	p.setBrush(QColor(0,0,0));
	p.drawRect(trackX, trackY, trackW, trackH);

	//g.setPen(NeColor(70,70,200));
	//g.setBrush(NeColor(70,70,200));

	if(isEnabled()) {
		p.setPen(m_levelColor);
		p.setBrush(m_levelColor);
	} else {
		p.setPen(QColor(120,120,120));
		p.setBrush(QColor(120,120,120));
	}

	int faderX,faderY,faderW,faderH;

	if(m_vertical) {
		//p.drawRect(trackX+1,trackY+faderPos+(m_faderCapHeight/2),trackW-2,trackH-1-(faderPos+(m_faderCapHeight/2)));
		////p.drawPixmap(0,faderPos,*pixFaderCap);
		//p.drawRect(1, m_faderPosition, width()-3,  height()-m_faderPosition-2);
		
		p.drawRect(trackX+1, m_faderPosition, trackW-2, trackH-m_faderPosition-1);

		//for(int i=trackH-1; i>m_faderPosition; i-=2) { //+m_capLength+2
		//	p.drawLine(trackX+1, i, trackX+1+trackW-2, i);
		//}

		faderX = 0;
		faderY = m_faderPosition - (m_capLength/2);
		faderW = m_capDepth;
		faderH = m_capLength;
	} else {
		
		
		p.drawRect(1,1,m_faderPosition,height()-3);
	//	p.fillRect(1,1,m_faderPosition,1,QColor(255,255,255,40));
		//p.drawPixmap(faderPos,0,*pixFaderCap);

		//for(int i=trackX+1; i<m_faderPosition; i+=2) { //+m_capLength+2
		//	p.drawLine(i, trackY+1, i, trackY+1+trackH-2);
		//}

		faderX = m_faderPosition - (m_capLength/2); 
		faderY = 0;
		faderW = m_capLength;
		faderH = m_capDepth;
	}

	//faderY++; faderH-=2;

	p.setPen(QColor(200,200,200));
	p.setBrush(QColor(180,180,180));
	p.drawRect(faderX, faderY, faderW, faderH); 

	QColor col(0,0,0);
	QLinearGradient lgBack(QPointF(faderX, faderY), QPointF(faderX, faderY+faderH));
	col.setAlpha(60);//240//90 // 230
	lgBack.setColorAt(0, col);
	col.setAlpha(120);//255//50 // 255
	lgBack.setColorAt(1, col);

	p.setBrush(lgBack);
	p.setPen(QColor(0,0,0,0));
	p.drawRect(faderX, faderY, faderW, faderH);
	//p.fillRect(faderX, faderY, faderW, faderH, lgBack);

	p.setPen(QColor(200,200,200,100));
	if(m_vertical) {
		p.drawLine(faderX, faderY+(faderH/2), faderX+faderW, faderY+(faderH/2));
	} else {
		p.drawLine(faderX+(faderW/2), faderY, faderX+(faderW/2), faderH);
	}
	p.setPen(QColor(40,40,40));
	p.drawLine(faderX, faderY+faderH, faderX+faderW, faderY+faderH);
	p.drawLine(faderX+faderW-1, faderY, faderX+faderW-1, faderY+faderH);

	p.setPen(QColor(50,50,50,100));
	if(m_vertical) {
		p.drawLine(faderX, faderY+(faderH/2)-1, faderX+faderW, faderY+(faderH/2)-1);
	} else {
		p.drawLine(faderX+(faderW/2)-1, faderY+1, faderX+(faderW/2)-1, faderH);
	}
	
}

void UiFader::updateFaderPositionFromValue(void)
{
	//if(m_power) {
	//	m_value = pow(m_value,0.5f);
	//}
	if(m_vertical) {
		float length = height()-1-m_capLength; //aLog("length = %f", length);
		float x = length/(float)1023; //aLog("x = %f", x);
		x = x * (float)m_value; //aLog("x = %f (m_value = %d)", x, m_value);
		m_faderPosition = (m_capLength/2)+(length)-(int)x; //aLog("m_faderPosition = %d", m_faderPosition);
	} else {
		float length = width()-1-m_capLength;
		float x = length/(float)1023;
		x = x * (float)m_value;
		m_faderPosition = (m_capLength/2)+(int)x;
	}
	
	
	
	//qDebug("m_faderPosition = %d", m_faderPosition);
}

void UiFader::updateFaderValueFromPosition(void)
{
	if(m_faderPosition < (m_capLength/2)) {
		m_faderPosition = (m_capLength/2);
	}

	if(m_vertical) {

		if(m_faderPosition>height()-1-(m_capLength/2)) {
			m_faderPosition = height()-1-(m_capLength/2);
		}

		float unit = (float)1023/(height()-1-m_capLength);
		float v = unit * (float)(m_faderPosition-(m_capLength/2));
		m_value = (1023-v);

	} else {

		if(m_faderPosition > width()-1-(m_capLength/2)) {
			m_faderPosition = width()-1-(m_capLength/2);
		}

		float unit = (float)1023 / (width()-1-m_capLength);
		float v = unit * (float)(m_faderPosition - (m_capLength/2));
		m_value = v;

		
	}

	

	//aLog("v = %d", m_value);

	//m_fader->setValue(diff);

	return;
}
/*
void UiFader::updateFaderValueFromPosition(void)
{
	if(m_faderPosition < (m_capLength/2)) {
		m_faderPosition = (m_capLength/2);
	}

	int l = width()-1;
	if(m_vertical) {
		l = height()-1;
	}

	if(m_faderPosition>height()-1-(m_capLength/2)) {
		m_faderPosition = height()-1-(m_capLength/2);
	}

	float unit = (float)1023/(height()-1-m_capLength);
	float v = unit * (float)(m_faderPosition-(m_capLength/2));
	m_value = (1023-v);

	

	//aLog("v = %d", m_value);

	//m_fader->setValue(diff);

	return;
}*/