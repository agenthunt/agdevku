#include "mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include "../utils/debug.h"
#include "../utils/ErrorLookupTable.h"
#include <string.h>
#include "../global/ExternDefOfGlobalVariables.h"
#include "../catalog/CatalogUtil.h"
#include "../tests/TestLinkedListFreePageManager.cpp"
#include <QInputDialog>
#include <QDir>
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent) {
	ui.setupUi(this);
	mainMenuTablWidget = new MainMenuTabWidget();
	gridLayout = new QGridLayout();
	gridLayout->addWidget(mainMenuTablWidget);
	ui.centralwidget->setLayout(gridLayout);
	createDatabaseDialog = new CreateDatabaseDialog();
	openDatabaseDialog = new OpenDatabaseDialog();
	initializeBufferDialog = new InitializeBufferDialog();
	diskViewer = new DiskViewer();
	connect(ui.actionCREATE_DATABASE, SIGNAL(activated()), this,
			SLOT(createDatabaseAction()));
	connect(ui.actionOPEN_DATABASE, SIGNAL(activated()), this,
			SLOT(openDatabaseAction()));
	connect(ui.actionInitialize_Buffer, SIGNAL(activated()), this,
			SLOT(initializeBufferAction()));
	connect(ui.actionOpen_Disk_File, SIGNAL(activated()), this,
			SLOT(openDiskViewerAction()));
	connect(ui.actionAbout_agdevku, SIGNAL(activated()), this,
			SLOT(aboutMenuAction()));
	connect(ui.actionClose_Database, SIGNAL(activated()), this,
			SLOT(closeDatabaseAction()));
	connect(ui.actionCommon_Debug_On, SIGNAL(triggered(bool)), this,
			SLOT(toggleCommonDebugAction(bool)));
	connect(ui.actionBPlustree_Debug_On, SIGNAL(triggered(bool)), this,
			SLOT(toggleBPlustreeDebugAction(bool)));
	connect(ui.actionHeap_Verbose_On, SIGNAL(triggered(bool)), this,
			SLOT(toggleHeapVerboseAction(bool)));
	connect(ui.actionIndex_On, SIGNAL(triggered(bool)), this,
			SLOT(toggleUseIndexAction(bool)));
	connect(ui.actionRun_Tests, SIGNAL(activated()), this,
			SLOT(runTestsAction()));
	connect(ui.actionHeap_Viewer, SIGNAL(activated()), this,
			SLOT(openHeapViewerAction()));
	connect(ui.actionUse_Client_Side_Sort, SIGNAL(triggered(bool)), this,
			SLOT(toggleClientSideSortAction(bool)));
	connect(ui.actionUse_MMAP, SIGNAL(triggered(bool)), this,
				SLOT(toggleUseMMAP(bool)));
	std::string str = "NO";
	setTitle(str);
	//ui.actionCommon_Debug_On->setChecked(true);
	//COMMON_DEBUG = true;
}

MainWindow::~MainWindow() {
	//	delete gridLayout;
	//	delete mainMenuTablWidget;
	//	delete createDatabaseDialog;
}

void MainWindow::createDatabaseAction() {
	BufferManager *bufMgr = BufferManager::getInstance();
	if (bufMgr->isDatabaseOpen_ == true) {
		int error = CLOSE_OPENED_DATABASE;
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	} else {
		createDatabaseDialog->show();
	}
	DEBUG("inside createDatabaseAction");
}

void MainWindow::openDatabaseAction() {
	openDatabaseDialog->setUpView();
	openDatabaseDialog->show();
	DEBUG("inside openDatabaseAction")
}

void MainWindow::initializeBufferAction() {
	initializeBufferDialog->show();
	mainMenuTablWidget->bufferMenuTabWidget->refreshTabs();
}

void MainWindow::openDiskViewerAction() {
	diskViewer->openFileDialog();

}

void MainWindow::aboutMenuAction() {
	QString aboutMessage = tr("AgDevKu\nCreated By \n"
		"Manish (Ag)rawal, Ravindra (Dev)agiri and Shailesh (Ku)mar\n"
		"IIIT,Bangalore\n"
		"version 1.0\n"
		"CopyRight(C) 2010");
	QMessageBox::about(this, "About Agdevku", aboutMessage);
}

void MainWindow::closeDatabaseAction() {
	BufferManager *bufMgr = BufferManager::getInstance();
	if (bufMgr->isDatabaseOpen_ == false) {
		int error = DATABASE_NOT_OPEN;
		if (error != SUCCESS) {
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			QMessageBox::warning(0, "ERROR", errMsg);
		}
	} else {
		bufMgr->closeDatabase();
		std::string str = "NO";
		setTitle(str);
	}
}

