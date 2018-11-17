#ifndef SEGUIITEMSELECTMUSICTOOL_H
#define SEGUIITEMSELECTMUSICTOOL_H

#include "UiListItem.h"
//#include "ui_SeGuiItemSelectMusicTool.h"

#include "SamVersion.h"
#include "SamChannel.h"

#include "SamGuiSelectMusicTool.h"

class SeGuiItemSelectMusicTool : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemSelectMusicTool(QWidget *parent, int propertyId, QString label);
	~SeGuiItemSelectMusicTool();

	void setVersion(SamVersion *version, SamChannel *channel);
	//void setOutput(int outputListId);

	//void updateOutputList(void);

public slots:
	//void internalSetOutput(int output);

signals:
	//void outputChanged(int, float);

protected:
	void changeEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemSelectMusicToolClass ui;
	SamVersion *m_version;
	SamChannel *m_channel;
	int m_propertyId;
	QString m_label;
	SamGuiSelectMusicTool *m_selectMusicTool;
};

#endif // SEGUIITEMSELECTMUSICTOOL_H
