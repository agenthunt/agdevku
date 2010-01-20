/*
 * LeafNode.h
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#ifndef LEAFNODE_H_
#define LEAFNODE_H_
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"
#include "IndexHeaderPage.h"
#include<vector>
#include<string.h>
#include<string>
using namespace std;
class LeafNode {
public:
	LeafNode();
	LeafNode(int leafPageNumber);
	int getIndexHeaderPageNumber();
	LeafNode(IndexHeaderPage *indexHeaderPage, int leafPageNumber);
	virtual ~LeafNode();
	STATUS_CODE createLeafPage(IndexHeaderPage *indexHeaderPage_);
	STATUS_CODE insertIntoLeafPage(const char* key, RIDStruct rid);
	int searchInLeafNode(const char* key);
	int searchInLeafNode(const char* key, RIDStruct rid);
	STATUS_CODE deleteFromLeafPage(const char* key, RIDStruct &rid,
			std::vector<int> &deletedPages);
	STATUS_CODE redistributeLeafPage(int pagenumber);
	int getLevelOfNode();
	void setLevelOfNode(int level);
	int getParentPageNumber();
	void setParentPageNumber(int parentPageNumber);
	int getNoOfRecordsInNode();
	void setNoOfRecordsInNode(int noOfRecords);
	int getRightPageNumber();
	void setRightPageNumber(int newRightPageNumber);
	int getLeftPageNumber();
	int ridsCompare(RIDStruct rid1, RIDStruct rid2);
	void setLeftPageNumber(int newLeftPageNumber);
	static int getLeafNodeHeaderSize();
	int getPageNumber();
	STATUS_CODE getFirstKey(char* firstKey, RIDStruct &rid);
	STATUS_CODE getLastKey(char* lastKey, RIDStruct &rid);
	STATUS_CODE getFirstKey(char* firstKey);
	STATUS_CODE searchKeyInLeafNode(const char* key,
			std::vector<RIDStruct> &RIDVector);
	//To find key at keynumber location
	void getKey(int keyNumber, char* tempKey);
	void getKeyAndRID(int keyNumber, char*tempKey, RIDStruct &rid);
	int dummyKeyCompare(char* key1, char* key2);
	void getParentKey(char* key);
	void UILeafNode();
	void display();
	void UIDisplayOfLeafNode(int numOfTabs);
	void DisplayKeyAt(int keyPosition);
	int searchKeyInLeafNodeWithOp(const char* key, int op, std::vector<
			RIDStruct> &RIDVector, std::vector<string> &keyVector);
	std::string toString();
	int searchFirstKeyToSecondKey(const char* firstKey,const char* secondKey,std::vector<RIDStruct> &RIDVector,
			std::vector<string> &keyVector);
	void getKeysAndRIDs(std::vector<RIDStruct> &RIDVector,std::vector<string> &keyVector);
	static int testPinCount;
	bool indexHeaderObjCreatedHere;
private:
	typedef struct LeafNodePageHeaderStructure {
		GeneralPageHeaderStruct generalPageHeader;
		int leftPageNumber;
		int rightPageNumber;
		int level;
		int noOfRecords;
		int parentPageNumber;
		int indexHeaderPageNumber;
	} LeafNodePageHeaderStruct;
	LeafNodePageHeaderStruct leafNodePageHeader_;
	IndexHeaderPage *indexHeader_;
	char* pageData_;
};

#endif /* LEAFNODE_H_ */
