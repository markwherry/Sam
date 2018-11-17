#include "UiSelectMidiChannel.h"

UiSelectMidiChannel::UiSelectMidiChannel(QWidget *parent)
	: UiSelectMenu(parent, 42)
{
	//ui.setupUi(this);
	addItem("All");
	for(int i=0; i<16; i++) {
		addItem(QString::number(i+1));
	}
}

UiSelectMidiChannel::~UiSelectMidiChannel()
{

}
