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
#include <string>
class DBMainHeaderPage {
public:
	//DBMainHeaderPage();
	DBMainHeaderPage(char *pageData);
	DBMainHeaderPage(int pageNumber);
	virtual ~DBMainHeaderPage();
	/**
	 * This method is called to create the header page
	 * first time when the database is created
	 * This is on disk that is why having this signature
	 * for rest of other pages this should no arg method
	 */
	static STATUS_CODE createHeaderPage(int pageSize, char *pageData,
			int pageNumber, int numOfPages);
	int getSysTablePageNumber();
	int getSysColumnsHeaderPageNumber();
	int getSysIndexHeaderPageNumber();
	int getSysIndexColumnsHeaderPageNumber();
	int getFreePagePointer();
	int getTotalNumberOfPages();
	int getPageSizeOfDatabase();
	int getNumberOfTables();
	int getNumberOfPagesUsed();

	void updateFreePagePointer(int pageNumber);
	void updateTotalNumberOfPages(int totalNumberOfPages);
	void updateSysTableHeaderPageNumber(int sysTableHeaderPageNumber);
	void updateSysColumnsHeaderPageNumber(int sysColumnsHeaderPageNumber);
	void updateSysIndexHeaderPageNumber(int sysIndexHeaderPageNumber);
	void updateSysIndexColumnsHeaderPageNumber(
			int sysIndexColumnsHeaderPageNumber);
	void updateNumberOfPagesUsed(int numberOfPagesUsed);
	void updateNumberOfTables(int numberOfTables);
	std::string toString();
private:
	char *pageData_;
	typedef struct DBMainHeaderStructure {
		GeneralPageHeaderStruct generalPageHeaderStruct;
		int pageType;
		int totalNumberOfPages;
		int freePagePointer;
		int pageSizeOfDatabase;
		int sysTableHeaderPageNumber;
		int sysColumnsHeaderPageNumber;
		int sysIndexHeaderPageNumber;
		int sysIndexColumnsHeaderPageNumber;
		int numberOfTables;
		int numberOfPagesUsed;
	} DBMainheaderStruct;
	DBMainheaderStruct dbMainHeader_;
	bool haveIPinnedThePage;
	bool isDirty;
};

#endif /* DBMAINHEADERPAGE_H_ */
