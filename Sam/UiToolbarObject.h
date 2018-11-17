#ifndef UITOOLBAROBJECT_H
#define UITOOLBAROBJECT_H

#include <QWidget>
//#include "ui_UiToolbarObject.h"

class UiToolbarObject : public QWidget
{
	Q_OBJECT

public:
	enum UiToolBarObjectType {
		Button    = 0,
		Separator = 1,
		User      = 2
	};
	UiToolbarObject(UiToolBarObjectType type, int width);
	~UiToolbarObject();

	void setLabel(QString topLine, QString bottomLine = "") { m_label[0] = topLine; m_label[1] = bottomLine; }
	void setImage(QString filename, int state = 0);
	void setState(int state) { m_state = state; update(); }

signals:
	void clicked(void);

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::UiToolbarObjectClass ui;
	UiToolBarObjectType m_type;
	QString m_label[2];
	QPixmap *m_pixmap[2];
	bool m_mouseOver;
	int m_state;
};

#endif // UITOOLBAROBJECT_H
