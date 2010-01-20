#include "selectview.h"
#include "../global/ExternDefOfGlobalVariables.h"
SelectView::SelectView(Select *select, QWidget *parent) :
	QWidget(parent) {
	ui.setupUi(this);
	select_ = select;
	navigationHelperLayout = new QHBoxLayout();
	mainLayout = new QVBoxLayout();
	firstButton = new QPushButton("<<FIRST");
	prevButton = new QPushButton("<PREV");
	curPageLabel = new QLabel("Page 1 of");
	nextButton = new QPushButton("NEXT>");
	lastButton = new QPushButton("LAST>>");

	msgLayout = new QHBoxLayout();
	executionTimeMsg = new QLabel("");
	numberOfRowsReturnedMsg = new QLabel("");
	msgLayout->addWidget(numberOfRowsReturnedMsg);
	msgLayout->addWidget(executionTimeMsg);

	connect(firstButton, SIGNAL(clicked()), this,
			SLOT(handleFirstButtonClicked()));
	connect(prevButton, SIGNAL(clicked()), this,
			SLOT(handlePrevButtonClicked()));
	connect(nextButton, SIGNAL(clicked()), this,
			SLOT(handleNextButtonClicked()));
	connect(lastButton, SIGNAL(clicked()), this,
			SLOT(handleLastButtonClicked()));

	navigationHelperLayout->addWidget(firstButton);
	navigationHelperLayout->addWidget(prevButton);
	//navigationHelperLayout->addWidget(curPageLabel);
	navigationHelperLayout->addWidget(nextButton);
	navigationHelperLayout->addWidget(lastButton);

	selectModel = new SelectModel(select);
	resultSetTableView = new QTableView();
	resultSetTableView->setModel(selectModel);
	//resultSetTableView->setSortingEnabled(true);
	mainLayout->addLayout(msgLayout);
	mainLayout->addLayout(navigationHelperLayout);
	mainLayout->addWidget(resultSetTableView);

	setLayout(mainLayout);
	refreshView();
}

SelectView::~SelectView() {
	delete select_;
}

void SelectView::refreshView() {

	resultSetTableView->reset();
	for (int i = 0; i < selectModel->columnCount(); i++) {
		resultSetTableView->setColumnWidth(i, 150);
	}
	resultSetTableView->setModel(selectModel);
	executionTimeMsg->setText(QString::fromAscii(executionTime));
	numberOfRowsReturnedMsg->setText(
			QString::fromAscii(numberOfRowsReturned));
}

void SelectView::handleFirstButtonClicked() {
	selectModel->loadFirstPage();
	refreshView();
}

void SelectView::handlePrevButtonClicked() {
	selectModel->loadPrevPage();
	refreshView();
}

void SelectView::handleNextButtonClicked() {
	selectModel->loadNextPage();
	refreshView();
}

void SelectView::handleLastButtonClicked() {
	selectModel->loadLastPage();
	refreshView();
}
