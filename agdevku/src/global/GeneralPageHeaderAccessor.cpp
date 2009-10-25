/*
 * GeneralPageHeaderAccessor.cpp
 *
 *  Created on: 20-Oct-2009
 *      Author: shailesh
 */

#include "GeneralPageHeaderAccessor.h"
#include <string.h>
GeneralPageHeaderAccessor::GeneralPageHeaderAccessor(char *pageData) {
	// TODO Auto-generated constructor stub
	pageData_ = pageData;
	memcpy(&generalPageHeader_, pageData, sizeof(GeneralPageHeaderStruct));
}

GeneralPageHeaderAccessor::~GeneralPageHeaderAccessor() {
	// TODO Auto-generated destructor stub
}

int GeneralPageHeaderAccessor::getPageNumber() {
	return generalPageHeader_.pageNumber;
}

int GeneralPageHeaderAccessor::getNextPageNumber() {
	return generalPageHeader_.nextPageNumber;
}

void GeneralPageHeaderAccessor::setPageNumber(int pageNumber) {
	generalPageHeader_.pageNumber = pageNumber;
	memcpy(pageData_, &generalPageHeader_, sizeof(GeneralPageHeaderStruct));
}

void GeneralPageHeaderAccessor::setNextPageNumber(int nextPageNumber) {
	generalPageHeader_.nextPageNumber = nextPageNumber;
	memcpy(pageData_, &generalPageHeader_, sizeof(GeneralPageHeaderStruct));
}
