#include "buffermenutabwidget.h"
#include "../utils/debug.h"
#include "../bufmgr/BufferManager.h"
#include "../utils/ErrorLookupTable.h"
#include <QMessageBox>
#include <string.h>
#include "../global/GlobalStructures.h"

BufferMenuTabWidget::BufferMenuTabWidget(QWidget *parent) :
	QTabWidget(parent) {
	ui.setupUi(this);

	gridLayout = new QGridLayout();
	viewFrameBufferTableView = new ViewFrameBufferTableView();

	gridLayout->addWidget(viewFrameBufferTableView);
	ui.viewFrameBufferTab->setLayout(gridLayout);
	connect(this, SIGNAL(currentChanged(int)), this,
			SLOT(currentTabChanged(int)));
	connect(ui.readPagePushButton, SIGNAL(clicked()), this,
			SLOT(handleReadPagePushButton()));
	connect(ui.writePagePushButton, SIGNAL(clicked()), this,
			SLOT(handleWritePagePushButton()));
	bufMgr = BufferManager::getInstance();


	hitRateTabLayout = new QVBoxLayout();
	hitRateView = new HitRateView();
	hitRateTabLayout->addWidget(hitRateView);
	ui.hitRateTab->setLayout(hitRateTabLayout);
}

BufferMenuTabWidget::~BufferMenuTabWidget() {
	DEBUG("destructing")
//	delete gridLayout;
//	delete viewFrameBufferTableView;
}

void BufferMenuTabWidget::currentTabChanged(int tabNumber) {
	DEBUG("tabNumber"<<tabNumber);

	if (tabNumber == 0) {
		viewFrameBufferTableView->refreshView();
		ui.viewFrameBufferTab->repaint();
	}else if (tabNumber == 3) {
		hitRateView->refreshView();
		ui.hitRateTab->repaint();
	}
}

void BufferMenuTabWidget::handleReadPagePushButton() {
	DEBUG("hello")
	int pageNumber = ui.readPageNumberLineEdit->text().toInt();

	char *pageData;
	int error = bufMgr->pinAndGetPage(pageNumber, pageData);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "INFO", errMsg);
	}
	bufMgr->unPinPage(pageNumber, false);

}

void BufferMenuTabWidget::handleWritePagePushButton() {
	DEBUG("handleWritePagePushButton")
	int pageNumber = ui.writePageNumberLineEdit->text().toInt();
	const char *toBeWrittenData =
			ui.someDataToBeWritten->toPlainText().toStdString().c_str();
	DEBUG("data to be written="<<toBeWrittenData)
	int len = strlen(toBeWrittenData);
	char *pageData;
	int error = bufMgr->pinAndGetPage(pageNumber,pageData);
	int offset = sizeof(GeneralPageHeaderStruct);
	//int offset = 100;
	memcpy(&pageData[offset],toBeWrittenData,len);

	bufMgr->unPinPage(pageNumber,true);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "INFO", errMsg);
	}
}


void BufferMenuTabWidget::refreshTabs(){
	DEBUG("refreshing buffer tabs");
	viewFrameBufferTableView->refreshView();
	repaint();
}

