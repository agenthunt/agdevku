/*
 * DBMainHeaderPage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DBMainHeaderPage.h"
#include "../bufmgr/BufferManager.h"
#include <string.h>
#include "../global/GlobalDefines.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include <sstream>
#include "../utils/ErrorLookupTable.h"
#include "../utils/debug.h"
#include "../global/ExternDefOfGlobalVariables.h"
//DBMainHeaderPage::DBMainHeaderPage() {
//	pageData_ = NULL; // check for this before doing any write/read operations
//}

DBMainHeaderPage::DBMainHeaderPage(int pageNumber) {
	// TODO Auto-generated constructor stub
	haveIPinnedThePage = true;
	isDirty = false;
	BufferManager *bufMgr = BufferManager::getInstance();
	constructorErrorStatus = bufMgr->pinAndGetPage(pageNumber, pageData_);
	DEBUG("constructorErrorStateus=" << ErrorLookupTable::lookup(
			constructorErrorStatus));
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	DEBUG("in constructor"<<toString());
}

DBMainHeaderPage::~DBMainHeaderPage() {
	// TODO Auto-generated destructor stub
	if (haveIPinnedThePage == true) {
		BufferManager *bufMgr = BufferManager::getInstance();
		bufMgr->unPinPage(DB_MAIN_HEADER_PAGE, isDirty);
	}
}

/**
 * Dont use this often, instead use the constructor with pageNumber
 * used only while opening database
 */
DBMainHeaderPage::DBMainHeaderPage(char *pageData) {
	haveIPinnedThePage = false;
	isDirty = false;
	pageData_ = pageData;
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	DEBUG("in constructor"<<toString());
}

/**
 * This create is on actual disk
 */
STATUS_CODE DBMainHeaderPage::createHeaderPage(int pageSize, char *pageData,
		int pageNumber, int numOfPages) {
	DEBUG("pageSize while create DBMAin headerpage"<<pageSize);
	memset(pageData, 0, pageSize);
	//default values for creation of header page
	DBMainheaderStruct dbMainHeader;
	dbMainHeader.generalPageHeaderStruct.pageNumber = 0;
	dbMainHeader.generalPageHeaderStruct.nextPageNumber = 1;
	dbMainHeader.generalPageHeaderStruct.pageType = TYPE_DB_MAIN_HEADER_PAGE;
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
	isDirty = true;
}

int DBMainHeaderPage::getTotalNumberOfPages() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.totalNumberOfPages;
}

int DBMainHeaderPage::getPageSizeOfDatabase() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	DEBUG("while getting back pageSize"<<dbMainHeader_.pageSizeOfDatabase);
	return dbMainHeader_.pageSizeOfDatabase;
}

int DBMainHeaderPage::getNumberOfPagesUsed() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.numberOfPagesUsed;
}

int DBMainHeaderPage::getNumberOfTables() {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	return dbMainHeader_.numberOfTables;
}

void DBMainHeaderPage::updateTotalNumberOfPages(int totalNumberOfPages) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.totalNumberOfPages = totalNumberOfPages;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

void DBMainHeaderPage::updateSysTableHeaderPageNumber(
		int sysTableHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysTableHeaderPageNumber = sysTableHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

void DBMainHeaderPage::updateSysColumnsHeaderPageNumber(
		int sysColumnsHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysColumnsHeaderPageNumber = sysColumnsHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

void DBMainHeaderPage::updateSysIndexHeaderPageNumber(
		int sysIndexHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysIndexHeaderPageNumber = sysIndexHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

void DBMainHeaderPage::updateSysIndexColumnsHeaderPageNumber(
		int sysIndexColumnsHeaderPageNumber) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.sysIndexColumnsHeaderPageNumber
			= sysIndexColumnsHeaderPageNumber;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

void DBMainHeaderPage::updateNumberOfPagesUsed(int numberOfPagesUsed) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.numberOfPagesUsed = numberOfPagesUsed;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

void DBMainHeaderPage::updateNumberOfTables(int numberOfTables) {
	memcpy(&dbMainHeader_, pageData_, sizeof(DBMainheaderStruct));
	dbMainHeader_.numberOfTables = numberOfTables;
	memcpy(pageData_, &dbMainHeader_, sizeof(DBMainheaderStruct));
	isDirty = true;
}

std::string DBMainHeaderPage::toString() {
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "TOTAL NUMBER OF PAGES:  " << getTotalNumberOfPages() << "\n"
			<< "FREE PAGE POINTER:  " << getFreePagePointer() << "\n"
			<< "DATABASE PAGE SIZE:  " << getPageSizeOfDatabase() << "\n"
			<< "SYS TABLE HEADER PAGE NO:  " << getSysTablePageNumber() << "\n"
			<< "SYS COL HEADER PAGE NO:  " << getSysColumnsHeaderPageNumber()
			<< "\n" << "SYS INDEX HEADER PAGE NO:  "
			<< getSysIndexHeaderPageNumber() << "\n"
			<< "SYS INDEX COL HEADER PAGE NO:  "
			<< getSysIndexColumnsHeaderPageNumber() << "\n"
			<< "NUMBER OF TABLES:  " << getNumberOfTables() << "\n"
			<< "NUMBER OF PAGES USED:  " << getNumberOfPagesUsed();
	return mystream.str();
}

