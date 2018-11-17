#ifndef SEGUIVERSIONPROPERTIES_H
#define SEGUIVERSIONPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiVersionProperties.h"

#include "SamChannel.h"

#include "UiList.h"
#include "SeGuiItemText.h"
#include "SeGuiItemValue.h"
#include "SeGuiItemHeading.h"
#include "SeGuiItemSelectMusicTool.h"
#include "SeGuiItemSelectArticuation.h"

#include "SamVersion.h"

class SeGuiVersionProperties : public QWidget
{
	Q_OBJECT

public:
	SeGuiVersionProperties(QWidget *parent = 0);
	~SeGuiVersionProperties();

	void setVersion(SamVersion *version, SamChannel *channel);

public slots:
	void setValue(int propertyId, float value);
	void setText(int propertyId, QString text);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	//Ui::SeGuiVersionPropertiesClass ui;
	SamVersion *m_version;
	SamChannel *m_channel;
	UiList *m_list;
	SeGuiItemText *m_name;
	SeGuiItemValue *m_defaultVoices;
	SeGuiItemValue *m_reference;
	SeGuiItemSelectArticuation *m_articulation;
	//SeGuiItemHeading *m_instrumentMusicTools;
	//SeGuiItemSelectMusicTool *m_musicTools[SamChannel::maxMusicTools];
};

#endif // SEGUIVERSIONPROPERTIES_H
