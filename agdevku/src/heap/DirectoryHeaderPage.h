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
	STATUS_CODE createDirectoryHeaderPage(int pageSize,char *pageData,int pageNumber);
private:
	typedef struct DirectoryHeaderPageStructure{
		int pageNumber;
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
