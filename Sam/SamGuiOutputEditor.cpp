#include "SamGuiOutputEditor.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <qpainter.h>
#include <QMouseEvent>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputEditor::SamGuiOutputEditor(QWidget *parent)
	: QWidget(parent)
	, m_clickedOnPin(-1)
{
	//ui.setupUi(this);

	setMouseTracking(true);

	updateOutputs();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputEditor::~SamGuiOutputEditor()
{
	clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::updateOutputs(void)
{
	clear();

	int numBuses = samEngine->project()->numAudioHardwareBuses();
	int numOutputs = samEngine->numOutputs();

	/*for(int i=0; i<numBuses; i++) {
		SamGuiOutputEditorItem *item = new SamGuiOutputEditorItem(this);
		item->move(0, i * 25);
		item->show();
		m_items.append(item);
	}*/

	m_pins.clear();
	for(int i=0; i<numBuses; i++) {
		SamAudioHardwareBus *bus = samEngine->project()->audioHardwareBus(i);
		for(int j=0; j<bus->numStreams(); j++) {
			Pin pin;
			pin.bus = i;
			pin.output = bus->streamHardwareChannel(j);
			pin.stream = j;
			m_pins.append(pin);
		}
	}

	setFixedSize(numOutputs *25, numBuses * 25);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::clear(void)
{
	/*for(int i=0; i<m_items.size(); i++) {
		delete m_items[i];
	}

	m_items.clear();*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::leaveEvent(QEvent *ev)
{
	emit highlighted(-1,-1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::mouseDoubleClickEvent(QMouseEvent *ev)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::mouseMoveEvent(QMouseEvent *ev)
{
	int output = outputFromX(ev->x());
	int bus = busFromY(ev->y());
	
	if(output > -1 && bus > -1) {
		emit highlighted(bus, output);
	}

	if(m_clickedOnPin > -1 && output > -1) {

		int clickedOnBus = m_pins[m_clickedOnPin].bus;

		if(m_pins[m_clickedOnPin].output != output) {
			bool alreadyAssigned = false;
			for(int i=0; i<m_pins.size(); i++) {
				if(m_pins[i].bus == clickedOnBus && m_pins[i].output == output) {
					alreadyAssigned = true;
				}
			}
			if(!alreadyAssigned) {
				m_pins[m_clickedOnPin].output = output;
				update();
			}
		}
	}

	/*int numBuses = samEngine->project()->numAudioHardwareBuses();
	int numOutputs = samEngine->numOutputs();

	for(int i=0; i<numBuses; i++) {
		if(ev->y() >= i*25 && ev->y()<(i+1)*25) {
			for(int j=0; j<numOutputs; j++) {
				if(ev->x() >= j*25 && ev->x() < (j+1)*25) {
					emit highlighted(i, j);
				}
			}
		}
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::mousePressEvent(QMouseEvent *ev)
{
	m_clickedOnPin = -1;

	int output = outputFromX(ev->x());
	int bus = busFromY(ev->y());

	for(int i=0; i<m_pins.size(); i++) {
		if(m_pins[i].output == output && m_pins[i].bus == bus) {
			m_clickedOnPin = i;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::mouseReleaseEvent(QMouseEvent *ev)
{
	if(m_clickedOnPin > -1) {
		int busIndex = m_pins[m_clickedOnPin].bus;
		SamAudioHardwareBus *bus = samEngine->project()->audioHardwareBus(busIndex);
		bus->setStreamHardwareChannel(m_pins[m_clickedOnPin].stream, m_pins[m_clickedOnPin].output);
	}

	m_clickedOnPin = -1;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputEditor::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int numBuses = samEngine->project()->numAudioHardwareBuses();
	int numOutputs = samEngine->numOutputs();

	QColor cols[2] = { QColor(90,90,90), QColor(75,75,75) };

	p.setFont(QFont("Calibri",8,QFont::Bold));

	for(int i=0; i<numBuses; i++) {
		p.fillRect(0, i*25, width(), 25, cols[i%2]);
		p.setPen(QColor(150,150,150));
		p.drawLine(0, (i+1)*25-1, width(), (i+1)*25-1);
		
		SamAudioHardwareBus *bus = samEngine->project()->audioHardwareBus(i);

		for(int j=0; j<numOutputs; j++) {
			p.setPen(QColor(150,150,150));
			p.drawLine((j+1)*25-1, 0, (j+1)*25-1, height()-1);

			//if(bus->streamHardwareChannel(
			//p.fillRect(j*25+4, i*25+4, 16, 16, QColor(0,255,0));
			//p.setPen(QColor(0,0,0));
			//p.drawText(j*25+4, i*25+4, 16, 16, Qt::AlignCenter, "Rs");
		}
		
	}

	QString label[4] = { "L", "R", "Ls", "Rs" }; // Quick and dirty!
	for(int i=0; i<m_pins.size(); i++) {
		p.fillRect(m_pins[i].output*25+4, m_pins[i].bus*25+4, 16, 16, QColor(0,255,0));
		p.setPen(QColor(0,0,0));
		p.drawText(m_pins[i].output*25+4, m_pins[i].bus*25+4, 16, 16, Qt::AlignCenter, label[m_pins[i].stream]);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiOutputEditor::outputFromX(int x)
{
	int numOutputs = samEngine->numOutputs();

	for(int j=0; j<numOutputs; j++) {
		if(x >= j*25 && x < (j+1)*25) {
			return j;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiOutputEditor::busFromY(int y)
{
	int numBuses = samEngine->project()->numAudioHardwareBuses();

	for(int i=0; i<numBuses; i++) {
		if(y >= i*25 && y<(i+1)*25) {
			return i;
		}
	}

	return -1;
}