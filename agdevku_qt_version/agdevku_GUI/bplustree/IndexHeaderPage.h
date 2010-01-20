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
#include<vector>
#include<string>
class IndexHeaderPage {
public:
	IndexHeaderPage();
	IndexHeaderPage(int indexHeaderPageNumber);
	virtual ~IndexHeaderPage();

	/**
	 *This will create New header page and initialize,
	 * it with 1.NUMBER OF COUMNS 2.COLUMN TYPES(ARRAY) 3.COLUMN SIZES(ARRAY) 4.TOTAL KEY SIZE
	 */
	STATUS_CODE createIndexHeaderPage(int numOfColumns, int colTypes[],
			int colSizes[], int keySize);//FOR HEAP INITIAL CREATE INDEX

	/**
	 *
	 */
	int getRootPage();
	int getFanout();
	int getMaxNoOfRecordsInLeaf();
	void updateMaxNoOfRecordsInLeaf(int maxNoOfRecordsInLeaf);
	int getHeightOfTheTree();
	void setRootPage(int pageNumber);
	void updateHeightOfTheTree(int height);
	int getPageNumber();
	void setFanout(int fanout);
	int getNumOfColumns();
	int getLeafPageSize();
	int getIndexPageSize();
	int getKeySize();
	int getNumberOfKeys();
	void setNumberOfKeys(int numberOfKeys);
	void UIIndexHeaderPage();
	void pushLeafPageNumbers(int pageNumber);
	void pushIndexPageNumbers(int pageNumber);
	int popLeafPageNumbers(int i);
	int popIndexPageNumbers(int i);
	int leafPageNumbersSize();
	int indexPageNumbersSize();
	std::string toString();
	int *columnTypes_;
	int *columnSizes_;
private:
	void initColInfo();
	typedef struct IndexHeaderStructure {
		GeneralPageHeaderStruct generalPageHeaderStruct;
		int rootPageNumber;
		int fanout;
		int heightOfTheTree;
		int maxNoOfRecordsInLeaf;
		int keySize;
		int numberOfKeys;
		int noOfcolumns;
		//		std::vector<int> indexPageNumbers;
		//		std::vector<int> leafPageNumbers;
	} IndexHeaderStruct;
	IndexHeaderStruct indexHeader_;
	char *pageData_;
};

#endif /* INDEXHEADERPAGE_H_ */
