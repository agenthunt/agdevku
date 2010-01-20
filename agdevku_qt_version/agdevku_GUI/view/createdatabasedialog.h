#ifndef CREATEDATABASEDIALOG_H
#define CREATEDATABASEDIALOG_H

#include <QtGui/QDialog>
#include "ui_createdatabasedialog.h"
#include "../bufmgr/BufferManager.h"

class CreateDatabaseDialog : public QDialog
{
    Q_OBJECT

public:
    CreateDatabaseDialog(QWidget *parent = 0);
    ~CreateDatabaseDialog();

private slots:
	void handleOKPushButton();
	void handleCancelPushButton();

private:
    Ui::CreateDatabaseDialogClass ui;
    BufferManager *bufMgr;

};

#endif // CREATEDATABASEDIALOG_H
