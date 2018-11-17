#include "SamGuiArticulationVector.h"

#include <QPainter>

#include <math.h>

SamGuiArticulationVector::SamGuiArticulationVector(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	setWindowTitle("Articulation Vector");
	setFixedSize(300,300);
}

SamGuiArticulationVector::~SamGuiArticulationVector()
{

}

void SamGuiArticulationVector::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	p.fillRect(0,0,width()-1,height()-1, QColor(60,60,60));

	p.setPen(QColor(120,120,120));
	p.drawEllipse(20,20,width()-40,height()-40);

	p.setPen(QColor(90,90,90));
	p.drawEllipse(40,40,width()-80,height()-80);
	p.drawEllipse(60,60,width()-120,height()-120);
	p.drawEllipse(80,80,width()-160,height()-160);
	p.drawEllipse(100,100,width()-200,height()-200);
	p.drawEllipse(120,120,width()-240,height()-240);
	p.drawEllipse(140,140,width()-280,height()-280);

	p.setPen(QColor(255,255,255));

	int x1 = 150;
	int y1 = 20;
	int x2 = 150;
	int y2 = 150;

	

	for(int i=0; i<360; i+=10) {
	x1 = (int)150 + 130*cos((i * 3.142)/180.0f);
	y1 = (int)150 + 130*sin((i * 3.142)/180.0f);

	p.drawLine(x1,y1,x2,y2);
	}
}