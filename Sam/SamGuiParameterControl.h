#ifndef SAMGUIPARAMETERCONTROL_H
#define SAMGUIPARAMETERCONTROL_H

#include <QWidget>
//#include "ui_SamGuiParameterControl.h"

#include "SamChannel.h"
#include "SamParameter.h"

class SamGuiParameterControl : public QWidget
{
	Q_OBJECT

public:
	SamGuiParameterControl(QWidget *parent, SamChannel *channel, SamParameter *parameter);
	~SamGuiParameterControl();

protected:
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void hideEvent(QShowEvent *ev);

private:
	//Ui::SamGuiParameterControlClass ui;
	SamChannel::Ptr m_channel;
	SamParameter *m_parameter;
};

#endif // SAMGUIPARAMETERCONTROL_H
