/*
 * FrameBufferModel.h
 *
 *  Created on: 12-Nov-2009
 *      Author: shailesh
 */

#ifndef FRAMEBUFFERMODEL_H_
#define FRAMEBUFFERMODEL_H_
#include <QStandardItemModel>
#include "../bufmgr/BufferManager.h"

class FrameBufferModel: public QStandardItemModel {
public:
	FrameBufferModel();
	virtual ~FrameBufferModel();
	void initModel();
	void populateModel();
	BufferManager *bufferManager;
	QList<QList<QStandardItem*> > frameBufferList;

};

#endif /* FRAMEBUFFERMODEL_H_ */
