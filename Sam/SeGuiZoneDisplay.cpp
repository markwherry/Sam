#include "SeGuiZoneDisplay.h"

#include <QPainter>
#include <QMouseEvent>

#include "SamEngine.h"
#include "SamPreferences.h"

#include "SamGuiEditor.h"
#include "SamZoneCommand.h"

SeGuiZoneDisplay::SeGuiZoneDisplay(QWidget *parent)
	: QWidget(parent)
	, m_pitchWidth(10)
	, m_velocityHeight(4)
	, m_layer(0)
	, m_zdiInfo(1)
	, m_velocitySplitDisplay(0)
	, m_rubberBand(0)
	, pitchDiff(0)
{
	//ui.setupUi(this);

	setFixedSize(m_pitchWidth*128, m_velocityHeight*127);
	//setMouseTracking(true);

	for(int i=0; i<128; i++) {
		m_activePitch[i] = 0;
	}

	m_zdiInfo = samPreferences->editor()->preference("zoneDisplayItemInfo", "1").toInt();

	m_popupMenu = new QMenu(0);
	QString zdiInfos[9] = { "Hide", "Number of Cells", "Velocity", "Sample Start", "Gain", "Release Gain", "Pre-Loop Release Gain", "Pitch", "Fine-Tune" };
	for(int i=0; i<9; i++) {
		m_zdiInfoActions[i] = m_popupMenu->addAction(zdiInfos[i]);
		m_zdiInfoActions[i]->setCheckable(true);
	}
	m_zdiInfoActions[7]->setEnabled(false);
	m_zdiInfoActions[m_zdiInfo]->setChecked(true);
	connect(m_popupMenu, SIGNAL(triggered(QAction*)), this, SLOT(setZdiInfo(QAction*)));

	m_velocitySplitDisplay = samPreferences->editor()->preference("velocitySplitDisplay", "0").toInt();

	//samEngine->registerListener(this, SamEngine::ContextZone);

	//connect(this, SIGNAL(zoneSelected(SamZone*)), this, SLOT(update()));

}

SeGuiZoneDisplay::~SeGuiZoneDisplay()
{

}

/*void SeGuiZoneDisplay::onState(AStateEvent *ev)
{
	qDebug("SeGuiZoneDisplay::onState()");

	switch(ev->item()) {
		case SamZone::ItemSelected:
			emit zoneSelected((SamZone*)ev->updatedObject());
			break;
	}

}*/

void SeGuiZoneDisplay::setLayer(SamLayer *layer, SamChannel *channel)
{
	qDebug("SeGuiZoneEditor::setLayer()");

	m_layer = layer;
	m_channel = channel;

	rebuild();
}

void SeGuiZoneDisplay::clear(void)
{
	qDebug("SeGuiZoneDisplay::clear()");

	clearSelection(false);

	for(int i=0; i<m_zoneItems.size(); i++) {
		delete m_zoneItems[i];
	}
	m_zoneItems.clear();
}

void SeGuiZoneDisplay::rebuild(void)
{
	qDebug("SeGuiZoneDisplay::rebuild()");

	clear();

	if(!m_layer) {
		return;
	}

	for (int i=0; i<m_layer->numZones(); i++) {
		qDebug("addZone(%d)", i);
		addZone(m_layer->zone(i));
	}

	update();
}

void SeGuiZoneDisplay::addZone(SamZone *zone)
{
	if(m_layer) {
		if(zone) {
			SeGuiZoneDisplayItem *zdi = new SeGuiZoneDisplayItem(this, zone); // 0 was this
			////zdi->setParent(this);
			//int x = zone->midiNoteLow()*m_pitchWidth;
			//int y = invertMidiVelocity(zone->midiVelocityHigh())*m_velocityHeight;
			//qDebug ("x=%d, y=%d", x, y);
			//zdi->move (x,y);
			//int w = (1+(zone->midiNoteHigh()-zone->midiNoteLow()))*m_pitchWidth;
			//int h = (1+(zone->midiVelocityHigh()-zone->midiVelocityLow()))*m_velocityHeight;
			//zdi->setFixedSize(w,h);
			
			m_zoneItems.append(zdi);
			setZoneXywh(zone);
			zdi->show(); // Previously required
			update();
		}
	}
}

