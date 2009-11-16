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
#include "../heap/DirectoryHeaderPage.h"
#include "../catalog/SysTableUtil.h"
#include <cassert>
BufferManager::BufferManager() {
	// TODO Auto-generated constructor stub
	bufferPool_ = NULL;
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
	if (bufferPool_ != NULL) {
		delete[] bufferPool_;
		bufferPool_ = NULL;
	}
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
	error = openDatabase(name);
	if (error != SUCCESS) {
		return error;
	}

	DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
	/****start of creation of sys table headerpage*/

	DirectoryHeaderPage sysTableDirectoryHeaderPage;
	error = sysTableDirectoryHeaderPage.createDirectoryHeaderPage();
	if (error != SUCCESS) {
		return error;
	}
	dbMainHeaderPage.updateSysTableHeaderPageNumber(
			sysTableDirectoryHeaderPage.getPageNumber());
	/****end of creation of sys table headerpage*/

	/****start of creation of sys columns headerpage*/
	DirectoryHeaderPage sysColDirectoryHeaderPage;
	error = sysColDirectoryHeaderPage.createDirectoryHeaderPage();
	if (error != SUCCESS) {
		return error;
	}
	dbMainHeaderPage.updateSysColumnsHeaderPageNumber(
			sysColDirectoryHeaderPage.getPageNumber());
	/****end of creation of sys columns headerpage*/

	/****start of creation of sys index headerpage*/
	DirectoryHeaderPage sysIndexDirectoryHeaderPage;
	error = sysIndexDirectoryHeaderPage.createDirectoryHeaderPage();
	if (error != SUCCESS) {
		return error;
	}
	dbMainHeaderPage.updateSysIndexHeaderPageNumber(
			sysIndexDirectoryHeaderPage.getPageNumber());
	/****end of creation of sys index headerpage*/

	/****start of creation of sys index columns headerpage*/
	DirectoryHeaderPage sysIndexColDirectoryHeaderPage;
	error = sysIndexColDirectoryHeaderPage.createDirectoryHeaderPage();
	if (error != SUCCESS) {
		return error;
	}
	dbMainHeaderPage.updateSysColumnsHeaderPageNumber(
			sysIndexColDirectoryHeaderPage.getPageNumber());
	/****end of creation of sys index headerpage*/

	SysTableUtil sysTableUtil;
	sysTableUtil.addInitialSysTableRecord();
	error = closeDatabase();
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::resizeDatabase(int numOfPages) {
	int error = diskManager_.resizeDatabase(numOfPages, pageSize_);
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
	DEBUG("newPage="<<pageNumber);
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
		pageData = bufferPool_[frameNumber]->pageData_;//page exists in the pool,just return the pointer
	} else {
		frameNumber = getFreeFrame();//find free frame
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

			//remove the  kicked out page number from lookup table,
			frameLookupTable_.erase(bufferPool_[frameNumber]->pageNumber_);

			//read the required pagenumber into the free frame
			error = diskManager_.readPage(pageNumber, pageSize_,
					bufferPool_[frameNumber]->pageData_);
			if (error != SUCCESS) {
				return error;
			}

			pageData = bufferPool_[frameNumber]->pageData_;
			GeneralPageHeaderAccessor gp(pageData);
			DEBUG("pageNumber="<<gp.getPageNumber());
			insertIntoLookupTable(pageNumber, frameNumber);
		}
	}

	replacementPolicy_->increasePriority(frameNumber);
	bufferPool_[frameNumber]->pinCount_++;//indicates somebody is using it
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
	assert(bufferPool_[frameNumber]->pinCount_>0);
	bufferPool_[frameNumber]->pinCount_--;//somebody released it
	if (dirty == true && bufferPool_[frameNumber]->dirty_ == false) {
		bufferPool_[frameNumber]->dirty_ = dirty;
	}
}

int BufferManager::getFreeFrame() {
	//implement the replacement algorithm
	for (int i = 0; i < numOfFrames_; i++) {
		if (bufferPool_[i]->pinCount_ == -1) {//-1 indicates free frame, frame not allocated
			bufferPool_[i]->pinCount_++;//here it gets allocated so becomes 0
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
	isDatabaseOpen_ = true;
	//	char *pageData = new char[DEFAULT_PAGE_SIZE];
	//	diskManager_.readPage(0, DEFAULT_PAGE_SIZE, pageData);
	//	if (error != SUCCESS) {
	//		return error;
	//	}
	DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
	int pageSize = dbMainHeaderPage.getPageSizeOfDatabase();
	if (pageSize != pageSize_) {
		initializeBuffer(bufferSize_, pageSize);
	}

	//lets load some important pages
	char *dummyPageData;
	pinAndGetPage(0, dummyPageData);
	unPinPage(0, false);

	//delete pageData;
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

STATUS_CODE BufferManager::freePage(int pageNumber) {
	LinkedListFreePageManager linkedListFreePageManager;
	int error = linkedListFreePageManager.freePage(pageNumber);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

int BufferManager::getCurrentlyUsingPageSize() {
	return pageSize_;
}

