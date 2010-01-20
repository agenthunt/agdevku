/*
 * SplitFrameBufferModel.cpp
 *
 *  Created on: 05-Jan-2010
 *      Author: shailesh
 */

#include "math.h"
#include "SplitFrameBufferModel.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include "../utils/debug.h"

SplitFrameBufferModel::SplitFrameBufferModel() {
	// TODO Auto-generated constructor stub
	bufferManager = BufferManager::getInstance();

	numOfPages = ceil((double) bufferManager->numOfFrames_
			/ (double) ROWS_PER_PAGE);
	currentPage = 1;
	DEBUG("numOfPages"<<numOfPages);
	loadFirstPage();
}

SplitFrameBufferModel::~SplitFrameBufferModel() {
	// TODO Auto-generated destructor stub
}

void SplitFrameBufferModel::loadData() {
	numOfPages = ceil((double) bufferManager->numOfFrames_
			/ (double) ROWS_PER_PAGE);
	frameBufferList.clear();
	DEBUG("frameBufferListLength="<<frameBufferList.length());
	clear();
	QStringList headers;
	headers << "FRAME NUMBER" << "PRIORITY" << "PAGE NUMBER" << "DIRTY FLAG"
			<< "VIEW" << "COMMIT BUFFER";// << "PIN COUNT";
	setHorizontalHeaderLabels(headers);
	DEBUG("currentPage="<<currentPage);
	for (int j = ((currentPage - 1) * ROWS_PER_PAGE), i = 0; (j < (currentPage
			* ROWS_PER_PAGE)) && (j < bufferManager->numOfFrames_); j++, i++) {
		QList<QStandardItem*> list;
		list.append(new QStandardItem(QString::number(j)));
		list.append(new QStandardItem(QString::number(
				bufferManager->bufferPool_[j]->priority_)));
		list.append(new QStandardItem(QString::number(
				bufferManager->bufferPool_[j]->pageNumber_)));
		if (bufferManager->bufferPool_[j]->dirty_ == true) {
			list.append(new QStandardItem("true"));
		} else if (bufferManager->bufferPool_[j]->dirty_ == false){
			list.append(new QStandardItem("false"));
		}
		list.append(new QStandardItem("view_frame"));
		list.append(new QStandardItem("commit_buffer"));
//		list.append(new QStandardItem(QString::number(
//						bufferManager->bufferPool_[j]->pinCount_)));
		frameBufferList.append(list);
		insertRow(i, list);
	}
}

void SplitFrameBufferModel::loadFirstPage() {
	numOfPages = ceil((double) bufferManager->numOfFrames_
			/ (double) ROWS_PER_PAGE);
	currentPage = 1;
	loadData();
}

void SplitFrameBufferModel::loadNextPage() {
	numOfPages = ceil((double) bufferManager->numOfFrames_
			/ (double) ROWS_PER_PAGE);
	if (currentPage < numOfPages) {
		currentPage++;
	}
	loadData();
}

void SplitFrameBufferModel::loadPrevPage() {
	numOfPages = ceil((double) bufferManager->numOfFrames_
			/ (double) ROWS_PER_PAGE);
	if (currentPage > 1) {
		currentPage--;
	}
	loadData();
}

void SplitFrameBufferModel::loadLastPage() {
	numOfPages = ceil((double) bufferManager->numOfFrames_
			/ (double) ROWS_PER_PAGE);
	currentPage = numOfPages;
	loadData();
}
