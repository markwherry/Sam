#include "SeGuiCellProperties.h"

#include "SamGuiEditor.h"
#include "SamCellCommand.h"
#include "SeGuiCellItem.h"
#include "SamEngine.h"

#include <QPainter>

SeGuiCellProperties::SeGuiCellProperties(QWidget *parent)
	: QWidget(parent)
	, m_zone(0)
//	, m_cell(0)
{
	//ui.setupUi(this);

	m_cellList = new UiList(this);
	m_cellList->setMovable(false);
	m_cellList->setEmptyText("No Zone Selected");

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	m_sampleRate = new SeGuiItemValue(this, -1, "Sample Rate", "Hz", 0, 192000, 44100, 1, 2);
	m_sampleRate->setEditable(false);
	m_list->addItem(m_sampleRate);

	m_length = new SeGuiItemValue(this, -1, "Length", "samples", 0, 0, 0, 1, 2);
	m_length->setEditable(false);
	m_list->addItem(m_length);

	m_lengthRelease = new SeGuiItemValue(this, -1, "Release Length", "samples", 0, 0, 0, 1, 2);
	m_lengthRelease->setEditable(false);
	m_list->addItem(m_lengthRelease);

	m_baseNote = new SeGuiItemValue(this, SamCellCommand::BaseNote, "Base Note Pitch", "", 0, 127, 1, 1, 2);
	//m_baseNote->setTint(true, QColor(120,120,200));
	m_baseNote->setExtraInfo(1);
	connect(m_baseNote, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_baseNote);

	m_fineTune = new SeGuiItemValue(this, SamCellCommand::FineTune, "Fine Tune", "cents", -99, 99, 1, 1, 2);
	//m_fineTune->setTint(true, QColor(120,120,200));
	connect(m_fineTune, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_fineTune);

	m_sourceStart = new SeGuiItemValue(this, SamCellCommand::SourceStart, "Source Start", "samples", 0, 44100, 0, 1, 2);
	//m_sourceStart->setTint(true, QColor(120,120,200));
	connect(m_sourceStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_sourceStart);

	m_releaseStart = new SeGuiItemValue(this, SamCellCommand::ReleaseStart, "Release Start", "samples", 0, 0, 0, 1, 2);
	//m_releaseStart->setTint(true, QColor(120,120,200));
	connect(m_releaseStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseStart);

	/*m_releaseAttenuation = new SeGuiItemValue(this, SamCellCommand::ReleaseAttenuation, "Release Attenuation", "dB", -12, 12, 0, 1, 2);
	//m_releaseAttenuation->setTint(true, QColor(120,120,200));
	connect(m_releaseAttenuation, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseAttenuation);*/

	m_releaseAssigned = new SeGuiItemBool(this, -1, "Release Assigned");
	m_releaseAssigned->setEditable(false);
	m_list->addItem(m_releaseAssigned);

	// ---
	m_loop = new SeGuiItemHeading(this, "Loop");
	m_list->addItem(m_loop);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_loopEnable = new SeGuiItemBool(this, SamCellCommand::LoopEnable, "Loop Enable");
	connect(m_loopEnable, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_loopEnable);

	m_loopStart = new SeGuiItemValue(this, SamCellCommand::LoopStart, "Loop Start", "samples", 0, 0, 0, 1, 2);
	connect(m_loopStart, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_loopStart);

	m_loopEnd = new SeGuiItemValue(this, SamCellCommand::LoopEnd, "Loop End", "samples", 0, 0, 0, 1, 2);
	connect(m_loopEnd, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_loopEnd);

	setDisabled(true);

	// ----
	m_slices = new SeGuiItemHeading(this, "Slices");
	m_list->addItem(m_slices);

	m_tempo = new SeGuiItemValue(this, -1, "Tempo", "bpm", 0, 0, 0, 1, 2);
	m_tempo->setEditable(false);
	m_list->addItem(m_tempo);

	m_numSlices = new SeGuiItemValue(this, -1, "Slices", "", 0, 0, 0, 1, 2);
	m_numSlices->setEditable(false);
	m_list->addItem(m_numSlices);


	//setFixedSize(296, m_list->height());

	samEngine->registerListener(this, SamEngine::ContextCell);
	connect(this, SIGNAL(cellSelected()), this, SLOT(updateCellSelection()));
}

SeGuiCellProperties::~SeGuiCellProperties()
{

}

void SeGuiCellProperties::onState(AStateEvent *ev)
{
	switch(ev->item()) {
		case SamCell::ItemSelected:
			emit cellSelected();
			break;
	}
}

void SeGuiCellProperties::setZone(SamZone *zone, int numZonesSelected)
{
	//qDebug("SeGuiCellProperties::setZone(%d, %d)", zone, numZonesSelected);

	//if(m_zone == zone) { return; }

	m_zone = zone;

	m_cellList->removeAllItems();
	int h = height()-m_list->height()-8-8;
	m_cellList->setFixedSize(width()-8, h);
	//qDebug("%d-%d-12=%d", height(), m_list->height(), height()-m_list->height()-12);

	setDisabled(true);

	if(m_zone && numZonesSelected == 1 && m_zone->numCells() > 0) {
		for(int i=0; i<m_zone->numCells(); i++) {
			SeGuiCellItem *item = new SeGuiCellItem(0, m_zone->cell(i));
			item->setFixedSize(width()-8, h/m_zone->numCells());
			m_cellList->addItem(item);
		}
		setDisabled(false);
		m_cellList->itemFromIndex(0)->select();
		setCell(m_zone->cell(0));
	}

	if(numZonesSelected > 1) {
		m_cellList->setEmptyText("Multiple Zones Selected");
	} else {
		m_cellList->setEmptyText("No Zone Selected");
	}
}

