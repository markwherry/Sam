#include "UiEditor.h"

#include <QPainter>
#include <QScrollBar>

UiEditor::UiEditor(QWidget *parent, bool scrollableV, bool scrollableH)
	: QWidget(parent)
	, m_scrollableV(scrollableV)
	, m_scrollableH(scrollableH)
	, m_headerHeight(0)
	, m_scrollArea(0)
	, m_vScrollBar(0)
	, m_hScrollBar(0)
	, m_widget(0)
	, m_headerColor(QColor(90,90,90))
	, m_sideBarLeftWidth(0)
	, m_sideBarRightWidth(0)
	, m_tabBar(0)
	, m_backgroundTextVisible(false)
{
	//ui.setupUi(this);

	if(m_scrollableV || m_scrollableH) {
		QPalette p = palette();
		p.setColor(QPalette::Dark, QColor(40,40,40,0)); //50

		m_scrollArea = new QScrollArea(this);
		m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		m_scrollArea->setPalette(p);
		m_scrollArea->setBackgroundRole(QPalette::Dark);
		m_scrollArea->setFrameShape(QFrame::NoFrame);
		
		if(m_scrollableV) {
			//m_vScrollBar = new UiScrollBar(this, UiScrollBar::Vertical);
			m_vScrollBar = new QScrollBar(Qt::Vertical, this);

			connect(m_scrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(updateVScrollBarRange(int, int)));
			connect(m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateVScrollBarValue(int)));
			connect(m_vScrollBar, SIGNAL(valueChanged(int)), m_scrollArea->verticalScrollBar(), SLOT(setValue(int)));
		}
		
		if(m_scrollableH) {
			//m_hScrollBar = new UiScrollBar(this, UiScrollBar::Horizontal);
			m_hScrollBar = new QScrollBar(Qt::Horizontal, this);
			connect(m_scrollArea->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(updateHScrollBarRange(int, int)));
			connect(m_scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateHScrollBarValue(int)));
			connect(m_hScrollBar, SIGNAL(valueChanged(int)), m_scrollArea->horizontalScrollBar(), SLOT(setValue(int)));
		}

		
	}

	m_pixBackground = new QPixmap(":/Resources/black_linen_v2.png");
}

UiEditor::~UiEditor()
{

}

void UiEditor::setTitle(QString title)
{
	m_title = title;
	if(m_headerHeight < 24) {
		m_headerHeight = 24;
	}
	update();
}

void UiEditor::setWidget(QWidget *widget, int offset)
{
	m_widget = widget;
	if(m_scrollArea) {
		m_scrollArea->setWidget(m_widget);
	} else {
		m_widget->setParent(this);
		m_widget->move(3,3+m_headerHeight+0+offset); //3
		//m_widget->move(0,0+m_headerHeight+0+offset); //3
	}
}

void UiEditor::setHeaderColor(QColor color)
{
	m_headerColor = color;
	update();
}

void UiEditor::setSideBarWidth(UiEditorSideBar side, int width)
{
	if(side == Left) {
		m_sideBarLeftWidth = width;
	} else {
		m_sideBarRightWidth = width;
	}
}

void UiEditor::addWidgetToSideBar(UiEditorSideBar side, QWidget *widget)
{
	int y = 6+24;

	if(side == Left) {
		for(int i=0; i<m_sideBarLeftWidgets.size(); i++) {
			//qDebug("listSidebarWidget[%d]->height() = %d", i, listSidebarWidget[i]->height());
			y += m_sideBarLeftWidgets[i]->height()+6;
		}

		widget->setParent(this); 
		widget->setFixedWidth(m_sideBarLeftWidth-4);
		widget->move(3, y);
		m_sideBarLeftWidgets.push_back(widget);
	} else {
		qDebug("+++++ Adding widget to right");
		for(int i=0; i<m_sideBarRightWidgets.size(); i++) {
			//qDebug("listSidebarWidget[%d]->height() = %d", i, listSidebarWidget[i]->height());
			y += m_sideBarRightWidgets[i]->height()+6;
		}

		widget->setParent(this); 
		widget->setFixedWidth(m_sideBarRightWidth-4);
		widget->move(width() - 1 - m_sideBarRightWidth + 2, y);
		m_sideBarRightWidgets.push_back(widget);

	}

	
}

