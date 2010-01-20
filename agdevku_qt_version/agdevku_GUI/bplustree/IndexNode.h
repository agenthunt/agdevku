/*
 * IndexNode.h
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#ifndef INDEXNODE_H_
#define INDEXNODE_H_
#include "../global/StatusCodes.h"
#include "../utils/debug.h"
#include "../global/GlobalStructures.h"
#include "IndexHeaderPage.h"
#include<string>
using namespace std;
class IndexNode {
private:
	int pagenumber;
	typedef struct IndexNodePage {
		GeneralPageHeaderStruct generalPageHeaderStruct;
		int leftPageNumber;
		int rightPageNumber;
		int noOfKeys;
		int level;
		int parentPageNumber;
		int indexHeaderPageNumber;
	} IndexNodePageHeaderStruct;
	IndexNodePageHeaderStruct indexNodePageHeader_;
	IndexHeaderPage *indexHeader_;
	char* pageData_;
	static int debugCount_;
public:
	IndexNode();
	IndexNode(int indexPageNumber);
	int getIndexHeaderPageNumber();
	IndexNode(IndexHeaderPage *indexHeaderPage, int indexPageNumber);

	STATUS_CODE createIndexNode(IndexHeaderPage *indexHeaderPage_);
	STATUS_CODE insertIntoIndexNode(const char* key, int leftPageNumber,
			int rightPageNumber);
	STATUS_CODE insertAtTheEndOfIndexNode(const char* key, int rightPagenumber);
	STATUS_CODE
			insertAtTheFrontOfIndexNode(const char* key, int leftPageNumber);
	//STATUS_CODE inserIntoIndexNode(int pagenumber,char* key,char*data);
	//STATUS_CODE deletefromIndexNode(int pagenumber,char* key);
	//	STATUS_CODE reorganizeIndexNode(int pagenumber);
	int searchInIndexNode(const char* key);
	int foundInIndexNode(char* key);
	void replaceIfKeyFound(char* oldKey, char* newKey);
	int getLevelOfIndexNode();
	void setLevelOfIndexNode(int level);
	int dummyKeyCompare(char* key1, char* key2);
	//int searchInIndexPage(int key,IndexNodePageStruct indexNodeStruct);
	//int findChildPage(int rootPageNumber,int key,char* pageData);
	int getParentOfIndexNode();
	void setParentOfIndexNode(int parentPageNumber);
	int getNoOfKeysInIndexNode();
	void setNoOfKeysInIndexNode(int noOfKeys);
	int getLeftPageNumberOfIndexNode();
	void setLeftPageNumberOfIndexNode(int leftPageNumber);
	int getRightPageNumber();
	void setRightPageNumber(int rightPageNumber);
	int getPageNumber();
	int isKeyFound(char* key);
	STATUS_CODE replaceKey(const char* oldKey, const char* newKey);
	STATUS_CODE replaceKeyPreviousToPageNumber(int pageNumber,
			const char *oldKey, const char *newKey);
	/* finding size of Index Node*/
	static int getIndexNodeHeaderSize();
	void display();
	void indexNodeOnlyDisplay();
	void getKeyNextToPageNumber(int pageNumber, char* key);
	void getKeyPreviousToPageNumber(int pageNumber, char* key);
	int getPageNumberNextToGivenPageNumber(int pageNumber);
	int getPageNumberPreviousToGivenPageNumber(int pageNumber);
	STATUS_CODE
			getSiblings(int pageNumber, int &leftSibling, int &rightSibling);
	STATUS_CODE deleteKeyFromIndexNode(const char* key, bool flag, std::vector<
			int> &deletedPages);
	void deleteFirstKeyFromIndexNode();
	void deleteLastKeyFromIndexNode();
	//	STATUS_CODE deleteKeyFromIndexNode(int pageNumber);
	int getFirstPageNumberPresentInIndexNode();
	int getLastPageNumberPresentInIndexNode();
	void getFirstKeyPresentInIndexNode(char* key);
	void getLastKeyPresentInIndexNode(char* key);
	void getSeperatingKey(int leftPage, int rightPage, char* key);
	void UIIndexNode();
	void UIDisplayOfIndexNode(int numberOfTabs);
	void deleteChilds(std::vector<int> &allPageNumbers);
	void findPages(std::vector<int> &leafPages, std::vector<int> &indexPages);
	std::string toString();
	void findFirstLeafPage(std::vector<int> &leafPages);
	bool indexHeaderObjCreatedHere;
	virtual ~IndexNode();

};

#endif /* INDEXNODE_H_ */
