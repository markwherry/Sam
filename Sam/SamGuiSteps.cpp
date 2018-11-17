#include "SamGuiSteps.h"

#include <QPainter>
#include <QMouseEvent>

SamGuiSteps::SamGuiSteps(QWidget *parent)
	: QWidget(parent)
	, m_numSteps(16)
	, m_showAccents(false)
	, m_showMutes(false)
	, m_midZero(false)
{
	//ui.setupUi(this);

	pixMute = new QPixmap(":/Resources/mute16.png");
	pixAccent = new QPixmap(":/Resources/accent16.png");
}

SamGuiSteps::~SamGuiSteps()
{

}

void SamGuiSteps::setNumSteps(int numSteps)
{
	m_numSteps = numSteps;
	if(m_numSteps < 1) m_numSteps = 1;
	if(m_numSteps > 128) m_numSteps = 128;

	update();
}

void SamGuiSteps::setValue(int step, float value)
{
	m_steps[step].value = value;
	update();
}

void SamGuiSteps::setMute(int step, bool mute)
{
	m_steps[step].mute = mute;
	update();
}

void SamGuiSteps::setAccent(int step, bool accent)
{
	m_steps[step].accent = accent;
	update();
}

void SamGuiSteps::mousePressEvent(QMouseEvent *ev)
{
	int stepHeight = 96;
	int step = xStep(ev->x());
	if(step < 0) {
		return;
	}

	/*
	if(ev->y()>3+stepHeight+3 && ev->y() < 3+stepHeight+3+16) {
		//if(m_stepSequencer) {
		//	m_stepSequencer->setStepMute(step, !m_stepSequencer->stepMute(step));
		//	update();
		//}
		m_steps[step].mute = !m_steps[step].mute;
		emit muteUpdated(step, m_steps[step].mute);
		update();
	} else if(ev->y()>3+stepHeight+3+16+3 && ev->y() < 3+stepHeight+3+16+3+16) {
		//if(m_stepSequencer) {
		//	m_stepSequencer->setStepAccent(step, !m_stepSequencer->stepAccent(step));
		//	update();
		//}
		m_steps[step].accent = !m_steps[step].accent;
		emit accentUpdated(step, m_steps[step].accent);
		update();
	} else {*/
		mouseMoveEvent(ev);
	//}
}

void SamGuiSteps::mouseMoveEvent(QMouseEvent *ev)
{
	int step = xStep(ev->x());
	if(step < 0) {
		return;
	}

	int stepHeight = 96;
	if(ev->y() >= 3 && ev->y() <= 3+stepHeight) {
		//int velocity = yRange(ev->y());
		//if(velocity < 0) {
		//	return;
		//}

		//if(m_stepSequencer) {
		//	m_stepSequencer->setStepVelocity(step, velocity);
		//	update();
		//}
		m_steps[step].value = yRange(ev->y());//velocity;
		emit valueUpdated(step, m_steps[step].value);
		update();

	}

	
}

void SamGuiSteps::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(80,80,80));
	p.setBrush(QColor(0,0,0));
	p.drawRect(0,0,width()-1,height()-1);

	float stepWidth = ((float) ((float)width()-3.0)/(float)m_numSteps);
	
	float stepHeight = 96.0f;
	float valueHeight = stepHeight;
	float yOffset = 0.0f;

	if(m_midZero) {
		valueHeight /= 2.0f;
		yOffset += stepHeight;
	}

	

	
	
	//float velocityStep = (float)stepHeight/127.0f;


	
	bool beatGroup = false;
	int beatCounter = 0;

	QColor bgColor(25, 50, 25);

	for(int i=0; i<m_numSteps; i++) {
		//qDebug("Step %d/%d", i+1. m_numSteps);

		if(beatCounter >= 4) {
			beatCounter = 0;
			beatGroup = !beatGroup;
		}
		beatCounter++;


		if(!beatGroup) {
			bgColor = QColor(20, 40, 20);
		} else {
			bgColor = QColor(25, 50, 25);
		}

		/*if(beatGroup>=4) {
			//p.setPen(QColor(70,70,70));
			//p.drawLine(i*stepWidth, 1, i*stepWidth, height()-2);
			//beatGroup = 0;
		}
		beatGroup++;*/

		p.setPen(bgColor);
		p.setBrush(bgColor);
		p.drawRect(3+(i*stepWidth),3,stepWidth-4,stepHeight);

		
			//if(m_steps[i].value > 0) {
				if(m_steps[i].mute) {
					p.setPen(QColor(50,100,50));
					p.setBrush(QColor(50,100,50));
				} else {
					p.setPen(QColor(100,200,100));
					p.setBrush(QColor(100,200,100));
				}
				float h = valueHeight * (float)m_steps[i].value;
				//qDebug("stepHeight = %d (%f)", (int)h, h);
				//float h = step

				if(h > 0.0f) {
					p.drawRect(3+(i*stepWidth), 3+valueHeight-(int)h, stepWidth-4, (int)h);
				} else {
					p.drawRect(3+(i*stepWidth), 3+valueHeight, stepWidth-4, -h);
				}
				//p.drawRect(3+(i*stepWidth), 3+stepHeight-(int)h, stepWidth-4, (int)h);
			//}

			
			/*if(!m_steps[i].mute) {
				p.setPen(QColor(118,103,88));
				p.setBrush(QColor(118,103,88));
			} else {
				p.setPen(QColor(255,130,0));
				p.setBrush(QColor(255,130,0));
			}

			p.drawRect(3+(i*stepWidth), 3+stepHeight+3, stepWidth-4, 16);
			p.drawPixmap(2+(i*stepWidth)+((stepWidth/2)-8), 3+stepHeight+3, *pixMute);

			if(!m_steps[i].accent) {
				p.setPen(QColor(118,115,88));
				p.setBrush(QColor(118,115,88));
			} else {
				p.setPen(QColor(255,230,0));
				p.setBrush(QColor(255,230,0));
			}

			p.drawRect(3+(i*stepWidth), 3+stepHeight+3+16+3, stepWidth-4, 16);
			p.drawPixmap(2+(i*stepWidth)+((stepWidth/2)-8), 3+stepHeight+3+16+3, *pixAccent);*/
		
		
	}

	
}

int SamGuiSteps::xStep(int x)
{
	float stepWidth = (float) ((float)width()-1.0)/(float)m_numSteps;

	for(int i=0; i<m_numSteps; i++) {
		if(x >= 3+(i*stepWidth) && x <= (3+(i*stepWidth))+stepWidth-4) {
			return i;
		}
	}

	return -1;
}

float SamGuiSteps::yRange(int y)
{
	float stepHeight = 96;
	float pixelRange = 1.0f/(float)stepHeight;

	if(y >= 3 && y <= 3+stepHeight) {
		y -= 3;

		float value = y * pixelRange;
		value = 1.0f - value;

		if(m_midZero) {
			value *= 2.0f;
			value -= 1.0f;
		}

		qDebug("value = %f", value);
		return value;
	}

	return 0.0f;
}