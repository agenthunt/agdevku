/*
 * DirectoryPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DirectoryPage.h"
#include "../global/StatusCodes.h"
#include "../bufmgr/BufferManager.h"
#include <string.h>
#include "DirectoryEntry.h"
DirectoryPage::DirectoryPage() {
	// TODO Auto-generated constructor stub

}

DirectoryPage::DirectoryPage(int pageNumber) {
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->pinAndGetPage(pageNumber, pageData_);
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
}

DirectoryPage::~DirectoryPage() {
	// TODO Auto-generated destructor stub
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->unPinPage(directoryPageHeader_.generalPageHeader.pageNumber, true);
}

STATUS_CODE DirectoryPage::createDirectoryPage() {
	BufferManager *bufMgr = BufferManager::getInstance();
	int pageNumber;
	bufMgr->newPage(pageNumber, pageData_);
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	directoryPageHeader_.nextDirectoryPageNumber = -1;
	directoryPageHeader_.numberOfDirectoryEntries = 0;
	memcpy(pageData_, &directoryPageHeader_, sizeof(DirectoryPageHeaderStruct));
	return SUCCESS;
}

int DirectoryPage::getPageNumber() {
	return directoryPageHeader_.generalPageHeader.pageNumber;
}

int DirectoryPage::getNumberOfDirectoryEntries() {
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	return directoryPageHeader_.numberOfDirectoryEntries;
}

void DirectoryPage::getDirectoryEntry(int directoryEntryNumber,
		DirectoryEntry& dirEntry) {
	int offset = sizeof(DirectoryPageHeaderStruct) + directoryEntryNumber
			* sizeof(DirectoryEntry);
	memcpy(&(dirEntry.directoryEntry_), &pageData_[offset],
			dirEntry.getSizeOfDirectoryEntry());
}

void DirectoryPage::updateFreeSpace(int directoryEntryNumber, int freeSpace) {
	DirectoryEntry dirEntry;
	int offset = sizeof(DirectoryPageHeaderStruct) + directoryEntryNumber
			* sizeof(DirectoryEntry);
	memcpy(&(dirEntry.directoryEntry_), &pageData_[offset],
			dirEntry.getSizeOfDirectoryEntry());
	dirEntry.directoryEntry_.freeSpace = freeSpace;
	memcpy(&pageData_[offset], &(dirEntry.directoryEntry_),
			dirEntry.getSizeOfDirectoryEntry());
}

void DirectoryPage::addDirectoryEntry(int pageNumber, int freeSpace,
		DirectoryEntry& dirEntry) {
	int offset = sizeof(DirectoryPageHeaderStruct)
			+ directoryPageHeader_.numberOfDirectoryEntries
					* sizeof(DirectoryEntry);
	dirEntry.directoryEntry_.entryNumber
			= directoryPageHeader_.numberOfDirectoryEntries + 1;
	dirEntry.directoryEntry_.pageNumber = pageNumber;
	dirEntry.directoryEntry_.freeSpace = freeSpace;
	memcpy(&pageData_[offset], &(dirEntry.directoryEntry_),
			dirEntry.getSizeOfDirectoryEntry());
	directoryPageHeader_.numberOfDirectoryEntries
			= directoryPageHeader_.numberOfDirectoryEntries + 1;
	memcpy(pageData_, &directoryPageHeader_,
			sizeof(DirectoryPageHeaderStruct));
}

int DirectoryPage::getNextDirectoryPageNumber() {
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	return directoryPageHeader_.nextDirectoryPageNumber;
}
