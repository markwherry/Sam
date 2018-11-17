#include "SamGuiIndicatorBlock.h"

#include <QPainter>

SamGuiIndicatorBlock::SamGuiIndicatorBlock(QWidget *parent, QString label, QColor color, int controller)
	: QWidget(parent)
	, m_value(0)
	, m_label(label)
	, m_color(color)
	, m_controller(controller)
{
	//ui.setupUi(this);
	setFixedSize(70, 40); 
}

SamGuiIndicatorBlock::~SamGuiIndicatorBlock()
{

}

void SamGuiIndicatorBlock::setValue(int value)
{
	m_value = value;

	update();
}

void SamGuiIndicatorBlock::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.setPen(QColor(0,0,0,45));
	//p.drawLine(0,-2,0,height()-1);

	p.fillRect(width()-8,0,8,height(),QColor(30,30,30));

	//int range = m_maximumValue - m_minimumValue;
	float unit = (float)(height()-2)/(float)127;
	float h = unit * m_value;
	p.fillRect(width()-7, height() - (int)h - 1, 6, (int)h, m_color);

	p.setFont(QFont("Calibri", 8));
	p.setPen(QColor(200,200,200));
	p.drawText(0,0,width()-12, 20, Qt::AlignTop|Qt::AlignRight, m_label);

	if(m_value > 0) {
		p.setFont(QFont("Calibri",12));
		p.setPen(QColor(255,255,255));
		p.drawText(0,10,width()-12, 20, Qt::AlignTop|Qt::AlignRight, QString::number(m_value));
	}

	/*p.setRenderHint(QPainter::Antialiasing);

	p.setBrush(QColor(0,0,0,0));
	p.setPen(QColor(255,255,255));
	//p.drawRect(0,0,width()-1,height()-1);
	p.fillRect(0,0,width(),height(),QColor(0,0,0,50));

	p.setBrush(QColor(0,0,0,100));
	p.setPen(QColor(0,0,0,100));
	p.drawEllipse(1,2,20,20);

	QColor midiChannelColor[17] = { QColor(160,160,160),
									QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
		                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	//if(m_parameter->type() == 0) { // Trigger

	//} else if(m_parameter->type() == 2) { // Range
		midiChannelColor[1].setAlpha(200); //m_channel->midiInputChannel()
		p.setBrush(midiChannelColor[1]);
		p.setPen(midiChannelColor[1]);
		float v = (360.0*16.0) * 1.0f * 0.5;// * m_parameter->value
		p.drawPie(3,4,16,16,90*16, v);

		p.setBrush(QColor(0,0,0));
		p.setPen(QColor(0,0,0));
		p.drawEllipse(7,8,8,8);
	//}
	

	p.setRenderHint(QPainter::Antialiasing,false);
	p.setPen(QColor(0,0,0,45));
	//p.drawLine(width()-1,0,width()-1,height()-1);
	//p.fillRect(0, height()-14, width(), 13, QColor(0,0,0,45));

	p.setPen(QColor(220,220,220));
	p.setFont(QFont("Calibri", 8));
	p.drawText(2,0,width()-1,height()-1,Qt::AlignBottom|Qt::AlignLeft, "Name");//m_parameter->name().toChar());*/
}