void SeGuiZoneDisplay::setZoneXywh(SamZone *zone)
{
	for(int i=0; i<m_zoneItems.size(); i++) {
		if(m_zoneItems[i]->zone()==zone) {
			int x = zone->midiNoteLow()*m_pitchWidth;
			int y = (127-zone->midiVelocityHigh())*m_velocityHeight;
			m_zoneItems[i]->move(x,y);
			int w = (1+(zone->midiNoteHigh()-zone->midiNoteLow()))*m_pitchWidth;
			int h = (1+(zone->midiVelocityHigh()-zone->midiVelocityLow()))*m_velocityHeight;
			m_zoneItems[i]->setFixedSize(w,h);
			qDebug("%d,%d,%d,%d", x, y, w, h);
		}
	}
	update();
}

void SeGuiZoneDisplay::clearSelection(bool shouldUpdate)
{
	qDebug("!*!*!* SeGuiZoneDisplay::clearSelection()");
	
	for(int i=0; i<m_zoneItems.size()-1; i++) { 
		if(m_zoneItems[i]->zone()->isSelected()) {
			m_zoneItems[i]->zone()->setSelected(false, this, false);
		}
	}

	if(m_zoneItems.size() > 0) {
		m_zoneItems[m_zoneItems.size()-1]->zone()->setSelected(false, this, shouldUpdate);
	}
	
	//qDebug("EXIT: SeGuiZoneDisplay::clearSelection()");
}

void SeGuiZoneDisplay::selectAll(void)
{
	aLog("SeGuiZoneDisplay::selectAll()");

	for(int i=0; i<m_zoneItems.size()-1; i++) { 
		if(!m_zoneItems[i]->zone()->isSelected()) {
			m_zoneItems[i]->zone()->setSelected(true, this, false);
		}
	}

	//if(!m_zoneItems[m_zoneItems.size()-1]->zone()->isSelected()) {
	if(m_zoneItems.size() > 0) {
			m_zoneItems[m_zoneItems.size()-1]->zone()->setSelected(true);
	}
	//	}

	aLog("EXIT: SeGuiZoneDisplay::selectAll()");
}

void SeGuiZoneDisplay::selectZonesByPitch(int pitch, int exclusive)
{
	if(exclusive) {
		clearSelection();
	}

	for(int i=0; i<m_zoneItems.size(); i++) {
		if(m_zoneItems[i]->zone()->isMidiNoteInRange(pitch)) {
			m_zoneItems[i]->zone()->setSelected(true);
		}
	}
}

void SeGuiZoneDisplay::selectZonesByVelocity(int velLo, int velHi, bool exclusive)
{
	qDebug("*** SeGuiZoneDisplay::selectZonesByVelocity(%d, %d, %d)", velLo, velHi, (int)exclusive);

	if(exclusive) {
		clearSelection();
	}

	for(int i=0; i<m_zoneItems.size(); i++) {
		if(m_zoneItems[i]->zone()->midiVelocityLow()==velLo && m_zoneItems[i]->zone()->midiVelocityHigh()==velHi) {
			m_zoneItems[i]->zone()->setSelected(true);
		}
	}
}

void SeGuiZoneDisplay::updateSelection(void)
{
	update();
}

void SeGuiZoneDisplay::setVelocityHeight(float velocityHeight)
{
	m_velocityHeight = velocityHeight;
	if(m_velocityHeight<3) {
		m_velocityHeight = 3;
	}
	for(int i=0; i<m_zoneItems.size(); i++) {
		setZoneXywh(m_zoneItems[i]->zone());
	}
	setFixedSize(m_pitchWidth*128, m_velocityHeight*127);
	update();

	emit sizeChanged();
}

