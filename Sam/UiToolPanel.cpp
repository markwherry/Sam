#include "UiToolPanel.h"
#include <QPainter>
#include <QMouseEvent>

UiToolPanel::UiToolPanel(QWidget *parent)
	: QWidget(parent)
	, m_mouseOver(false)
{
	//ui.setupUi(this);
	setMouseTracking(true);
	setFixedWidth(304);

	pixAddRemove[0] = new QPixmap(":/Resources/panel_add_remove_60.png");
	pixAddRemove[1] = new QPixmap(":/Resources/panel_add_remove_100.png");
}

UiToolPanel::~UiToolPanel()
{

}

void UiToolPanel::addSection(QString title, QWidget *widget)
{
	if(!widget) {
		return;
	}

	UiToolPanelSection *section = new UiToolPanelSection(title, widget);
	widget->setParent(this);
	if(section->isOpen()) {
	} else {
		widget->hide();
	}
	sectionList.append(section);

	calculatePositions();
}

void UiToolPanel::addActionToMenu(int sectionId, QAction *action)
{
	if(sectionId >= 0 && sectionId < sectionList.size()) {
		sectionList[sectionId]->addActionToMenu(action);
	}
}

void UiToolPanel::addSeparatorToMenu(int sectionId)
{
	if(sectionId >= 0 && sectionId < sectionList.size()) {
		sectionList[sectionId]->addSeparatorToMenu();
	}
}

void UiToolPanel::setSectionTitle(int index, QString title)
{
	if(index >= 0 && index < sectionList.size()) {
		sectionList[index]->setTitle(title);
		update();
	}
}

void UiToolPanel::enterEvent(QEvent *ev)
{
	//qDebug("ToolPanel::enterEvent()");
	m_mouseOver = true;
	update();
}

void UiToolPanel::leaveEvent(QEvent *ev)
{
	m_mouseOver = false;

	for(int i=0; i<sectionList.size(); i++) {
		sectionList[i]->setMouseOver(false);
		sectionList[i]->setMouseOverMenu(false);
	}

	update();
}

void UiToolPanel::mousePressEvent(QMouseEvent *ev)
{
	int sectionId = -1;

	for(int i=0; i<sectionList.size(); i++) {
		if(ev->y() >= sectionList[i]->y() && ev->y() < sectionList[i]->y()+HEADER_SIZE) {
			sectionId = i;
			break;
		}
	}

	if(sectionId > -1) {
		//qDebug("Clicked on section %d", sectionId);
		if(sectionList[sectionId]->hasMenu() && ev->x() > (width()-pixAddRemove[0]->width()-30)) {
			QPoint point = mapToGlobal(QPoint((width()-pixAddRemove[0]->width()-15),sectionList[sectionId]->y()+pixAddRemove[0]->height()+10));
			sectionList[sectionId]->menu()->popup(point);
		} else {
			sectionList[sectionId]->toggle();
			calculatePositions();
		}
	}
}

void UiToolPanel::mouseMoveEvent(QMouseEvent *ev)
{
	//qDebug("ToolPanel::mouseMoveEvent()");

	int sectionId = -1;

	for(int i=0; i<sectionList.size(); i++) {
		if(ev->y() >= sectionList[i]->y() && ev->y() < sectionList[i]->y()+HEADER_SIZE) {
			sectionId = i;
			break;
		}
	}

	if(sectionId > -1) {
		if(sectionList[sectionId]->hasMenu() && ev->x() > (width()-pixAddRemove[0]->width()-30)) {
			if(!sectionList[sectionId]->isMouseOverMenu()) {
				for(int i=0; i<sectionList.size(); i++) {
					sectionList[i]->setMouseOver(false);
					sectionList[i]->setMouseOverMenu(false);
				}
				
				sectionList[sectionId]->setMouseOverMenu(true);
				update();
			}
		} else {
			if(!sectionList[sectionId]->isMouseOver()) {
				for(int i=0; i<sectionList.size(); i++) {
					sectionList[i]->setMouseOver(false);
					sectionList[i]->setMouseOverMenu(false);
				}
				
				sectionList[sectionId]->setMouseOver(true);
				update();
			}
		}
	} else {
		for(int i=0; i<sectionList.size(); i++) {
			sectionList[i]->setMouseOver(false);
			sectionList[i]->setMouseOverMenu(false);
		}

		update();
	}
}

