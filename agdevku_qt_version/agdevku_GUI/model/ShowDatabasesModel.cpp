/*
 * ShowDatabasesModel.cpp
 *
 *  Created on: 02-Dec-2009
 *      Author: shailesh
 */

#include "ShowDatabasesModel.h"
#include <QStringList>
#include "../utils/debug.h"
#include <vector>
#include <string>

ShowDatabasesModel::ShowDatabasesModel() {
	// TODO Auto-generated constructor stub
	bufferManager = BufferManager::getInstance();
	QStringList headers;
	headers << "NUMBER" << "DATABASE NAME" << "ACTION";
	setHorizontalHeaderLabels(headers);
	initModel();
}

ShowDatabasesModel::~ShowDatabasesModel() {
	// TODO Auto-generated destructor stub
}

void ShowDatabasesModel::initModel() {
	std::vector<std::string> dbList = bufferManager->getListOfDatabases();
	for (unsigned i = 0; i < dbList.size(); i++) {
		QList<QStandardItem*> list;
		list.append(new QStandardItem(QString::number(i)));
		QStandardItem *item = new QStandardItem();
		const char *str = dbList.at(i).c_str();
		item->setText(QString::fromAscii(str, strlen(str)));
		list.append(item);
		list.append(new QStandardItem("dummy"));
		databasesItemList.append(list);
		insertRow(i, list);
	}

}



