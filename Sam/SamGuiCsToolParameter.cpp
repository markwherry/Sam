#include "SamGuiCsToolParameter.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMenu>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiCsToolParameter::SamGuiCsToolParameter(QWidget *parent, SamTool *tool, SamToolParameter* parameter)
	: QWidget(parent)
	, m_tool(tool)
	, m_parameter(parameter)
	, m_menuLearn(0)
{
	//ui.setupUi(this);

	setFixedSize(400,30); //y=21

	//m_id = id;
	//m_name = name.c_str();

	//slider = new SamGuiSlider(this, 125);
	//slider->move(160,8);
	//slider->setValue(defaultValue);
	//connect(slider, SIGNAL(valueChanged(float)), this, SLOT(internalSetValue(float)));
	m_fader = new UiFader(this, false, 160, 16, 8, 0);
	m_fader->setLevelColor(QColor(0, 255, 255)); // 70, 70, 200
	m_fader->setPower(false);
	m_fader->move(200, (height()/2)-(m_fader->height()/2));
	connect(m_fader, SIGNAL(valueChanged(float, int)), this, SLOT(updateFromSlider(float, int)));

	m_value = new UiValue(this, 80,20, m_parameter->unit().toChar(), m_parameter->minValue(), m_parameter->maxValue(), 0, 1, 2);
	//m_value->setValueNormalised(defaultValue);
	m_value->setValueTextFont(QFont("",0,QFont::Bold));
	m_value->setValueTextColor(QColor(240,240,240));
	m_value->setUnitTextColor(QColor(220,220,220));
	m_value->move(100, (height()/2)-(m_value->height()/2));
	connect(m_value, SIGNAL(valueChangedNormalised(float)), this, SLOT(updateFromValue(float)));

	m_menuLearn = new QMenu(0);
	m_actionLearn   = m_menuLearn->addAction("Learn MIDI Controller");
	m_actionUnlearn = m_menuLearn->addAction("Unlearn MIDI Controller");
	connect(m_actionLearn,   SIGNAL(triggered()), this, SLOT(learn()));
	connect(m_actionUnlearn, SIGNAL(triggered()), this, SLOT(unlearn()));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiCsToolParameter::~SamGuiCsToolParameter()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::setValueNormalised(float value)
{
	m_value->setValueNormalised(value);
	m_fader->setValueNormalised(value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::updateFromSlider(float value, int flags)
{
	m_value->setValueNormalised(value);
	emit parameterChanged(m_parameter->id(), value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::updateFromValue(float value)
{
	m_fader->setValueNormalised(value);
	emit parameterChanged(m_parameter->id(), value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::learn(void)
{
	m_tool->setLearningParameter(m_parameter->id());

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::unlearn(void)
{
	m_parameter->setMidiController(-1);
	m_tool->setLearningParameter(-1);

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::mousePressEvent(QMouseEvent *ev)
{
	if(ev->button() == Qt::RightButton) {
		if(m_parameter->midiController() > -1 || m_tool->learningParameter() > -1) {
			m_actionUnlearn->setEnabled(true);
		} else {
			m_actionUnlearn->setEnabled(false);
		}
		m_menuLearn->exec(ev->globalPos());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolParameter::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	/*p.setPen(QColor(1,1,1,100));
	p.setBrush(QColor(0,0,0,0));
	p.drawRect(0,0,width()-1,height()-1); //*/
	//p.fillRect(3,2,width()-6,height()-4, QColor(1,1,1,100));

	QColor c(0,0,0);
	if(m_parameter->midiController() > -1) {
		c = QColor(0, 255, 255, 50);	
	}
	if(m_tool->learningParameter() == m_parameter->id()) {
		c = QColor(100,0,0,50);
	}

	if(c != QColor(0,0,0)) {
		p.fillRect(3,2,width()-6,height()-4, c);
	}

	p.setPen(QColor(220,220,220));
	QFont font("Verdana",7,QFont::Bold);
	font.setStyleStrategy(QFont::PreferAntialias);
	p.setFont(font);
	//QByteArray ba = m_parameter->name()toAscii();
	p.drawText(0,0,90,height()-1,Qt::AlignRight|Qt::AlignVCenter, m_parameter->name().toChar());

	if(m_parameter->midiController() > -1) {
		p.drawText(width()-35,0,35,height(), Qt::AlignLeft|Qt::AlignVCenter, QString("CC ")+QString::number(m_parameter->midiController()));
	}
}