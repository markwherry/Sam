/*#include "SamGuiPerVoiceFxGroupEditor.h"

#include <qpainter.h>
#include <QMouseEvent>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiPerVoiceFxGroupEditor::SamGuiPerVoiceFxGroupEditor(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
	, m_adjustVelocity(false)
{
	//ui.setupUi(this);

	setFixedSize(880, 40);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiPerVoiceFxGroupEditor::~SamGuiPerVoiceFxGroupEditor()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiPerVoiceFxGroupEditor::setChannel(SamChannel *channel)
{
	m_channel = channel;

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiPerVoiceFxGroupEditor::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if(!m_channel) return;

	int zone = validateZone(ev->y());

	for(int i=0; i<88; i++) {
		m_channel->setPerVoiceGroup(i+21, zone, m_channel->currentPerVoiceGroup());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiPerVoiceFxGroupEditor::mouseMoveEvent(QMouseEvent *ev)
{
	if(!m_channel) return;

	if(m_adjustVelocity) {

		float diff = m_pixel - ev->y();
		
		diff = diff / 4.0f;
		int id = m_clickVelocity + (int)diff;
		//m_value = (initialValue + (pd * m_step));

		if(id< 1) id = 1;
		if(id > 127) id = 127;
		m_channel->setPerVoiceVelocitySplit(id);
	
	} else {
		int pitch = validatePitch(ev->x());
		int zone = validateZone(ev->y());

		m_channel->setPerVoiceGroup(pitch+21, zone, m_channel->currentPerVoiceGroup());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiPerVoiceFxGroupEditor::mousePressEvent(QMouseEvent *ev)
{
	if(!m_channel) return;
		
	if(ev->modifiers()&Qt::ShiftModifier) {
		m_adjustVelocity = true;
		m_pixel = ev->y();
		m_clickVelocity = m_channel->perVoiceVelocitySplit();
	} else {
		int pitch = validatePitch(ev->x());
		int zone = validateZone(ev->y());

		m_channel->setPerVoiceGroup(pitch+21, zone, m_channel->currentPerVoiceGroup());
		
	}
	
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiPerVoiceFxGroupEditor::mouseReleaseEvent(QMouseEvent *ev)
{
	m_adjustVelocity = false;

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiPerVoiceFxGroupEditor::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	QColor groupColors[15] = { QColor(143,26,26), QColor(143,26,143), QColor(84,26,143),
		                       QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
						    	QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	bool isBlackNote[12] = { false, true, false, false, true, false, true, false, false, true, false, true };

	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(255,255,255));
	
	for(int i=0; i<88; i++) {

		if(isBlackNote[i%12]) {
			p.setBrush(QColor(160,160,160));
		} else {
			p.setBrush(QColor(200,200,200));
		}
		if(m_channel) {
			float vu = (float)height() / 128.0;
			float vd = (float)m_channel->perVoiceVelocitySplit() * vu;
			int  vdi = height() - (int)vd;

			

			//p.drawRect(i*10, 0, 10, 20);
			//p.drawRect(i*10, 20, 10, 19);

			p.drawRect(i*10, 0, 10, vdi);
			p.drawRect(i*10, vdi, 10, height() - vdi - 1);

		
			QColor gc1 = groupColors[m_channel->perVoiceGroup(i+21, 1)];
			QColor gc2 = groupColors[m_channel->perVoiceGroup(i+21, 0)];
			gc1.setAlpha(150);
			gc2.setAlpha(150);

			
			//p.fillRect((i*10)+1, 1, 9, 19, gc1);
			//p.fillRect((i*10)+1, 21, 9, 18, gc2);
			p.fillRect((i*10)+1, 1, 9, vdi-1, gc1);
			p.fillRect((i*10)+1, vdi+1, 9, height() - vdi - 2, gc2);

			
		}
	}

	if(m_channel && m_adjustVelocity) {
				p.setPen(QColor(0,0,0,200));
				p.setFont(QFont("Calibri", 28));
				p.drawText(10,0,100,height(),Qt::AlignVCenter|Qt::AlignLeft,QString::number(m_channel->perVoiceVelocitySplit()));
			}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiPerVoiceFxGroupEditor::validatePitch(int x)
{
	return x/10;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiPerVoiceFxGroupEditor::validateZone(int y)
{
	if(y > 20) return 0;

	return 1;
}*/