#ifndef SAMGUISELECTAUDIOTOOL_H
#define SAMGUISELECTAUDIOTOOL_H

#include <QWidget>
//#include "ui_SamGuiSelectAudioTool.h"

#include <QMenu>

#include "SamChannel.h"

#include <QTimer>

class SamGuiSelectAudioTool : public QWidget
{
	Q_OBJECT

public:
	SamGuiSelectAudioTool(QWidget *parent, bool includeVst);
	~SamGuiSelectAudioTool();

	void setChannel(SamChannel *channel, int outputIndex, int insertIndex);
	//void setInsertIndex(int insertIndex);

public slots:
	void selectAction(QAction *action);
	void timerElapsed(void);
	//void selectTool(QAction *action);
	///void updateTool(void);

signals:
	void doubleClicked(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	//void timerEvent(QTimerEvent *ev);

private:
	//Ui::SamGuiSelectAudioTool ui;
	int m_outputIndex;
	int m_insertIndex;
	SamChannel::Ptr m_channel;
	SamAudioBus *m_bus;
	int outputIndex;
	bool m_instrument;
	QAction *m_actionNone;
	QList<QAction*> m_actionsInternal;
	QList<QAction*> m_actionsVst;


	QMenu *m_menu;
	QMenu *m_menuInternal;
	QMenu *m_menuVst;

	//int m_timerId;
	bool m_doubleClicked;
	//QTimer *m_timer;
};

#endif // SAMGUISELECTAUDIOTOOL_H
