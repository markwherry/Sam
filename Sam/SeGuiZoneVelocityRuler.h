#ifndef SEGUIZONEVELOCITYRULER_H
#define SEGUIZONEVELOCITYRULER_H

#include <QWidget>
//#include "ui_SeGuiZoneVelocityRuler.h"
#include <QMenu>

#include "SamChannel.h"

class SeGuiZoneVelocityRuler : public QWidget
{
	Q_OBJECT

public:
	SeGuiZoneVelocityRuler(QWidget *parent = 0);
	~SeGuiZoneVelocityRuler();

	void setChannel(SamChannel *channel);
	void setVelocityHeight(float velocityHeight);

public slots:
	void setVelocitySplitDisplay(QAction *action);
	void setMouseVelocity(int velocity);

signals:
	void velocitySplitDisplayChanged(int display);

protected:
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SeGuiZoneVelocityRulerClass ui;
	float m_velocityHeight;
	QMenu *m_popupMenu;
	QAction *m_velocitySplitDisplayActions[2];
	int m_velocitySplitDisplay;
	SamChannel::Ptr m_channel;
	int m_mouseVelocity;
};

#endif // SEGUIZONEVELOCITYRULER_H
