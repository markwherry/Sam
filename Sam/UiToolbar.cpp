#include "UiToolbar.h"

UiToolbar::UiToolbar(QWidget *parent, int height)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFixedHeight(height);
}

UiToolbar::~UiToolbar()
{

}

void UiToolbar::addObject(UiToolbarObject *object)
{
	object->setParent(this);

	int xPos = 0;
	for(int i=0; i<m_objects.size(); i++) {
		xPos += m_objects[i]->width();
		xPos += 5;
	}

	object->move(xPos, 5);
	object->setFixedHeight(height()-10);

	m_objects.push_back(object);
}

void UiToolbar::addSeparator(void)
{
	UiToolbarObject *object = new UiToolbarObject(UiToolbarObject::Separator, 2);
	addObject(object);
}