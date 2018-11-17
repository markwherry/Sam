#ifndef UILABEL_H
#define UILABEL_H

#include "UiObject.h"
//#include "ui_UiLabel.h"

#include <QLineEdit>

class UiLabel : public UiObject
{
	Q_OBJECT

public:
	UiLabel(QWidget *parent, QString text = "", int preferredHeight = 16, int preferredWidth = -1);
	~UiLabel();

	void setImage(QString imagePath);
	void setText(QString text);
	void setTextBold(bool bold = true);
	void setTextSize(int size);
	void setDrawUnderline(bool drawUnderline = true);
	void setEditable(bool editable = true) { m_editable = editable; }

public slots:
	void acceptEdit(void);

signals:
	void textChanged(QString text);

protected:
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paint(QPainter &p);

private:
	//Ui::UiLabelClass ui;
	void calculateSize(void);
	QPixmap *m_pixmap;
	
	int m_preferredHeight;
	int m_preferredWidth;
	bool m_drawUnderline;

	bool m_editable;
	QLineEdit *m_lineEdit;
	QWidget *previousKeyboardGrabber;
	QWidget *previousFocus;
};

#endif // UILABEL_H