void SeGuiZoneDisplay::updateZoneSizes(void)
{
	for(int i=0; i<m_zoneItems.size(); i++) {
		setZoneXywh(m_zoneItems[i]->zone());
	}
}

void SeGuiZoneDisplay::rebuildZones(void)
{
	clear();
	rebuild();
}

void SeGuiZoneDisplay::updateActivePitch(void)
{
	update();
}

void SeGuiZoneDisplay::setZdiInfo(QAction *action)
{
	int item = -1;
	for(int i=0; i<9; i++) {
		if(action == m_zdiInfoActions[i]) {
			item = i;
			m_zdiInfoActions[i]->setChecked(true);
		} else {
			m_zdiInfoActions[i]->setChecked(false);
		}
	}
	
	if(item > -1) {
		m_zdiInfo = item;
		update();
		samPreferences->editor()->setPreference("zoneDisplayItemInfo", AString::number(m_zdiInfo));
	}
}

void SeGuiZoneDisplay::setVelocitySplitDisplay(int display)
{
	m_velocitySplitDisplay = display;
	update();
}

//void SeGuiZoneDisplay::leaveEvent(QEvent *ev)
//{
//	//emit mouseOverVelocity(0);
//}

void SeGuiZoneDisplay::mouseDoubleClickEvent(QMouseEvent *ev)
{
	clickX = ev->x();
	clickY = ev->y();
	mouseAction = 0;
	clickedOnItem = false;

	if(ev->button() == Qt::LeftButton) {
	 
		for(int i=0; i<m_zoneItems.size(); i++) {
			QRect rect(m_zoneItems[i]->x(), m_zoneItems[i]->y(), m_zoneItems[i]->width(), m_zoneItems[i]->height());
			if(rect.contains(ev->x(), ev->y())) {
			
				SamZone *zone = m_zoneItems[i]->zone();
				selectZonesByVelocity(zone->midiVelocityLow(), zone->midiVelocityHigh(), !(ev->modifiers()&Qt::ControlModifier));

				clickedOnItem = true;
			}
		}

		if(clickedOnItem) {
			qDebug("ClickedOnItem");
			return;
		}

		qDebug("No item clicked.");

		selectAll();

	} 
}