void UiToolPanel::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	

	int bgc = 60; // was 40
	p.setPen(QColor(bgc,bgc,bgc));
	p.setBrush(QColor(bgc,bgc,bgc));
	p.drawRect(0,0,width()-1,height()-1);

	QFont font("Calibri", 12);
	font.setStyleStrategy(QFont::PreferAntialias);
	p.setFont(font);

	//p.setPen(QColor(160,160,160));
	//p.drawLine(0,0,width()-1,0);
	//p.setPen(QColor(30,30,30));
	//p.drawLine(0,1,width()-1,1);

	for(int i=0; i<sectionList.size(); i++) {
		//p.drawRect(0, sectionList[i]->y(), width()-1, HEADER_SIZE-1);
		//p.setPen(QColor(30,30,30));
		//p.drawLine(0,sectionList[i]->y()+HEADER_SIZE-2,width()-1,sectionList[i]->y()+HEADER_SIZE-2);

		

		

		if(sectionList[i]->isOpen()) {
			int sx = sectionList[i]->widget()->x()-1;
			int sy = sectionList[i]->widget()->y()-1;
			int sxx = sx+sectionList[i]->widget()->width()+2;
			int syy = sy+sectionList[i]->widget()->height()+2;
			p.setPen(QColor(0,0,0));
			p.drawLine(sx, syy, sx, sy);
			p.drawLine(sx, sy, sxx, sy);
			p.setPen(QColor(00,00,00));
			p.drawLine(sxx, sy, sxx, syy);
			p.drawLine(sxx, syy, sx, syy);
			
		}

		{
		QColor color(255,255,255);
		QLinearGradient linearGradient(QPointF(0, sectionList[i]->y()), QPointF(0, sectionList[i]->y()+HEADER_SIZE-1));
	color.setAlpha(20);//90//40
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	p.drawRect(0,sectionList[i]->y(),width()-1,HEADER_SIZE-1);
	}

		p.setRenderHint(QPainter::Antialiasing, false);

		p.setPen(QColor(50,50,50)); //180
		p.drawLine(0,sectionList[i]->y()+sectionList[i]->height()-1,width()-1,sectionList[i]->y()+sectionList[i]->height()-1);
		p.setPen(QColor(30,30,30)); //40
		p.drawLine(0,sectionList[i]->y()+sectionList[i]->height(),width()-1,sectionList[i]->y()+sectionList[i]->height());
		
		int textColor = 180;
		if(sectionList[i]->isOpen()) {
			textColor = 250;
		} else {
		}
		if(sectionList[i]->isMouseOver()) {
			textColor = 250;
		}
		p.setPen(QColor(textColor,textColor,textColor));
		p.drawText(27,sectionList[i]->y(),width(),HEADER_SIZE-1,Qt::AlignVCenter|Qt::AlignLeft, sectionList[i]->title());

		
		float triangleSize = 8.0;
		float triangleMargin = (HEADER_SIZE/2.0)-(triangleSize/2.0);

		QPointF points[3]; 
		
		if(sectionList[i]->isOpen()) {
			points[0] = QPointF(10.0, sectionList[i]->y()+triangleMargin);
			points[1] = QPointF(10.0 + triangleSize, sectionList[i]->y()+triangleMargin);
			points[2] = QPointF(10.0 + (triangleSize/2.0), sectionList[i]->y()+(HEADER_SIZE-triangleMargin));
		} else {
			points[0] = QPointF(10.0, sectionList[i]->y()+triangleMargin);
			points[1] = QPointF(10.0, (sectionList[i]->y()+HEADER_SIZE)-triangleMargin);
			points[2] = QPointF(10.0 + triangleSize, sectionList[i]->y()+(HEADER_SIZE/2.0));
		}
		p.setRenderHint(QPainter::Antialiasing);
		//p.setPen(QColor(50,50,50));
		p.setBrush(QColor(textColor,textColor,textColor));
		p.drawPolygon(points, 3);

		if(sectionList[i]->hasMenu()) {
			int mom = (int)sectionList[i]->isMouseOverMenu();
			p.drawPixmap(width()-pixAddRemove[mom]->width()-15, sectionList[i]->y()+10, *pixAddRemove[mom]);
		}
	}

	int yacc = (sectionList[sectionList.size()-1]->y() + sectionList[sectionList.size()-1]->height());

	/*p.setRenderHint(QPainter::Antialiasing, false);
	p.setPen(QColor(160,160,160));
	p.drawLine(0,yacc,width()-1,yacc);
	p.setPen(QColor(45,45,45));
	p.drawLine(0,yacc+1,width()-1,yacc+1);*/

	
}

void UiToolPanel::calculatePositions(void)
{
	int yacc = 0;
	for(int i=0; i<sectionList.size(); i++) {
			sectionList[i]->setY(yacc);
		if(sectionList[i]->isOpen()) {
			sectionList[i]->setHeight(sectionList[i]->widget()->height() + HEADER_SIZE + 6);
			sectionList[i]->widget()->move(4, sectionList[i]->y() + HEADER_SIZE);
			yacc += (HEADER_SIZE + sectionList[i]->widget()->height() + 6);
		} else {
			sectionList[i]->setHeight(HEADER_SIZE);
			yacc += HEADER_SIZE;
		}
	}
	setFixedHeight(yacc+1);
	qDebug("yacc=%d", yacc);
	update();
}