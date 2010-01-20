#include "viewframebuffertableview.h"

#include "../utils/debug.h"
#include "viewframedialog.h"
#include "../utils/ErrorLookupTable.h"
#include "../bufmgr/BufferManager.h"
ViewFrameBufferTableView::ViewFrameBufferTableView(QWidget *parent) :
	QWidget(parent) {
	//ui.setupUi(this);

	frameBufferModel = new SplitFrameBufferModel();

	navigationHelperLayout = new QHBoxLayout();
	mainLayout = new QVBoxLayout();
	refreshButton = new QPushButton("REFRESH");
	firstButton = new QPushButton("<<FIRST");
	prevButton = new QPushButton("<PREV");
	curPageLabel = new QLabel("Page 1 of");
	nextButton = new QPushButton("NEXT>");
	lastButton = new QPushButton("LAST>>");
	frameBufferTableView = new QTableView();

	frameBufferTableView->setSortingEnabled(true);

	refreshView();

	connect(firstButton, SIGNAL(clicked()), this,
			SLOT(handleFirstButtonClicked()));
	connect(prevButton, SIGNAL(clicked()), this,
			SLOT(handlePrevButtonClicked()));
	connect(nextButton, SIGNAL(clicked()), this,
			SLOT(handleNextButtonClicked()));
	connect(lastButton, SIGNAL(clicked()), this,
			SLOT(handleLastButtonClicked()));
	connect(refreshButton, SIGNAL(clicked()), this,
			SLOT(handleRefreshButtonClicked()));

	navigationHelperLayout->addWidget(firstButton);
	navigationHelperLayout->addWidget(prevButton);
	//navigationHelperLayout->addWidget(curPageLabel);
	navigationHelperLayout->addWidget(nextButton);
	navigationHelperLayout->addWidget(lastButton);
	navigationHelperLayout->addWidget(refreshButton);
	mainLayout->addLayout(navigationHelperLayout);
	mainLayout->addWidget(frameBufferTableView);

	setLayout(mainLayout);
}

ViewFrameBufferTableView::~ViewFrameBufferTableView() {
	//delete frameBufferModel;
	//delete viewFrameButtonGroup;

}

void ViewFrameBufferTableView::refreshView() {
	//frameBufferModel = new FrameBufferModel();
	//setModel(frameBufferModel);
//	mainLayout->removeWidget(frameBufferTableView);
//	delete frameBufferTableView;
//	frameBufferTableView = new QTableView();
//	mainLayout->addWidget(frameBufferTableView);
	frameBufferTableView->reset();
	frameBufferTableView->setModel(frameBufferModel);
	viewFrameButtonGroup = new QButtonGroup(this);
	commitBufferButtonGroup = new QButtonGroup(this);
	viewFrameButtonList.clear();
	commitBufferButtonList.clear();
	DEBUG("frameBufferListLEngth"<<frameBufferModel->frameBufferList.length());
	for (int i = 0; i < frameBufferModel->frameBufferList.length(); i++) {
		QPushButton *pushButton = new QPushButton("VIEW FRAME");
		viewFrameButtonList.append(pushButton);
		viewFrameButtonGroup->addButton(pushButton, i);
		QModelIndex index = frameBufferModel->index(i, 4);
		//setIndexWidget(index,pushButton);
		frameBufferTableView->setIndexWidget(index, viewFrameButtonList.at(i));

		QPushButton *pushButton_1 = new QPushButton("COMMIT BUFFER");
		commitBufferButtonList.append(pushButton_1);
		commitBufferButtonGroup->addButton(pushButton_1, i);
		QModelIndex index_1 = frameBufferModel->index(i, 5);
		frameBufferTableView->setIndexWidget(index_1,
				commitBufferButtonList.at(i));
	}

	for (int i = 0; i < frameBufferModel->columnCount(); i++) {
		frameBufferTableView->setColumnWidth(i, 150);
	}
	connect(viewFrameButtonGroup, SIGNAL(buttonClicked(int)), this,
			SLOT(handleViewFrameClick(int)));
	connect(commitBufferButtonGroup, SIGNAL(buttonClicked(int)), this,
			SLOT(handleCommitBufferClick(int)));

	//frameBufferModel->populateModel();
	//setModel(frameBufferModel);
	//	for (int i = 0; i < frameBufferModel->columnCount(); i++) {
	//		setColumnWidth(i, 150);
	//	}
	//	for (int i = 0; i < frameBufferModel->frameBufferList.length(); i++) {
	//		QModelIndex index = frameBufferModel->index(i, 4);
	//		QPushButton *pushButton = viewFrameButtonList.at(i);
	//		setIndexWidget(index,pushButton);
	//		//setIndexWidget(index, );
	//	}
}

void ViewFrameBufferTableView::handleViewFrameClick(int id) {
	DEBUG("I am here="<<id);
	char *pageData;
	//frameBufferModel->bufferManager->pinAndGetPage(id,pageData);

	BufferManager *bufMgr = BufferManager::getInstance();
	if (bufMgr->isDatabaseOpen_ == false) {
		int error = DATABASE_NOT_OPEN;
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		int pageNumber;
		int error = bufMgr->getPageNumber(id, pageNumber);
		if (error != SUCCESS) {
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			QMessageBox::warning(0, "ERROR", errMsg);
		} else {
			ViewFrameDialog *viewFrameDialog = new ViewFrameDialog(id);
			viewFrameDialog->show();
		}
	}
}

void ViewFrameBufferTableView::handleCommitBufferClick(int id) {
	DEBUG("I am here in commit buffer="<<id);
	BufferManager *bufMgr = BufferManager::getInstance();

	int pageNumber;
	int error = bufMgr->getPageNumber(id, pageNumber);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		error = bufMgr->flushPageToDisk(pageNumber);
		if (error != SUCCESS) {
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			QMessageBox::warning(0, "ERROR", errMsg);
		} else {
			std::string str = "\nYou can use DiskViewer to verify\n";
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			errMsg.append(str.c_str());
			QMessageBox::warning(0, "SUCCESS", errMsg);
		}
	}
}

void ViewFrameBufferTableView::handleFirstButtonClicked() {
	//	firstButton->setEnabled(false);
	//	prevButton->setEnabled(false);
	frameBufferModel->loadFirstPage();
	//frameBufferTableView->reset();
	refreshView();
}

void ViewFrameBufferTableView::handlePrevButtonClicked() {
	frameBufferModel->loadPrevPage();
	refreshView();
}

void ViewFrameBufferTableView::handleNextButtonClicked() {
	frameBufferModel->loadNextPage();
	refreshView();
}

void ViewFrameBufferTableView::handleLastButtonClicked() {
	frameBufferModel->loadLastPage();
	refreshView();
	//	lastButton->setEnabled(false);
	//	nextButton->setEnabled(false);
	//	firstButton->setEnabled(true);
	//	prevButton->setEnabled(true);
}

void ViewFrameBufferTableView::handleRefreshButtonClicked(){
	frameBufferModel->loadData();
	refreshView();
}
