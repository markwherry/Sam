#include "SamGuiCopyLayerFromInstrument.h"

#include <QPainter>
#include "SamEngine.h"

#include "Sam.h"
#include "SamCommands.h"

SamGuiCopyLayerFromInstrument::SamGuiCopyLayerFromInstrument(QWidget *parent)
	: QWidget(parent)
	, m_instrument(0)
{
	//ui.setupUi(this);

	setFixedSize(270, 120);

	setWindowTitle("Copy Layer from Instrument");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	setWindowModality(Qt::ApplicationModal);

	m_selectInstrument = new UiSelectMenu(this, 250);
	m_selectInstrument->move(10,50);
	//m_selectInstrument->setDrawBackground();

	m_ok = new QPushButton("Copy", this);
	m_ok->setDefault(true);
	m_ok->move(185,90);

	m_cancel = new QPushButton("Cancel", this);
	m_cancel->move(10, 90);

	connect(m_ok, SIGNAL(clicked()), this, SLOT(ok()));
	connect(m_cancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

SamGuiCopyLayerFromInstrument::~SamGuiCopyLayerFromInstrument()
{

}

void SamGuiCopyLayerFromInstrument::setInstrument(SamInstrument *instrument)
{
	m_instrument = instrument;

	if(m_instrument) {
		m_selectInstrument->clear();
		for(int i=0; i<samEngine->numInstruments(); i++) {
			m_selectInstrument->addItem(samEngine->instrument(i)->name().toChar());
		}
		m_selectInstrument->setEnabled(true);
		m_ok->setEnabled(true);
	} else {
		m_selectInstrument->addItem("No Instruments Loaded");
		m_selectInstrument->setEnabled(false);
		m_ok->setEnabled(false);
	}
}

void SamGuiCopyLayerFromInstrument::ok(void)
{
	hide();

	samGui->commandManager()->executeInBackground(new CopyLayerFromInstrument(samEngine->instrument(m_selectInstrument->selectedItem()), m_instrument));
}

void SamGuiCopyLayerFromInstrument::cancel(void)
{
	hide();
}

void SamGuiCopyLayerFromInstrument::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width()-1, height()-1, QColor(70,70,70));

	//EStyle::paintBackground(&p,this);
	p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}
	//
	//p.setPen(QColor(0,0,0,140));
	//p.setBrush(QColor(0,0,0,140));
	//p.drawRect(0,0,width()-1,height()-1);

	p.setPen(QColor(255,255,255));
	p.setFont(QFont("Calibri", 10)); //Select the Instrument to be merged as a new layer in the current Instrument:
	p.drawText(10,10,width(), 40, Qt::AlignLeft|Qt::AlignTop, "Select the Instrument to copy as a new Layer\nin the currently selected Instrument.");
	
}