#include "heapviewer.h"

#include <QTreeView>
#include <QTreeWidget>
#include <QListWidget>

#include "../model/SplitFrameBufferModel.h"
#include "../heap/DirectoryHeaderPage.h"
#include "../heap/DirectoryPage.h"
#include "../heap/DirectoryEntry.h"
HeapViewer::HeapViewer(QString tableName, QWidget *parent) :
	QDialog(parent) {
	ui.setupUi(this);
	bufMgr = BufferManager::getInstance();
	tableName_ = tableName;
	mainLayout = new QVBoxLayout();
	myTreeWidget = new MyTreeWidget();
	mainLayout->addWidget(myTreeWidget);
	initHeapViewer();
	setLayout(mainLayout);
}

HeapViewer::~HeapViewer() {

}

void HeapViewer::initHeapViewer() {
	heapFile = new HeapFile(tableName_.toUpper().toStdString().c_str());
	int directoryHeaderPageNum = heapFile->getDirectoryHeaderPageNumber();
	DirectoryHeaderPage dirHeaderPage(directoryHeaderPageNum);
	QTreeWidgetItem *rootItem = new QTreeWidgetItem();
	QIcon icon("/home/shailesh/test.bmp");
	//rootItem->setText(0, QString::number(directoryHeaderPageNum));
	rootItem->setIcon(0,icon);
	int firstDirPageNum = dirHeaderPage.getFirstDirectoryPageNumber();
	for (int i = firstDirPageNum; i != -1;) {
		DirectoryPage dirPage(i);
		QTreeWidgetItem *dirPageItem = new QTreeWidgetItem();
		dirPageItem->setText(0, QString::number(i));
		rootItem->addChild(dirPageItem);
		int numOfDEs = dirPage.getNumberOfDirectoryEntries();
		for (int j = 0; j < numOfDEs; j++) {
			DirectoryEntry dirEntry;
			dirPage.getDirectoryEntry(j, dirEntry);
			int dirEntryPageNumber = dirEntry.getPageNumber();
			QTreeWidgetItem *dirEntryItem = new QTreeWidgetItem();
			dirEntryItem->setText(0, QString::number(dirEntryPageNumber));
			dirPageItem->addChild(dirEntryItem);
		}
		i = dirPage.getNextDirectoryPageNumber();
	}
	myTreeWidget->addTopLevelItem(rootItem);
}
