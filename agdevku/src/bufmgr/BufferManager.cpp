/*
 * BufferManager.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include <utility>
#include <iterator>
#include "BufferManager.h"
#include "../utils/debug.h"
#include "Frame.h"
#include "../utils/util.h"
#include "../diskmgr/DBMainHeaderPage.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "LRUReplacementPolicy.h"
#include "../diskmgr/LinkedListFreePageManager.h"
BufferManager::BufferManager() {
	// TODO Auto-generated constructor stub
	initializeBuffer(1, DEFAULT_PAGE_SIZE);
	isDatabaseOpen_ = false;
	//either do it here or do it in some config class
	delete replacementPolicy_;
	replacementPolicy_ = new LRUReplacementPolicy();
}

BufferManager::~BufferManager() {
	// TODO Auto-generated destructor stub
	delete[] bufferPool_;
}

BufferManager* BufferManager::bufferManagerInstance_ = NULL;
BufferManager* BufferManager::getInstance() {
	if (bufferManagerInstance_ == NULL) {
		bufferManagerInstance_ = new BufferManager();
	}
	return bufferManagerInstance_;
}

/* Initializing Buffer, Making a buffer pool and each element in it will be a Frame object of the size of pageSize*/

STATUS_CODE BufferManager::initializeBuffer(double sizeInMB, int pageSize) {
	delete bufferPool_;
	pageSize_ = pageSize;
	bufferSize_ = sizeInMB;
	numOfFrames_ = sizeInMB * 1024 * 1024 / pageSize;
	DEBUG("pageSize "<<pageSize_<< "bytes");
	bufferPool_ = new Frame *[numOfFrames_];
	for (int i = 0; i < numOfFrames_; i++) {
		bufferPool_[i] = new Frame(pageSize_);
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::createDatabase(const char *name, int numOfPages) {
	DEBUG(numOfPages);
	int error = diskManager_.createDatabase(name, numOfPages, pageSize_);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::resizeDatabase(int numOfPages) {
	int error = diskManager_.resizeDatabase(numOfPages,pageSize_);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::newPage(int& pageNumber, char*& pageData) {
	if (isDatabaseOpen_ == false) {
		return DATABASE_NOT_OPEN;
	}
	int error = freePageManager_.getFreePageNumber(pageNumber);
	if (error != SUCCESS) {
		return error;
	}
	return pinAndGetPage(pageNumber, pageData);
}

STATUS_CODE BufferManager::pinAndGetPage(int pageNumber, char*& pageData) {
	//check frameLookupTable_ to see if page exists, if yes
	//if no, find free frame,
	//if free frame marked dirty, write the pageData to disk (use bufferPool_.pageNumber_)
	//delete from frameLookupTable_
	//then read page from disk into frame
	//add to frameLookupTable_
	//pageData = bufferPool_[frameNumber]->pageData_
	//pin the page
	if (isDatabaseOpen_ == false) {
		return DATABASE_NOT_OPEN;
	}
	int error = FAILURE;
	int frameNumber = getFrame(pageNumber);
	if (frameNumber != FRAME_NOT_FOUND) {
		pageData = bufferPool_[frameNumber]->pageData_;
	} else {
		frameNumber = getFreeFrame();
		if (FRAME_NOT_FOUND == frameNumber) {
			return BUFFER_FULL;
		} else {
			if (bufferPool_[frameNumber]->dirty_ == true) {
				int pageNumberToBeWrittenToDisk =
						bufferPool_[frameNumber]->pageNumber_;
				char *pageDataToBeWrittenToDisk =
						bufferPool_[frameNumber]->pageData_;
				error = diskManager_.writePage(pageNumberToBeWrittenToDisk,
						pageSize_, pageDataToBeWrittenToDisk);
				if (error != SUCCESS) {
					return error;
				}

			}

			//if page number exists then erase
			frameLookupTable_.erase(bufferPool_[frameNumber]->pageNumber_);
			error = diskManager_.readPage(pageNumber, pageSize_,
					bufferPool_[frameNumber]->pageData_);
			if (error != SUCCESS) {
				return error;
			}

			pageData = bufferPool_[frameNumber]->pageData_;
			GeneralPageHeaderAccessor gp(pageData);
			DEBUG("pageNumber"<<gp.getPageNumber());
			insertIntoLookupTable(pageNumber, frameNumber);
		}
	}

	replacementPolicy_->increasePriority(frameNumber);
	bufferPool_[frameNumber]->pinCount_++;
	return SUCCESS;
}

void BufferManager::insertIntoLookupTable(int pageNumber, int frameNumber) {
	std::pair<int, int> element(pageNumber, frameNumber);
	frameLookupTable_.insert(element);
	bufferPool_[frameNumber]->pageNumber_ = pageNumber;
}

int BufferManager::getFrame(int pageNumber) {

	std::map<int, int>::iterator iter = frameLookupTable_.find(pageNumber);
	if (iter != frameLookupTable_.end()) {
		return frameLookupTable_[iter->second];
	}
	return FRAME_NOT_FOUND;
}

void BufferManager::unPinPage(int pageNumber, bool dirty) {
	int frameNumber = getFrame(pageNumber);
	bufferPool_[frameNumber]->pinCount_--;
	bufferPool_[frameNumber]->dirty_ = dirty;
}

int BufferManager::getFreeFrame() {
	//implement the replacement algorithm
	for (int i = 0; i < numOfFrames_; i++) {
		if (bufferPool_[i]->pinCount_ == -1) {//-1 indicates free frame, frame not allocated
			bufferPool_[i]->pinCount_++;
			return i;
		}
	}

	//if it comes here, it means all frames have been allocated
	//kick off some page based on policy
	//LRU or MRU depending on the algorithm
	int frameToBeReplaced = replacementPolicy_->getFrameTobeReplaced();

	replacementPolicy_->resetPriority(frameToBeReplaced);
	return frameToBeReplaced;
}

STATUS_CODE BufferManager::openDatabase(const char *databaseName) {
	int error = diskManager_.openDatabase(databaseName);
	if (error != SUCCESS) {
		return error;
	}
	char pageData[DEFAULT_PAGE_SIZE];
	diskManager_.readPage(0, DEFAULT_PAGE_SIZE, pageData);
	if (error != SUCCESS) {
		return error;
	}
	DBMainHeaderPage dbMainHeaderPage(pageData);
	int pageSize = dbMainHeaderPage.getPageSizeOfDatabase();
	if (pageSize != pageSize_) {
		initializeBuffer(bufferSize_, pageSize);
	}
	isDatabaseOpen_ = true;

	//lets load some important pages
	char *dummyPageData;
	pinAndGetPage(0, dummyPageData);
	unPinPage(0, false);

	return SUCCESS;
}

STATUS_CODE BufferManager::closeDatabase() {
	flushAllPagesToDisk();
	int error = diskManager_.closeDatabase();
	if (error != SUCCESS) {
		return error;
	}
	isDatabaseOpen_ = false;
	return SUCCESS;
}

STATUS_CODE BufferManager::flushAllPagesToDisk() {
	int error = SUCCESS;
	for (int i = 0; i < numOfFrames_; i++) {
		if (bufferPool_[i]->dirty_ == true) {
			error = flushPageToDisk(bufferPool_[i]->pageNumber_,
					bufferPool_[i]->pageData_);
			if (error != SUCCESS) {
				return error;
			}
		}
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::flushPageToDisk(int pageNumber, char *pageData) {
	int error = diskManager_.writePage(pageNumber, pageSize_, pageData);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::freePage(int pageNumber){
	LinkedListFreePageManager linkedListFreePageManager;
	int error = linkedListFreePageManager.freePage(pageNumber);
	if(error != SUCCESS){
		return error;
	}
	return SUCCESS;
}

