/*
 * FrameBufferModel.cpp
 *
 *  Created on: 12-Nov-2009
 *      Author: shailesh
 */

#include "FrameBufferModel.h"
#include <QStringList>
#include "../utils/debug.h"
FrameBufferModel::FrameBufferModel() {
	// TODO Auto-generated constructor stub
	bufferManager = BufferManager::getInstance();
	QStringList headers;
	headers << "FRAME NUMBER" << "PRIORITY" << "PAGE NUMBER" << "DIRTY FLAG"
			<< "VIEW" << "COMMIT BUFFER";
	setHorizontalHeaderLabels(headers);
	initModel();
	populateModel();

}

FrameBufferModel::~FrameBufferModel() {
	// TODO Auto-generated destructor stub

}

void FrameBufferModel::initModel() {
	for (int i = 0; i < bufferManager->numOfFrames_; i++) {
		QList<QStandardItem*> list;
		list.append(new QStandardItem(QString::number(i)));
		list.append(new QStandardItem(QString::number(
				bufferManager->bufferPool_[i]->priority_)));
		list.append(new QStandardItem(QString::number(
				bufferManager->bufferPool_[i]->pageNumber_)));
		if (bufferManager->bufferPool_[i]->dirty_ == true) {
			list.append(new QStandardItem("true"));
		} else if (bufferManager->bufferPool_[i]->dirty_ == false){
			list.append(new QStandardItem("false"));
		}
		list.append(new QStandardItem("view_frame"));
		list.append(new QStandardItem("commit_buffer"));
		frameBufferList.append(list);
		insertRow(i, list);
	}

}

void FrameBufferModel::populateModel() {
	for (int i = 0; i < bufferManager->numOfFrames_; i++) {
		QList<QStandardItem*> list = frameBufferList.at(i);
		list.at(0)->setText(QString::number(i));
		list.at(1)->setText(QString::number(
				bufferManager->bufferPool_[i]->priority_));

		list.at(2)->setText(QString::number(
				bufferManager->bufferPool_[i]->pageNumber_));
		if (bufferManager->bufferPool_[i]->dirty_ == true) {
			list.at(3)->setText("true");
		} else if (bufferManager->bufferPool_[i]->dirty_ == false){
			list.at(3)->setText("false");
		}
		list.append(new QStandardItem("view_frame"));
		list.append(new QStandardItem("commit_buffer"));
	}

}
