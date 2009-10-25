/*
 * IndexHeaderPage.h
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#ifndef INDEXHEADERPAGE_H_
#define INDEXHEADERPAGE_H_
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"

class IndexHeaderPage {
public:
	IndexHeaderPage();
	virtual ~IndexHeaderPage();
	/**
	 *
	 */
	STATUS_CODE createIndexHeaderPage(int &pageNumber);
	/**
	 *
	 */
	STATUS_CODE initializeHeaderPage(int &pagenumber);
	int getRootPage();
	int getFanout();
	int getMaxNoOfRecordsInLeaf();

private:
	int pagenumber;
	typedef struct IndexHeaderStructure {
		GeneralPageHeaderStruct generalPageHeaderStruct;
		int keyType;
		int size;
		int rootPageNumber;
		int fanout;
		int heightOfTheTree;
		int MaxNoOfRecordsInLeaf;
	} IndexHeaderStruct;

};

#endif /* INDEXHEADERPAGE_H_ */
