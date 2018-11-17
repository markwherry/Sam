#include "UiList.h"

#include <QMouseEvent>
#include <QPainter>

UiList::UiList(QWidget *parent)
	: QWidget(parent)
	, m_selectable(true)
	, m_movable(true)
	, m_didMove(false)
	, m_clickedOnSelectedItem(false)
	, movingItemIndex(-1)
	, m_clickedItem(0)
	, m_movePosition(-1)
{
	//ui.setupUi(this);
	m_rect = new UiRect(this);
	m_rect->setFixedHeight(3);
	m_rect->setPenColor(QColor(250,250,50));
	m_rect->setBrushColor(QColor(250,250,50));
	m_rect->hide();
}

UiList::~UiList()
{
	delete m_rect;
}

void UiList::addItem(UiListItem *item)
{
	int y = 0;
	for(int i=0; i<m_items.size(); i++) {
		y += m_items[i]->height();
	}

	item->setParent(this);
	item->setPositionInList(m_items.size());
	m_items.push_back(item);
	item->move(0, y); 
	setFixedHeight(y+item->height());
	item->show();

	connect(item, SIGNAL(selected(QString, bool)), this, SLOT(selectItem(QString, bool)));
	connect(item, SIGNAL(heightChanged()), this, SLOT(updateHeights()));

	if(m_selectable) {
		selectItem(item->uuid());
	}
}

void UiList::removeItem(UiListItem *item)
{
	qDebug("UiList::removeItem()");

	//item->hide();
	//item->setEnabled(false);
	//item->setParent(0);

	//selectPrevItem();


	int positionRemoved = item->positionInList();
	qDebug("positionRemoved = %d", positionRemoved);
	for(int i=positionRemoved+1; i<m_items.size(); i++) {
		qDebug("%d to %d", i, i-1);
		UiListItem *item = itemFromPosition(i);
		item->setPositionInList(i-1);
	}

	m_items.removeOne(item);
	delete item;

	//renumberPositions();
	
	

	reorganise();
}

void UiList::renumberPositions(void)
{
	QList<UiListItem*> items;

	for(int i=0; i<m_items.size(); i++) {
		items.append(m_items[i]);
		int origPos = items.size()-1;
		for(int j=items.size()-1; j>0; j--) {
			if(items[j-1]->positionInList() <= m_items[i]->positionInList() && items[j]->positionInList() > m_items[i]->positionInList()) {
				items.move(origPos, j-1);
			}
		}
	}

	for(int i=0; i<items.size(); i++) {
		qDebug("%d/%d pos = %d", i+1, items.size(), items[i]->positionInList());
		items[i]->setPositionInList(i);
	}

	/*zones.append(zone);
			int origPos = zones.size()-1;
			for(int j=zones.size()-1; j>0; j--) {
				if(zones[j-1]->midiVelocityLow() <= zone->midiVelocityLow() && zones[j]->midiVelocityLow() > zone->midiVelocityLow()) {
					zones.move(origPos, j-1);
				}
			}*/
}

void UiList::removeAllItems(void)
{
	for(int i=0; i<m_items.size(); i++) {
		delete m_items[i];
	}
	m_items.clear();
}

void UiList::reorganise(void)
{
	qDebug("UiList::reorganise()");
	int y = 0;
	for(int i=0; i<m_items.size(); i++) {
		UiListItem* item = itemFromPosition(i);
		if(item) {
			item->move(0, y);
			y += item->height();
		}
	}
	debugPositions();
}

void UiList::debugPositions(void)
{
	for(int i=0; i<m_items.size(); i++) {
		qDebug("[%d] %d", i, m_items[i]->positionInList());
	}
}

UiListItem* UiList::itemFromUuid(QString uuid)
{
	for(int i=0; i<m_items.size(); i++) {
		if(m_items[i]->uuid().contains(uuid)) {
			return m_items[i];
		}
	}

	return 0;
}

UiListItem* UiList::itemFromPosition(int position)
{
	for(int i=0; i<m_items.size(); i++) {
		if(m_items[i]->positionInList() == position) {
			return m_items[i];
		}
	}

	return 0;
}

int UiList::firstSelectedItemPosition(void)
{
	int selectedPosition = -1;

	for(int i=0; i<m_items.size(); i++) {
		if(m_items[i]->isSelected()) {
			if(selectedPosition == -1) {
				selectedPosition = m_items[i]->positionInList();
			} else {
				if(m_items[i]->positionInList() < selectedPosition) {
					selectedPosition = m_items[i]->positionInList();
				}
			}
		}
	}

	return selectedPosition;
}

