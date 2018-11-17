#include "SeGuiItemCurve.h"

#include <QPainter>

SeGuiItemCurve::SeGuiItemCurve(QWidget *parent, QString title, bool showStartAndEnd)
	: UiListItem(parent, 112)
	, m_shapePropertyId(-1)
	, m_startPropertyId(-1)
	, m_endPropertyId(-1)
	, m_depthPropertyId(-1)
{
	//ui.setupUi(this);

	m_title = title;

	curve = new UiCurve(this);
	curve->move(10,30);
	curve->setFixedSize(72,72);
	curve->setMargin(4);
	curve->setDrawBackground();
	connect(curve, SIGNAL(depthAdjusted(float)), this, SLOT(internalSetDepth(float)));

	labelShape = new UiLabel(this, "Shape");
	labelShape->move(92, 30);
	//labelShape->setTextBold();
	labelShape->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelShape->setTextColor(QColor(200,200,200));

	labelStart = new UiLabel(this, "Start");
	labelStart->move(92, 60);
	//labelStart->setTextBold();
	labelStart->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelStart->setTextColor(QColor(200,200,200));

	labelEnd = new UiLabel(this, "End");
	labelEnd->move(92, 80);
	//labelEnd->setTextBold();
	labelEnd->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelEnd->setTextColor(QColor(200,200,200));

	selectShape = new UiSelectMenu(this, 120);
	connect(selectShape, SIGNAL(itemSelected(int)), this, SLOT(internalSetShape(int)));
	selectShape->move(149, 30);
	selectShape->setTextColor(QColor(250,250,250));
	selectShape->setTextBold(false);
	//selectShape->setDrawBackground();
	selectShape->addItem("Linear");
	selectShape->addItem("Linear Root");
	selectShape->addItem("Linear Squared");
	selectShape->addItem("Quadratic");
	selectShape->addItem("Quadratic Root");
	selectShape->addItem("Quadratic Squared");
	selectShape->addItem("Sine");
	//selectShape->addItem("Linear Cubed");
	//selectShape->addItem("Linear Quad");

	valueStart = new UiValue(this, 40, 20, "%", 0, 100, 0, 1, 3);
	valueStart->move(149,60);
	valueStart->setLiveUpdate();
	valueStart->setValueTextColor(QColor(255,255,255));
	valueStart->setUnitTextColor(QColor(200,200,200));
	connect(valueStart, SIGNAL(valueChanged(float)), this, SLOT(internalSetStart(float)));

	valueEnd = new UiValue(this, 40, 20, "%", 0, 100, 0, 1, 3);
	valueEnd->move(149,80);
	valueEnd->setLiveUpdate();
	valueEnd->setValueTextColor(QColor(255,255,255));
	valueEnd->setUnitTextColor(QColor(200,200,200));
	connect(valueEnd, SIGNAL(valueChanged(float)), this, SLOT(internalSetEnd(float)));

	if(!showStartAndEnd) {
		labelStart->hide();
		labelEnd->hide();
		valueStart->hide();
		valueEnd->hide();
	}
}

SeGuiItemCurve::~SeGuiItemCurve()
{

}

void SeGuiItemCurve::setShape(int id)
{
	curve->setShape(id);
	selectShape->setSelectedItem(id);
}

void SeGuiItemCurve::setStart(float start)
{
	curve->setStart(start);
	valueStart->setValue(start*100);
}

void SeGuiItemCurve::setEnd(float end)
{
	curve->setEnd(end);
	valueEnd->setValue(end*100);
}

void SeGuiItemCurve::setDepth(float q)
{
	curve->setDepth(q);
}

void SeGuiItemCurve::internalSetShape(int id)
{
	curve->setShape(id);
	emit shapeChanged(m_shapePropertyId, id);
}

void SeGuiItemCurve::internalSetStart(float percentage)
{
	curve->setStart(percentage/100);
	emit startChanged(m_startPropertyId, percentage/100);
}

void SeGuiItemCurve::internalSetEnd(float percentage)
{
	curve->setEnd(percentage/100);
	emit endChanged(m_endPropertyId, percentage/100);
}

void SeGuiItemCurve::internalSetDepth(float q)
{
	emit depthChanged(m_depthPropertyId, q);
}

void SeGuiItemCurve::paintEvent(QPaintEvent *event)
{
	UiListItem::paintEvent(event);

	QPainter p(this);

	p.setPen(QColor(200,200,200));
	QFont f("", 8);
	f.setBold(true);
	p.setFont(f);
	p.drawText(10,4,width()-1,20,Qt::AlignLeft|Qt::AlignTop,m_title);

	p.setPen(QColor(200,200,200,100));
	p.drawLine(10,20,width()-10,20);
}