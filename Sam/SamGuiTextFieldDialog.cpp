#include "SamGuiTextFieldDialog.h"

#include <QPainter>

SamGuiTextFieldDialog::SamGuiTextFieldDialog(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	setFixedSize(220, 100);

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(70,70,70));
	setPalette(plt);

	setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
	setWindowModality(Qt::ApplicationModal);

	m_text = new QLineEdit(this);
	m_text->move(10,10);
	m_text->setFixedSize(200,30);
	m_text->setText("Preset Name");
	
	//m_name->setPlaceholderText("Type in a name for the preset...");
	m_text->setFont(QFont("Calibri", 14));

	m_ok = new QPushButton("Add", this);
	m_ok->setDefault(true);
	m_ok->move(135,70);

	m_cancel = new QPushButton("Cancel", this);
	m_cancel->move(10, 70);

	connect(m_ok, SIGNAL(clicked()), this, SLOT(ok()));
	connect(m_text, SIGNAL(returnPressed()), this, SLOT(ok()));
	connect(m_cancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

SamGuiTextFieldDialog::~SamGuiTextFieldDialog()
{

}

void SamGuiTextFieldDialog::setText(QString text)
{
	m_text->setText(text);
}

void SamGuiTextFieldDialog::ok(void)
{
	hide();

	emit editConfirmed(m_text->text());
}

void SamGuiTextFieldDialog::cancel(void)
{
	hide();
}

void SamGuiTextFieldDialog::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(255,255,255,100));
	p.setBrush(QColor(0,0,0,80));
	p.drawRect(0,0,width()-1,height()-1);
	//p.fillRect(1,1,width()-2,height()-2,QColor(0,0,0,100));
}

void SamGuiTextFieldDialog::showEvent(QShowEvent *ev)
{
	m_text->selectAll();
	m_text->setFocus(); //Qt::OtherFocusReason
	activateWindow();
}