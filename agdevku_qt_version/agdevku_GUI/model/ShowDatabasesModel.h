/*
 * ShowDatabasesModel.h
 *
 *  Created on: 02-Dec-2009
 *      Author: shailesh
 */

#ifndef SHOWDATABASESMODEL_H_
#define SHOWDATABASESMODEL_H_
#include <QStandardItemModel>
#include "../bufmgr/BufferManager.h"
class ShowDatabasesModel: public QStandardItemModel  {
public:
	ShowDatabasesModel();
	virtual ~ShowDatabasesModel();
	void initModel();
	BufferManager *bufferManager;
	QList<QList<QStandardItem*> > databasesItemList;
};

#endif /* SHOWDATABASESMODEL_H_ */
