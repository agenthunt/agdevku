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
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);

		DiskManager diskManager;

		diskManager.openDatabase("first");
		char headerPageData[DEFAULT_PAGE_SIZE];
		error = diskManager.readPage(0, DEFAULT_PAGE_SIZE, headerPageData);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		DBMainHeaderPage dbMainHeaderPage(headerPageData);
		DEBUG("freePagePoitner is "<<dbMainHeaderPage.getFreePagePointer()<<endl);

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
		if (pageNumber != 1) {
			DEBUG(testName << ":" << methodName << "=FAILURE,error="<<error << endl);
			return FAILURE;
		}
		error = bufferManager->newPage(pageNumber, pageData);
		if (pageNumber != 2) {
			DEBUG(testName << ":" << methodName << "=FAILURE,error="<<error << endl);
			return FAILURE;
		}
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

	void basicTest() {
		//createDatabaseTest();
		//dropDatabaseTest();
		newPageTest();
	}

};
