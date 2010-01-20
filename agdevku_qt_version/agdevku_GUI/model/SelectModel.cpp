/*
 * SelectModel.cpp
 *
 *  Created on: 25-Dec-2009
 *      Author: shailesh
 */

#include "SelectModel.h"
#include <QStringList>
#include <QStandardItem>
#include "../global/ExternDefOfGlobalVariables.h"
SelectModel::SelectModel(Select *select) {
	select_ = select;
	loadData();
}

SelectModel::~SelectModel() {
	// TODO Auto-generated destructor stub
}

void SelectModel::loadData() {

	clear();
	if(select_ == NULL){
		return;
	}
	QStringList headers;
	// TODO Auto-generated constructor stub
	if (select_->allColumns_ == false) {
		for (unsigned i = 0; i < select_->resultSetColumnList_.size(); i++) {
			headers.append(QString::fromStdString(
					select_->resultSetColumnList_[i]));
		}
	} else {
		for (unsigned i = 0; i < select_->schema.numOfColumns; i++) {
			headers.append(
					QString::fromStdString(select_->schema.columnNames[i]));
		}
	}
	setHorizontalHeaderLabels(headers);


	for (unsigned i = 0; i < select_->resultSet.size(); i++) {
		Tuple tuple = select_->resultSet[i];
		QList<QStandardItem*> list;
		if (select_->allColumns_ == false) {
			for (unsigned j = 0; j < select_->resultSetColumnList_.size(); j++) {
				std::string data = tuple.getFieldInStringFormat(
						select_->resultSetColumnList_[j]);
				list.append(new QStandardItem(QString::fromStdString(data)));
			}
		} else {
			for (unsigned j = 0; j < select_->schema.numOfColumns; j++) {
				std::string data = tuple.getFieldInStringFormat(
						select_->schema.columnNames[j]);
				list.append(new QStandardItem(QString::fromStdString(data)));
			}
		}
		insertRow(i, list);

	}
}

void SelectModel::loadFirstPage() {
	select_->loadFirstPage();
	loadData();
}

void SelectModel::loadNextPage() {
	select_->loadNextPage();
	loadData();
}

void SelectModel::loadPrevPage() {
	select_->loadPrevPage();
	loadData();
}

void SelectModel::loadLastPage() {
	select_->loadLastPage();
	loadData();
}

