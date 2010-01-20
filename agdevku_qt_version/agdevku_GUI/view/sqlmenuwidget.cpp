#include "sqlmenuwidget.h"
#include "../parser/ParseUtil.h"
#include "../global/GlobalStructures.h"

#include "../utils/ErrorLookupTable.h"
#include "../utils/debug.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>
#include <QWidget>
#include <QLineEdit>
#include <QTextStream>
#include "../global/ExternDefOfGlobalVariables.h"
#include "../bufmgr/BufferManager.h"
#include "../utils/timer_util.h"
SQLMenuWidget::SQLMenuWidget(QWidget *parent) :
	QWidget(parent) {
	ui.setupUi(this);

	queryLabel = new QLabel(tr("Query:"));

	queryEdit = new QTextEdit();
	queryEdit->setMaximumHeight(100);

	enterButton = new QPushButton("ENTER");
	clearButton = new QPushButton("CLEAR");
	importButton = new QPushButton("IMPORT");

	queryLayout = new QHBoxLayout();

	queryLayout->addWidget(queryLabel);
	queryLayout->addWidget(queryEdit);
	buttonLayout = new QVBoxLayout();
	buttonLayout->addWidget(enterButton);
	buttonLayout->addWidget(clearButton);
	buttonLayout->addWidget(importButton);
	queryLayout->addLayout(buttonLayout);

	mainLayout = new QVBoxLayout();
	mainLayout->addLayout(queryLayout);

	setLayout(mainLayout);
	queryResultViewHanlder = new QueryResultViewHandler();
	connect(enterButton, SIGNAL(clicked()), this, SLOT(handleQueryEnter()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(handleClearButton()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(handleImportButton()));
	resultLayout = new QVBoxLayout();
	//queryLayout->setAlignment(Qt::AlignTop);
	//buttonLayout->setAlignment(Qt::AlignTop);
	//resultLayout->setAlignment(Qt::AlignTop);
	mainLayout->setAlignment(Qt::AlignTop);

	mainLayout->addLayout(resultLayout);

	dummy = new QLabel("HIH");
	//stackedLayout = new QStackedLayout();
	//stackedLayout->addWidget(dummy);
	//mainLayout->addLayout(stackedLayout);
	//stackedLayout->setStackingMode(QStackedLayout::StackAll);
	widgetList = new QWidgetList();
	widgetQueue = new QQueue<QWidget*> ();

}

SQLMenuWidget::~SQLMenuWidget() {

}

STATUS_CODE SQLMenuWidget::handleQueryEnter() {
	DEBUG("query text="<<queryEdit->toPlainText().toStdString())

	while (widgetQueue->empty() == false) {
		QWidget *widget = widgetQueue->dequeue();
		resultLayout->removeWidget(widget);
		delete widget;
	}
	resultLayout->invalidate();
	QString queryString = queryEdit->toPlainText();
	int error = queryResultViewHanlder->handleQuery(queryString, widgetQueue);
	for (int i = 0; i < widgetQueue->count(); i++) {
		QWidget *widget = widgetQueue->at(i);
		resultLayout->addWidget(widget);
	}
	resultLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	//mainLayout->setAlignment(Qt::AlignTop);
	queryEdit->clear();
	return error;
}

void SQLMenuWidget::handleClearButton() {
	queryEdit->clear();
}
//void SQLMenuWidget::keyPressEvent(QKeyEvent *keyEvent) {
//	DEBUG(keyEvent->key() <<"val="<<Qt::Key_Enter)
//	if (keyEvent->key() == Qt::Key_Enter) {
//		DEBUG("keyPressed")
//		handleQueryEnter();
//	}
//
//	return QWidget::keyPressEvent(keyEvent);
//}

void SQLMenuWidget::keyReleaseEvent(QKeyEvent *keyEvent) {
	//DEBUG(keyEvent->key() <<"val="<<Qt::Key_Return)
	if (keyEvent->key() == Qt::Key_Return) {
		DEBUG("keyPressed")
		handleQueryEnter();
	}

	return QWidget::keyReleaseEvent(keyEvent);
}

void SQLMenuWidget::handleImportButton() {
	QFileDialog::Options options = QFileDialog::DontUseNativeDialog;
	QString selectedFilter;
	QString openFilesPath = tr("/home/shailesh/workspace/agdevku_GUI/sql");

	BufferManager *bufMgr = BufferManager::getInstance();
	//	if (bufMgr->isDatabaseOpen_ == false) {
	//		int error = DATABASE_NOT_OPEN;
	//		QString errMsg(ErrorLookupTable::lookup(error).c_str());
	//		QMessageBox::warning(0, "ERROR", errMsg);
	//		return;
	//	}

	QString fileName = QFileDialog::getOpenFileName(this, tr(
			"Select a sql file"), openFilesPath, tr(
			"All Files (*);;DB Files (*.sql)"), &selectedFilter, options);

	if (!fileName.isEmpty()) {
		executeSQLFile(fileName);
	} else {
		QString errMsg = tr("FILE NAME EMPTY");
		QMessageBox::warning(0, "ERROR", errMsg);
		return;
	}

}

void SQLMenuWidget::executeSQLFile(QString fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	QList<QString> queryList;
	QString line;

	while (!in.atEnd()) {
		line = line.append(in.readLine());
		if (line.contains(";") == true) {
			line.append("\n");
			queryList.push_back(line);
			line.clear();
		}
	}

	TimerUtil timerUtil;
	timerUtil.startTimer();
	for (int i = 0; i < queryList.size(); i++) {
		queryEdit->setPlainText(queryList[i]);
		if (SUCCESS != handleQueryEnter()) {
			break;
		}
	}
	timerUtil.endTimer();
	QString msg(" Time to execute ");
	msg.append(QString::number(queryList.size()));
	msg.append(" queries is ");
	msg.append(QString::fromStdString(timerUtil.getElapsedTimeInSeconds()));
	msg.append(" seconds");
	QString errMsg = tr("EXECUTION TIME\n");
	errMsg.append(msg);
	QMessageBox::warning(0, "INFO", errMsg);
}
