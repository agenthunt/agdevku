/*
 * GeneralPageHeaderAccessor.cpp
 *
 *  Created on: 20-Oct-2009
 *      Author: shailesh
 */

#include "GeneralPageHeaderAccessor.h"
#include "../utils/PageTypeLookup.h"
#include <string.h>
#include <sstream>
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

int GeneralPageHeaderAccessor::getPageType() {
	return generalPageHeader_.pageType;
}

void GeneralPageHeaderAccessor::setPageType(int pageType) {
	generalPageHeader_.pageType = pageType;
	memcpy(pageData_, &generalPageHeader_, sizeof(GeneralPageHeaderStruct));
}

std::string GeneralPageHeaderAccessor::toString() {
	std::ostringstream mystream;
	mystream << "PAGE NUMBER:  " << getPageNumber() << "\n"
			<< "NEXT PAGE NUMBER:  " << getNextPageNumber() << "\n"
			<< "PAGE TYPE:  " << PageTypeLookup::lookup(getPageType()) << "\n";
	return mystream.str();
}
