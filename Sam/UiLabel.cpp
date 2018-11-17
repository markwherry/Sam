#include "UiLabel.h"

#include <QPainter>
#include <QFontMetrics>
#include <QMouseEvent>

UiLabel::UiLabel(QWidget *parent, QString text, int preferredHeight, int preferredWidth)
	: UiObject(parent)
	, m_pixmap(0)
	, m_preferredHeight(preferredHeight)
	, m_preferredWidth(preferredWidth)
	, m_drawUnderline(false)
	, m_editable(false)
{
	//ui.setupUi(this);

	m_lineEdit = new QLineEdit(this);
	
	m_lineEdit->hide();
	connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(acceptEdit()));
	previousKeyboardGrabber = 0;
	previousFocus = 0;

	setDrawLine(false);
	setDrawTint(false);
	setDrawFill(false);
	setDrawMouseOverFill(false);
	setTextColor(QColor(220,220,220));
	setTextBold(false);

	setText(text);

	setFont(QFont("Calibri", 10));

	if(preferredWidth > -1) {
		setFixedSize(preferredWidth, preferredHeight);
	}
}

UiLabel::~UiLabel()
{

}

void UiLabel::setImage(QString imagePath)
{
	m_pixmap = new QPixmap(imagePath);

	calculateSize();

	update();
}

void UiLabel::setText(QString text)
{
	m_text = text;
	calculateSize();
	update();
}

void UiLabel::setTextBold(bool bold)
{
	UiObject::setTextBold(bold);
	m_font.setBold(isTextBold());
	calculateSize();
	update();
}

void UiLabel::setTextSize(int size)
{
	UiObject::setTextSize(size);
	m_font.setPointSize(size);
	
	calculateSize();
	update();
}

void UiLabel::setDrawUnderline(bool drawUnderline)
{
	m_drawUnderline = drawUnderline;
	update();
}

void UiLabel::acceptEdit(void)
{
	qDebug("UiLabel::acceptEdit()");
	
//	releaseMouse();
	m_lineEdit->releaseKeyboard();
	if(previousKeyboardGrabber) {
		qDebug("Grabbing old keyboard");
		previousKeyboardGrabber->grabKeyboard();
	}

	if(previousFocus) {
		previousFocus->setFocus();
	}
	
	m_text = m_lineEdit->text();
	emit textChanged(m_text);
	


	m_lineEdit->hide();
	update();
	
	qDebug("EXIT: UiValue::acceptEdit()");
}

void UiLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	qDebug("UiLabel::mouseDoubleClickEvent()");

	if(m_editable) {

		//m_lineEdit->setText(m_text);
		m_lineEdit->setText(m_text);
		m_lineEdit->selectAll();
		m_lineEdit->show();

		previousFocus = QWidget::focusWidget();
		m_lineEdit->setFocus();

//		grabMouse();
	
		previousKeyboardGrabber = QWidget::keyboardGrabber();
		qDebug("prviousKeyboardGrabber = %d", previousKeyboardGrabber);

		m_lineEdit->grabKeyboard();

		
	}
}

void UiLabel::mousePressEvent(QMouseEvent *ev)
{
	if(!m_lineEdit->isHidden() && !m_lineEdit->rect().contains(ev->x(), ev->y())) {
		qDebug("1");
		acceptEdit();	
		qDebug("2");
	} else {
		ev->setAccepted(false);
		//QPoint point = mapToGlobal(ev->pos());
		//yClick = point.y();
		//yDirection = 0;
		//initialValue = m_value;
	}
}

void UiLabel::paint(QPainter &p)
{
	//qDebug("UiLabel::onPaint()");
	//p.fillRect(0,0,width()-1,height()-1,QColor(255,255,255));
	int xOffset = 0;
	if(m_pixmap) {
		xOffset += (m_pixmap->width()+5);
		p.drawPixmap(0, 0, *m_pixmap);
	}

	p.setFont(m_font);
	p.setPen(textColor());
	p.drawText(xOffset, 0, width()-xOffset, height(), textAlignment(), m_text);
	if(m_drawUnderline) {
		p.drawLine(0,height()-1,width()-1,height()-1);
	}
}

void UiLabel::calculateSize(void)
{
	QFontMetrics fm(m_font);

	int textW = fm.width(text());
	int textH = fm.height();

	int h = m_preferredHeight;
	int w = textW;
	if(m_preferredWidth > -1) {
		w = m_preferredWidth;
	}
	if(m_pixmap) {
		w += (m_pixmap->width() + 8);
	}
	setFixedSize(w, h);
	m_lineEdit->setFixedSize(w, h);

}