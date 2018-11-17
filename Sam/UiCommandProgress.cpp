#include "UiCommandProgress.h"

#include <QCloseEvent>
#include <QPainter>

UiCommandProgress::UiCommandProgress(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);
	setWindowTitle("Progress");
	setWindowModality(Qt::ApplicationModal);

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(50,50,50));
	setPalette(plt);

	setFixedSize(400,130);

	progressBar = new UiProgressBar(this);
	progressBar->move(10,60);
	progressBar->setFixedSize(width()-20, 20);

	for(int i=0; i<2; i++) {
		label[i] = new UiLabel(this);
		label[i]->setTextColor(QColor(255,255,255));
		label[i]->setFixedSize(380,20);
		//label[i]->move(10, 10+(i*60));
		//label[i]->setFont(QFont("Calibri", 9));
	}

	label[0]->setFont(QFont("Calibri", 12));
	label[1]->setFont(QFont("Calibri", 9));
	label[0]->move(10, 10);
	label[1]->move(10, 95);
	//label[0]->setTextBold(true);
	//label[1]->setTextBold(false);	
}

UiCommandProgress::~UiCommandProgress()
{

}

void UiCommandProgress::clear(void)
{
	progressBar->setValueNormalised(0);
	for(int i=0; i<2; i++) {
		label[i]->setText("");
	}
}

void UiCommandProgress::setProgress(int commandId, float progress)
{
	progressBar->setValueNormalised(progress);
}

void UiCommandProgress::setProgressLabel(int commandId, int labelId, QString text)
{
	label[labelId]->setText(text);
}

void UiCommandProgress::closeEvent(QCloseEvent *ev)
{
	ev->setAccepted(false);
}

void UiCommandProgress::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.fillRect(0,0,width()-1,40,QColor(0,0,0,100));

	/*QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, 60));
	linearGradient.setColorAt(0, QColor(0,0,0,220));
	linearGradient.setColorAt(1, QColor(0,0,0,0));

	p.fillRect(0,0,width()-1,60,linearGradient);*/

	//EStyle::paintBackground(&p,this);
	p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	/*QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}//*/
	//
}