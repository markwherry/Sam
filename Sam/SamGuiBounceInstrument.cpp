#include "SamGuiBounceInstrument.h"

#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

#include "Sam.h"
#include "ScBounceInstrument.h"

SamGuiBounceInstrument::SamGuiBounceInstrument(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	setWindowTitle("Bounce Instrument");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	setFixedSize(800,410);

	int y=110;
	for(int i=0; i<8; i++) {
		m_bounceGroups[i] = new SamGuiBounceGroup(this, i);
		m_bounceGroups[i]->move(10, y);
		y+=(m_bounceGroups[i]->height()-1);
		connect(m_bounceGroups[i], SIGNAL(activated(int)), this, SLOT(setActive(int)));
	}

	QPalette p = palette();
	p.setColor(QPalette::Dark, QColor(50,50,50));

	m_scrollArea = new QScrollArea(this);
	m_scrollArea->move(10+m_bounceGroups[0]->width(),10);
	m_scrollArea->setFixedSize(width()-20-m_bounceGroups[0]->width(), 340+10);
	m_scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	m_scrollArea->setFrameShape(QFrame::NoFrame);
	m_scrollArea->setPalette(p);
	m_scrollArea->setBackgroundRole(QPalette::Dark);

	m_bounceRows = new SamGuiBounceRows(this);
	//m_bounceRows->move(10+m_bounceGroups[0]->width(),10);
	m_scrollArea->setWidget(m_bounceRows);
	

	m_buttonBounce = new UiButton(this);
	m_buttonBounce->move(width()-10-m_buttonBounce->width(), height()-10-m_buttonBounce->height());
	m_buttonBounce->setText("Bounce");
	m_buttonBounce->setDefault(true);

	connect(m_buttonBounce, SIGNAL(clicked()), this, SLOT(bounce()));

	m_buttonCancel = new UiButton(this);
	m_buttonCancel->move(m_buttonBounce->x()-10-m_buttonCancel->width(), m_buttonBounce->y());
	m_buttonCancel->setText("Cancel");

	connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));
}

SamGuiBounceInstrument::~SamGuiBounceInstrument()
{

}

void SamGuiBounceInstrument::setChannel(SamChannel *channel)
{
	m_channel = channel;
	m_bounceRows->setChannel(channel);

	//update();
}

void SamGuiBounceInstrument::setActive(int id)
{
	m_bounceRows->setActive(id, m_bounceGroups[id]->isActive());
}

void SamGuiBounceInstrument::bounce(void)
{
	// Verify we can bounce something
	// ------------------------------
	int activeGroups = 0;
	bool activeStreams = false;
	for(int i=0; i<8; i++) {
		if(m_bounceGroups[i]->isActive()) {
			activeGroups++;
			if(m_bounceRows->row(i)->numStreamsSelected() > 0) {
				activeStreams = true;
			} else {
				activeStreams = false;
				break;
			}
		}
		
	}

	if(activeGroups < 1 || !activeStreams) {
		QMessageBox::critical(this, "Sam", "<b>The bounce configuration is not set correctly.</b><br><br>Please enable at least one Bounce Group, and make sure at least one Stream is enabled for each active Bounce Group.");
		return;
	}

	hide();

	SamInstrument *instrument = m_channel->instrument();

	// Prompt the user where the new instrument file should be saved
	// -------------------------------------------------------------
	QString path;
	if(instrument->path().length()>0) {
		path = instrument->path().toChar();
	}
	QString file = QFileDialog::getSaveFileName(this, "Type in a name for the bounced Instrument:", path, "Sam Instrument Files (*.sam)");
	if(file.size()<1) {
		qDebug("Incorrect filename.");	
		return;
	}
	QByteArray ba = file.toAscii();

	// Create Bounce Groups
	// --------------------

	AList<SamBounceGroup*> *bounceGroups = new AList<SamBounceGroup*>;
	for(int i=0; i<8; i++) {
		if(m_bounceGroups[i]->isActive()) {
			SamBounceGroup *bg = m_bounceGroups[i]->createBounceGroup();
			bounceGroups->append(bg);
			for(int j=0; j<m_channel->streamMix()->numStreams(); j++) {
				bg->setStreamEnabled(j, m_bounceRows->isStreamSelected(i, j));
			}
		}
	}


	samGui->commandManager()->executeInBackground(new ScBounceInstrument(instrument, ba.data(), m_channel->streamMix(), bounceGroups));
}

void SamGuiBounceInstrument::paintEvent(QPaintEvent *ev)
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
}