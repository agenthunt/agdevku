/*
 * DirectoryHeaderPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DirectoryHeaderPage.h"
#include <string.h>
#include "DirectoryEntry.h"
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
}

STATUS_CODE DirectoryHeaderPage::createDirectoryHeaderPage(int pageSize, char *pageData,
		int pageNumber) {
	directoryHeaderPageStruct_.pageNumber = pageNumber;
	directoryHeaderPageStruct_.numberOfRecords = 0;
	directoryHeaderPageStruct_.numberOfDirectoryPages = 1;
	DirectoryEntry directoryEntry;
	directoryHeaderPageStruct_.directoryEntrySize = directoryEntry.getSizeOfDirectoryEntry();
	directoryHeaderPageStruct_.firstDirectoryPageNumber = -1;//indicates has not been created yet.
	directoryHeaderPageStruct_.maxNumberOfDirectoryEntries = (pageSize - 128 *8) / directoryHeaderPageStruct_.directoryEntrySize;
	return SUCCESS;
}
