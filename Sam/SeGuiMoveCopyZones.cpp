#include "SeGuiMoveCopyZones.h"

#include <QPainter>

#include "Sam.h"
#include "SamCommands.h"

SeGuiMoveCopyZones::SeGuiMoveCopyZones(QWidget *parent)
	: QWidget(parent)
	, m_layer(0)
{
	//ui.setupUi(this);

	setFixedSize((90*3)+(10*4), 120);

	setWindowTitle("Move / Copy Zones");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	setWindowModality(Qt::ApplicationModal);

	m_semitones = new QSpinBox(this);
	m_semitones->move(10,10);
	m_semitones->setSuffix(" semitones");
	m_semitones->setMinimum(-48);
	m_semitones->setMaximum(48);

	m_transposeRootNote = new UiCheckBox(this);
	m_transposeRootNote->move(10, 40);
	m_transposeRootNote->setText("Transpose Root Note");
	m_transposeRootNote->setChecked(true);

	m_buttonMove = new UiButton(this);
	int x = width()-10-m_buttonMove->width();
	m_buttonMove->move(x, height()-10-m_buttonMove->height());
	m_buttonMove->setText("Move");
	m_buttonMove->setDefault(true);
	connect(m_buttonMove, SIGNAL(clicked()), this, SLOT(move()));

	m_buttonCopy = new UiButton(this);
	x -= (m_buttonCopy->width()+10);
	m_buttonCopy->move(x, height()-10-m_buttonCopy->height());
	m_buttonCopy->setText("Copy");
	connect(m_buttonCopy, SIGNAL(clicked()), this, SLOT(copy()));

	m_buttonCancel = new UiButton(this);
	x -= (m_buttonCancel->width()+10);
	m_buttonCancel->move(x, height()-10-m_buttonCancel->height());
	m_buttonCancel->setText("Cancel");
	connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));
}

SeGuiMoveCopyZones::~SeGuiMoveCopyZones()
{

}

void SeGuiMoveCopyZones::setLayer(SamLayer *layer)
{
	m_layer = layer;
}

void SeGuiMoveCopyZones::move(void)
{
	hide();

	if(m_layer) {
		samGui->commandManager()->executeInBackground(new MoveCopyZones(m_layer, m_semitones->value(), m_transposeRootNote->isChecked(), false));
	}
}

void SeGuiMoveCopyZones::copy(void)
{
	hide();

	if(m_layer) {
		samGui->commandManager()->executeInBackground(new MoveCopyZones(m_layer, m_semitones->value(), m_transposeRootNote->isChecked(), true));
	}
}

void SeGuiMoveCopyZones::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));

	//EStyle::paintBackground(&p,this);
	/*p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}*/
	//
	//p.setPen(QColor(0,0,0,140));
	//p.setBrush(QColor(0,0,0,140));
	//p.drawRect(0,0,width()-1,height()-1);

	//p.setPen(QColor(255,255,255));
	//p.setFont(QFont("Calibri", 10)); //Select the Instrument to be merged as a new layer in the current Instrument:
	//p.drawText(10,10,width(), 40, Qt::AlignLeft|Qt::AlignTop, "Select the Instrument to copy as a new Layer\nin the currently selected Instrument.");
}