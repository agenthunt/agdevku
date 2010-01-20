/*
 * QueryResultViewHandler.cpp
 *
 *  Created on: 02-Dec-2009
 *      Author: shailesh
 */

#include "QueryResultViewHandler.h"
#include <QWidget>
#include "../parser/ParseUtil.h"
#include "../utils/ErrorLookupTable.h"
#include "../utils/debug.h"
#include <QLineEdit>
#include "../global/ExternDefOfGlobalVariables.h"
#include "../executor/ExecuteManager.h"
#include "../view/viewframebuffertableview.h"
#include <QLabel>
#include <QVBoxLayout>
#include "../model/ShowDatabasesModel.h"
#include "../view/mainwindow.h"
#include "../view/selectview.h"
#include <QListWidget>
extern MainWindow *mainWindow;
#include <QTableView>
QueryResultViewHandler::QueryResultViewHandler() {
	// TODO Auto-generated constructor stub

}

QueryResultViewHandler::~QueryResultViewHandler() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE QueryResultViewHandler::handleQuery(QString& queryString, QQueue<
		QWidget*> *widgetQueue) {

	ParseUtil parseUtil;
	int error = parseUtil.parseQuery(queryString);

	DEBUG("hanleQuery,result="<<error)
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		widgetQueue->enqueue(new QLabel(errMsg));
		//		ViewFrameBufferTableView *dummy = new ViewFrameBufferTableView();
		//		widgetQueue->append(dummy);
		buildResultView();
		widgetQueue->append(resultWidget);
		return error;
	}
	ExecuteManager executeManager;
	error = executeManager.execute(executeStructures);
	if (error != SUCCESS) {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		widgetQueue->enqueue(new QLabel(errMsg));
	} else {
		QString errMsg(ErrorLookupTable::lookup(error).c_str());
		widgetQueue->enqueue(new QLabel(errMsg));
		buildResultView();
		widgetQueue->append(resultWidget);
	}

	return error;
}

STATUS_CODE QueryResultViewHandler::buildResultView() {
	DEBUG("building result view");
	ShowDatabasesModel *model;
	QString databaseName;
	QStandardItemModel *resultModel;
	switch (executeStructures->executeStatement) {
	case ExecuteStructures::SHOW_DATABASES_STATEMENT:
		//		resultWidget = new QTableView();
		//		model = new ShowDatabasesModel();
		//		((QTableView*) resultWidget)->setModel(model);
		resultWidget = new QTableView();
		resultModel = new QStandardItemModel();
		resultModel->setHorizontalHeaderLabels(QStringList("DATABASES"));
		for (unsigned i = 0; i < executeStructures->listOfDatabases.size(); i++) {
			QStandardItem *item = new QStandardItem(QString(
					executeStructures->listOfDatabases.at(i).c_str()));
			resultModel->appendRow(item);
		}
		((QTableView*) resultWidget)->setModel(resultModel);
		break;
	case ExecuteStructures::OPEN_DATABASE_STATEMENT:
		mainWindow->setTitle(executeStructures->databaseName);
		resultWidget = new QLabel("DATABASE OPENED");
		break;
	case ExecuteStructures::SHOW_TABLES_STATEMENT:
		resultWidget = new QTableView();
		resultModel = new QStandardItemModel();
		resultModel->setHorizontalHeaderLabels(QStringList("TABLES"));
		for (unsigned i = 0; i < executeStructures->tableList.size(); i++) {
			QStandardItem *item = new QStandardItem(QString(
					executeStructures->tableList.at(i).c_str()));
			resultModel->appendRow(item);
		}
		((QTableView*) resultWidget)->setModel(resultModel);
		break;
	case ExecuteStructures::DESCRIBE_STATEMENT:
		resultWidget = new QTextEdit();
		((QTextEdit*) resultWidget)->setText(QString::fromStdString(
				executeStructures->schemaDescription));
		((QTextEdit*) resultWidget)->setReadOnly(true);
		break;
	case ExecuteStructures::SELECT_STATEMENT:
		DEBUG("building select view");
		resultWidget = new SelectView(executeStructures->select);
		break;
	case ExecuteStructures::DELETE_STATEMENT:
		DEBUG("building delete view")
		;
		resultWidget = new QLabel(executeStructures->errorMessage);
		break;
	case ExecuteStructures::DROP_TABLE_STATEMENT:
		DEBUG("drop table view")
		;
		resultWidget = new QLabel(executeStructures->errorMessage);
		break;
	case ExecuteStructures::INSERT_STATEMENT: {
		DEBUG("build insert view");

		QString msg(numberOfRowsReturned);
		msg.append("\n");
		msg.append(QString::fromAscii(executionTime));
		resultWidget = new QLabel(msg);
//		resultWidget = new QListWidget();
//		((QListWidget*) resultWidget)->addItem(QString::fromAscii(
//				numberOfRowsReturned));
//		((QListWidget*) resultWidget)->addItem(
//				QString::fromAscii(executionTime));
		//resultWidget = new QLabel(numberOfRowsReturned);

		break;
	}
	default:
		resultWidget = new QLabel(executeStructures->errorMessage);
		break;
	}

	return SUCCESS;
}
