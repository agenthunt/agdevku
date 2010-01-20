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
#include <string>
class DirectoryPage {
public:
	DirectoryPage();
	DirectoryPage(char *pageData);
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
	void setNextDirectoryPageNumber(int pageNumber);
	std::string toString();
private:
	typedef struct DirectoryPageHeaderStructure {
		GeneralPageHeaderStruct generalPageHeader;
		int nextDirectoryPageNumber;
		int numberOfDirectoryEntries;
	} DirectoryPageHeaderStruct;
	DirectoryPageHeaderStruct directoryPageHeader_;
	char *pageData_;
	bool haveIPinnedThePage;
	bool isDirty;
};

#endif /* DIRECTORYPAGE_H_ */
