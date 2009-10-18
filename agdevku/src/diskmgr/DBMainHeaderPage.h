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
class DBMainHeaderPage {
public:
	DBMainHeaderPage();
	DBMainHeaderPage(char *pageData);
	virtual ~DBMainHeaderPage();
	/**
	 * This method is called to create the header page
	 * first time when the database is created
	 */
	STATUS_CODE createHeaderPage(int pageSize,char *pageData,int pageNumber);
	int getSysTablePageNumber();
private:
	char *pageData_;
	typedef struct DBMainHeaderStructure{

		int pageNumber;
		int pageType;
		int pageNumberOfFreePageList;
		int pageSizeOfDatabase;
		int sysTablePageNumber;
		int sysColumnsPageNumber;
		int numberOfTables;
		int numberOfPages;
	}DBMainheaderStruct;
	DBMainheaderStruct dbMainHeader_;
};

#endif /* DBMAINHEADERPAGE_H_ */
