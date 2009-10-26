/*
 * DBMainHeaderPage.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DBMAINHEADERPAGE_H_
#define DBMAINHEADERPAGE_H_
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"
#include "DiskMgrRelatedStructures.h"
class DBMainHeaderPage {
public:
	DBMainHeaderPage();
	DBMainHeaderPage(char *pageData);
	virtual ~DBMainHeaderPage();
	/**
	 * This method is called to create the header page
	 * first time when the database is created
	 */
	STATUS_CODE createHeaderPage(int pageSize,char *pageData,int pageNumber,int numOfPages);
	int getSysTablePageNumber();
	int getFreePagePointer();
	void updateFreePagePointer(int pageNumber);
	void updateTotalNumberOfPages(int totalNumberOfPages);
	int getTotalNumberOfPages();
	int getPageSizeOfDatabase();
private:
	char *pageData_;
	typedef struct DBMainHeaderStructure{
		GeneralPageHeaderStruct generalPageHeaderStruct;
		int pageType;
		int totalNumberOfPages;
		int freePagePointer;
		int pageSizeOfDatabase;
		int sysTablePageNumber;
		int sysColumnsPageNumber;
		int sysIndexPageNumber;
		int sysIndexColumnsPageNumber;
		int numberOfTables;
		int numberOfPagesUsed;
	}DBMainheaderStruct;
	DBMainheaderStruct dbMainHeader_;
};

#endif /* DBMAINHEADERPAGE_H_ */
