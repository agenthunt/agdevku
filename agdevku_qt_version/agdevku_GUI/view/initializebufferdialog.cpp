#include "initializebufferdialog.h"
#include "../global/StatusCodes.h"
#include "../utils/ErrorLookupTable.h"
#include <QErrorMessage>
#include <QMessageBox>

InitializeBufferDialog::InitializeBufferDialog(QWidget *parent) :
	QDialog(parent) {
	ui.setupUi(this);
	bufMgr = BufferManager::getInstance();
	ui.bufferSizeInMBLineEdit->setText("200");
	ui.pageSizeLineEdit->setText("8192");
	connect(ui.okPushButton, SIGNAL(clicked()), this,
			SLOT(handleOKPushButton()));
	connect(ui.cancelPushButton, SIGNAL(clicked()), this,
			SLOT(handleCancelPushButton()));
}

InitializeBufferDialog::~InitializeBufferDialog() {

}

void InitializeBufferDialog::handleOKPushButton() {
	double sizeInMB = ui.bufferSizeInMBLineEdit->text().toDouble();
	int pageSize = ui.pageSizeLineEdit->text().toInt();
	int error = bufMgr->initializeBuffer(sizeInMB, pageSize);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "INFO", errMsg);
	}
	close();
}

void InitializeBufferDialog::handleCancelPushButton() {
	close();
}
