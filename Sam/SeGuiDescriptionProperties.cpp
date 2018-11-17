#include "SeGuiDescriptionProperties.h"

#include "SamGuiEditor.h"
#include "SamDescriptionCommand.h"

SeGuiDescriptionProperties::SeGuiDescriptionProperties(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	QString metaDataTag[12] = { "author", "creator", "creationDate", "instrumentGroup", "instrumentType", "film", "keywords", "description", "comments", "copyright", "library", "numberOfPlayers" };
	QString metaDataLabel[12] = { "Author", "Creator", "Creation Date", "Instrument Group", "Instrument Type", "Film", "Keywords", "Description", "Comments", "Copyright", "Library", "Number Of Players" };

	for(int i=0; i<12; i++) {
		SeGuiItemText *itemMetaData = new SeGuiItemText(this, i, metaDataLabel[i], metaDataTag[i]);
		connect(itemMetaData, SIGNAL(textChanged(int, QString, QString)), this, SLOT(setText(int, QString, QString)));
		m_list->addItem(itemMetaData);
		m_metaDataItemList.push_back(itemMetaData);
	}

	setFixedSize(296, m_list->height());
}

SeGuiDescriptionProperties::~SeGuiDescriptionProperties()
{

}

void SeGuiDescriptionProperties::setInstrument(SamInstrument *instrument)
{
	m_instrument = instrument;
	if(m_instrument) {
		SamDescription *description = instrument->description();
		for(int i=0; i<m_metaDataItemList.size(); i++) {
			QByteArray ba = m_metaDataItemList[i]->tag().toAscii();
			int tagId = description->metaDataContentId(ba.data());
			m_metaDataItemList[i]->setText(description->metaData(tagId).toChar());
		}
	}
}

void SeGuiDescriptionProperties::setText(int propertyId, QString text, QString tag)
{
	if(!m_instrument) {
		return;
	}

	QByteArray ba = text.toAscii();
	samEditor->commandManager()->execute(new SamDescriptionCommand(m_instrument->description(), propertyId, AString(ba.data())));
}

void SeGuiDescriptionProperties::resizeEvent(QResizeEvent *ev)
{
	m_list->setFixedSize(width(),height());
}