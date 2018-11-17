#include "SamGuiInstrumentList.h"

#include "SamEngine.h"

SamGuiInstrumentList::SamGuiInstrumentList(QWidget *parent)
	: UiList(parent)
{
	//ui.setupUi(this);

	setMovable(false);

	for(int i=0; i<samEngine->database()->numEntries(); i++) {
		UiListItem *item = new UiListItem(this, 20);
		item->setLabel(samEngine->database()->entry(i)->name().toChar());
		addItem(item);
	}
}

SamGuiInstrumentList::~SamGuiInstrumentList()
{

}
