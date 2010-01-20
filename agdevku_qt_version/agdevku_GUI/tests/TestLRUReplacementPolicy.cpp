/*
 * TestLRUReplacementPolicy.cpp
 *
 *  Created on: 26-Oct-2009
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
class TestLRUReplacementPolicy {

public:
	string testName;
	TestLRUReplacementPolicy() {
		testName = "TestLRUReplacementPolicy";
	}

	STATUS_CODE basicTest() {
		string methodName = "basicTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->initializeBuffer(0.02, DEFAULT_PAGE_SIZE);
		error = bufferManager->createDatabase("first", 20);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		bufferManager->openDatabase("first");
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}
		//		for (int i = 0; i < bufferManager->numOfFrames_; ++i) {
		//			DEBUG("priority of frame["<<i<<"]"<<bufferManager->bufferPool_[i]->priority_);
		//		}
		char *pageData;
		bufferManager->pinAndGetPage(0, pageData);
		bufferManager->unPinPage(0, false);
		bufferManager->pinAndGetPage(1, pageData);
		bufferManager->unPinPage(1, false);
		bufferManager->pinAndGetPage(1, pageData);
		bufferManager->unPinPage(1, false);
		bufferManager->pinAndGetPage(1, pageData);
		bufferManager->unPinPage(1, false);
		for (int i = 0; i < bufferManager->numOfFrames_; ++i) {
			DEBUG("priority of frame["<<i<<"],priority"<<bufferManager->bufferPool_[i]->priority_);
		}
		bufferManager->pinAndGetPage(4, pageData);
		bufferManager->unPinPage(1, false);
		for (int i = 0; i < bufferManager->numOfFrames_; ++i) {
			DEBUG("priority of frame["<<i<<"],priority"<<bufferManager->bufferPool_[i]->priority_);
		}
		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}
};
