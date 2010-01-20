#ifndef DISKVIEWER_H
#define DISKVIEWER_H

#include <QtGui/QMainWindow>
#include "ui_diskviewer.h"
#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QSplitter>
#include <string>
#include "../diskmgr/SimpleDiskFileAccessor.h"
#include "../bufmgr/BufferManager.h"
class DiskViewer: public QMainWindow {
Q_OBJECT

public:
	DiskViewer(QWidget *parent = 0);
	~DiskViewer();
	void openFileDialog();
	void loadDataToDisplay();
	void setUpView();
	void closeEvent(QCloseEvent *event);

private slots:
	void handleNextPageButton();
	void handlePrevPageButton();
	void handleRefreshPageButton();

private:
	Ui::DiskViewerClass ui;
	QLabel *openFileNameLabel;
	QFileDialog *fileDialog;
	QString fileName;
	QLabel *pathLabel;
	QHBoxLayout *buttonLayout;
	QPushButton *nextPageButton;
	QPushButton *prevPageButton;
	QPushButton *refreshPageButton;
	SimpleDiskFileAccessor *simpleDiskFileAccessor;
	QVBoxLayout *mainLayout;
	QSplitter *splitter;
	QTextEdit *formattedViewer;
	QTextEdit *hexViewer;
	std::string hexData;
	std::string intelliData;
	BufferManager *bufMgr;
	int pageNumber;
	int pageSize;
	char *pageData;
};

#endif // DISKVIEWER_H