void MainWindow::toggleCommonDebugAction(bool enabled) {

	if (enabled == true) {
		COMMON_DEBUG = true;
	} else {
		COMMON_DEBUG = false;
	}
	//	if (ui.actionCommon_Debug_On->isChecked() == true) {
	//		COMMON_DEBUG = false;
	//		ui.actionCommon_Debug_On->setChecked(false);
	//	} else if (ui.actionCommon_Debug_On->isChecked() == false) {
	//		COMMON_DEBUG = true;
	//		ui.actionCommon_Debug_On->setChecked(true);
	//	}
	//HeapViewer *heapViewer = new HeapViewer();
	//heapViewer->show();
}

void MainWindow::toggleHeapVerboseAction(bool enabled) {
	if (enabled == true) {
		HEAP_VERBOSE = true;
	} else {
		HEAP_VERBOSE = false;
	}
	//	if (ui.actionHeap_Verbose_On->isChecked() == true) {
	//		HEAP_VERBOSE = false;
	//		ui.actionHeap_Verbose_On->setChecked(false);
	//	} else if (ui.actionHeap_Verbose_On->isChecked() == false) {
	//		HEAP_VERBOSE = true;
	//		ui.actionHeap_Verbose_On->setChecked(true);
	//	}

}

void MainWindow::toggleUseIndexAction(bool enabled) {
	if (enabled == true) {
		USING_INDEX = true;
	} else {
		USING_INDEX = false;
	}
	//	if (ui.actionIndex_On->isChecked() == true) {
	//		USING_INDEX = false;
	//		ui.actionIndex_On->setChecked(false);
	//	} else if (ui.actionIndex_On->isChecked() == false) {
	//		USING_INDEX = true;
	//		ui.actionIndex_On->setChecked(true);
	//	}

}

void MainWindow::runTestsAction() {
	TestLinkedListFreePageManager t;
	t.basicTest();
}

void MainWindow::toggleBPlustreeDebugAction(bool enabled) {
	if (enabled == true) {
		BPLUSTREE_DEBUG = true;
	} else {
		BPLUSTREE_DEBUG = false;
	}
}

void MainWindow::openHeapViewerAction() {
	BufferManager *bufMgr = BufferManager::getInstance();
	if (bufMgr->isDatabaseOpen_ == false) {
		int error = DATABASE_NOT_OPEN;
		if (error != SUCCESS) {
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			QMessageBox::warning(0, "ERROR", errMsg);
		}
		return;
	}

	bool ok;
	QString tableName = QInputDialog::getText(this, tr("HeapViewer)"), tr(
			"Table Name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
	CatalogUtil catalogUtil;
	if (ok && !tableName.isEmpty()) {
		Schema schema;
		int error = catalogUtil.getSchema(tableName.toUpper().toStdString().c_str(),
				schema);
		if (error != TABLE_EXISTS) {
			QString errMsg(ErrorLookupTable::lookup(error).c_str());
			QMessageBox::warning(0, "ERROR", errMsg);
			return;
		}
		heapViewer = new HeapViewer(tableName);
		heapViewer->show();
	}
}

void MainWindow::toggleClientSideSortAction(bool enabled){
	if (enabled == true) {
		USE_CLIENT_SIDE_SORT = true;
	} else {
		USE_CLIENT_SIDE_SORT = false;
	}
}


void MainWindow::toggleUseMMAP(bool enabled){
	if (enabled == true) {
		USE_MMAP = true;
	} else {
		USE_MMAP = false;
	}
}

void QWidget::closeEvent(QCloseEvent *event) {
	//do cleanup
	DEBUG("closing main window")
	BufferManager *bufMgr = BufferManager::getInstance();
	//if open only, check
	bufMgr->closeDatabase();
	event->accept();
}

void MainWindow::setTitle(QString databaseName) {
	DEBUG("setting title"<<databaseName.toStdString());
	const char *cStr = databaseName.toStdString().c_str();
	char *windowTitle = "  DATABASE OPENED";
	memset(currentlyOpenedDatabase, '\0', 256);
	strcpy(currentlyOpenedDatabase, cStr);
	strcat(currentlyOpenedDatabase, windowTitle);
	setWindowTitle(QString::fromAscii(currentlyOpenedDatabase, strlen(
			currentlyOpenedDatabase)));
}

void MainWindow::setTitle(std::string databaseName) {
	QString qStr(databaseName.c_str());
	setTitle(qStr);
}
