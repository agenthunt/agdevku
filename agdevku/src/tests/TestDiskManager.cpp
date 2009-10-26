/*
 * TestDiskManager.cpp
 *
 *  Created on: 17-Oct-2009
 *      Author: shailesh
 */

#include "../diskmgr/DiskManager.h"
#include "../utils/debug.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include <iostream>
#include <string>
using namespace std;
class TestDiskManager {

public:
	string testName;
	TestDiskManager() {
		testName = "TestDiskManager";
	}

	STATUS_CODE createDatabaseTest() {
		string methodName = "createDatabaseTest";
		DiskManager diskManager;
		int error = diskManager.createDatabase("first", 20, DEFAULT_PAGE_SIZE);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	STATUS_CODE dropDatabaseTest() {
		string methodName = "dropDatabaseTest";
		DiskManager diskManager;
		int error = diskManager.dropDatabase("first");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}



	void basicTest() {
		createDatabaseTest();
		//dropDatabaseTest();
	}

};
