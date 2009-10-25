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
	for (i = rootPageNumber; i < rootPageNumber + numberOfPages - 1; i++) {
		GeneralPageHeaderStruct generalPageHeaderStruct = { i, i + 1 };
		char pageData[pageSize];
		memcpy(pageData, &generalPageHeaderStruct,
				sizeof(GeneralPageHeaderStruct));
		diskFileAccessor->writePage(i, pageSize, pageData);
	}
	//last page has nextPageNumber as -1,i.e grounded
	GeneralPageHeaderStruct generalPageHeaderStruct = { i, -1 };
	char pageData[pageSize];
	memcpy(pageData, &generalPageHeaderStruct, sizeof(GeneralPageHeaderStruct));
	diskFileAccessor->writePage(i, pageSize, pageData);
	return SUCCESS;
}

STATUS_CODE  LinkedListFreePageManager::getFreePageNumber(int& freePageNumber){
	BufferManager *bufferManager = BufferManager::getInstance();
	char *headerPageData;
	// dbmainheader page number is always 0.

	int error = bufferManager->pinAndGetPage(DB_MAIN_HEADER_PAGE,headerPageData);
	if(error!=SUCCESS){
		bufferManager->unPinPage(DB_MAIN_HEADER_PAGE,false);
		return error;
	}
	DBMainHeaderPage dbMainHeaderPage(headerPageData);
	freePageNumber = dbMainHeaderPage.getFreePagePointer();
	DEBUG("freepagePointer = "<<freePageNumber);
	if(freePageNumber == -1){//-1 indicates end of free page linked list
		//do all the following from bufferManager
		//extend pages by some percentage
		//create linked list of chains
	}
	char *pageData;
	bufferManager->pinAndGetPage(freePageNumber,pageData);
	GeneralPageHeaderAccessor generalPageHeaderAccessor(pageData);
	int newFreePagePointer = generalPageHeaderAccessor.getNextPageNumber();
	DEBUG("newFreePagePointer="<<newFreePagePointer);
	//update freePagePointer
	dbMainHeaderPage.updateFreePagePointer(newFreePagePointer);
	int toVerifyfreePageNumber = dbMainHeaderPage.getFreePagePointer();
	DEBUG("toVerifyfreepagePointer = "<<toVerifyfreePageNumber);
	bufferManager->unPinPage(DB_MAIN_HEADER_PAGE,true);

	return SUCCESS;
}
