#ifndef UILISTITEM_H
#define UILISTITEM_H

#include <QWidget>
//#include "ui_ListItem.h"

class UiListItem : public QWidget
{
	Q_OBJECT

public:
	UiListItem(QWidget *parent, int initialHeight);
	~UiListItem();

	QString uuid(void) { return m_uuid; }

	virtual void setPositionInList(int position) { m_positionInList = position; }
	virtual int positionInList(void) { return m_positionInList; }

	virtual void setSelected(bool selected);
	virtual bool isSelected(void) { return m_selected; }

	void select(bool exclusive = true);

	void setLabel(QString label);

	void changeHeight(int h);

	void setIndent(bool indent) { m_indent = indent; }
	void setBackgroundColor(int index, QColor color) { m_backgroundColor[index] = color; }

signals:
	void selected(QString uuid, bool exclusive);
	void heightChanged(void);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Ui::ListItemClass ui;
	QString m_uuid;
	int m_positionInList;
	bool m_selected;
	QColor m_backgroundColor[2];
	QString m_label;
	bool m_indent;
	
};

#endif // UILISTITEM_H
