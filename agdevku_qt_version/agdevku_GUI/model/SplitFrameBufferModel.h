/*
 * SplitFrameBufferModel.h
 *
 *  Created on: 05-Jan-2010
 *      Author: shailesh
 */

#ifndef SPLITFRAMEBUFFERMODEL_H_
#define SPLITFRAMEBUFFERMODEL_H_
#include <QStandardItemModel>
#include "../bufmgr/BufferManager.h"
class SplitFrameBufferModel : public QStandardItemModel {
public:
	SplitFrameBufferModel();
	virtual ~SplitFrameBufferModel();
	BufferManager *bufferManager;
	QList<QList<QStandardItem*> > frameBufferList;
	void loadFirstPage();
	void loadNextPage();
	void loadPrevPage();
	void loadLastPage();
	void loadData();
	int currentPage;
	int numOfPages;
};

#endif /* SPLITFRAMEBUFFERMODEL_H_ */
