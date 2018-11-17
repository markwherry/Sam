#include "SamGuiParameterControl.h"

#include <QPainter>

SamGuiParameterControl::SamGuiParameterControl(QWidget *parent, SamChannel *channel, SamParameter *parameter)
	: QWidget(parent)
	, m_channel(channel)
	, m_parameter(parameter)
{
	//ui.setupUi(this);
	setFixedSize(80,40);
}

SamGuiParameterControl::~SamGuiParameterControl()
{

}

void SamGuiParameterControl::mouseMoveEvent(QMouseEvent *ev)
{
}

void SamGuiParameterControl::mousePressEvent(QMouseEvent *ev)
{

}

void SamGuiParameterControl::mouseReleaseEvent(QMouseEvent *ev)
{

}

void SamGuiParameterControl::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setRenderHint(QPainter::Antialiasing);

	p.setBrush(QColor(0,0,0,0));
	p.setPen(QColor(255,255,255));
	//p.drawRect(0,0,width()-1,height()-1);

	p.setBrush(QColor(0,0,0,100));
	p.setPen(QColor(0,0,0,100));
	p.drawEllipse(1,2,20,20);

	QColor midiChannelColor[17] = { QColor(160,160,160),
									QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
		                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	if(m_parameter->type() == 0) { // Trigger

	} else if(m_parameter->type() == 2) { // Range
		midiChannelColor[m_channel->midiInputChannel()].setAlpha(200);
		p.setBrush(midiChannelColor[m_channel->midiInputChannel()]);
		p.setPen(midiChannelColor[m_channel->midiInputChannel()]);
		float v = (360.0*16.0) * 1.0f;// * m_parameter->value
		p.drawPie(3,4,16,16,90*16, v);

		p.setBrush(QColor(0,0,0));
		p.setPen(QColor(0,0,0));
		p.drawEllipse(7,8,8,8);
	}
	

	p.setRenderHint(QPainter::Antialiasing,false);
	p.setPen(QColor(0,0,0,45));
	//p.drawLine(width()-1,0,width()-1,height()-1);
	p.fillRect(0, height()-14, width(), 13, QColor(0,0,0,45));

	p.setPen(QColor(220,220,220));
	p.setFont(QFont("Calibri", 8));
	p.drawText(2,0,width()-1,height()-1,Qt::AlignBottom|Qt::AlignLeft, m_parameter->name().toChar());
}

void SamGuiParameterControl::hideEvent(QShowEvent *ev)
{
	qDebug("SamGuiParameterControl::hideEvent()");
}