void SeGuiCellProperties::setCell(SamCell *cell)
{
	//m_cell = cell;

	m_sampleRate->setMinValue((int)cell->sampleRate());
	m_sampleRate->setMaxValue((int)cell->sampleRate());
	m_sampleRate->setValue((int)cell->sampleRate());

	m_length->setMinValue(cell->lengthFrames());
	m_length->setMaxValue(cell->lengthFrames());
	m_length->setValue(cell->lengthFrames());

	m_lengthRelease->setMinValue(cell->releaseLengthFrames());
	m_lengthRelease->setMaxValue(cell->releaseLengthFrames());
	m_lengthRelease->setValue(cell->releaseLengthFrames());

	m_baseNote->setValue((int)cell->noteBase());
	m_fineTune->setValue((int)cell->fineTune());

	m_sourceStart->setValue(cell->sourceStart());

	m_releaseStart->setMaxValue(cell->lengthFrames());
	m_releaseStart->setValue(cell->releaseStart());

	m_releaseAssigned->setChecked(cell->hasRelease());

	m_loopEnable->setChecked(cell->isLoop());

	m_loopStart->setMaxValue(cell->lengthFrames());
	m_loopStart->setValue(cell->loopStart());
	

	m_loopEnd->setMaxValue(cell->lengthFrames());
	m_loopEnd->setValue(cell->loopEnd());

	m_tempo->setValue(cell->tempo());
	m_tempo->setMaxValue(cell->tempo());
	m_tempo->setMinValue(cell->tempo());

	m_numSlices->setValue(cell->numSlices());
	m_numSlices->setMinValue(cell->numSlices());
	m_numSlices->setMaxValue(cell->numSlices());
	
}

void SeGuiCellProperties::updateCellSelection(void)
{
	update();

	if(!m_zone) { return; }

	for(int i=0; i<m_zone->numCells(); i++) {
		if(m_zone->cell(i)->isSelected()) {
			setCell(m_zone->cell(i));
			break;
		}
	}
}

void SeGuiCellProperties::setValue(int propertyId, float value)
{
	/*SamLayer *layer = (SamLayer*)m_zone->parent();
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamZoneCommand(layer->zone(i), propertyId, value));
		}
	}*/

	for(int i=0; i<m_zone->numCells(); i++) {
		if(m_zone->cell(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamCellCommand(m_zone->cell(i), propertyId, value));
		}
	}
}

void SeGuiCellProperties::setState(int propertyId, bool state)
{
	/*SamLayer *layer = (SamLayer*)m_zone->parent();
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamZoneCommand(layer->zone(i), propertyId, state));
		}
	}*/

	for(int i=0; i<m_zone->numCells(); i++) {
		if(m_zone->cell(i)->isSelected()) {
			samEditor->commandManager()->execute(new SamCellCommand(m_zone->cell(i), propertyId, state));
		}
	}
}

/*void SeGuiCellProperties::changeEvent(QEvent *ev)
{
	//resizeEvent(0);
}*/

void SeGuiCellProperties::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0,0,width(),height(),QColor(40,40,40));
p.setRenderHint(QPainter::Antialiasing);
	int sx = m_list->x()-1;
			int sy = m_list->y()-1;
			int sxx = sx+m_list->width()+2;
			int syy = sy+m_list->height()+2;
			p.setPen(QColor(0,0,0));
			p.drawLine(sx, syy, sx, sy);
			p.drawLine(sx, sy, sxx, sy);
			
			p.drawLine(sxx, sy, sxx, syy);
			p.drawLine(sxx, syy, sx, syy);

		 sx = m_cellList->x()-1;
			 sy = m_cellList->y()-1;
			 sxx = sx+m_cellList->width()+2;
			 syy = sy+m_cellList->height()+2;
			
			p.drawLine(sx, syy, sx, sy);
			p.drawLine(sx, sy, sxx, sy);
			
			p.drawLine(sxx, sy, sxx, syy);
			p.drawLine(sxx, syy, sx, syy);
}

void SeGuiCellProperties::resizeEvent(QResizeEvent *ev)
{
	qDebug("SeGuiCellProperties::resizeEvent()");

	m_list->setFixedSize(width()-8,m_list->height());

	m_list->move(4,height()-m_list->height()-4);

	m_cellList->move(4,4);
	m_cellList->setFixedSize(width()-8, height()-m_list->height()-8-8);
	//qDebug("%d, %d-%d-12=%d", width(), height(), m_list->height(), height()-m_list->height()-12);

	if(m_cellList->numItems() > 0) {
		int h = height()-m_list->height()-8-8;
		for(int i=0; i<m_cellList->numItems(); i++) {
			m_cellList->itemFromIndex(i)->setFixedHeight(h/m_cellList->numItems());
		}
		m_cellList->updateHeights();
	}

	//qDebug("visible = %d", m_cellList->isVisible());
}

void SeGuiCellProperties::showEvent(QShowEvent *ev)
{
	QWidget::showEvent(ev);
	resizeEvent(0);
}