void SeGuiZoneDisplay::mouseMoveEvent(QMouseEvent *ev)
{
	qDebug("SeGuiZoneDisplay::mouseMoveEvent()");
	int v = ev->y() / m_velocityHeight;
	//emit mouseOverVelocity(127-v);

	if(clickedOnItem) {
		int diffX = clickX - ev->x();
		pitchDiff = diffX/m_pitchWidth;
		//mouseAction = 1;
		//qDebug("pitchDiff = %d", pitchDiff);

		for(int i=0; i<m_zoneItems.size(); i++) {
			SamZone *zone = m_zoneItems[i]->zone();
			if(zone->isSelected()) {
				int x = (zone->midiNoteLow()-pitchDiff)*m_pitchWidth;
				
				switch(mouseAction) {
					case 1: // Move
						m_zoneItems[i]->move(x, m_zoneItems[i]->y());
						break;

					case 2: // Drag left
						m_zoneItems[i]->move(x, m_zoneItems[i]->y());
						m_zoneItems[i]->setFixedWidth((pitchDiff+1+(zone->midiNoteHigh()-zone->midiNoteLow()))*m_pitchWidth);
						break;

					case 3: // Drag right
						m_zoneItems[i]->setFixedWidth((-pitchDiff+1+(zone->midiNoteHigh()-zone->midiNoteLow()))*m_pitchWidth);
						break;
				}



				//int y = (127-zone->midiVelocityHigh())*m_velocityHeight;
				
				//int w = (1+(zone->midiNoteHigh()-zone->midiNoteLow()))*m_pitchWidth;
				//int h = (1+(zone->midiVelocityHigh()-zone->midiVelocityLow()))*m_velocityHeight;
				//m_zoneItems[i]->setFixedSize(w,h);
				//qDebug("%d,%d,%d,%d", x, y, w, h);
			}
		}
		update();//
	} else {
		if(m_rubberBand) {
			
			m_rubberBand->setGeometry(QRect(m_rbOrigin, ev->pos()).normalized());
			QRect band(m_rubberBand->pos().x(), m_rubberBand->pos().y(), m_rubberBand->size().width(), m_rubberBand->size().height());
			//qDebug("---Moving %d, %d (%d, %d, %d, %d)", ev->pos().x(), ev->pos().y(), band.x(), band.y(), band.width(), band.height());
			//SamZone *firstZone = 0;
			//int numSelectedZones = 0;

			for(int i=0; i<m_zoneItems.size(); i++) {
				QRect zoneArea(m_zoneItems[i]->x(), m_zoneItems[i]->y(), m_zoneItems[i]->width(), m_zoneItems[i]->height());
				if(band.intersects(zoneArea)) {

			
						m_zoneItems[i]->zone()->setSelected(true);
						//if(!firstZone) {
						//	firstZone = zdiList[i]->zone();
						//}
						//numSelectedZones++;
			
				} else {
					//if(ev->modifiers()&Qt::ControlModifier) {
						//qDebug("Qt::ControlModifier");
				//	} else {
						if(m_zoneItems[i]->zone()->isSelected()) {
							m_zoneItems[i]->zone()->setSelected(false);
						}
					//}
			
				}
			}
		}
	}

	//ev->setAccepted(false);
}

void SeGuiZoneDisplay::mouseReleaseEvent(QMouseEvent *ev)
{
	aLog("SeGuiZoneDisplay::mouseReleaseEvent() mouseAction = %d", mouseAction);
	switch(mouseAction) {
		case 0: // Do nothing
			break;

		case 1: // Move (Pitch)
			if(pitchDiff != 0) { 
				for(int i=0; i<m_zoneItems.size(); i++) {
					SamZone *zone = m_zoneItems[i]->zone();
					if(zone->isSelected()) {
						samEditor->commandManager()->execute(new SamZoneCommand(zone, SamZoneCommand::PitchMove, (float)pitchDiff));
					}
				}
			}
			break;

		case 2: // Adjust lo (Pitch)
			if(pitchDiff != 0) {
				for(int i=0; i<m_zoneItems.size(); i++) {
					SamZone *zone = m_zoneItems[i]->zone();
					if(zone->isSelected()) {
						samEditor->commandManager()->execute(new SamZoneCommand(zone, SamZoneCommand::PitchLo, (float)zone->midiNoteLow()-pitchDiff));
					}
				}
			}
			break;

		case 3: // Adjust hi (Pitch)
			if(pitchDiff != 0) {
				for(int i=0; i<m_zoneItems.size(); i++) {
					SamZone *zone = m_zoneItems[i]->zone();
					if(zone->isSelected()) {
						samEditor->commandManager()->execute(new SamZoneCommand(zone, SamZoneCommand::PitchHi, (float)zone->midiNoteHigh()-pitchDiff));
					}
				}
			}
			break;
	}

	clickedOnItem = false;
	mouseAction = 0;
	pitchDiff = 0;

	if(m_rubberBand) {
		m_rubberBand->hide();
	}
}

