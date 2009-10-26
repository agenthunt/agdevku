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
	memcpy(&dbMainHeader_,pageData_,sizeof(DBMainheaderStruct));
}

DBMainHeaderPage::~DBMainHeaderPage() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE DBMainHeaderPage::createHeaderPage(int pageSize, char *pageData,
		int pageNumber, int numOfPages) {
	memset(pageData, 0, pageSize);
	//default values for creation of header page
	dbMainHeader_.generalPageHeaderStruct.pageNumber = 0;
	dbMainHeader_.generalPageHeaderStruct.nextPageNumber = 1;
	dbMainHeader_.pageType = DB_MAIN_HEADER_PAGE;
	dbMainHeader_.totalNumberOfPages = numOfPages;
	dbMainHeader_.freePagePointer = 1;
	dbMainHeader_.pageSizeOfDatabase = pageSize;
	dbMainHeader_.sysTablePageNumber = -1;
	dbMainHeader_.sysColumnsPageNumber = -1;
	dbMainHeader_.sysIndexPageNumber = -1;
	dbMainHeader_.sysIndexColumnsPageNumber = -1;
	dbMainHeader_.numberOfTables = 0;
	dbMainHeader_.numberOfPagesUsed = 1;
	memcpy(pageData, &dbMainHeader_, sizeof(DBMainheaderStruct));
	return SUCCESS;
}

int DBMainHeaderPage::getSysTablePageNumber() {
	memcpy(&dbMainHeader_,pageData_,sizeof(DBMainheaderStruct));
	return dbMainHeader_.sysTablePageNumber;
}


int DBMainHeaderPage::getFreePagePointer(){
	memcpy(&dbMainHeader_,pageData_,sizeof(DBMainheaderStruct));
	return dbMainHeader_.freePagePointer;
}

void DBMainHeaderPage::updateFreePagePointer(int pageNumber){
	dbMainHeader_.freePagePointer = pageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}


int DBMainHeaderPage::getTotalNumberOfPages(){
	memcpy(&dbMainHeader_,pageData_,sizeof(DBMainheaderStruct));
	return dbMainHeader_.totalNumberOfPages;
}

int DBMainHeaderPage::getPageSizeOfDatabase(){
	memcpy(&dbMainHeader_,pageData_,sizeof(DBMainheaderStruct));
	return dbMainHeader_.pageSizeOfDatabase;
}

void DBMainHeaderPage::updateTotalNumberOfPages(int totalNumberOfPages){
	dbMainHeader_.totalNumberOfPages = totalNumberOfPages;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}
