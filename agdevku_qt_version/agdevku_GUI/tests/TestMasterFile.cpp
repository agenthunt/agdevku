/*
 * TestMasterFile.cpp
 *
 *  Created on: 27-Nov-2009
 *      Author: shailesh
 */

#include "../bufmgr/BufferManager.h"
#include "../utils/debug.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include "../catalog/CatalogUtil.h"

#include <iostream>
#include <string>
#include <assert.h>
#include <string.h>
#include "../utils/ErrorLookupTable.h"
using namespace std;
class TestMasterFile {

public:
	string testName;
	string methodName;
	BufferManager *bufferManager;
	int error;
	TestMasterFile() {
		methodName = "constructor";
		testName = "TestMasterFile";
		bufferManager = BufferManager::getInstance();

	}

	~TestMasterFile() {

	}

	STATUS_CODE insertMasterFile() {
		methodName = "insertMasterFile";
		error = bufferManager->createDatabase("new_1", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			DEBUG("error message"<<ErrorLookupTable::lookup(error));
		}

		error = bufferManager->createDatabase("new_2", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			DEBUG("error message"<<ErrorLookupTable::lookup(error));
		}
		return error;
	}

	STATUS_CODE deleteFromMasterFile() {
		methodName = "deleteFromMasterFile";
		error = bufferManager->dropDatabase("new_1");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			DEBUG("error message"<<ErrorLookupTable::lookup(error));
		}
		error = bufferManager->dropDatabase("new_2");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			DEBUG("error message"<<ErrorLookupTable::lookup(error));
		}
		return error;
	}

};
