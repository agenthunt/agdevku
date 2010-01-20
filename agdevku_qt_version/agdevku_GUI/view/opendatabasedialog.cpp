#include "opendatabasedialog.h"
#include <QStandardItemModel>
#include "../bufmgr/BufferManager.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include <vector>
#include <string>
#include <QMessageBox>
#include "../utils/ErrorLookupTable.h"
#include "../utils/debug.h"
#include "../view/mainwindow.h"
extern MainWindow *mainWindow;
OpenDatabaseDialog::OpenDatabaseDialog(QWidget *parent) :
	QDialog(parent) {
	ui.setupUi(this);
	setModal(true);
	showDatabasesView = new QTableView();
	mainLayout = new QGridLayout();
	setUpView();
	mainLayout->addWidget(showDatabasesView);
	setLayout(mainLayout);
	bufMgr = BufferManager::getInstance();

}

OpenDatabaseDialog::~OpenDatabaseDialog() {

}

void OpenDatabaseDialog::setUpView() {
	model = new ShowDatabasesModel();
	showDatabasesView->setModel(model); //make sure this line is always before setting Index Widget else button will not appear
	databasesButtonList.clear();
	databasesButtonGroup = new QButtonGroup(this);
	for (int i = 0; i < model->databasesItemList.length(); i++) {
		QPushButton *pushButton = new QPushButton("OPEN");
		databasesButtonList.append(pushButton);
		databasesButtonGroup->addButton(pushButton, i);
		QModelIndex index = model->index(i, 2);

		//showDatabasesView->setIndexWidget(index, databasesButtonList.at(i));
		DEBUG("setting indexWidget")
		showDatabasesView->setIndexWidget(index, pushButton);
	}

	for (int i = 0; i < model->columnCount(); i++) {
		showDatabasesView->setColumnWidth(i, 120);
	}

	connect(databasesButtonGroup, SIGNAL(buttonClicked(int)), this,
			SLOT(handleOpenButtonClick(int)));
}

void OpenDatabaseDialog::handleOpenButtonClick(int id) {
	QStandardItem *item = model->item(id,1);

	const char *databaseName = item->text().trimmed().toStdString().c_str();
	DEBUG("opening database="<<databaseName);
	int error = bufMgr->openDatabase(databaseName);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "INFO", errMsg);
		mainWindow->setTitle(item->text());
	}
	close();
}