void UiEditor::addTab(QString name)
{
	if(!m_tabBar) {
		m_tabBar = new UiTabBar(this);
		m_tabBar->setBackgroundHighlights(true);
		m_tabBar->setSeparatorColor(QColor(0,0,0));
		connect(m_tabBar, SIGNAL(tabSelected(int)), this, SLOT(selectTab(int)));
		m_tabBar->move(3,3);
		m_tabBar->setFixedWidth(width()-6);
		resizeEvent(0);
		m_tabBar->show();
	}

	m_tabBar->addTab(name);
}

void UiEditor::clearTabs(void)
{
	if(m_tabBar) {
		m_tabBar->clear();
	}
}

int UiEditor::selectedTab(void)
{
	if(m_tabBar) {
		return m_tabBar->selectedTab();
	}

	return -1;
}

void UiEditor::setSelectedTab(int index)
{
	if(m_tabBar) {
		m_tabBar->selectTab(index);
	}
}

void UiEditor::selectTab(int id)
{
	emit tabSelected(id);
}

void UiEditor::updateVScrollBarRange(int min, int max)
{
	//qDebug("UiToolPanelContainer::updateScrollBarRange(%d, %d)", min, max);
	m_vScrollBar->setMinimum(min);
	m_vScrollBar->setMaximum(max);
	m_vScrollBar->setPageStep(m_scrollArea->verticalScrollBar()->pageStep());
}

void UiEditor::updateHScrollBarRange(int min, int max)
{
	//qDebug("UiToolPanelContainer::updateScrollBarRange(%d, %d)", min, max);
	m_hScrollBar->setMinimum(min);
	m_hScrollBar->setMaximum(max);
	m_hScrollBar->setPageStep(m_scrollArea->horizontalScrollBar()->pageStep());
}

void UiEditor::updateVScrollBarValue(int value)
{
	//qDebug("UiToolPanelContainer::updateScrollBarValue(%d)");
	m_vScrollBar->setValue(value);
	
}

void UiEditor::updateHScrollBarValue(int value)
{
	//qDebug("UiToolPanelContainer::updateScrollBarValue(%d)");
	m_hScrollBar->setValue(value);
	
}

void UiEditor::setSideBarWidgetsVisible(bool visible)
{
	for(int i=0; i<m_sideBarRightWidgets.size(); i++) {
		m_sideBarRightWidgets[i]->setVisible(visible);
	}

	for(int i=0; i<m_sideBarLeftWidgets.size(); i++) {
		m_sideBarLeftWidgets[i]->setVisible(visible);
	}
}

void UiEditor::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);
	//p.setRenderHint(QPainter::Antialiasing);

	p.setPen(QColor(90, 90, 90)); // 90
	p.setBrush(QColor(40, 40, 40)); //40 // 20
	p.drawRect(0, 0, width()-1, height()-1);//, 6.0, 6.0);

	p.setClipRegion(QRegion(3,3,width()-6,height()-6));
	for(int ix = 3; ix<width(); ix+=m_pixBackground->width()) {
		for(int iy = 3; iy<height(); iy+=m_pixBackground->height()) {
			p.drawPixmap(ix,iy,*m_pixBackground);
		}
	}//*/

	/*QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height()-1));
    linearGrad.setColorAt(0, QColor(40,40,40));
    linearGrad.setColorAt(1, QColor(30,30,30));
	p.fillRect(3, 3, width()-6, height()-6, linearGrad);//*/

	if(m_backgroundText.length() > 0 && m_backgroundTextVisible) {
		//p.fillRect((width()/2)-(480/2), (height()/2)-(120/2), 480, 120, QColor(0,0,0,40));
		p.setPen(QColor(130,130,130));
		p.setFont(QFont("Calibri", 13));
		p.drawText(0,0,width(),height(),Qt::AlignCenter|Qt::AlignVCenter, m_backgroundText);
	}
	if(m_headerHeight > 0  && !m_tabBar) {
		/*p.setPen(m_headerColor);
		p.setBrush(m_headerColor);
		p.drawRect(3, 3, width()-1-6, m_headerHeight);//, 6.0, 6.0);*/


		p.fillRect(3, 3, width()-6, m_headerHeight+1, QColor(90,90,90)); // 150
		m_headerColor.setAlpha(120);
		p.fillRect(3, 3, width()-6, m_headerHeight+1, m_headerColor);//*/

		QFont font("Calibri", 12);
		font.setStyleStrategy(QFont::PreferAntialias);
		p.setFont(font);
	
		p.setPen(QColor(240,240,240));
		p.drawText(10,3,width()-1,m_headerHeight,Qt::AlignVCenter|Qt::AlignLeft, m_title);

		/*QColor color(255,255,255);
		QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, m_headerHeight));
		color.setAlpha(40);//90
		linearGradient.setColorAt(0, color);
		color.setAlpha(5);//50
		linearGradient.setColorAt(1, color);

		p.setBrush(linearGradient);
		p.setPen(QColor(0,0,0,0));
		p.drawRect(3,3,width()-6,(m_headerHeight/2)+1);//, 6.0, 6.0);*/
	} else if(m_tabBar) {
		p.fillRect(3, 3, width()-6, m_tabBar->height(), QColor(60,60,60));
	}

	if(m_sideBarRightWidth > 0) {
		p.fillRect(width() - m_sideBarRightWidth - 1, 1, m_sideBarRightWidth, height()-2, QColor(60, 60, 60));
		p.setPen(QColor(75,75,75));
		p.drawLine(width() - m_sideBarRightWidth - 1, 1, width() - m_sideBarRightWidth - 1, height()-2);
	}

	if(m_sideBarLeftWidth > 0) {
		p.fillRect(1, 1, m_sideBarLeftWidth-1, height()-2, QColor(60, 60, 60));
		p.setPen(QColor(75,75,75));
		p.drawLine(m_sideBarLeftWidth-1, 1, m_sideBarLeftWidth-1, height()-2);
	}
}

