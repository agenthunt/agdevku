/*
 * TestFreePageManager.cpp
 *
 *  Created on: 18-Oct-2009
 *      Author: shailesh
 */

#include "../diskmgr/FreePageManager.h"

#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include "../utils/debug.h"
#include <iostream>
#include <string>
using namespace std;
class TestFreePageManager {

public:
	string testName;
	TestFreePageManager() {
		testName = "TestFreePageManager";
	}

	STATUS_CODE isFreePageTest() {
		string methodName = "isFreePageTest";
		char pageData[DEFAULT_PAGE_SIZE];
		FreePageManager freePageManager(pageData);
		freePageManager.createFreePageManagerPage(DEFAULT_PAGE_SIZE, pageData,
				1);
		DEBUG("byte 32="<<(int)pageData[32]);
		DEBUG("byte 32 = "<<(int)pageData[32 + 0/8]);
		char mask = 1;
		DEBUG("masking="<<(pageData[32 + 0/8]&mask));
		bool result = freePageManager.isPageFree(0);
		if (result == true) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		result = freePageManager.isPageFree(5);
		if (result == false) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		cout << testName << ":" << methodName << "=SUCCESS" << endl;
		return SUCCESS;
	}

	STATUS_CODE basicTest() {
		string methodName = "basicTest";
		char pageData[DEFAULT_PAGE_SIZE];
		FreePageManager freePageManager(pageData);

		freePageManager.createFreePageManagerPage(DEFAULT_PAGE_SIZE, pageData,
				1);
		DEBUG("byte 32="<<(int)pageData[32]);
		DEBUG("byte 32 = "<<(int)pageData[32 + 0/8]);
		char mask = 1;
		DEBUG("masking="<<(pageData[32 + 0/8]&mask));
		bool actual = freePageManager.isPageFree(0);
		bool expected = false;
		if (actual != expected) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		actual = freePageManager.isPageFree(5);
		expected = true;
		if (actual != expected) {
			DEBUG( testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		freePageManager.setPage(5);
		actual = freePageManager.isPageFree(5);
		expected = false;
		if (actual != expected) {
			DEBUG(testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		freePageManager.freePage(5);
		actual = freePageManager.isPageFree(5);
		expected = true;
		if (actual != expected) {
			DEBUG( testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}


		freePageManager.setPages(10, 14);
		for (int i = 10; i <= 14; i++) {
			actual = freePageManager.isPageFree(i);
			expected = false;
			if (actual != expected) {
				DEBUG( testName << ":" << methodName << "=FAILURE" << endl);
				return FAILURE;
			}
		}

		actual = freePageManager.isPageFree(15);
		expected = true;
		if (actual != expected) {
			DEBUG( testName << ":" << methodName << "=FAILURE" << endl);
			return FAILURE;
		}

		DEBUG(testName << ":" << methodName << "=SUCCESS" << endl);
		return SUCCESS;
	}

};
