/*
 * FreePageManager.cpp
 *
 *  Created on: Oct 10, 2009
 *      Author: shailesh
 */
#include <string.h>
#include "FreePageManager.h"
#include "../global/StatusCodes.h"
FreePageManager::FreePageManager(char *pageData) {
	// TODO Auto-generated constructor stub
	pageData_ = pageData;

}

FreePageManager::~FreePageManager() {
	// TODO Auto-generated destructor stub
}

int FreePageManager::getFreePageNumber() {
	getContinuousFreePageNumbers(1);
}

int FreePageManager::getContinuousFreePageNumbers(int howMany) {

}

STATUS_CODE FreePageManager::createFreePageManagerPage(int pageSize) {
	memset(pageData_,0,pageSize);
	freePageHeader_.isPageFull = FALSE;
	freePageHeader_.totalNumberOfPages = 2;
	/*initially only db main header page and free page are allocated
	 * therefore the value 00000011 = 0x3 is stored
	 */
	pageData_[32] = 0x3;
	return SUCCESS;
}
