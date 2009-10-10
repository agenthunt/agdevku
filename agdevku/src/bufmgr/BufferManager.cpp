/*
 * BufferManager.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "BufferManager.h"
#include "../utils/debug.h"
#include "Frame.h"
#include "../utils/util.h"
BufferManager::BufferManager() {
	// TODO Auto-generated constructor stub

}

BufferManager::~BufferManager() {
	// TODO Auto-generated destructor stub
}

/* Initializing Buffer, Making a buffer pool and each element in it will be a Frame object of the size of pageSize*/

STATUS_CODE BufferManager::initializeBuffer(int sizeInMB, int pageSize) {
	pageSize_ = pageSize;
	numOfFrames_ = sizeInMB * 1024 * 1024 / pageSize;
	DEBUG("pageSize "<<pageSize_);
	bufferPool_ = new Frame * [numOfFrames_];
	for (int i = 0; i < numOfFrames_; i++) {
		bufferPool_[i] = new Frame(pageSize_);
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::createDatabase(const char *name, int numOfPages) {
	DEBUG(numOfPages);
	return SUCCESS;
}
