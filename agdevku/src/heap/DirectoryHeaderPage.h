/*
 * DirectoryHeaderPage.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DIRECTORYHEADERPAGE_H_
#define DIRECTORYHEADERPAGE_H_
#include "../global/GlobalStructures.h"
#include "../global/StatusCodes.h"
class DirectoryHeaderPage {
public:
	DirectoryHeaderPage();
	DirectoryHeaderPage(char *pageData);
	virtual ~DirectoryHeaderPage();
	STATUS_CODE createDirectoryHeaderPage();
	STATUS_CODE createFirstDirectoryPage();
	int getFirstDirectoryPageNumber();
	int getNumberOfRecords();
	int getNumberOfDirectoryPages();
	int getDirectoryEntrySize();
	int getMaxNumberOfDirectoryEntries();
	int getPageNumber();
	void updateFirstDirectoryPageNumber(int pageNumber);
	void updateNumOfRecords(int numOfRecords);
private:
	typedef struct DirectoryHeaderPageStructure{
		GeneralPageHeaderStruct generalPageHeader;
		int numberOfRecords;
		int numberOfDirectoryPages;
		int firstDirectoryPageNumber;
		int directoryEntrySize;
		int maxNumberOfDirectoryEntries;//(pageSize-128x8)/directoryentrySize

	} DirectoryHeaderPageStruct;

	DirectoryHeaderPageStruct directoryHeaderPageStruct_;
	char *pageData_;
};

#endif /* DIRECTORYHEADERPAGE_H_ */
