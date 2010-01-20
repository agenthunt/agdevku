/*
 * SelectModel.h
 *
 *  Created on: 25-Dec-2009
 *      Author: shailesh
 */


#ifndef SELECTMODEL_H_
#define SELECTMODEL_H_
#include <QStandardItemModel>
#include "../executor/Select.h"
class SelectModel: public QStandardItemModel {
public:
	SelectModel(Select *select);
	virtual ~SelectModel();
	void loadFirstPage();
	void loadNextPage();
	void loadPrevPage();
	void loadLastPage();
	void loadData();
	Select *select_;
};

#endif /* SELECTMODEL_H_ */
