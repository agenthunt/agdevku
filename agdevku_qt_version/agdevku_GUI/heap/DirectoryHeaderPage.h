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
#include <string>
class DirectoryHeaderPage {
public:
	DirectoryHeaderPage();
	DirectoryHeaderPage(int pageNumber);
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
	std::string toString();
private:
	typedef struct DirectoryHeaderPageStructure{
		GeneralPageHeaderStruct generalPageHeader;
		int numberOfRecords;
		int numberOfDirectoryPages;
		int firstDirectoryPageNumber;
		int directoryEntrySize;
		int maxNumberOfDirectoryEntries;

	} DirectoryHeaderPageStruct;

	DirectoryHeaderPageStruct directoryHeaderPageStruct_;
	char *pageData_;
	bool haveIPinnedThePage;
	bool isDirty;
};

#endif /* DIRECTORYHEADERPAGE_H_ */
