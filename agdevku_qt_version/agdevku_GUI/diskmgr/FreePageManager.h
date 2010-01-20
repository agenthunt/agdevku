/*
 * FreePageManager.h
 *
 *  Created on: Oct 10, 2009
 *      Author: shailesh
 */

#ifndef FREEPAGEMANAGER_H_
#define FREEPAGEMANAGER_H_
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"
class FreePageManager {
public:
	FreePageManager();
	FreePageManager(char *pageData);
	virtual ~FreePageManager();
	/**
	 * ok I know that the name is little funny, but it makes sense
	 * This method is called when DB is created. It creates a template
	 * for the free page manager which is written to page number 1.
	 */

	STATUS_CODE createFreePageManagerPage(int pageSize, char *pageData,
			int pageNumber);
	/**
	 *
	 */
	int getFreePageNumber();
	/**
	 * checks if "howMany" number of pages are contiguous are available
	 * and returns the first page number of that.
	 */
	int getContinuousFreePageNumbers(int howMany = 1);
	//making it public to test, ideally should be private
	bool isPageFree(int pageNumber);
	void setPage(int pageNumber);
	void setPages(int from,int to);//inclusive of both from and to
	void freePage(int pageNumber);
	void freePages(int from,int to);
private:

	char *pageData_;
	typedef struct FreePageHeaderStructure {
		int pageNumber;
		int isPageFull;//TRUE indicated full
		int totalNumberOfPages;//totalNumber of pages currently allocated
		int maxNumberOfPages;
		int nextPageNumber;
		int headerOffset;
		int dummy[26];
	} FreePageHeaderStruct;

	FreePageHeaderStruct freePageHeader_;
};

#endif /* FREEPAGEMANAGER_H_ */
