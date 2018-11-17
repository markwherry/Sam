#ifndef SAMGUITEXTFIELDDIALOG_H
#define SAMGUITEXTFIELDDIALOG_H

#include <QWidget>
//#include "ui_SamGuiTextFieldDialog.h"

#include <QLineEdit>
#include <QPushButton>

class SamGuiTextFieldDialog : public QWidget
{
	Q_OBJECT

public:
	SamGuiTextFieldDialog(QWidget *parent);
	~SamGuiTextFieldDialog();

	void setText(QString text);

public slots:
	void ok(void);
	void cancel(void);

signals:
	void editConfirmed(QString text);

protected:
	void paintEvent(QPaintEvent *ev);
	void showEvent(QShowEvent *ev);

private:
	//Ui::SamGuiTextFieldDialogClass ui;
	QLineEdit *m_text;
	QPushButton *m_ok;
	QPushButton *m_cancel;
};

#endif // SAMGUITEXTFIELDDIALOG_H
