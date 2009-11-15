/*
 * DirectoryPage.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DIRECTORYPAGE_H_
#define DIRECTORYPAGE_H_
#include "../global/GlobalStructures.h"
#include "DirectoryEntry.h"
class DirectoryPage {
public:
	DirectoryPage();
	DirectoryPage(int pageNumber);
	virtual ~DirectoryPage();
	int getPageNumber();
	STATUS_CODE createDirectoryPage();
	void getDirectoryEntry(int directoryEntryNumber, DirectoryEntry& dirEntry);
	int getNumberOfDirectoryEntries();
	void updateFreeSpace(int directoryEntryNumber, int freeSpace);
	void addDirectoryEntry(int pageNumber, int freeSpace,
			DirectoryEntry& dirEntry);
	int getNextDirectoryPageNumber();
private:
	typedef struct DirectoryPageHeaderStructure {
		GeneralPageHeaderStruct generalPageHeader;
		int nextDirectoryPageNumber;
		int numberOfDirectoryEntries;
	} DirectoryPageHeaderStruct;
	DirectoryPageHeaderStruct directoryPageHeader_;
	char *pageData_;
};

#endif /* DIRECTORYPAGE_H_ */
