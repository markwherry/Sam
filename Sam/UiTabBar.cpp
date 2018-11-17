#include "UiTabBar.h"

#include <QPainter>
#include <QMouseEvent>

UiTabBar::UiTabBar(QWidget *parent)
	: QWidget(parent)
	, m_selectedTab(-1)
	, m_backgroundHighlights(false)
	, m_separatorColor(QColor(180,180,180))
	, m_ceiling(0)
	, m_font(QFont("Calibri", 14))
{
	//ui.setupUi(this);
	setMouseTracking(true);
	setFixedHeight(35);
}

UiTabBar::~UiTabBar()
{

}

void UiTabBar::addTab(QString name)
{
	UiTab *tab = new UiTab(name);
	tabList.append(tab);

	if(tabList.size() == 1) {
		m_selectedTab = 0;
	}
}

void UiTabBar::selectTab(int id, bool sendSignal)
{
	if(id > -1 && id < tabList.size()) {
		m_selectedTab = id;
		if(sendSignal) {
			emit tabSelected(m_selectedTab);
		}
		update();
	}
}

void UiTabBar::clear(void)
{
	for(int i=0; i<tabList.size(); i++) {
		delete tabList[i];
	}
	tabList.clear();
	update();
}

void UiTabBar::leaveEvent(QEvent *ev)
{
	for(int i=0; i<tabList.size(); i++) {
		tabList[i]->setMouseOver(false);
	}

	update();
}

void UiTabBar::mousePressEvent(QMouseEvent *ev)
{
	int tabWidth = width()/tabList.size();

	for(int i=0; i<tabList.size(); i++) {
		if(tabList[i]->isEnabled()) {
			if(ev->x() > tabWidth*i && ev->x() < tabWidth*(i+1)) {
				selectTab(i);
				break;
			}
		}
	}
}

void UiTabBar::mouseMoveEvent(QMouseEvent *ev)
{
	int tabWidth = width()/tabList.size();

	int tabId = -1;

	for(int i=0; i<tabList.size(); i++) {
		if(ev->x() > tabWidth*i && ev->x() < tabWidth*(i+1)) {
			tabId = i;
			break;
		}
	}

	if(tabId > -1) {
		if(tabList[tabId]->isEnabled()) {
			if(!tabList[tabId]->isMouseOver()) {
				for(int i=0; i<tabList.size(); i++) {
					tabList[i]->setMouseOver(false);
				}
				tabList[tabId]->setMouseOver(true);
				update();
			}
		}
	}
}

void UiTabBar::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.fillRect(0,0,width(),height(), QColor(255,255,255,10));

	//p.setPen(QColor(255, 255, 255));
	//p.setBrush(QColor(60, 60, 60, 0));
	//p.drawRect(0, 0, width()-1, height()-1);

	//p.drawRect(3, 41, width()-7, height()-45);

	//QFont font("Calibri", m_fontSize);
	m_font.setStyleStrategy(QFont::PreferAntialias);
	p.setFont(m_font);

	p.setPen(QColor(250,250,250));

	int tabWidth = width()/tabList.size();

	for(int i=0; i<tabList.size(); i++) {
		if(tabList[i]->isMouseOver() || i == m_selectedTab) {
			p.setPen(QColor(250,250,250));
			if(m_backgroundHighlights) {

				if(i == m_selectedTab) {
					p.fillRect((tabWidth*i), 0, tabWidth, height(), QColor(255,255,255,60));
				} else {
					p.fillRect((tabWidth*i), 0, tabWidth, height(), QColor(255,255,255,30));
				}

				
			}
		} else {
			p.setPen(QColor(150,150,150));
		}
		if(!tabList[i]->isEnabled()) {
			p.setPen(QColor(60,60,60));
		}
		p.drawText((tabWidth*i)+10,0,tabWidth-20,height()-1,Qt::AlignVCenter|Qt::AlignCenter, tabList[i]->name());
		

		if(tabList[i]->isUnderline()) {
			//p.setPen(tabList[i]->underlineColor());
			//p.drawLine((tabWidth*i)+4, height()-6, (tabWidth*(i+1))-5, height()-6);
			p.fillRect((tabWidth*i), height()-4, tabWidth, 4, tabList[i]->underlineColor());
		}
	}

	for(int i=0; i<tabList.size(); i++) {
		if(i < tabList.size()-1) {
			p.setPen(m_separatorColor);
			p.drawLine(tabWidth*(i+1), m_ceiling, tabWidth*(i+1), height()-m_ceiling); //5 30
			//p.fillRect(tabWidth*(i+1)-1, 0, 3, height(), m_separatorColor);
		}
	}

	
	
}