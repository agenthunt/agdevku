/*
 * TestLinkedListFreePageManager.cpp
 *
 *  Created on: 21-Oct-2009
 *      Author: shailesh
 */

#include "../bufmgr/BufferManager.h"
#include "../utils/debug.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include "../diskmgr/LinkedListFreePageManager.h"
#include "../diskmgr/DBMainHeaderPage.h"
#include <iostream>
#include <string>
using namespace std;
class TestLinkedListFreePageManager {

public:
	string testName;
	TestLinkedListFreePageManager() {
		testName = "TestLinkedListFreePageManager";
	}

	STATUS_CODE basicTest() {
		string methodName = "createFreePageTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->createDatabase("first", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		LinkedListFreePageManager linkedListFreePageManager;
		int freePageNumber;
		error = linkedListFreePageManager.getFreePageNumber(freePageNumber);
		if (freePageNumber != 1) {
			DEBUG(testName << ":" << methodName << "=FAILURE ,freepageNumber" << freePageNumber<< endl);
			return error;
		}

		error = linkedListFreePageManager.getFreePageNumber(freePageNumber);
		if (freePageNumber != 2) {
			DEBUG(testName << ":" << methodName << "=FAILURE ,freepageNumber" << freePageNumber<< endl);
			return error;
		}

		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

};
