#include "SamGuiPresetDialog.h"

#include <QPainter>

SamGuiPresetDialog::SamGuiPresetDialog(QWidget *parent, QString uuid)
	: QWidget(parent)
	, m_uuid(uuid)
{
	//ui.setupUi(this);

	setFixedSize(220, 100);

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(70,70,70));
	setPalette(plt);

	setWindowTitle("Add Preset...");
	setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
	setWindowModality(Qt::ApplicationModal);

	m_name = new QLineEdit(this);
	m_name->move(10,10);
	m_name->setFixedSize(200,30);
	m_name->setText("Preset Name");
	
	//m_name->setPlaceholderText("Type in a name for the preset...");
	m_name->setFont(QFont("Calibri", 14));

	m_ok = new QPushButton("Add", this);
	m_ok->setDefault(true);
	m_ok->move(135,70);

	m_cancel = new QPushButton("Cancel", this);
	m_cancel->move(10, 70);

	connect(m_ok, SIGNAL(clicked()), this, SLOT(ok()));
	connect(m_name, SIGNAL(returnPressed()), this, SLOT(ok()));
	connect(m_cancel, SIGNAL(clicked()), this, SLOT(cancel()));
	

	
	
}

SamGuiPresetDialog::~SamGuiPresetDialog()
{

}

void SamGuiPresetDialog::ok(void)
{
	hide();

	emit presetAdded(m_uuid, m_name->text());
}

void SamGuiPresetDialog::cancel(void)
{
	hide();
}

void SamGuiPresetDialog::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(255,255,255,100));
	p.setBrush(QColor(0,0,0,0));
	p.drawRect(0,0,width()-1,height()-1);
}

void SamGuiPresetDialog::showEvent(QShowEvent *ev)
{
	m_name->selectAll();
	m_name->setFocus(); //Qt::OtherFocusReason
	activateWindow();
}