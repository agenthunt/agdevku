#include "viewframedialog.h"
#include "../utils/HexFormatter.h"
#include <QMessageBox>
#include <QStringList>
#include "../utils/ErrorLookupTable.h"
#include "../utils/IntelliFormatter.h"
#include "../utils/PageTypeLookup.h"
#include <string>
ViewFrameDialog::ViewFrameDialog(int frameNumber, QWidget *parent) :
	QDialog(parent) {
	ui.setupUi(this);
	refreshButton = new QPushButton("REFRESH");
	mainLayout = new QVBoxLayout();
	splitter = new QSplitter();
	formattedViewer = new QTextEdit();
	formattedViewer->setReadOnly(true);
	hexViewer = new QTextEdit();
	splitter->addWidget(formattedViewer);
	splitter->addWidget(hexViewer);
	hexViewer->setReadOnly(true);
	mainLayout->addWidget(refreshButton);
	mainLayout->addWidget(splitter);
	setLayout(mainLayout);
	frameNumber_ = frameNumber;
	refreshView();
	connect(refreshButton, SIGNAL(clicked()), this,
			SLOT(handleRefreshButtonClicked()));
}

ViewFrameDialog::~ViewFrameDialog() {
	bufMgr->unPinPage(pageNumber, false);
}

void ViewFrameDialog::refreshView() {
	bufMgr = BufferManager::getInstance();
	//pageNumber = bufMgr->bufferPool_[frameNumber]->pageNumber_;
	int error = bufMgr->getPageNumber(frameNumber_, pageNumber);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
		close();
	} else {
		char *pageData;

		error = bufMgr->pinAndGetPage(pageNumber, pageData);
		if (error != SUCCESS) {
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			QMessageBox::warning(0, "ERROR", errMsg);
			close();
		} else {

			//hexData= HexFormatter::format(pageData,bufMgr->getCurrentlyUsingPageSize());


			//QString *hexQString = new QString(hexData.c_str());
			QString *hexQString;
			char *formattedString;
			hexData = HexFormatter::format(pageData,
					bufMgr->getCurrentlyUsingPageSize(), formattedString);
			hexQString = new QString(hexData.c_str());
			hexViewer->setText(*hexQString);

			intelliData = IntelliFormatter::format(pageData);
			QString *intelliString = new QString(intelliData.c_str());
			formattedViewer->setText(*intelliString);

			GeneralPageHeaderAccessor genPageAccessor(pageData);
			int pageType = genPageAccessor.getPageType();

			QString titleQString;

			titleQString.append(QString::fromStdString("PAGE NUMBER="));
			titleQString.append(QString::number(pageNumber));
			titleQString.append(QString::fromStdString("  TYPE="));
			titleQString.append(QString::fromStdString(PageTypeLookup::lookup(
					pageType)));
			setWindowTitle(titleQString);
		}
	}

}

void ViewFrameDialog::handleRefreshButtonClicked() {
	refreshView();
}
