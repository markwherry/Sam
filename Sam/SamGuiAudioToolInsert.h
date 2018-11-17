#ifndef SAMGUIAUDIOTOOLINSERT_H
#define SAMGUIAUDIOTOOLINSERT_H

#include <QWidget>
//#include "ui_SamGuiAudioToolInsert.h"

#include "SamChannel.h"

#include "UiStateButton.h"
#include "SamGuiSelectAudioTool.h"

class SamGuiAudioToolInsert : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiAudioToolInsert(QWidget *parent, int insertIndex);
	~SamGuiAudioToolInsert();

	void onState(AStateEvent *ev);

	void setChannel(SamChannel *channel, int index = 0);

public slots:
	void setToolActive(int id, int flags);
	void updateToolActive(void);
	void showToolEditor(int id, int flags);

signals:
	void toolActiveUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiAudioToolInsert ui;

	int m_insertIndex;
	SamChannel::Ptr m_channel;
	int m_index;
	UiStateButton *m_buttonAudioTool;
	UiStateButton *m_buttonAudioToolEdit;
	SamGuiSelectAudioTool* m_selectAudioTool;
};

#endif // SAMGUIAUDIOTOOLINSERT_H
