#ifndef SAMGUIBOUNCEGROUP_H
#define SAMGUIBOUNCEGROUP_H

#include <QWidget>
//#include "ui_SamGuiBounceGroup.h"

#include "UiLabel.h"
#include "UiSelectMenu.h"

#include "SamBounceGroup.h"

class SamGuiBounceGroup : public QWidget
{
	Q_OBJECT

public:
	SamGuiBounceGroup(QWidget *parent, int id);
	~SamGuiBounceGroup();

	void setActive(bool active);
	bool isActive(void) { return m_active; }

	QString prefix(void) { return m_label->text(); }

	SamBounceGroup* createBounceGroup(void);

signals:
	void activated(int id);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiBounceGroupClass ui;
	int m_id;
	bool m_active;
	UiLabel *m_label;
	UiSelectMenu *m_selectMenu;
};

#endif // SAMGUIBOUNCEGROUP_H
