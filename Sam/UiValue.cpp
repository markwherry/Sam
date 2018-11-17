#include "UiValue.h"

#include <QPainter>
#include <QMouseEvent>

#include "CsMidiName.h"

UiValue::UiValue(QWidget *parent, int w, int h, QString unit, float min, float max, float def, float step, int pixels)
	: QWidget(parent)
	, m_unit(unit)
	, m_min(min)
	, m_max(max)
	, m_def(def)
	, m_value(def)
	, m_step(step)
	, m_pixels(pixels)
	, m_drawArrows(false)
	, m_liveUpdate(false)
	, m_extraInfo(-1)
	, m_editable(true)
{
	//ui.setupUi(this);

	setFixedSize(w, h);

	m_lineEdit = new QLineEdit(this);
	m_lineEdit->setFixedSize(w, h);
	m_lineEdit->hide();
	connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(acceptEdit()));
	previousKeyboardGrabber = 0;
	previousFocus = 0;

	m_font = QFont("Calibri", 10);
}

UiValue::~UiValue()
{

}

void UiValue::setValue(float value)
{
	m_value = value;

	update();
}

void UiValue::setValueNormalised(float value)
{
	float displayValue = m_min+((m_max-m_min)*value);
	setValue(displayValue);
}

void UiValue::acceptEdit(void)
{
	qDebug("UiValue::acceptEdit()");
	
//	releaseMouse();
	m_lineEdit->releaseKeyboard();
	if(previousKeyboardGrabber) {
		qDebug("Grabbing old keyboard");
		previousKeyboardGrabber->grabKeyboard();
	}

	if(previousFocus) {
		previousFocus->setFocus();
	}
	//m_text = m_lineEdit->text();
	//emit textChanged(m_text);
	m_value = m_lineEdit->text().toFloat();
	validateValue();
	emit valueChanged(m_value);
	emit valueChangedNormalised((1.0f/(m_max-m_min)) * ((m_max-m_min) - (m_max-m_value)));
	//emit valueChangedNormalised((1.0/(m_max-m_min))*m_value);

	m_lineEdit->hide();
	update();
	
	qDebug("EXIT: UiValue::acceptEdit()");
}

/*bool UiValue::event(QEvent *ev)
{
	qDebug("UiValue::event() %d", ev->type());

	return QWidget::event(ev);
}*/

void UiValue::enterEvent(QEvent *ev)
{
	if(isEnabled()) {
		m_drawArrows = true;
	}
	update();
}

void UiValue::leaveEvent(QEvent *ev)
{
	m_drawArrows = false;
	update();
}

void UiValue::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if(m_editable) {

		//m_lineEdit->setText(m_text);
		m_lineEdit->setText(QString::number((int)m_value));
		m_lineEdit->selectAll();
		m_lineEdit->show();

		previousFocus = QWidget::focusWidget();
		m_lineEdit->setFocus();

//		grabMouse();
	
		previousKeyboardGrabber = QWidget::keyboardGrabber();
		qDebug("prviousKeyboardGrabber = %d", previousKeyboardGrabber);

		m_lineEdit->grabKeyboard();

		
	}
}

void UiValue::mouseMoveEvent(QMouseEvent *ev)
{
	QPoint point = mapToGlobal(ev->pos());
	int yDiff = yClick - point.y();
	int xDiff = xClick - point.x();

	if(directionAdjustment == 1) { // Vertical
		float pd = yDiff/m_pixels; //qDebug("pd = %f (%d/%d)", pd, yDiff, m_pixels);
		m_value = (initialValue + (pd * m_step));
	} else if(directionAdjustment == 2) { // Horizontal
		xDiff = -xDiff;
		float pd = xDiff/2.0f;
		m_value = (initialValue + (pd * (m_step*4)));
	}

	if(directionAdjustment == 0) {
		if(yDiff > 5 || yDiff < -5) {
			directionAdjustment = 1;
			yClick = point.y();
			//qDebug("Going up");
		} else if(xDiff > 5 || xDiff < -5) {
			directionAdjustment = 2;
			xClick = point.x();
			//qDebug("Going sideways");
		}
	}

	validateValue();

	if(m_liveUpdate) {
		emit valueChanged(m_value);

		emit valueChangedNormalised((1.0f/(m_max-m_min)) * ((m_max-m_min) - (m_max-m_value)));

		//emit valueChangedNormalised((1.0/(m_max-m_min))*m_value);
	}

	
	/*if(yDirection==0) {
		if(yDiff>0) {
			yDirection = 1;
		} else {
			yDirection = -1;
		}
	} else {
		if(yDirection>0 && yLast-point.y()<0) {
			yClick = point.y();
			yDiff = 0;
			yDirection = -1;
			initialValue = m_value;
		} else if(yDirection<0 && yLast-point.y()>0) {
			yClick = point.y();
			yDiff = 0;
			yDirection = 1;
			initialValue = m_value;
		}
	}

	int pd = yDiff/m_pixels;
	//qDebug("pd=%d",pd);
	m_value = initialValue + (float)(pd*m_step);

	validateValue();

	if(m_liveUpdate) {
		emit valueChanged(m_value);
		emit valueChangedNormalised((1.0/(m_max-m_min))*m_value);
	}*/
	
	update();

	yLast = point.y();
}

