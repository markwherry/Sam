#ifndef SAMGUISELECTMUSICTOOL_H
#define SAMGUISELECTMUSICTOOL_H

#include "UiSelectMenu.h"
//#include "ui_SamGuiSelectMusicTool.h"

#include "SamChannel.h"

class SamGuiSelectMusicTool : public UiSelectMenu
{
	Q_OBJECT

public:
	SamGuiSelectMusicTool(QWidget *parent, bool instrument = false);
	~SamGuiSelectMusicTool();

	void setChannel(SamChannel *channel);
	void setInsertIndex(int insertIndex);

	void refresh(void);

public slots:
	void selectTool(QAction *action);
	void updateTool(void);
	void doubleClickMenu(void);

signals:
	void editorOpened(int, int);

private:
	//Ui::SamGuiSelectMusicToolClass ui;
	SamChannel::Ptr m_channel;
	bool m_instrument;
	int m_insertIndex;
	QAction *m_actionNoTool;
	QList<QAction*> m_toolActions;
};

#endif // SAMGUISELECTMUSICTOOL_H