void UiEditor::resizeEvent(QResizeEvent *ev)
{
	if(m_tabBar) {
		qDebug("***** There's a tab bar ***** (%d)", m_tabBar->height());
		m_tabBar->setFixedWidth(width()-6);
		m_headerHeight = m_tabBar->height();
	}

	if(m_scrollArea) {
		int headerOffset = 0;
		if(m_headerHeight > 0) {
			headerOffset = 3 + m_headerHeight;
		}

		int widthOffset = m_sideBarLeftWidth + m_sideBarRightWidth;
		if(m_vScrollBar) {
			m_vScrollBar->setFixedSize(12, height()-6-headerOffset);
			widthOffset += m_vScrollBar->width() + 3;
			m_vScrollBar->move(width() - widthOffset, 3 + headerOffset);
		}

		int heightOffset = 0;
		if(m_hScrollBar) {
			m_hScrollBar->setFixedSize(width()-6-widthOffset, 12);
			heightOffset += (m_hScrollBar->height() + 3);
			m_hScrollBar->move(m_sideBarLeftWidth + 3, height() - m_hScrollBar->height() -6);
		}

		m_scrollArea->move(m_sideBarLeftWidth + 3, 3 + headerOffset);
		m_scrollArea->setFixedSize(width()-6-widthOffset, height()-6-headerOffset-heightOffset);

		if(m_vScrollBar) {
			updateVScrollBarRange(m_scrollArea->verticalScrollBar()->minimum(),m_scrollArea->verticalScrollBar()->maximum());
		}

		if(m_hScrollBar) {
			updateHScrollBarRange(m_scrollArea->horizontalScrollBar()->minimum(),m_scrollArea->horizontalScrollBar()->maximum());
		}

		if(m_widget) {
			//if(m_widget->width() < m_scrollArea->width()) {
				m_widget->setFixedWidth(m_scrollArea->width());
			//}
			if(m_widget->height() < m_scrollArea->height()) {
				//m_widget->setFixedHeight(m_scrollArea->height());
				sizeWidgetHeightToFit();
			}
		}
	} else {
		if(m_widget) {
			m_widget->move(3 + m_sideBarLeftWidth,3 + m_headerHeight + 0); //0 = 3
			m_widget->setFixedSize(width()-6 - (m_sideBarLeftWidth + m_sideBarRightWidth), height()-6 - m_headerHeight -0); // 0 = 3

		}
	}

	if(m_sideBarRightWidth > 0) {
		int y = 4;
		for(int i=0; i<m_sideBarRightWidgets.size(); i++) {
			m_sideBarRightWidgets[i]->move(width() - 1 - m_sideBarRightWidth + 2, y);
			y += m_sideBarRightWidgets[i]->height()+6;
		}
		
		
	}

	if(m_sideBarLeftWidth > 0) {
		int y = 3;
		for(int i=0; i<m_sideBarLeftWidgets.size(); i++) {
			m_sideBarLeftWidgets[i]->move(3, y);
			y += m_sideBarLeftWidgets[i]->height()+6;
		}
		
		
	}

	
}

void UiEditor::sizeWidgetHeightToFit(void)
{
	if(m_widget) {
		m_widget->setFixedHeight(m_scrollArea->height());
	}
}