void UiValue::mousePressEvent(QMouseEvent *ev)
{
	if(!m_lineEdit->isHidden() && !m_lineEdit->rect().contains(ev->x(), ev->y())) {
		qDebug("1");
		acceptEdit();	
		qDebug("2");
	} else {
		directionAdjustment = 0;
		QPoint point = mapToGlobal(ev->pos());
		yClick = point.y();
		xClick = point.x();
		yDirection = 0;
		initialValue = m_value;
	}
}

void UiValue::mouseReleaseEvent(QMouseEvent *ev)
{
	m_drawArrows = false;

	if(!m_liveUpdate) {
		emit valueChanged(m_value);
		//emit valueChangedNormalised((1.0/(m_max-m_min))*m_value);
		emit valueChangedNormalised((1.0f/(m_max-m_min)) * ((m_max-m_min) - (m_max-m_value)));
	}
}

void UiValue::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	m_valueTextColor.setAlpha(255);
	m_unitTextColor.setAlpha(255);

	if(!isEnabled()) {
		m_valueTextColor.setAlpha(100);
		m_unitTextColor.setAlpha(100);
	}


	p.setPen(m_valueTextColor);
	p.setFont(m_font);
	QString text = QString::number((int)m_value);

	if(m_extraInfo==1) {
		text.append(QString(" ")+QString("(")+QString(CsMidiName::pitchToName((int)m_value,3,0).toChar())+QString(")"));	
	} else if(m_extraInfo==2) {
		switch((int)m_value) {
			case 0:
				text.append(" (Not Assigned)");
				break;
			case 1:
				text.append(" (pppp)");
				break;
			case 2:
				text.append(" (ppp)");
				break;
			case 3:
				text.append(" (pp)");
				break;
			case 4:
				text.append(" (p)");
				break;
			case 5:
				text.append(" (mp)");
				break;
			case 6:
				text.append(" (mf)");
				break;
			case 7:
				text.append(" (f)");
				break;
			case 8:
				text.append(" (ff)");
				break;
			case 9:
				text.append(" (fff)");
				break;
			case 10:
				text.append(" (ffff)");
				break;
		}
	} else if(m_extraInfo==3) {
		switch((int)m_value) {
			case 0:
				text.append(" (Single Cell)");
				break;
			case 1:
				text.append(" (Round Robin)");
				break;
			case 2:
				text.append(" (Random)");
				break;
		}
	} else if(m_extraInfo==4) {
		switch((int)m_value) {
			case 0:
				text.append(" (Zone via Vel)");
				break;
			case 1:
				text.append(" (Zones via Cf)");
				break;
		}
	} else if(m_extraInfo==5) {
		switch((int)m_value) {
			case -1:
				text.append(" (Not Assigned)");
				break;
			case 0:
				text.append(" (Long)");
				break;
			case 5:
				text.append(" (Espressivo)");
				break;
			case 10:
				text.append(" (Mutes)");
				break;
			case 15:
				text.append(" (Trills)");
				break;
			case 20:
				text.append(" (Tremolo)");
				break;
			case 25:
				text.append(" (Harmonics Long)");
				break;
			case 30:
				text.append(" (Harmonics Tremolo)");
				break;
			case 35:
				text.append(" (Tremolo, Sul Pont)");
				break;
			case 60:
				text.append(" (Short)");
				break;
			case 65:
				text.append(" (Quarter Notes)");
				break;
			case 70:
				text.append(" (Harmonics Accents)");
				break;
			case 75:
				text.append(" (Pizzicato)");
				break;
			case 80:
				text.append(" (Short Mutes)");
				break;
			case 85:
				text.append(" (Col Legno)");
				break;
			case 100:
				text.append(" (Single Hits)");
				break;
			case 105:
				text.append(" (Flams)");
				break;
		}
	} else if(m_extraInfo==6) {
		switch((int)m_value) {
			case 0:
				text.append(" (Single Layer)");
				break;
			case 1:
				text.append(" (Cycle Layers)");
				break;
			case 2:
				text.append(" (All Layers)");
				break;
			case 3:
				text.append(" (Legato)");
				break;
		}
	} else if(m_extraInfo==7) {
		switch((int)m_value) {
			case 0:
				text.append(" (Switch)");
				break;
			case 1:
				text.append(" (Scale)");
				break;
		}
	} else if(m_extraInfo==8) { // Parameter Type
		switch((int)m_value) {
			case 0:
				text.append(" (Trigger)");
				break;
			case 1:
				//text.append(" (Toggle)");
				break;
			case 2:
				text.append(" (Range)");
				break;
		}
	} else if(m_extraInfo==9) { // MIDI Type
		switch((int)m_value) {
			case 0:
				text.append(" (Note Pitch)");
				break;
			case 1:
				text.append(" (Note Velocity)");
				break;
			case 2:
				text.append(" (Controller)");
				break;
		}
	} else if(m_extraInfo==10) { // Parameter Target/Source
		switch((int)m_value) {
			case 10:
				text.append(" (Layer Select Mode)");
				break;
			case 11:
				text.append(" (Switch To Layer)");
				break;

			case 40:
				text.append(" (Env 1 Amp Attack)");
				break;
			case 41:
				text.append(" (Env 1 Amp Hold)");
				break;
			case 42:
				text.append(" (Env 1 Amp Decay)");
				break;
			case 43:
				text.append(" (Env 1 Amp Sustain)");
				break;
			case 44:
				text.append(" (Env 1 Amp Release)");
				break;
			case 45:
				text.append(" (Env 1 Amp Attack Offset)");
				break;

			case 50:
				text.append(" (Zone Sample Start)");
				break;
			case 51:
				text.append(" (Cell Source Start)");
				break;

			case 60:
				text.append(" (Minimum Velocity)");
				break;

			case 70:
				text.append(" (Filter Cut-off)");
				break;

			case 140:
				text.append(" (Env 2 Rel Attack)");
				break;
			case 141:
				text.append(" (Env 2 Rel Hold)");
				break;
			case 142:
				text.append(" (Env 2 Rel Decay)");
				break;
			case 143:
				text.append(" (Env 2 Rel Sustain)");
				break;
			case 144:
				text.append(" (Env 2 Rel Release)");
				break;
			//case 145:
			//	text.append(" (Env 2 Rel Attack Offset)");
			//	break;
		}
	} else if(m_extraInfo == 11) {
		switch((int)m_value) {
			case 0:
				text.append(" (Off)");
				break;
			case 1:
				text.append(" (Up)");
				break;
			case 2:
				text.append(" (Down)");
				break;
		}
	}
	
	if(m_unit.length() > 0) {
		p.drawText(0, 0, width()-1,height()-1, Qt::AlignLeft|Qt::AlignVCenter, text+" "+m_unit);
	} else {
		p.drawText(0, 0, width()-1,height()-1, Qt::AlignLeft|Qt::AlignVCenter, text);
	}

	//p.setPen(m_unitTextColor);
	//p.setFont(QFont("",0,0));
	//p.drawText(0, 0, width()-1,height()-1, Qt::AlignRight|Qt::AlignVCenter, m_unit);

	if(m_drawArrows) {
		//p.drawPixmap(0,1,*pixArrowUp);
		//p.drawPixmap(0,height()-1-pixArrowDown->height(),*pixArrowDown);
	}
}

void UiValue::validateValue(void)
{
	if(m_value>(float)m_max)
		m_value = (float)m_max;
	if(m_value<(float)m_min)
		m_value = (float)m_min;
}