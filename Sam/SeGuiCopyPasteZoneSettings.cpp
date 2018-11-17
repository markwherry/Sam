#include "SeGuiCopyPasteZoneSettings.h"

#include <QPainter>
#include <QMessageBox>

#include "SamGuiEditor.h"

#include "Sam.h"
#include "SamCommands.h"

SeGuiCopyPasteZoneSettings::SeGuiCopyPasteZoneSettings(QWidget *parent)
	: QWidget(parent)
	, m_zone(0)
{
	//ui.setupUi(this);

	

	setWindowTitle("Copy / Paste Zone Settings");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	m_buttonCopy = new UiButton(this);
	m_buttonCopy->move(10,10);
	m_buttonCopy->setFixedWidth(200);
	m_buttonCopy->setText("Copy Zone Settings");
	//m_buttonCopy->setDefault(true);
	connect(m_buttonCopy, SIGNAL(clicked()), this, SLOT(copy()));

	m_buttonPaste = new UiButton(this);
	m_buttonPaste->move(10, 50);
	m_buttonPaste->setFixedWidth(200);
	m_buttonPaste->setText("Paste Zone Settings");
	//m_buttonCopy->setDefault(true);
	connect(m_buttonPaste, SIGNAL(clicked()), this, SLOT(paste()));

	QString zoneSettings[11] = { "Zone Sample Start",
		                        "Levels",
						   	    "Pitch (Except Hi/Lo Mapping)",
							    "Velocity (Except Hi/Lo Mapping)",
							    "Triggering",
							    "Filter",
							    "Envelope 1 (Amplifier)",
							    "Envelope 2 (Release)",
							    "Envleope 3 (Filter)",
							    "Tempo Release Modulation",
							    "Crossfade" };

	int y = 100;
	for(int i=0; i<11; i++) {
		m_check[i] = new UiCheckBox(this);
		m_check[i]->move(m_buttonPaste->x(), y);
		y += (m_check[i]->height() + 10);
		m_check[i]->setText(zoneSettings[i]);
		//m_check[i]->setChecked(true);
	}

	setFixedSize(220,y);
}

SeGuiCopyPasteZoneSettings::~SeGuiCopyPasteZoneSettings()
{

}

void SeGuiCopyPasteZoneSettings::copy(void)
{
	SamLayer *layer = samEditor->layer();
	
	if(layer->numSelectedZones() == 0) {
		QMessageBox::critical(this, "Sam", "<b>There is no Zone selected.</b><br><br>Please select the Zone whose setting you want to be copied.");
		return;
	} else if(layer->numSelectedZones() > 1) {
		QMessageBox::critical(this, "Sam", "<b>There is more than one Zone selected.</b><br><br>Please select only the Zone whose setting you want to be copied.");
		return;
	}

	SamZone *z = 0;
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->isSelected()) {
			z = layer->zone(i);
			break;
		}
	}

	if(z) {
		if(m_zone) {
			delete m_zone;
		}
		m_zone = new SamZone(0, *z, 0, true);
	}

	m_buttonPaste->setDisabled(false);
}

void SeGuiCopyPasteZoneSettings::paste(void)
{
	if(!m_zone) {
		QMessageBox::critical(this, "Sam", "<b>No Zone has been copied.</b><br><br>Please select the Zone whose setting you want to be copied and click the copy button before using the paste feature.");
		return;
	}

	char settings[11];
	for(int i=0; i<11; i++) {
		settings[i] = 0;
		if(m_check[i]->isChecked()) {
			settings[i] = 1;
		}
	}

	samGui->commandManager()->executeInBackground(new PasteZoneSettings(m_zone, settings, samEditor->layer()));
}

void SeGuiCopyPasteZoneSettings::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));
	p.fillRect(0, 0, width(), 90, QColor(50,50,50));

	//EStyle::paintBackground(&p,this);
	//p.setPen(QColor(70,70,70));
	//p.setBrush(QColor(70,70,70));
	//p.drawRect(0,0,width()-1, height()-1);

	/*QPen pen(QColor(55,55,55));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}*/
}

void SeGuiCopyPasteZoneSettings::showEvent(QShowEvent *ev)
{
	if(!m_zone) {
		m_buttonPaste->setDisabled(true);
	}
}