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
#include "../global/GlobalDefines.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include <sstream>

DirectoryPage::DirectoryPage() {
	haveIPinnedThePage = false;
	isDirty = false;
}

DirectoryPage::DirectoryPage(char *pageData) {
	// TODO Auto-generated constructor stub
	haveIPinnedThePage = false;
	isDirty = false;
	pageData_ = pageData;
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
}

DirectoryPage::DirectoryPage(int pageNumber) {
	BufferManager *bufMgr = BufferManager::getInstance();
	constructorErrorStatus = bufMgr->pinAndGetPage(pageNumber, pageData_);
	haveIPinnedThePage = true;
	isDirty = false;
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
}

DirectoryPage::~DirectoryPage() {
	// TODO Auto-generated destructor stub
	if (haveIPinnedThePage == true) {
		BufferManager *bufMgr = BufferManager::getInstance();
		bufMgr->unPinPage(directoryPageHeader_.generalPageHeader.pageNumber,
				isDirty);
	}
}

STATUS_CODE DirectoryPage::createDirectoryPage() {
	BufferManager *bufMgr = BufferManager::getInstance();
	int pageNumber;
	bufMgr->newPage(pageNumber, pageData_);
	haveIPinnedThePage = true;
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	directoryPageHeader_.nextDirectoryPageNumber = -1;
	directoryPageHeader_.numberOfDirectoryEntries = 0;
	directoryPageHeader_.generalPageHeader.pageType = TYPE_DIRECTORY_PAGE;
	memcpy(pageData_, &directoryPageHeader_, sizeof(DirectoryPageHeaderStruct));
	isDirty = true;
	return SUCCESS;
}

int DirectoryPage::getPageNumber() {
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	return directoryPageHeader_.generalPageHeader.pageNumber;
}

int DirectoryPage::getNumberOfDirectoryEntries() {
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	return directoryPageHeader_.numberOfDirectoryEntries;
}

void DirectoryPage::getDirectoryEntry(int directoryEntryNumber,
		DirectoryEntry& dirEntry) {
	int offset = sizeof(DirectoryPageHeaderStruct) + directoryEntryNumber
			* dirEntry.getSizeOfDirectoryEntry();
	memcpy(&(dirEntry.directoryEntry_), &pageData_[offset],
			dirEntry.getSizeOfDirectoryEntry());
}

void DirectoryPage::updateFreeSpace(int directoryEntryNumber, int freeSpace) {
	DirectoryEntry dirEntry;
	int offset = sizeof(DirectoryPageHeaderStruct) + directoryEntryNumber
			* dirEntry.getSizeOfDirectoryEntry();
	memcpy(&(dirEntry.directoryEntry_), &pageData_[offset],
			dirEntry.getSizeOfDirectoryEntry());
	dirEntry.directoryEntry_.freeSpace = freeSpace;
	memcpy(&pageData_[offset], &(dirEntry.directoryEntry_),
			dirEntry.getSizeOfDirectoryEntry());
	isDirty = true;
}

void DirectoryPage::addDirectoryEntry(int pageNumber, int freeSpace,
		DirectoryEntry& dirEntry) {
	int offset = sizeof(DirectoryPageHeaderStruct)
			+ directoryPageHeader_.numberOfDirectoryEntries
					* dirEntry.getSizeOfDirectoryEntry();
	dirEntry.directoryEntry_.entryNumber
			= directoryPageHeader_.numberOfDirectoryEntries + 1;
	dirEntry.directoryEntry_.pageNumber = pageNumber;
	dirEntry.directoryEntry_.freeSpace = freeSpace;
	memcpy(&pageData_[offset], &(dirEntry.directoryEntry_),
			dirEntry.getSizeOfDirectoryEntry());
	directoryPageHeader_.numberOfDirectoryEntries
			= directoryPageHeader_.numberOfDirectoryEntries + 1;
	memcpy(pageData_, &directoryPageHeader_, sizeof(DirectoryPageHeaderStruct));
	isDirty = true;
}

int DirectoryPage::getNextDirectoryPageNumber() {
	memcpy(&directoryPageHeader_, pageData_, sizeof(DirectoryPageHeaderStruct));
	return directoryPageHeader_.nextDirectoryPageNumber;
}

void DirectoryPage::setNextDirectoryPageNumber(int pageNumber){
	directoryPageHeader_.nextDirectoryPageNumber = pageNumber;
	memcpy(pageData_, &directoryPageHeader_, sizeof(DirectoryPageHeaderStruct));
}

std::string DirectoryPage::toString() {
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "NEXT DIR PAGE NO: " << getNextDirectoryPageNumber() << "\n";
	mystream << "NUMBER OF DIRECTORY ENTRIES: "
			<< getNumberOfDirectoryEntries() << "\n";
	mystream << "**** DIR ENTRIES****\n";
	int numOfDirEntries = getNumberOfDirectoryEntries();
	for (int i = 0; i < numOfDirEntries; ++i) {
		//mystream << "DIR ENTRY NUM=" << i << "\n";
		DirectoryEntry dirEntry;
		getDirectoryEntry(i, dirEntry);
		int offset = sizeof(DirectoryPageHeaderStruct) + (dirEntry.getDirectoryEntryNumber()-1)
				* dirEntry.getSizeOfDirectoryEntry();
		mystream << "Directory Entry Offset: "<<offset << "\n";
		mystream << dirEntry.toString();
	}

	return mystream.str();
}


