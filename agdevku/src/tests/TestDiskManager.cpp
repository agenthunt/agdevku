/*
 * TestDiskManager.cpp
 *
 *  Created on: 17-Oct-2009
 *      Author: shailesh
 */

#include "../diskmgr/DiskManager.h"

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
		diskManager.createDatabase("first",20,DEFAULT_PAGE_SIZE);
		cout << testName << ":" << methodName << "=SUCCESS" << endl;
		return SUCCESS;
	}

};
