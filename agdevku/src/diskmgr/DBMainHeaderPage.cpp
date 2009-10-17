/*
 * DBMainHeaderPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DBMainHeaderPage.h"
#include <string.h>
DBMainHeaderPage::DBMainHeaderPage(char *pageData) {
	// TODO Auto-generated constructor stub
	pageData_ = pageData;
}

DBMainHeaderPage::~DBMainHeaderPage() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE DBMainHeaderPage::createHeaderPage(int pageSize) {
	memset(pageData_,0,pageSize);
	headerStruct_.pageType = DB_MAIN_HEADER_PAGE;
	headerStruct_.pageNumberOfFreePageList = 1;
	headerStruct_.pageSizeOfDatabase = pageSize;
	headerStruct_.sysTablePageNumber = 2;
	headerStruct_.sysColumnsPageNumber = 3;
	headerStruct_.numberOfTables = 0;
	headerStruct_.numberOfPages = 1;
	memcpy(pageData_,&headerStruct_,sizeof(DBMainheaderStruct));
	return SUCCESS;
}

int DBMainHeaderPage::getSysTablePageNumber(){
	return headerStruct_.sysTablePageNumber;
}
