#include "UiSelectMenu.h"

#include <QPainter>
#include <QEvent>

UiSelectMenu::UiSelectMenu(QWidget *parent, int width, int height)
	: UiObject(parent)
	, m_doubleClick(false)
	, m_doubleClicked(false)
	, m_needToOpenMenu(false)
{
	//ui.setupUi(this);

	setFixedSize(width, height);

	setDrawLine(false);
	setDrawFill(false);
	setDrawTint(false);

	setTextColor(QColor(255,255,255));
	setTextBold(true);

	menu = new QMenu();
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(selectItem(QAction*)));

	m_selectedItem = 0;
	m_textSize = 10;
}

UiSelectMenu::~UiSelectMenu()
{
	delete menu;
}

QAction* UiSelectMenu::addItem(QString item, bool enabled)
{
	QAction *action = new QAction(item, this);
	action->setEnabled(enabled);
	menu->addAction(action);
	m_actions.append(action);
	return action;
}

void UiSelectMenu::addSeparator(void)
{
	menu->addSeparator();
}

void UiSelectMenu::clear(void)
{
	m_selectedItem = 0;
	menu->clear();
	m_actions.clear();
}

void UiSelectMenu::selectItem(QAction *action)
{
	for(int i=0; i<m_actions.size(); i++){
		if(action==m_actions[i]){
			//qDebug("Item selected: %d",i);
			setSelectedItem(i);
			emit itemSelected(i);
			emit itemSelected(action);
			return;
		}
	}
}

void UiSelectMenu::setSelectedAction(QAction *action)
{
	for(int i=0; i<m_actions.size(); i++){
		if(action==m_actions[i]){
			//qDebug("Item selected: %d",i);
			setSelectedItem(i);
		}
	}
}

void UiSelectMenu::setSelectedItem(QString text)
{
	for(int i=0; i<m_actions.size(); i++){
		if(m_actions[i]->text() == text){
			//qDebug("Item selected: %d",i);
			setSelectedItem(i);
		}
	}
}

void UiSelectMenu::setSelectedItem(int item)
{
	m_selectedItem = item;
	if(m_selectedItem<0)m_selectedItem = 0;
	if(m_selectedItem>=m_actions.size()){
		//qDebug("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^PROBLEM! (%d/%d)",m_selectedItem,actionList.size());	
		m_selectedItem = 0;
	}
	update();
}

bool UiSelectMenu::event(QEvent *ev)
{
	//qDebug("UiSelectMenu::event(%d)", ev->type());

	return QWidget::event(ev);
}

void UiSelectMenu::leaveEvent(QEvent *event)
{
	if(!menu->isVisible()) {
		UiObject::leaveEvent(event);
	}
}

void UiSelectMenu::mousePressEvent(QMouseEvent *event)
{
	m_timerId = startTimer(300);
	m_doubleClicked = false;

	if(m_doubleClick) {
		m_needToOpenMenu = true;
	} else {
		menu->popup(mapToGlobal(QPoint(0,height())));
	}
}

void UiSelectMenu::mouseDoubleClickEvent(QMouseEvent *ev)
{
	//qDebug("UiSelectMenu::mouseDoubleClickEvent()");

	if(m_doubleClick) {
		m_doubleClicked = true;
		emit doubleClicked();
	}
}

void UiSelectMenu::timerEvent(QTimerEvent *event)
{
	if(!m_doubleClicked && m_needToOpenMenu) {
		menu->popup(mapToGlobal(QPoint(0,height())));
		m_doubleClicked = false;
		m_needToOpenMenu = false;
	}

	if(!menu->isVisible() && !underMouse()) {
		//emit menuClosed();
		m_mouseOver = false;
		update();
		killTimer(m_timerId);
	}
}

void UiSelectMenu::paint(QPainter &p)
{
	int c = height()/2;
	int u = c/5;

	QPoint points[3] = {
		QPoint(c-(u*2), c-(u*1)),
		QPoint(c, c+(u*1)),
		QPoint(c+(u*2), c-(u*1))
	};

	QColor tc = textColor();
	if(!isEnabled()) {
		tc.setAlpha(100);
	}

	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(QColor(0,0,0,0));
	p.setBrush(tc);
	p.drawPolygon(points, 3);
	p.setRenderHint(QPainter::Antialiasing, false);

	QFont f("Calibri", m_textSize);
	//f.setPixelSize((height()/2)+(height()/20));
	if(isTextBold()) {
		f.setBold(true);
	}
	p.setFont(f);

	p.setPen(tc);
	if(m_actions.size()>0){
		p.drawText(height(), 0, width()-20, height(), Qt::AlignLeft|Qt::AlignVCenter, m_actions[m_selectedItem]->text());
	}
}