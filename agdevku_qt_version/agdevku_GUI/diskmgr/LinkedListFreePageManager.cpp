/*
 * LinkedListFreePageManager.cpp
 *
 *  Created on: 20-Oct-2009
 *      Author: shailesh
 */

#include <string.h>
#include "LinkedListFreePageManager.h"
#include "../bufmgr/BufferManager.h"
#include "DBMainHeaderPage.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../utils/debug.h"
#include "../global/GlobalDefines.h"
LinkedListFreePageManager::LinkedListFreePageManager() {
	// TODO Auto-generated constructor stub

}

LinkedListFreePageManager::~LinkedListFreePageManager() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE LinkedListFreePageManager::createLinkedListOfFreePages(
		int rootPageNumber, int numberOfPages,
		DiskFileAccessor *diskFileAccessor, int pageSize) {
	int i = 0;
	int error = FAILURE;
	for (i = rootPageNumber; i < rootPageNumber + numberOfPages - 1; i++) {
		GeneralPageHeaderStruct generalPageHeaderStruct = { i, i + 1,
				TYPE_UNKNOWN_PAGE };
		char pageData[pageSize];
		memcpy(pageData, &generalPageHeaderStruct,
				sizeof(GeneralPageHeaderStruct));
		error = diskFileAccessor->writePage(i, pageSize, pageData);
		if (SUCCESS != error) {
			return error;
		}
	}
	//last page has nextPageNumber as -1,i.e grounded
	GeneralPageHeaderStruct generalPageHeaderStruct = { i, -1,
			TYPE_UNKNOWN_PAGE };
	char pageData[pageSize];
	memcpy(pageData, &generalPageHeaderStruct, sizeof(GeneralPageHeaderStruct));
	error = diskFileAccessor->writePage(i, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE LinkedListFreePageManager::getFreePageNumber(int& freePageNumber) {
	BufferManager *bufferManager = BufferManager::getInstance();
	DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
	freePageNumber = dbMainHeaderPage.getFreePagePointer();
	DEBUG("freepagePointer = "<<freePageNumber);
	if (freePageNumber == -1) {//-1 indicates end of free page linked list
		//do all the following from bufferManager
		//extend pages by some percentage
		//create linked list of chains
		int error = bufferManager->resizeDatabase(DEFAULT_NUM_OF_PAGES);
		if (error != SUCCESS) {
			bufferManager->unPinPage(DB_MAIN_HEADER_PAGE, false);
			return error;
		}
		//bufferManager->pinAndGetPage(0,headerPageData);
		//DBMainHeaderPage temp(headerPageData);
		//freePageNumber = temp.getFreePagePointer();
		freePageNumber = dbMainHeaderPage.getFreePagePointer();
	}
	char *pageData;
	bufferManager->pinAndGetPage(freePageNumber, pageData);
	GeneralPageHeaderAccessor generalPageHeaderAccessor(pageData);
	int newFreePagePointer = generalPageHeaderAccessor.getNextPageNumber();
	DEBUG("newFreePagePointer="<<newFreePagePointer);
	//update freePagePointer
	dbMainHeaderPage.updateFreePagePointer(newFreePagePointer);
	//update numberOfPagesUsed
	dbMainHeaderPage.updateNumberOfPagesUsed(
			dbMainHeaderPage.getNumberOfPagesUsed() + 1);
	bufferManager->unPinPage(freePageNumber, false);
	int toVerifyfreePageNumber = dbMainHeaderPage.getFreePagePointer();
	DEBUG("toVerifyfreepagePointer = "<<toVerifyfreePageNumber);
	return SUCCESS;
}

STATUS_CODE LinkedListFreePageManager::freePage(int pageNumber) {
	BufferManager *bufferManager = BufferManager::getInstance();
	DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
	int currentFreePageNumber = dbMainHeaderPage.getFreePagePointer();
	char *pageToBeFreed;
	bufferManager->pinAndGetPage(pageNumber, pageToBeFreed);
	GeneralPageHeaderAccessor generalPageHeaderAccessor(pageToBeFreed);
	generalPageHeaderAccessor.setNextPageNumber(currentFreePageNumber);
	dbMainHeaderPage.updateFreePagePointer(pageNumber);
	dbMainHeaderPage.updateNumberOfPagesUsed(
			dbMainHeaderPage.getNumberOfPagesUsed() - 1);
	bufferManager->unPinPage(pageNumber, false);
	return SUCCESS;
}
