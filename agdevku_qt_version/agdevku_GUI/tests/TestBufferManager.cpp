/*
 * TestBufferManager.cpp
 *
 *  Created on: 20-Oct-2009
 *      Author: shailesh
 */

#include "../bufmgr/BufferManager.h"
#include "../utils/debug.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include "../diskmgr/DBMainHeaderPage.h"
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;
class TestBufferManager {

public:
	string testName;
	TestBufferManager() {
		testName = "TestBufferManager";
	}

	STATUS_CODE createDatabaseTest() {
		string methodName = "createDatabaseTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->createDatabase("first", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}

		DiskManager diskManager;

		diskManager.openDatabase("first");
		char *headerPageData = new char[DEFAULT_PAGE_SIZE];
		error = diskManager.readPage(0, DEFAULT_PAGE_SIZE, headerPageData);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
		DEBUG("freePagePoitner is "<<dbMainHeaderPage.getFreePagePointer()<<endl);

		int sysTableHeaderPageNumber = dbMainHeaderPage.getSysTablePageNumber();
		assert(sysTableHeaderPageNumber==1);
		int sysColHeaderPageNumber =
				dbMainHeaderPage.getSysColumnsHeaderPageNumber();
		if (sysColHeaderPageNumber != 2) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<sysColHeaderPageNumber<< endl);
			return FAILURE;
		}
		int sysIndexHeaderPageNumber =
				dbMainHeaderPage.getSysIndexHeaderPageNumber();
		assert(sysIndexHeaderPageNumber == 3);
		int sysIndexColumnsHeaderPageNumber =
				dbMainHeaderPage.getSysIndexColumnsHeaderPageNumber();
		assert(sysIndexColumnsHeaderPageNumber == 4);
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	STATUS_CODE dropDatabaseTest() {
		string methodName = "dropDatabaseTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->dropDatabase("first");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	STATUS_CODE newPageTest() {
		string methodName = "newPageTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->initializeBuffer(1, DEFAULT_PAGE_SIZE);
		bufferManager->createDatabase("first", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		bufferManager->openDatabase("first");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		int pageNumber;
		char *pageData;
		error = bufferManager->newPage(pageNumber, pageData);
		if (pageNumber != 5) {
			DEBUG(testName << ":" << methodName << "=FAILURE,error="<<error << endl);
			return FAILURE;
		}

		error = bufferManager->newPage(pageNumber, pageData);
		if (pageNumber != 6) {
			DEBUG(testName << ":" << methodName << "=FAILURE,error="<<error << endl);
			return FAILURE;
		}

		error = bufferManager->newPage(pageNumber, pageData);
		if (pageNumber != 7) {
			DEBUG(testName << ":" << methodName << "=FAILURE,error="<<error << endl);
			return FAILURE;
		}


		DBMainHeaderPage mainHeaderPage(DB_MAIN_HEADER_PAGE);
		int freePagePointer = mainHeaderPage.getFreePagePointer();
		DEBUG("freepagepointer"<<freePagePointer<<","<<mainHeaderPage.getPageSizeOfDatabase());
		DEBUG("closing database");
		bufferManager->closeDatabase();
		error = bufferManager->newPage(pageNumber, pageData);
		if (error != DATABASE_NOT_OPEN) {
			DEBUG(testName << ":" << methodName << "=FAILURE,error="<<error << endl);
			return FAILURE;
		}

		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	STATUS_CODE resizeDatabaseTest() {
		string methodName = "resizeDatabaseTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->initializeBuffer(1, DEFAULT_PAGE_SIZE);
		bufferManager->createDatabase("first", 2);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		bufferManager->openDatabase("first");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		int newPageNumber;
		char *pageData;
		error = bufferManager->newPage(newPageNumber, pageData);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		DEBUG("newPageNumber="<<newPageNumber);
		bufferManager->pinAndGetPage(0, pageData);
		DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
		DEBUG("totalNumber of pages"<<dbMainHeaderPage.getTotalNumberOfPages());

		error = bufferManager->newPage(newPageNumber, pageData);
		DEBUG("newPageNumber="<<newPageNumber);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		DEBUG("totalNumber of pages"<<dbMainHeaderPage.getTotalNumberOfPages());
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	STATUS_CODE freePageTest() {
		string methodName = "freePageTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->initializeBuffer(1, DEFAULT_PAGE_SIZE);
		bufferManager->createDatabase("first", 10);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		bufferManager->openDatabase("first");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		int newPageNumber;
		char *pageData;
		error = bufferManager->newPage(newPageNumber, pageData);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		DEBUG("newPageNumber="<<newPageNumber);

		error = bufferManager->newPage(newPageNumber, pageData);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		DEBUG("newPageNumber="<<newPageNumber);

		error = bufferManager->freePage(newPageNumber);

		error = bufferManager->newPage(newPageNumber, pageData);
		assert(newPageNumber==2);
		DEBUG("newPageNumber="<<newPageNumber);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	void basicTest() {
		//createDatabaseTest();
		//dropDatabaseTest();
		newPageTest();
	}

};
