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
#include "../diskmgr/DBMainHeaderPage.h"
#include "../heap/DirectoryHeaderPage.h"
#include "../heap/HeapFile.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <string.h>
using namespace std;
class TestHeapFile {

public:
	string testName;
	BufferManager *bufferManager;
	int error;
	TestHeapFile() {
		string methodName = "constructor";
		testName = "TestHeapFile";
		bufferManager = BufferManager::getInstance();
		error = bufferManager->createDatabase("heap_test", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
		}
		bufferManager->openDatabase("heap_test");
	}

	~TestHeapFile() {
		bufferManager->closeDatabase();
	}

	STATUS_CODE insertRecordTest() {
		string methodName = "insertRecordTest";

		DirectoryHeaderPage dirHeaderPage;
		dirHeaderPage.createDirectoryHeaderPage();
		HeapFile heapFile(dirHeaderPage.getPageNumber());
		char *record = new char[20];
		record = "hi how r u";
		RIDStruct rid;
		error = heapFile.insertRecord(record, 20, rid);
		DEBUG("rid={"<<rid.pageNumber<<","<<rid.slotNumber<<"}");
		heapFile.deleteRecord(rid);

		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<"error code="<<error<< endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}


	STATUS_CODE updateRecordTest() {
		string methodName = "updateRecordTest";

		DirectoryHeaderPage dirHeaderPage;
		dirHeaderPage.createDirectoryHeaderPage();
		HeapFile heapFile(dirHeaderPage.getPageNumber());
		char *record = new char[10];
		record = "hi how r u";
		RIDStruct rid;
		error = heapFile.insertRecord(record, 10, rid);
		DEBUG("rid={"<<rid.pageNumber<<","<<rid.slotNumber<<"}");
		char *newRec = new char[20];
		newRec = "whatishappeningdude?";
		unsigned size = 20;
		heapFile.updateRecord(rid,newRec,size);

		HeapFile sysHeapFile(0);
		const RIDStruct sysRID = {6,0};
		char *sysRec;
		unsigned sysLen;
		sysHeapFile.getRecord(sysRID,sysRec,sysLen);
		DEBUG("SYSREC"<<(int)(*sysRec));
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<"error code="<<error<< endl);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

};
