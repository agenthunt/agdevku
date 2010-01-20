#include "createdatabasedialog.h"

#include "../global/StatusCodes.h"
#include "../utils/ErrorLookupTable.h"
#include "../utils/debug.h"
#include <QErrorMessage>
#include <QMessageBox>

CreateDatabaseDialog::CreateDatabaseDialog(QWidget *parent) :
	QDialog(parent) {
	bufMgr = BufferManager::getInstance();
	ui.setupUi(this);
	setModal(true);
	ui.bufferSizeInMBLineEdit->setText("200");
	ui.pageSizeLineEdit->setText("8192");
	ui.numberOfPagesLineEdit->setText("1000");
	connect(ui.okPushButton, SIGNAL(clicked()), this,
			SLOT(handleOKPushButton()));
	connect(ui.cancelPushButton, SIGNAL(clicked()), this,
			SLOT(handleCancelPushButton()));

}

CreateDatabaseDialog::~CreateDatabaseDialog() {

}

void CreateDatabaseDialog::handleOKPushButton() {
	double sizeInMB = ui.bufferSizeInMBLineEdit->text().toDouble();
	int pageSize = ui.pageSizeLineEdit->text().toInt();
	DEBUG("pageSize in create database dialog"<<pageSize);
	int numOfPages = ui.numberOfPagesLineEdit->text().toInt();
	std::string str = ui.databaseNameLineEdit->text().toUpper().toStdString();
	const char *databaseName = str.c_str();
	int error = bufMgr->initializeBuffer(sizeInMB, pageSize);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	}
	error = bufMgr->createDatabase(databaseName, numOfPages);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "INFO", errMsg);
	}
	close();
}

void CreateDatabaseDialog::handleCancelPushButton() {
	close();
}
