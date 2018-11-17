#include "SamGuiArticulationEditor.h"

#include <QMouseEvent>
#include <QPainter>

#include "SamEngine.h"

#include "Sam.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiArticulationEditor::SamGuiArticulationEditor(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	connect(this, SIGNAL(articulationUpdated()), this, SLOT(update()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiArticulationEditor::~SamGuiArticulationEditor()
{
	samEngine->unregisterListener(this, SamEngine::ContextChannel);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationEditor::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannel::onState()");

	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemArticulation:
				emit articulationUpdated();
				break;
			case SamChannel::ItemArticulationMode:
				emit articulationUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationEditor::setChannel(SamChannel *channel)
{
	m_channel = channel;

	m_shorts.clear();
	m_longs.clear();

	if(m_channel) {
		for(int i=0; i<m_channel->instrument()->version(0)->numArticulations(); i++) {
			SamArticulation *art = m_channel->instrument()->version(0)->articulation(i);
			if(art->containsLoops()) {
				m_longs.append(i);
			} else {
				m_shorts.append(i);
			}
		}
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
QString SamGuiArticulationEditor::formatArticulationName(QString str)
{
	if(str.contains(" ")) {
		for(int j=0; j<str.length(); j++) {
			if(str.at(j) == ' ') {
				QString strl = str.left(j);
				QString strr = str.right(str.length()-j-1);
				str = strl + QString("\n") + strr;
				return str;
			}
		}
	} else if(str.contains("/")) {
		for(int j=0; j<str.length(); j++) {
			if(str.at(j) == '/') {
				QString strl = str.left(j+1);
				QString strr = str.right(str.length()-j-1);
				str = strl + QString("\n") + strr;
				return str;
			}
		}
	}

	return str;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationEditor::mousePressEvent(QMouseEvent *ev)
{
	int index = articulationIndexFromPoint(ev->pos());
	qDebug("artId = %d", index);

	if(index > -1) {
		if(ev->modifiers() & Qt::ControlModifier) {
			SamEvent ev(SamEvent::Parameter);
			ev.subType = SamParameter::ArticulationDefault;
			ev.iValue = index;
			ev.from = this;
			m_channel->addEvent(ev);
		} else {
			SamEvent ev(SamEvent::Parameter);
			ev.subType = SamParameter::Articulation;
			ev.iValue = index;
			ev.from = this;
			m_channel->addEvent(ev);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiArticulationEditor::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(78,78,78));


	// Draw the short articulations
	// ----------------------------

	p.setPen(QColor(255,255,255,60));

	int y = 15;

	for(int i=0; i<5; i++) { 
		p.drawLine(0, y+(6*i), width(), y+(6*i));
	}

	int x = 54;
	p.setPen(QColor(255,255,255,200));
	p.setBrush(QColor(0,0,0,0));
	p.setFont(QFont("Calibri", 8));

	for(int i=0; i<m_shorts.size(); i++) {
		QString str;
		if(m_channel) {
			str = m_channel->instrument()->version(0)->articulation(m_shorts[i])->name().toChar();
			p.drawPixmap(x+2,y-6, *samGui->articulationPixmap(2, true));
			if(m_shorts[i] == m_channel->currentArticulationIndex() || (m_channel->isArticulationMode() && m_shorts[i] == m_channel->currentDefaultArticulationIndex())) {
				if(m_channel->isArticulationMode() && m_shorts[i] == m_channel->currentArticulationIndex()) {
					p.setPen(QColor(149,255,25));
				}

				p.drawRect(x+1, y-7, 38, 38);
				p.drawRect(x+2, y-6, 36, 36);
			}
		}

		
		p.setPen(QColor(255,255,255,200));
		str = formatArticulationName(str);
		p.drawText(x-18, y+35, 72, 40, Qt::AlignTop|Qt::AlignHCenter, str);
		
		//p.drawRect(x, 14, 36, 36);

		x+=90;
	}



	// Draw the long articulations
	// ---------------------------

	y = 90; //height() - 150;
	x = 54;

	if(m_channel) {
		if(m_channel->isArticulationMode()) {
			QPen pen(QColor(149,255,25));
			pen.setWidth(2);
			p.setPen(pen);
			int fromArt = 0;
			int toArt   = 0;
			for(int i=0; i<m_longs.size(); i++) {
				if(m_longs[i] == m_channel->currentArticulationIndex()) {
					toArt = i;
				} 
				if(m_longs[i] == m_channel->currentDefaultArticulationIndex()) {
					fromArt = i;
				}
			}
		
			int x1,x2,y1;

			if(toArt > fromArt) { 
				x1 = (x + 38) + (90 * fromArt);
				y1 = (y - 7) + 19;
				x2 = x + (90 * toArt);
				p.drawLine(x1, y1, x2, y1);
				p.drawLine(x2, y1, x2 - 16, y1 - 8);
				p.drawLine(x2, y1, x2 - 16, y1 + 8);
			} else if(toArt < fromArt) { //qDebug("%d > %d", toArt, fromArt);
				x1 = x + (90 * fromArt);
				y1 = (y - 7) + 19;
				x2 = (x + 38) + (90 * toArt);
				p.drawLine(x1, y1, x2, y1);
				p.drawLine(x2, y1, x2 + 16, y1 - 8);
				p.drawLine(x2, y1, x2 + 16, y1 + 8);
			}
		}
	}

	p.setPen(QColor(255,255,255,60));

	

	for(int i=0; i<5; i++) { 
		p.drawLine(0, y+(6*i), width(), y+(6*i));
	}

	// Articulations
	
	p.setPen(QColor(255,255,255,200));
	p.setBrush(QColor(0,0,0,0));

	for(int i=0; i<m_longs.size(); i++) {
		QString str;
		if(m_channel) {
			str = m_channel->instrument()->version(0)->articulation(m_longs[i])->name().toChar();
			p.drawPixmap(x+2,y-6, *samGui->articulationPixmap(1, true));
			if(m_longs[i] == m_channel->currentArticulationIndex() || (m_channel->isArticulationMode() && m_longs[i] == m_channel->currentDefaultArticulationIndex())) {
				if(m_channel->isArticulationMode() && m_longs[i] == m_channel->currentArticulationIndex()) {
					p.setPen(QColor(149,255,25));
				} else if(m_channel->isArticulationMode() && m_longs[i] == m_channel->currentDefaultArticulationIndex()) {
					p.setPen(QColor(82,153,0));
				} 

				p.drawRect(x, y-7, 38, 38);
				p.drawRect(x+1, y-6, 36, 36);
			}
		}
			
		p.setPen(QColor(255,255,255,200));
		str = formatArticulationName(str);
		p.drawText(x-18, y+35, 72, 40, Qt::AlignTop|Qt::AlignHCenter, str);
		
		//p.drawRect(x, 14, 36, 36);

		x+=90;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiArticulationEditor::articulationIndexFromPoint(QPoint point)
{
	int x = 54;
	int y;

	if(point.y() < 80) { // Short note
		y = 15;
		for(int i=0; i<m_shorts.size(); i++) {
			if(QRect(x-18, y-6, 72, 90).contains(point)) {
				return m_shorts[i];
			}
			x += 90;
		}
	} else { // Long note
		y = 90;
		for(int i=0; i<m_longs.size(); i++) {
			if(QRect(x-18, y-6, 72, 90).contains(point)) {
				return m_longs[i];
			}
			x += 90;
		}
	}

	return -1;
}