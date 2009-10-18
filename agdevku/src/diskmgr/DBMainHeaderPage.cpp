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
	headerStruct_.pageType = DB_MAIN_HEADER_PAGE;
	headerStruct_.pageNumber = pageNumber;
	headerStruct_.pageNumberOfFreePageList = 1;
	headerStruct_.pageSizeOfDatabase = pageSize;
	headerStruct_.sysTablePageNumber = 2;
	headerStruct_.sysColumnsPageNumber = 3;
	headerStruct_.numberOfTables = 0;
	headerStruct_.numberOfPages = 1;
	memcpy(pageData, &headerStruct_, sizeof(DBMainheaderStruct));
	return SUCCESS;
}

int DBMainHeaderPage::getSysTablePageNumber() {
	return headerStruct_.sysTablePageNumber;
}
