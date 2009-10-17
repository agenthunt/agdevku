/*
 * TestSimpleDiskFileAccessor.cpp
 *
 *  Created on: 12-Oct-2009
 *      Author: shailesh
 */
#include "../diskmgr/DiskFileAccessor.h"
#include "../diskmgr/SimpleDiskFileAccessor.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include <iostream>
#include <string>
using namespace std;
class TestSimpleDiskFileAccessor {

public:
	string testName;
	TestSimpleDiskFileAccessor() {
		testName = "TestSimpleDiskFileAccessor";
	}
	STATUS_CODE basicTest() {
		string methodName = "basicTest";
		DiskFileAccessor *diskFileAccessor = new SimpleDiskFileAccessor();
		char *fileName = "src/databases/test.db";
		int error = diskFileAccessor->createDiskFile(fileName, 1,
				DEFAULT_PAGE_SIZE);
		if (SUCCESS != error) {
			return error;
		}
		error = diskFileAccessor->openDiskFile(fileName);
		if (SUCCESS != error) {
			return error;
		}
		char *pageData = "hi how r u?";
		error = diskFileAccessor->writePage(1, DEFAULT_PAGE_SIZE, pageData);
		if (SUCCESS != error) {
			return error;
		}
		error = diskFileAccessor->closeDiskFile();
		if (SUCCESS != error) {
			return error;
		}
		error = diskFileAccessor->deleteDiskFile(fileName);
		if (SUCCESS != error) {
			return error;
		}
		cout << testName << ":" << methodName << "=SUCCESS" << endl;
		return SUCCESS;
	}


};