void SeGuiZoneDisplay::mousePressEvent(QMouseEvent *ev)
 {
	 qDebug("SeGuiZoneDisplay::mousePressEvent()");

	 clickX = ev->x();
	 clickY = ev->y();
	 mouseAction = 0;

	 clickedOnItem = false;

	 if(ev->button() == Qt::LeftButton) {

		 for(int i=0; i<m_zoneItems.size(); i++) {
			//qDebug("[%d]", i);
			QRect rect(m_zoneItems[i]->x(), m_zoneItems[i]->y(), m_zoneItems[i]->width(), m_zoneItems[i]->height());
			//qDebug("Here1");
			if(rect.contains(ev->x(), ev->y())) {
				//qDebug("Here2");
				//m_zoneItems[i]->mousePress(event);
				if(!(ev->modifiers()&Qt::ControlModifier)) {
					clearSelection();
				}
				m_zoneItems[i]->zone()->setSelected(!m_zoneItems[i]->zone()->isSelected());
				//qDebug("Here3");
				mouseAction = m_zoneItems[i]->hoverMode();
				clickedOnItem = true;
			}
		}

		if(clickedOnItem) {
			qDebug("ClickedOnItem");
			return;
		}

		qDebug("No item clicked.");
		
		if(ev->modifiers()&Qt::ControlModifier) {
			//qDebug("Qt::ControlModifier");
		} else {
			clearSelection(); 
			//emit zoneSelected(0,0);
		}

		/*mouseDownOrigin = event->pos();
		if(!rubberBand) { 
			rubberBand = new QRubberBand(QRubberBand::Rectangle, this); 
		}
		rubberBand->setGeometry(QRect(mouseDownOrigin, QSize())); 
		rubberBand->show(); 

		lastSelectedZones = 0;*/

		m_rbOrigin = ev->pos();
		if(!m_rubberBand) {
			m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
		}
		if(m_rubberBand) {
			m_rubberBand->setGeometry(QRect(m_rbOrigin, QSize()));
			m_rubberBand->show();
		}


	 } else {
		m_popupMenu->exec(ev->globalPos());
	 }

	qDebug("EXIT: SeGuiZoneDisplay::mousePressEvent()");
}

