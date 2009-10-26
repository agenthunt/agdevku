/*
 * FreePageManager.cpp
 *
 *  Created on: Oct 10, 2009
 *      Author: shailesh
 */
#include <string.h>
#include "FreePageManager.h"
#include "../global/StatusCodes.h"
#include "../utils/debug.h"
FreePageManager::FreePageManager() {
	pageData_ = NULL;
}

FreePageManager::FreePageManager(char *pageData) {
	// TODO Auto-generated constructor stub
	pageData_ = pageData;

}

FreePageManager::~FreePageManager() {
	// TODO Auto-generated destructor stub
}

int FreePageManager::getFreePageNumber() {
	return getContinuousFreePageNumbers(1);
}

int FreePageManager::getContinuousFreePageNumbers(int howMany) {
	if (freePageHeader_.isPageFull == FALSE) {
		int freePageCount = 0;
		int startPageNumber = 0;
		for (int i = 0; i < freePageHeader_.maxNumberOfPages; i++) {
			if (isPageFree(i) == true) {
				startPageNumber = i;
				freePageCount++;
			} else {
				freePageCount = 0;
			}
			if (freePageCount == howMany) {
				setPages(startPageNumber, startPageNumber + howMany - 1);
				return startPageNumber;
			}
		}
	}
	return -1;
}

bool FreePageManager::isPageFree(int pageNumber) {
	char mask = 1;
	mask = mask << (pageNumber % 8);//left shift (pageNumber%8) times to set the bit
	DEBUG("mask="<< (int)mask);
	if ((pageData_[freePageHeader_.headerOffset + pageNumber / 8] & mask) == 0) {
		return true;
	}
	return false;
}

void FreePageManager::setPage(int pageNumber) {
	char whichBit = 1;
	whichBit = whichBit << (pageNumber % 8);//left shift (pageNumber%8) times to set the bit
	int whichByte = freePageHeader_.headerOffset + pageNumber / 8;
	pageData_[whichByte] = pageData_[whichByte] | whichBit;
}

void FreePageManager::setPages(int from, int to) {
	for (int i = from; i <= to; i++) {
		setPage(i);
	}
}

void FreePageManager::freePage(int pageNumber) {
	char whichBit = 1;
	whichBit = whichBit << (pageNumber % 8);//left shift (pageNumber%8) times to set the bit
	int whichByte = freePageHeader_.headerOffset + pageNumber / 8;
	pageData_[whichByte] = pageData_[whichByte] & (~whichBit);
}

void FreePageManager::freePages(int from, int to) {
	for (int i = from; i <= to; i++) {
		freePage(i);
	}
}

STATUS_CODE FreePageManager::createFreePageManagerPage(int pageSize,
		char *pageData, int pageNumber) {
	memset(pageData, 0, pageSize);
	freePageHeader_.pageNumber = pageNumber;
	freePageHeader_.isPageFull = FALSE;
	freePageHeader_.totalNumberOfPages = 2;
	freePageHeader_.nextPageNumber = INVALID_PAGE;
	freePageHeader_.maxNumberOfPages = pageSize - (128 * 8);
	freePageHeader_.headerOffset = 32;
	/*initially only db main header page, free page
	 * systable header page and syscol header page are allocated
	 * therefore the value 00001111 = 0xF is stored
	 */
	pageData[32] = 0xF;
	return SUCCESS;
}



