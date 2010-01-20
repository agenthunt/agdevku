#ifndef INITIALIZEBUFFERDIALOG_H
#define INITIALIZEBUFFERDIALOG_H

#include <QtGui/QDialog>
#include "ui_initializebufferdialog.h"
#include "../bufmgr/BufferManager.h"
class InitializeBufferDialog : public QDialog
{
    Q_OBJECT

public:
    InitializeBufferDialog(QWidget *parent = 0);
    ~InitializeBufferDialog();

private slots:
	void handleOKPushButton();
	void handleCancelPushButton();

private:
    Ui::InitializeBufferDialogClass ui;
    BufferManager *bufMgr;
};

#endif // INITIALIZEBUFFERDIALOG_H
