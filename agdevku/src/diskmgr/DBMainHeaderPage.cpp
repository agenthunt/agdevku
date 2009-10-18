/*
 * DBMainHeaderPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DBMainHeaderPage.h"
#include <string.h>
DBMainHeaderPage::DBMainHeaderPage() {
	pageData_ = NULL; // check for this before doing any write/read operations
}

DBMainHeaderPage::DBMainHeaderPage(char *pageData) {
	// TODO Auto-generated constructor stub
	pageData_ = pageData;
}

DBMainHeaderPage::~DBMainHeaderPage() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE DBMainHeaderPage::createHeaderPage(int pageSize,char *pageData,int pageNumber) {
	memset(pageData, 0, pageSize);
	//default values for creation of header page
	dbMainHeader_.pageType = DB_MAIN_HEADER_PAGE;
	dbMainHeader_.pageNumber = pageNumber;
	dbMainHeader_.pageNumberOfFreePageList = 1;
	dbMainHeader_.pageSizeOfDatabase = pageSize;
	dbMainHeader_.sysTablePageNumber = 2;
	dbMainHeader_.sysColumnsPageNumber = 3;
	dbMainHeader_.numberOfTables = 0;
	dbMainHeader_.numberOfPages = 4;
	memcpy(pageData, &dbMainHeader_, sizeof(DBMainheaderStruct));
	return SUCCESS;
}

int DBMainHeaderPage::getSysTablePageNumber() {
	return dbMainHeader_.sysTablePageNumber;
}
