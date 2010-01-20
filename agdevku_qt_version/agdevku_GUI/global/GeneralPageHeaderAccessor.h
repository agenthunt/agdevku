/*
 * GeneralPageHeaderAccessor.h
 *
 *  Created on: 20-Oct-2009
 *      Author: shailesh
 */

#ifndef GENERALPAGEHEADERACCESSOR_H_
#define GENERALPAGEHEADERACCESSOR_H_
#include <string>
#include "GlobalStructures.h"
class GeneralPageHeaderAccessor {
public:
	GeneralPageHeaderAccessor(char *pageData);
	virtual ~GeneralPageHeaderAccessor();
	int getPageNumber();
	int getPrevPageNumber();
	int getNextPageNumber();
	void setPageNumber(int pageNumber);
	void setPrevPageNumber(int prevPageNumber);
	void setNextPageNumber(int nextPageNumber);
	int getPageType();
	void setPageType(int pageType);
	std::string toString();
private:
	GeneralPageHeaderStruct generalPageHeader_;
	char *pageData_;
};

#endif /* GENERALPAGEHEADERACCESSOR_H_ */
