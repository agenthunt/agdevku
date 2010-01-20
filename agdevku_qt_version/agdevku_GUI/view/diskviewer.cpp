#include "diskviewer.h"
#include "../utils/HexFormatter.h"
#include <QMessageBox>
#include <QStringList>
#include "../utils/ErrorLookupTable.h"
#include "../utils/IntelliFormatter.h"
#include "../utils/PageTypeLookup.h"
#include "../utils/debug.h"
DiskViewer::DiskViewer(QWidget *parent) :
	QMainWindow(parent) {
	ui.setupUi(this);
	setWindowTitle("DISK VIEWER");
	openFileNameLabel = new QLabel();
	fileDialog = new QFileDialog();
	buttonLayout = new QHBoxLayout();
	prevPageButton = new QPushButton("PREV PAGE");
	nextPageButton = new QPushButton("NEXT PAGE");
	refreshPageButton = new QPushButton("REFRESH");
	pathLabel = new QLabel("NO PATH");
	buttonLayout->addWidget(pathLabel);
	buttonLayout->addWidget(refreshPageButton);
	buttonLayout->addWidget(prevPageButton);
	buttonLayout->addWidget(nextPageButton);

	simpleDiskFileAccessor = new SimpleDiskFileAccessor();

	mainLayout = new QVBoxLayout();
	mainLayout->addLayout(buttonLayout);

	splitter = new QSplitter();
	formattedViewer = new QTextEdit();
	formattedViewer->setReadOnly(true);
	hexViewer = new QTextEdit();
	splitter->addWidget(formattedViewer);
	splitter->addWidget(hexViewer);
	hexViewer->setReadOnly(true);
	mainLayout->addWidget(splitter);
	ui.centralwidget->setLayout(mainLayout);

	connect(refreshPageButton, SIGNAL(clicked()), this,
			SLOT(handleRefreshPageButton()));
	connect(prevPageButton, SIGNAL(clicked()), this,
			SLOT(handlePrevPageButton()));
	connect(nextPageButton, SIGNAL(clicked()), this,
			SLOT(handleNextPageButton()));

	bufMgr = BufferManager::getInstance();
	pageSize = 8192;
	pageData = new char[pageSize];
}

DiskViewer::~DiskViewer() {
	delete[] pageData;
}

void DiskViewer::openFileDialog() {
	QFileDialog::Options options = QFileDialog::DontUseNativeDialog;
	QString selectedFilter;
	QString openFilesPath =
			tr("/home/shailesh/workspace/agdevku_GUI/databases");

	fileName = fileDialog->getOpenFileName(this, tr("Select a database file"),
			openFilesPath, tr("All Files (*);;DB Files (*.db)"),
			&selectedFilter, options);

	//the formatter needs database to be open, so the check here
	if (!fileName.isEmpty() && bufMgr->isDatabaseOpen_ == true) {
		openFileNameLabel->setText(fileName);
		setUpView();
		show();
	} else {
		int error = DATABASE_NOT_OPEN;
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
	}
	DEBUG("selected file ="<<openFileNameLabel->text().toStdString());

}

void DiskViewer::setUpView() {
	pathLabel->setText(fileName);
	int error = simpleDiskFileAccessor->openDiskFile(
			fileName.toStdString().c_str());
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
		close();
	}
	pageNumber = 0;
	loadDataToDisplay();
}

void DiskViewer::loadDataToDisplay() {
	int error = simpleDiskFileAccessor->readPage(pageNumber, pageSize, pageData);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		QMessageBox::warning(0, "ERROR", errMsg);
		close();
	}
	QString *hexQString;
	char *formattedString;
	hexData = HexFormatter::format(pageData, pageSize, formattedString);
	hexQString = new QString(hexData.c_str());
	hexViewer->setText(*hexQString);

	intelliData = IntelliFormatter::formatUsingData(pageData);
	QString *intelliString = new QString(intelliData.c_str());
	formattedViewer->setText(*intelliString);

	GeneralPageHeaderAccessor genPageAccessor(pageData);
	int pageType = genPageAccessor.getPageType();

	QString titleQString;

	titleQString.append(QString::fromStdString("DISK VIEWER PAGE NUMBER="));
	titleQString.append(QString::number(pageNumber));
	titleQString.append(QString::fromStdString("  TYPE="));
	titleQString.append(
			QString::fromStdString(PageTypeLookup::lookup(pageType)));
	setWindowTitle(titleQString);
}

void DiskViewer::closeEvent(QCloseEvent *event) {
	DEBUG("disk Viewer close Event");
	simpleDiskFileAccessor->closeDiskFile();

}

void DiskViewer::handleNextPageButton() {

	pageNumber++;
	loadDataToDisplay();
}

void DiskViewer::handlePrevPageButton() {
	if (pageNumber > 0) {
		pageNumber--;
		loadDataToDisplay();
	}
}

void DiskViewer::handleRefreshPageButton() {
	loadDataToDisplay();
}
