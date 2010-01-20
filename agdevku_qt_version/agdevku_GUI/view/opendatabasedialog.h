#ifndef OPENDATABASEDIALOG_H
#define OPENDATABASEDIALOG_H

#include <QtGui/QDialog>
#include "ui_opendatabasedialog.h"
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>
#include "../model/ShowDatabasesModel.h"
#include "../bufmgr/BufferManager.h"
class OpenDatabaseDialog: public QDialog {
Q_OBJECT

public:
	OpenDatabaseDialog(QWidget *parent = 0);
	~OpenDatabaseDialog();
	void setUpView();

private:
	Ui::OpenDatabaseDialogClass ui;
	QTableView *showDatabasesView;
	QGridLayout *mainLayout;
	QList<QPushButton*> databasesButtonList;
	QButtonGroup *databasesButtonGroup;
	ShowDatabasesModel *model;
	BufferManager *bufMgr;


private slots:
	void handleOpenButtonClick(int id);


};

#endif // OPENDATABASEDIALOG_H