void SeGuiZoneDisplay::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen (Qt::white);
	p.setBrush (Qt::white);

	p.drawRect (0,0,width()-1, height()-1);


	p.setPen (QColor(168,199,208,255));
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height()-1));
    linearGrad.setColorAt(0, QColor(168,199,208,150));
    linearGrad.setColorAt(1, QColor(168,199,208,0));

	p.drawRect (0,0,width()-1, height()-1);

	//p.setPen (QColor(0,150,200,80));

	int noteInOctave = 0;

	p.setPen(QColor(80,80,80));
	p.setFont (QFont("Calibri",14,QFont::Bold));
	int vy = 0, vho = 0;
	if(m_layer->midiVelocityLow() > 1) {
		vho = m_velocityHeight * m_layer->midiVelocityLow();
		p.fillRect(0, height() - (m_velocityHeight * m_layer->midiVelocityLow()), width()-1, vho, QColor(0,0,0,100));
		//p.drawText(0, height() - (m_velocityHeight * m_layer->midiVelocityLow()), width()-1, vho, Qt::AlignLeft|Qt::AlignTop, QString::number(m_layer->midiVelocityLow()));
		//p.drawText(0, height() - (m_velocityHeight * m_layer->midiVelocityLow()), width()-1, vho, Qt::AlignRight|Qt::AlignTop, QString::number(m_layer->midiVelocityLow()));
	}

	if(m_layer->midiVelocityHigh() < 127) {
		vy = height()-(m_layer->midiVelocityHigh()*m_velocityHeight); //m_velocityHeight * (127-m_layer->midiVelocityHigh());
		p.fillRect(0, 0, width()-1, vy, QColor(0,0,0,100));
		//p.drawText(0, 0, width()-1, vy, Qt::AlignLeft|Qt::AlignBottom, QString::number(m_layer->midiVelocityHigh()));
		//p.drawText(0, 0, width()-1, vy, Qt::AlignRight|Qt::AlignBottom, QString::number(m_layer->midiVelocityHigh()));
	}

	if(m_layer->midiPitchLow() > 0) {
		p.fillRect(0, vy, m_layer->midiPitchLow() * m_pitchWidth, height()-1-vy-vho, QColor(0,0,0,100));
	}

	if(m_layer->midiPitchHigh() < 127) {
		p.fillRect(width() - ((127-m_layer->midiPitchHigh()) * m_pitchWidth), vy, (127-m_layer->midiPitchHigh()) * m_pitchWidth, height()-1-vy-vho, QColor(0,0,0,100));
	}

	


	for (int i=0; i<128; i++) {

		//if(m_activePitch[i]>0) {
		if(m_channel->velocity(i) > 0) {
			p.setPen(QColor(0,0,0,0));
			p.setBrush(QColor(100,150,100,200));
			p.drawRect((i*m_pitchWidth)+1, height()-(m_channel->velocity(i)*m_velocityHeight), m_pitchWidth-1, (m_channel->velocity(i)*m_velocityHeight));
		}
		
		if(noteInOctave==0) {
			p.setPen (QColor(0,150,200,200));
		} else {
			p.setPen (QColor(0,150,200,80));
		}
		p.drawLine(i*m_pitchWidth,0,i*m_pitchWidth,height()-1);

		if(noteInOctave==1 || noteInOctave==3 || noteInOctave==6 || noteInOctave==8 || noteInOctave==10) {
			p.setPen(QColor(0,0,0,0));
			p.setBrush(QColor(0,0,50,40));
			p.drawRect((i*m_pitchWidth)+1,1,m_pitchWidth-1,height()-2);
		}

		noteInOctave++;
		if(noteInOctave>=12) {
			noteInOctave = 0;
		}

		if(m_layer) {
			if(m_layer->aliasNote(i) > -1) {
				p.fillRect((i*m_pitchWidth)+1,1,m_pitchWidth-1,height()-2,QColor(100,0,0,100));
			}
		}

	}

	//if(m_channel) {
	//	p.fillRect(0, height()-(m_channel->velocity(m_channel->currentPitch())*m_velocityHeight), width(), m_velocityHeight, QColor(100,150,100,40));
	//}

	
	//int h=16;
	//for (int i=0; i<128; i+=(128/4)) {
	//	p.drawLine (0,i*m_velocityHeight,width()-1,i*m_velocityHeight);
	//}
	if(m_velocitySplitDisplay == 0) {
		int velocities[3] = { 31, 63, 95 };
		for (int i=0; i<3; i++) {
			//p.setPen (QColor(0,150,200,80));
			int vh = height()-(velocities[i]*m_velocityHeight);
			//p.drawLine (0,vh,width()-1,vh);
			p.fillRect(0, vh, width(), m_velocityHeight, QColor(0,150,200,40));
			//p.setPen (QColor(0,150,200));
			//p.drawText (0,vh+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(velocities[i]));
		}
	} else {
		
		int velocities[6] = { 51, 71, 91, 101, 111, 121 };
		for (int i=0; i<6; i++) {
			//p.setPen (QColor(0,150,200,80));
			int vh = height()-(velocities[i]*m_velocityHeight);
			//p.drawLine (0,vh,width()-1,vh);
			p.fillRect(0, vh, width(), m_velocityHeight, QColor(0,150,200,40));
			//p.setPen (QColor(0,150,200));
			//p.drawText (0,vh+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(velocities[i]));
		}
	}

	p.setPen (QColor(0,150,200,40));
	p.setBrush (linearGrad);
	p.drawRect (0,0,width()-1, height()-1);


	int lastX = 0;
	int lastY = 0;
	for(int i=0; i<m_zoneItems.size(); i++) {
		int newX = m_zoneItems[i]->x();
		int newY = m_zoneItems[i]->y();

		int diffX = newX - lastX;
		int diffY = newY - lastY;

		p.translate(QPoint(diffX, diffY));
		m_zoneItems[i]->paint(p);
		lastX = newX;
		lastY = newY;
	}//*/
}