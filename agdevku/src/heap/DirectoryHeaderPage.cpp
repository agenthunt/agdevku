/*
 * DirectoryHeaderPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DirectoryHeaderPage.h"
#include <string.h>
#include "DirectoryEntry.h"
#include "../bufmgr/BufferManager.h"
#include "DirectoryPage.h"
DirectoryHeaderPage::DirectoryHeaderPage() {
	// TODO Auto-generated constructor stub
	pageData_ = NULL;
}

DirectoryHeaderPage::DirectoryHeaderPage(char *pageData) {
	// TODO Auto-generated constructor stub
	pageData_ = pageData;
}

DirectoryHeaderPage::~DirectoryHeaderPage() {
	// TODO Auto-generated destructor stub
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->unPinPage(directoryHeaderPageStruct_.generalPageHeader.pageNumber,
			true);
}

STATUS_CODE DirectoryHeaderPage::createDirectoryHeaderPage() {
	BufferManager *bufferManager = BufferManager::getInstance();
	int pageNum;
	int error = bufferManager->newPage(pageNum, pageData_);
	if(error!=SUCCESS){
		return error;
	}
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	directoryHeaderPageStruct_.numberOfRecords = 0;
	directoryHeaderPageStruct_.numberOfDirectoryPages = 1;
	DirectoryEntry directoryEntry;
	directoryHeaderPageStruct_.directoryEntrySize
			= directoryEntry.getSizeOfDirectoryEntry();
	directoryHeaderPageStruct_.firstDirectoryPageNumber = -1;//indicates has not been created yet.
	directoryHeaderPageStruct_.maxNumberOfDirectoryEntries
			= (bufferManager->getCurrentlyUsingPageSize()
					- sizeof(DirectoryHeaderPageStruct))
					/ directoryHeaderPageStruct_.directoryEntrySize;
	memcpy(pageData_, &directoryHeaderPageStruct_,
			sizeof(DirectoryHeaderPageStruct));
	return SUCCESS;
}

STATUS_CODE DirectoryHeaderPage::createFirstDirectoryPage() {
	DirectoryPage directoryPage;
	directoryPage.createDirectoryPage();
	updateFirstDirectoryPageNumber(directoryPage.getPageNumber());
	return SUCCESS;
}

void DirectoryHeaderPage::updateFirstDirectoryPageNumber(int pageNumber) {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	directoryHeaderPageStruct_.firstDirectoryPageNumber = pageNumber;
	memcpy(pageData_, &directoryHeaderPageStruct_,
			sizeof(DirectoryHeaderPageStruct));
}

void DirectoryHeaderPage::updateNumOfRecords(int numOfRecords) {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	directoryHeaderPageStruct_.numberOfRecords = numOfRecords;
	memcpy(pageData_, &directoryHeaderPageStruct_,
			sizeof(DirectoryHeaderPageStruct));
}

int DirectoryHeaderPage::getFirstDirectoryPageNumber() {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	return directoryHeaderPageStruct_.firstDirectoryPageNumber;
}

int DirectoryHeaderPage::getNumberOfRecords() {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	return directoryHeaderPageStruct_.numberOfRecords;
}

int DirectoryHeaderPage::getNumberOfDirectoryPages() {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	return directoryHeaderPageStruct_.numberOfDirectoryPages;
}

int DirectoryHeaderPage::getDirectoryEntrySize() {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	return directoryHeaderPageStruct_.directoryEntrySize;
}

int DirectoryHeaderPage::getMaxNumberOfDirectoryEntries() {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	return directoryHeaderPageStruct_.maxNumberOfDirectoryEntries;
}

int DirectoryHeaderPage::getPageNumber(){
	return directoryHeaderPageStruct_.generalPageHeader.pageNumber;
}
