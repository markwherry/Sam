#ifndef SAMGUIMUSICTOOLINSERT_H
#define SAMGUIMUSICTOOLINSERT_H

#include <QWidget>
//#include "ui_SamGuiMusicToolInsert.h"

#include "SamChannel.h"

#include "UiStateButton.h"
#include "SamGuiSelectMusicTool.h"

class SamGuiMusicToolInsert : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiMusicToolInsert(QWidget *parent);
	~SamGuiMusicToolInsert();

	void onState(AStateEvent *ev);

	void setChannel(SamChannel *channel, int index);

public slots:
	void setMusicToolActive(int id, int flags);
	void updateMusicToolActive(void);
	void showMusicToolEditor(int id, int flags);

signals:
	void musicToolActiveUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiMusicToolInsertClass ui;
	SamChannel::Ptr m_channel;
	int m_index;
	UiStateButton *buttonMusicTool;
	UiStateButton *buttonMusicToolEdit;
	SamGuiSelectMusicTool* selectMusicTool;
};

#endif // SAMGUIMUSICTOOLINSERT_H