int UiList::lastSelectedItemPosition(void)
{
	int selectedPosition = -1;

	for(int i=0; i<m_items.size(); i++) {
		if(m_items[i]->isSelected()) {
			if(m_items[i]->positionInList() > selectedPosition) {
				selectedPosition = m_items[i]->positionInList();
			}
		}
	}

	return selectedPosition;
}

int UiList::numItemsSelected(void)
{
	int selectedItems = 0;

	for(int i=0; i<m_items.size(); i++) {
		if(m_items[i]->isSelected()) {
			selectedItems++;
		}
	}

	return selectedItems;
}

void UiList::setEmptyTitle(QString text)
{
	m_emptyTitle = text;
	update();
}

void UiList::setEmptyText(QString text)
{
	m_emptyText = text;
	update();
}

void UiList::selectItem(QString uuid, bool exclusive)
{
	if(!m_selectable) {
		return;
	}

	UiListItem *item = itemFromUuid(uuid);

	if(exclusive) {
		for(int i=0; i<m_items.size(); i++) {
			if(m_items[i] != item) {
				m_items[i]->setSelected(false);
			}
		}
	}

	if(item) {
		item->setSelected(true);
		emit itemSelectedListPosition(item->positionInList());
	}
}

void UiList::selectNextItem(bool exclusive)
{
	if(!m_selectable) {
		return;
	}

	int pos = firstSelectedItemPosition();
	if(pos > -1) {
		UiListItem *item = itemFromPosition(pos+1);
		if(item) {
			selectItem(item->uuid(), exclusive);
		}
	}
}

void UiList::selectPrevItem(bool exclusive)
{
	if(!m_selectable) {
		return;
	}

	int pos = firstSelectedItemPosition();
	if(pos > -1) {
		UiListItem *item = itemFromPosition(pos-1);
		if(item) {
			selectItem(item->uuid(), exclusive);
		}
	}
}

void UiList::updateHeights(void)
{
	int y = 0;
	for(int i=0; i<m_items.size(); i++) {
		UiListItem* item = itemFromPosition(i);
		if(item) {
			item->move(0, y);
			y += item->height();
		}
	}
	setFixedHeight(y);
}

void UiList::mouseMoveEvent(QMouseEvent *event)
{
	if(!m_didMove && m_movable) {
		m_rect->raise();
		m_rect->show();
	}
	m_didMove = true;

	if(!m_movable || !m_clickedItem) {
		return;
	}

	for(int i=0; i<m_items.size(); i++) {
		if(QRect(m_items[i]->pos(), m_items[i]->size()).contains(event->pos())) {
			m_movePosition = m_items[i]->positionInList();
			if(m_items[i]->positionInList() <= m_clickedItem->positionInList()) {
				m_rect->move(m_items[i]->pos());
			} else {
				m_rect->move(m_items[i]->x(), m_items[i]->y()+m_items[i]->height()-m_rect->height());
			}
		}
	}

	
}

void UiList::mousePressEvent(QMouseEvent *event)
{
	//qDebug("UiList::mousePressEvent()");
	for(int i=0; i<m_items.size(); i++) {
		if(QRect(m_items[i]->pos(), m_items[i]->size()).contains(event->pos())) {
			m_clickedItem = m_items[i];
			if(event->modifiers()&Qt::ShiftModifier) {
			} else {
				if(m_items[i]->isSelected() && numItemsSelected() > 1) {
					m_clickedOnSelectedItem = true;
				} else {
					m_items[i]->select(!(event->modifiers()&Qt::ControlModifier));
				}
			}
		}
	}
	//qDebug("EXIT: UiList::mousePressEvent()");
}

