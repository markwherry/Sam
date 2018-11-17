#ifndef SAMGUIPRESETDIALOG_H
#define SAMGUIPRESETDIALOG_H

#include <QWidget>
//#include "ui_SamGuiPresetDialog.h"

#include <QLineEdit>
#include <QPushButton>

class SamGuiPresetDialog : public QWidget
{
	Q_OBJECT

public:
	SamGuiPresetDialog(QWidget *parent, QString uuid);
	~SamGuiPresetDialog();

public slots:
	void ok(void);
	void cancel(void);

signals:
	void presetAdded(QString uuid, QString name);

protected:
	void paintEvent(QPaintEvent *ev);
	void showEvent(QShowEvent *ev);

private:
	//Ui::SamGuiPresetDialogClass ui;
	QLineEdit *m_name;
	QPushButton *m_ok;
	QPushButton *m_cancel;
	QString m_uuid;
};

#endif // SAMGUIPRESETDIALOG_H
