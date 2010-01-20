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
#include "../catalog/CatalogUtil.h"
#include <cassert>
#include "../global/ExternDefOfGlobalVariables.h"
//#include "../global/GlobalVariables.h"
#include <string.h>
BufferManager::BufferManager() {
	// TODO Auto-generated constructor stub
	bufferPool_ = NULL;
	initializeBuffer(200, DEFAULT_PAGE_SIZE);
	isDatabaseOpen_ = false;
	//strcpy(currentlyOpenedDatabase, "DATABASE NOT OPEN");
	//either do it here or do it in some config class
	//delete replacementPolicy_;//if you uncomment QT crashes dude
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
	frameLookupTable_.clear();
	numOfDiskAccess_ = 0;
	numOfPageReads_ = 0;
	numOfTimesPageFoundInBuffer_ = 0;
	totalNumOfRequestsToBuffer_ = 0;
	return SUCCESS;
}

STATUS_CODE BufferManager::createDatabase(const char *name, int numOfPages) {
	DEBUG(numOfPages);
	if (isDatabaseOpen_ == true) {
		return CLOSE_OPENED_DATABASE;
	}
	DEBUG("pagesize before creating"<<pageSize_);
	initializeBuffer(bufferSize_, pageSize_);//checking
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
	dbMainHeaderPage.updateSysIndexColumnsHeaderPageNumber(
			sysIndexColDirectoryHeaderPage.getPageNumber());
	/****end of creation of sys index headerpage*/

	CatalogUtil catalogUtil;
	catalogUtil.addInitialSysTableRecord();
	catalogUtil.addInitialSysColTableRecord();
	catalogUtil.addInitialSysIndexTableRecord();
	catalogUtil.addInitialSysIndexColTableRecord();
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
	totalNumOfRequestsToBuffer_++;
	int error = FAILURE;
	int frameNumber = getFrame(pageNumber);
	if (frameNumber != FRAME_NOT_FOUND) {
		pageData = bufferPool_[frameNumber]->pageData_;//page exists in the pool,just return the pointer
		numOfTimesPageFoundInBuffer_++;
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
			//update diskreads
			numOfDiskAccess_++;

			pageData = bufferPool_[frameNumber]->pageData_;
			GeneralPageHeaderAccessor gp(pageData);
			DEBUG("pageNumber="<<gp.getPageNumber());
			insertIntoLookupTable(pageNumber, frameNumber);
		}
	}

	replacementPolicy_->increasePriority(frameNumber);
	bufferPool_[frameNumber]->pinCount_++;//indicates somebody is using it
	numOfPageReads_++;
	return SUCCESS;
}

void BufferManager::insertIntoLookupTable(int pageNumber, int frameNumber) {
	assert(frameNumber > -1);
	std::pair<int, int> element(pageNumber, frameNumber);
	frameLookupTable_.insert(element);
	bufferPool_[frameNumber]->pageNumber_ = pageNumber;
}

int BufferManager::getFrame(int pageNumber) {

	std::map<int, int>::iterator iter = frameLookupTable_.find(pageNumber);
	if (iter != frameLookupTable_.end()) {
		//return frameLookupTable_[iter->second];
		return iter->second;
	}
	return FRAME_NOT_FOUND;
}

void BufferManager::unPinPage(int pageNumber, bool dirty) {
	DEBUG("unPinning page="<<pageNumber);
	int frameNumber = getFrame(pageNumber);
	assert(frameNumber > -1);
	assert(bufferPool_[frameNumber]->pinCount_ > 0);

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
	assert(frameToBeReplaced > -1);
	//hey can we reset the priority here?
	replacementPolicy_->resetPriority(frameToBeReplaced);
	return frameToBeReplaced;
}

STATUS_CODE BufferManager::openDatabase(const char *databaseName) {
	int error = diskManager_.openDatabase(databaseName);
	if (error != SUCCESS) {
		return error;
	}
	isDatabaseOpen_ = true;
	numOfDiskAccess_ = 0;
	numOfPageReads_ = 0;

	//	char *pageData = new char[DEFAULT_PAGE_SIZE];
	//	diskManager_.readPage(0, DEFAULT_PAGE_SIZE, pageData);
	//	if (error != SUCCESS) {
	//		return error;
	//	}

	char pageData[DEFAULT_PAGE_SIZE];
	diskManager_.readPage(0, DEFAULT_PAGE_SIZE, pageData);
	if (error != SUCCESS) {
		return error;
	}
	//DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
	DBMainHeaderPage dbMainHeaderPage(pageData);
	int pageSize = dbMainHeaderPage.getPageSizeOfDatabase();
	DEBUG("pageSize in openDatabase"<<pageSize);
	if (pageSize != pageSize_) {
		initializeBuffer(bufferSize_, pageSize);
	}

	//lets load some important pages
	//	char *dummyPageData;
	//	pinAndGetPage(0, dummyPageData);
	//	unPinPage(0, false);


	//delete pageData;
	return SUCCESS;
}

STATUS_CODE BufferManager::closeDatabase() {
	DEBUG("closing database");
	if (isDatabaseOpen_ == true) {
		flushAllPagesToDisk();
		int error = diskManager_.closeDatabase();
		if (error != SUCCESS) {
			return error;
		}
		isDatabaseOpen_ = false;
		//strcpy(currentlyOpenedDatabase, "DATABASE NOT OPEN");

		return SUCCESS;
	}
	return DATABASE_NOT_OPEN;
}

STATUS_CODE BufferManager::flushAllPagesToDisk() {
	DEBUG("flushing pages to disk");
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
	numOfDiskAccess_++;
	int error = diskManager_.writePage(pageNumber, pageSize_, pageData);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::flushPageToDisk(int pageNumber) {
	int frameNumber = getFrame(pageNumber);
	int error = diskManager_.writePage(pageNumber, pageSize_,
			bufferPool_[frameNumber]->pageData_);
	if (error != SUCCESS) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE BufferManager::freePage(int pageNumber) {
	DEBUG("freeing pageNumber="<<pageNumber);
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

/**
 * delegates the call to the Diskmanager.
 */
STATUS_CODE BufferManager::dropDatabase(const char *name) {
	return diskManager_.dropDatabase(name);
}

std::vector<std::string> BufferManager::getListOfDatabases() {
	return diskManager_.getListOfDatabases();
}

STATUS_CODE BufferManager::getPageNumber(int frameNumber, int& pageNumber) {
	if (isDatabaseOpen_ == false) {
		return DATABASE_NOT_OPEN;
	}
	DBMainHeaderPage mainHeaderPage(DB_MAIN_HEADER_PAGE);
	pageNumber = bufferPool_[frameNumber]->pageNumber_;
	if (pageNumber < 0 || pageNumber > mainHeaderPage.getTotalNumberOfPages()) {
		return INVALID_PAGE;
	}
	return SUCCESS;
}

float BufferManager::getHitRate() {
	//float hitRate = (float) (numOfDiskAccess_) / (float) (numOfPageReads_);
	float hitRate = (float) (numOfTimesPageFoundInBuffer_)
			/ (float) (totalNumOfRequestsToBuffer_);
	return hitRate;
}
