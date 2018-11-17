#ifndef SEGUIZONEDISPLAY_H
#define SEGUIZONEDISPLAY_H

#include <QWidget>
//#include "ui_SeGuiZoneDisplay.h"
#include <QMenu>
#include <QRubberBand>

#include "AStateManager.h"
using namespace Atom;

#include "SamLayer.h"
#include "SeGuiZoneDisplayItem.h"
#include "SamChannel.h"

class SeGuiZoneDisplay : public QWidget//, public AStateListener 
{
	Q_OBJECT

public:
	SeGuiZoneDisplay(QWidget *parent = 0);
	~SeGuiZoneDisplay();

	//void onState(AStateEvent *ev);

	void setLayer(SamLayer *layer, SamChannel *channel);
	void clear(void);
	void rebuild(void);
	void addZone(SamZone *zone);
	void setZoneXywh(SamZone *zone);
	void clearSelection(bool shouldUpdate = true);
	void updateSelection(void);
	void selectAll(void);
	void selectZonesByPitch(int pitch, int exclusive);
	void selectZonesByVelocity(int velLo, int velHi, bool exclusive);
	void setVelocityHeight(float velocityHeight);
	
	void updateActivePitch(void);
	int zdiInfo(void) { return m_zdiInfo; }

public slots:
	void setZdiInfo(QAction *action);
	void setVelocitySplitDisplay(int display);
	void updateZoneSizes(void);
	void rebuildZones(void);

signals:
	//void zoneSelected(SamZone *zone);
	void sizeChanged(void);
	void mouseOverVelocity(int velocity);

protected:
	//void leaveEvent(QEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SeGuiZoneDisplayClass ui;
	SamLayer *m_layer;
	int m_pitchWidth;
	float m_velocityHeight;
	int m_activePitch[128];
	QList<SeGuiZoneDisplayItem*> m_zoneItems;
	bool clickedOnItem;
	SamChannel::Ptr m_channel;
	QMenu *m_popupMenu;
	QAction *m_zdiInfoActions[9];
	int m_zdiInfo;
	int m_velocitySplitDisplay;

	int clickX;
	int clickY;
	int mouseAction;
	int pitchDiff;
	int velDiff;

	QRubberBand *m_rubberBand;
	QPoint      m_rbOrigin;
};

#endif // SEGUIZONEDISPLAY_H
