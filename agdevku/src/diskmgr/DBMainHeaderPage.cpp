/*
 * DBMainHeaderPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DBMainHeaderPage.h"
#include "../bufmgr/BufferManager.h"
#include <string.h>

DBMainHeaderPage::DBMainHeaderPage() {
	pageData_ = NULL; // check for this before doing any write/read operations
}

DBMainHeaderPage::DBMainHeaderPage(int pageNumber) {
	// TODO Auto-generated constructor stub
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->pinAndGetPage(pageNumber,pageData_);
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
}

DBMainHeaderPage::~DBMainHeaderPage() {
	// TODO Auto-generated destructor stub
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->unPinPage(dbMainHeader_.generalPageHeaderStruct.pageNumber,true);
}

/**
 * Dont use this often, instead use the constructor with pageNumber
 * used only while opening database
 */
DBMainHeaderPage::DBMainHeaderPage(char *pageData){
	pageData_ = pageData;
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
}

/**
 * This create is on actual disk
 */
STATUS_CODE DBMainHeaderPage::createHeaderPage(int pageSize, char *pageData,
		int pageNumber, int numOfPages) {
	memset(pageData, 0, pageSize);
	//default values for creation of header page
	DBMainheaderStruct dbMainHeader;
	dbMainHeader.generalPageHeaderStruct.pageNumber = 0;
	dbMainHeader.generalPageHeaderStruct.nextPageNumber = 1;
	dbMainHeader.pageType = DB_MAIN_HEADER_PAGE;
	dbMainHeader.totalNumberOfPages = numOfPages;
	dbMainHeader.freePagePointer = 1;
	dbMainHeader.pageSizeOfDatabase = pageSize;
	dbMainHeader.sysTableHeaderPageNumber = -1;
	dbMainHeader.sysColumnsHeaderPageNumber = -1;
	dbMainHeader.sysIndexHeaderPageNumber = -1;
	dbMainHeader.sysIndexColumnsHeaderPageNumber = -1;
	dbMainHeader.numberOfTables = 0;
	dbMainHeader.numberOfPagesUsed = 1;
	memcpy(pageData, &dbMainHeader, sizeof(DBMainheaderStruct));
	return SUCCESS;
}

int DBMainHeaderPage::getSysTablePageNumber() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.sysTableHeaderPageNumber;
}

int DBMainHeaderPage::getSysColumnsHeaderPageNumber() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.sysColumnsHeaderPageNumber;
}

int DBMainHeaderPage::getSysIndexHeaderPageNumber() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.sysIndexHeaderPageNumber;
}

int DBMainHeaderPage::getSysIndexColumnsHeaderPageNumber() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.sysIndexColumnsHeaderPageNumber;
}

int DBMainHeaderPage::getFreePagePointer() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.freePagePointer;
}

void DBMainHeaderPage::updateFreePagePointer(int pageNumber) {
	dbMainHeader_.freePagePointer = pageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}

int DBMainHeaderPage::getTotalNumberOfPages() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.totalNumberOfPages;
}

int DBMainHeaderPage::getPageSizeOfDatabase() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.pageSizeOfDatabase;
}

void DBMainHeaderPage::updateTotalNumberOfPages(int totalNumberOfPages) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.totalNumberOfPages = totalNumberOfPages;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}

void DBMainHeaderPage::updateSysTableHeaderPageNumber(
		int sysTableHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysTableHeaderPageNumber = sysTableHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}



void DBMainHeaderPage::updateSysColumnsHeaderPageNumber(
		int sysColumnsHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysColumnsHeaderPageNumber = sysColumnsHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}

void DBMainHeaderPage::updateSysIndexHeaderPageNumber(
		int sysIndexHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysIndexHeaderPageNumber = sysIndexHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}

void DBMainHeaderPage::updateSysIndexColumnsHeaderPageNumber(
		int sysIndexColumnsHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysIndexColumnsHeaderPageNumber
			= sysIndexColumnsHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
}