void UiList::mouseReleaseEvent(QMouseEvent *event)
{
	m_rect->hide();

	if(m_clickedOnSelectedItem && !m_didMove && numItemsSelected() > 1) {
		for(int i=0; i<m_items.size(); i++) {
			if(QRect(m_items[i]->pos(), m_items[i]->size()).contains(event->pos())) {
				if(event->modifiers()&Qt::ControlModifier) {
					m_items[i]->setSelected(false);
				} else {
					m_items[i]->select();
				}
			}
		}
	}

	if(m_clickedItem && m_didMove && m_movePosition > -1) {

		int fromPosition = firstSelectedItemPosition();
		qDebug("fromPosition = %d", fromPosition);
		qDebug("m_movePosition = %d", m_movePosition);

		QList<UiListItem*> unselectedItems;
		QList<UiListItem*> selectedItems;

		int rangeLo = fromPosition;
		int rangeHi = m_movePosition;
		if(m_movePosition < fromPosition) {
			rangeLo = m_movePosition;
			rangeHi = lastSelectedItemPosition();
		}

		qDebug("rangeLo = %d, rangeHi = %d", rangeLo, rangeHi);

		for(int i=rangeLo; i<=rangeHi; i++) {
			UiListItem *item = itemFromPosition(i);
			if(item->isSelected()) {
				selectedItems.append(item);
			} else {
				unselectedItems.append(item);
			}
		}

		for(int i=0; i<selectedItems.size(); i++) {
			if(m_movePosition < fromPosition) {
				selectedItems[i]->setPositionInList(rangeLo+i);
			} else {
				selectedItems[i]->setPositionInList(rangeLo+(unselectedItems.size()+i));
			}
		}

		for(int i=0; i<unselectedItems.size(); i++) {
			if(m_movePosition < fromPosition) {
				unselectedItems[i]->setPositionInList(rangeLo+(selectedItems.size()+i));
			} else {
				unselectedItems[i]->setPositionInList(rangeLo+i);
			}
		}

		reorganise();
	}
	
	m_didMove = false;
	m_clickedOnSelectedItem = false;
	m_clickedItem = 0;
}
/*
void UiList::mouseReleaseEvent(QMouseEvent *event)
{
	m_rect->hide();

	if(m_clickedOnSelectedItem && !m_didMove && numItemsSelected() > 1) {
		for(int i=0; i<m_items.size(); i++) {
			if(QRect(m_items[i]->pos(), m_items[i]->size()).contains(event->pos())) {
				m_items[i]->select();
			}
		}
	}

	if(m_clickedItem && m_didMove && m_movePosition > -1) {
		qDebug("m_movePosition = %d", m_movePosition);
		int fromPosition = firstSelectedItemPosition(); //m_clickedItem->positionInList();
		qDebug("fromPosition = %d", fromPosition);
		int firstPosition = firstSelectedItemPosition();
		if(m_clickedItem->positionInList() != m_movePosition) {
			if(m_movePosition < fromPosition) {
				
				for(int i=0; i<m_items.size(); i++) {
					if(!m_items[i]->isSelected()) {
						if(m_items[i]->positionInList() >= m_movePosition && m_items[i]->positionInList() < fromPosition) {
							m_items[i]->setPositionInList(m_items[i]->positionInList() + numItemsSelected());
						}
					} else {
						m_items[i]->setPositionInList(m_movePosition + (m_items[i]->positionInList() - firstPosition));
					}
				}

			} else {

				for(int i=0; i<m_items.size(); i++) {
					if(!m_items[i]->isSelected()) {
						if(m_items[i]->positionInList() > fromPosition && m_items[i]->positionInList() <= m_movePosition) {
							m_items[i]->setPositionInList(m_items[i]->positionInList() - numItemsSelected());
						}
					} else {
						m_items[i]->setPositionInList(m_items[i]->positionInList() + (m_movePosition-(firstPosition+(numItemsSelected()-1))));
					}
				}

			}
		}
		reorganise();
	}
	
	m_didMove = false;
	m_clickedOnSelectedItem = false;
	m_clickedItem = 0;
}*/

void UiList::paintEvent(QPaintEvent *ev)
{
	//qDebug("UiList::paintEvent() %d, %d (%d)", width(), height(), isVisible());
	//QPainter p(this);

	//p.setPen(QColor(255,255,255));
	//p.setBrush(QColor(0,0,0));
	//p.drawRect(0,0,width()-1,height()-1);

	/*if(m_items.size() < 1) {
		if(m_emptyTitle.length() > 0) {
			p.setPen(QColor(255,255,255));
			p.setFont(QFont("Calibri", 22));
			if(m_emptyText.length() > 0) {
				p.drawText(0,0,width(),height()/2,Qt::AlignCenter|Qt::AlignVCenter, m_emptyTitle);
			} else {
				p.drawText(0,0,width(),height(),Qt::AlignCenter|Qt::AlignVCenter, m_emptyTitle);
			}
		}

		if(m_emptyText.length() > 0) {
			p.setPen(QColor(255,255,255));
			p.setFont(QFont("Calibri", 12));
			if(m_emptyTitle.length() > 0) {
				p.drawText(0,height()/2,width(),height()/2,Qt::AlignCenter|Qt::AlignVCenter, m_emptyText);
			} else {
				p.drawText(0,0,width(),height(),Qt::AlignCenter|Qt::AlignVCenter, m_emptyText);
			}
		}
	}*/
}

void UiList::resizeEvent(QResizeEvent *event)
{
	qDebug("*>*>*>*>******  UiList::resizeEvent() %d, %d", width(), height());

	m_rect->setFixedWidth(width());

	for(int i=0; i<m_items.size(); i++) {
		m_items[i]->setFixedWidth(width());
	}
}