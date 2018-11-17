#ifndef SAMGUIARTICULATIONEDITOR_H
#define SAMGUIARTICULATIONEDITOR_H

#include <QWidget>
//#include "ui_SamGuiArticulationEditor.h"

#include "AStateManager.h"

#include "SamChannel.h"

class SamGuiArticulationEditor : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiArticulationEditor(QWidget *parent = 0);
	~SamGuiArticulationEditor();

	void onState(AStateEvent *ev);

	void setChannel(SamChannel *channel);

	static QString formatArticulationName(QString str);

signals:
	void articulationUpdated(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	int articulationIndexFromPoint(QPoint point);

	//Ui::SamGuiArticulationEditor ui;
	SamChannel *m_channel;
	QList<int> m_shorts;
	QList<int> m_longs;
};

#endif // SAMGUIARTICULATIONEDITOR_H
