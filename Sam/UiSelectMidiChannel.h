#ifndef UISELECTMIDICHANNEL_H
#define UISELECTMIDICHANNEL_H

#include "UiSelectMenu.h"
//#include "ui_UiSelectMidiChannel.h"

class UiSelectMidiChannel : public UiSelectMenu
{
	Q_OBJECT

public:
	UiSelectMidiChannel(QWidget *parent = 0);
	~UiSelectMidiChannel();

private:
	//Ui::UiSelectMidiChannelClass ui;
};

#endif // UISELECTMIDICHANNEL_H
