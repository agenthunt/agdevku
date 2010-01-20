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
#include "../diskmgr/DBMainHeaderPage.h"
#include "../global/GlobalDefines.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include <assert.h>
#include <sstream>
DirectoryHeaderPage::DirectoryHeaderPage() {
	// TODO Auto-generated constructor stub
	pageData_ = NULL;
	haveIPinnedThePage = false;
	isDirty = false;
}

DirectoryHeaderPage::DirectoryHeaderPage(int pageNumber) {
	BufferManager *bufMgr = BufferManager::getInstance();
	constructorErrorStatus = bufMgr->pinAndGetPage(pageNumber, pageData_);
	haveIPinnedThePage = true;
	isDirty = false;
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
}

DirectoryHeaderPage::DirectoryHeaderPage(char *pageData) {
	// TODO Auto-generated constructor stub
	haveIPinnedThePage = false;
	isDirty = false;
	pageData_ = pageData;
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
}

DirectoryHeaderPage::~DirectoryHeaderPage() {
	// TODO Auto-generated destructor stub
	if (haveIPinnedThePage == true) {
		BufferManager *bufMgr = BufferManager::getInstance();
		bufMgr->unPinPage(getPageNumber(), isDirty);
	}
}

STATUS_CODE DirectoryHeaderPage::createDirectoryHeaderPage() {
	BufferManager *bufferManager = BufferManager::getInstance();
	int pageNum;
	int error = bufferManager->newPage(pageNum, pageData_);

	if (error != SUCCESS) {
		return error;
	}
	haveIPinnedThePage = true;
	isDirty = true;
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
	directoryHeaderPageStruct_.generalPageHeader.pageType
			= TYPE_DIRECTORY_HEADER_PAGE;
	memcpy(pageData_, &directoryHeaderPageStruct_,
			sizeof(DirectoryHeaderPageStruct));
	return SUCCESS;
}

STATUS_CODE DirectoryHeaderPage::createFirstDirectoryPage() {
	DirectoryPage directoryPage;
	directoryPage.createDirectoryPage();
	int pageNumber = directoryPage.getPageNumber();
	DBMainHeaderPage mainPage(DB_MAIN_HEADER_PAGE);
	assert(pageNumber <= mainPage.getTotalNumberOfPages());
	updateFirstDirectoryPageNumber(pageNumber);
	isDirty = true;
	return SUCCESS;
}

void DirectoryHeaderPage::updateFirstDirectoryPageNumber(int pageNumber) {

	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	directoryHeaderPageStruct_.firstDirectoryPageNumber = pageNumber;
	memcpy(pageData_, &directoryHeaderPageStruct_,
			sizeof(DirectoryHeaderPageStruct));
	isDirty = true;
}

void DirectoryHeaderPage::updateNumOfRecords(int numOfRecords) {
	memcpy(&directoryHeaderPageStruct_, pageData_,
			sizeof(DirectoryHeaderPageStruct));
	directoryHeaderPageStruct_.numberOfRecords = numOfRecords;
	memcpy(pageData_, &directoryHeaderPageStruct_,
			sizeof(DirectoryHeaderPageStruct));
	isDirty = true;
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

int DirectoryHeaderPage::getPageNumber() {
	return directoryHeaderPageStruct_.generalPageHeader.pageNumber;
}

std::string DirectoryHeaderPage::toString() {
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "NUMBER OF RECORDS: " << getNumberOfRecords() << "\n";
	mystream << "NUMBER OF DIRECTORY PAGES: " << getNumberOfDirectoryPages()
			<< "\n";
	mystream << "FIRST DIRECTORY PAGE NO: " << getFirstDirectoryPageNumber()
			<< "\n";
	mystream << "NUMBER OF RECORDS: " << getNumberOfRecords() << "\n";
	mystream << "DIRECTORY ENTRY SIZE: " << getDirectoryEntrySize() << "\n";
	mystream << "MAX NO. OF DIRECTORY ENTRIES: "
			<< getMaxNumberOfDirectoryEntries() << "\n";
	return mystream.str();
}
