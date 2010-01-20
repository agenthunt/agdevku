/*
 * CatalogUtilTest.cpp
 *
 *  Created on: 26-Nov-2009
 *      Author: shailesh
 */

/*
 * TestHeapFile.cpp
 *
 *  Created on: 06-Nov-2009
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
using namespace std;
class TestCatalogUtil {

public:
	string testName;
	BufferManager *bufferManager;
	int error;
	TestCatalogUtil() {
		string methodName = "constructor";
		testName = "TestCatalogUtil";
		bufferManager = BufferManager::getInstance();
		error = bufferManager->createDatabase("cat_test", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
		}
		bufferManager->openDatabase("cat_test");
	}

	~TestCatalogUtil() {
		bufferManager->closeDatabase();
	}

	STATUS_CODE addInitialRecordTest() {
		string methodName = "addInitialRecordTest";
		CatalogUtil catalogUtil;
		catalogUtil.addInitialSysTableRecord();

		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<"error code="<<error<< endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

	STATUS_CODE getSchemaTest() {
		string methodName = "getSchemaTest";
		CatalogUtil catalogUtil;
		Schema sysTableSchema;
		catalogUtil.getSchema("systable", sysTableSchema);
		DEBUG(sysTableSchema.tableName);
		for (unsigned i = 0; i < sysTableSchema.fieldTypes.size(); i++) {
			DEBUG(sysTableSchema.fieldTypes.at(i)<<","<<sysTableSchema.columnNames.at(i)<<","<<sysTableSchema.fieldLengths.at(i));

		}

		Schema sysColSchema;
		catalogUtil.getSchema("syscol", sysColSchema);
		DEBUG(sysColSchema.tableName);
		for (unsigned i = 0; i < sysColSchema.fieldTypes.size(); i++) {
			DEBUG(sysColSchema.fieldTypes.at(i)<<","<<sysColSchema.columnNames.at(i)<<","<<sysColSchema.fieldLengths.at(i));

		}

		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<"error code="<<error<< endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

};
