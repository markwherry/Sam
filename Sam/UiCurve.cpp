#include "UiCurve.h"

#include <QPainter>

UiCurve::UiCurve(QWidget *parent)
	: QWidget(parent)
	, m_drawBackground(false)
	, m_margin(0)
	, m_mouseOver(false)
	, m_color(QColor(255,255,255))
{
	//ui.setupUi(this);

	m_curve = new CsCurve();

	m_fader = new UiFader(this,true,0,10, 10);
	m_fader->setValueNormalised(m_curve->depth());
	connect(m_fader, SIGNAL(valueChanged(int, float, int)), this, SLOT(setDepthFromFader(int, float, int)));
	m_fader->setOpacity(0.8);
	m_fader->setLevelColor(QColor(255,255,255,150));
	//m_fader->setRoundedStyle();
	//m_fader->setValue((int)((float)m_curve->q()*100));
	m_fader->hide();

	m_mouseOver = false;
}

UiCurve::~UiCurve()
{

}

void UiCurve::setDepth(int q)
{
	
	float fQ = (float)q/100;
	m_fader->setValueNormalised(fQ);
	setDepth(fQ);
}

void UiCurve::setDepth(float q)
{
	//qDebug("SamGuiCurve::setQ(float=%f)", q);
	m_fader->setValueNormalised(q);
	m_curve->setDepth(q);
	update();
	//emit qAdjusted();
}

void UiCurve::setDepthFromFader(int, float q, int)
{
	qDebug("UiCurve::setDepthFromFader(%f)", q);
	//float fQ = (float)q/100;
	
	
	
	m_curve->setDepth(q);
	update();
	emit depthAdjusted(q);
}

void UiCurve::enterEvent(QEvent *event)
{
	m_mouseOver = true;
	m_fader->setFixedSize(10,height());
	m_fader->move((width()/2)-(10/2),0);
	m_fader->show();
}

void UiCurve::leaveEvent(QEvent *event)
{
	m_mouseOver = true;
	m_fader->hide();
}

void UiCurve::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	// Draw background
	if(m_drawBackground) {
		p.setPen(QColor(30,30,30));
		p.setBrush(QColor(60,60,60));
		p.drawRect(0,0,width()-1,height()-1);

		// Draw grid
		/*p.setPen(QColor(180,180,180));
		p.drawRect(m_margin,m_margin,(width())-(m_margin*2),(height())-(m_margin*2));
		int stepX = ((width())-(m_margin*2))/m_margin;
		int stepY = ((height())-(m_margin*2))/m_margin;
		for(int i=0; i<3; i++) {
			p.drawLine(m_margin+(stepX*(i+1)),m_margin,m_margin+(stepX*(i+1)),(height())-m_margin);
			p.drawLine(m_margin,m_margin+(stepY*(i+1)),(width())-m_margin,m_margin+(stepY*(i+1)));
		}//*/
	}

	// Draw curve
	if(m_curve) {
		p.setRenderHint(QPainter::Antialiasing, true);
		//QPen pen(QColor(0,0,150,80));
		QPen pen(m_color);
		//QPen pen(QColor(100,200,100));
		pen.setWidth(2);
		p.setPen(pen);
		float yUnit = ((height()-(m_margin*2))/1.0);
		//qDebug("unit = %f", unit);

		float xUnit = 1;//width()/m_curve->length();
		
			for(int i=1; i<m_curve->length(); i++) {
				p.drawLine(m_margin+(float)((i-1)*xUnit), (float)(height()-m_margin)-(m_curve->point(i-1)*yUnit), m_margin+(float)(i*xUnit), (float)(height()-m_margin)-(m_curve->point(i)*yUnit));
				//qDebug("%f,%f -> %f, %f",(float)(i-1)*2, (float)256-(point[i-1]*unit), (float)i*2, (float)256-(point[i]*unit));
			}
	}

	// Draw Tint
	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, height()));
	color.setAlpha(20);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(0);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	p.drawRect(1,1,width()-2,(height()/2));
}

void UiCurve::resizeEvent(QResizeEvent *event)
{
	m_curve->setLength(width()-(m_margin*2));

	update();
}