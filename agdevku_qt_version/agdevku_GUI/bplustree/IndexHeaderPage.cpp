/*
 * IndexHeaderPage.cpp
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#include "IndexHeaderPage.h"
#include "IndexNode.h"
#include "LeafNode.h"
#include "../bufmgr/BufferManager.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../utils/debug.h"
#include "../global/StatusCodes.h"
#include "../bufmgr/BufferManager.h"
#include "../global/GlobalDefines.h"
#include <string.h>
#include <iostream>
#include <math.h>
using namespace std;
IndexHeaderPage::IndexHeaderPage() {
	// TODO Auto-generated constructor stub

}

IndexHeaderPage::IndexHeaderPage(int indexHeaderPageNumber) {
	DEBUG_B("Opening index header page with page number : "<<indexHeaderPageNumber)
	// TODO Auto-generated constructor stub
	BufferManager *bufMgr = BufferManager::getInstance();
	int error = bufMgr->pinAndGetPage(indexHeaderPageNumber, pageData_);
	//	DEBUG_B("Pinning index header page with buffer manager : "<<indexHeaderPageNumber)
	//	DEBUG_B("pin page error="<<error<<"  "<<(int)pageData_)

	//	DEBUG_B("PAGE IN INDEX HEADER in BPLUS TREE "<<(int)pageData_)
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	//	DEBUG_B("in constructor"<<indexHeader_.noOfcolumns);
	initColInfo();
}

IndexHeaderPage::~IndexHeaderPage() {
	// TODO Auto-generated destructor stub
	//	DEBUG_B("In index header page destructor")
	BufferManager *bufMgr = BufferManager::getInstance();
	//	indexHeader_.indexPageNumbers.clear();
	//	indexHeader_.leafPageNumbers.clear();
	//	DEBUG_B("I AM HERE in desctructor : "<<getPageNumber())
	//	DEBUG_B("Unpinning index header page : "<<indexHeader_.generalPageHeaderStruct.pageNumber)
	bufMgr->unPinPage(indexHeader_.generalPageHeaderStruct.pageNumber, true);
	//	DEBUG_B("I AM HERE in desctructor end ")
	//	delete[] columnTypes_;
	//	delete[] columnSizes_;
}

STATUS_CODE IndexHeaderPage::createIndexHeaderPage(int numOfColumns,
		int colTypes[], int colSizes[], int keySize) {

	BufferManager *bufMgr = BufferManager::getInstance();
	int newPageNum;
	bufMgr->newPage(newPageNum, pageData_);
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	int currentlyUsingPageSize = bufMgr->getCurrentlyUsingPageSize();
	indexHeader_.generalPageHeaderStruct.pageType = TYPE_INDEX_HEADER_PAGE;
	indexHeader_.keySize = keySize;
	indexHeader_.maxNoOfRecordsInLeaf = (int) floor((currentlyUsingPageSize
			- (LeafNode::getLeafNodeHeaderSize())) / (keySize + RID_LEN));
	indexHeader_.fanout = (int) floor((currentlyUsingPageSize
			- IndexNode::getIndexNodeHeaderSize()) / (keySize + 4));
	indexHeader_.rootPageNumber = -1;
	indexHeader_.heightOfTheTree = 0;
	indexHeader_.numberOfKeys = 0;
	indexHeader_.noOfcolumns = numOfColumns;
	//	indexHeader_.leafPageNumbers.clear();
	//	indexHeader_.indexPageNumbers.clear();
	memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
	int offset = sizeof(IndexHeaderStruct);
	//	DEBUG_B("OFFSET "<<offset)
	//colTypesSizeData contain information about typesOfcolmns,and their sizes
	for (int i = 0; i < numOfColumns; i++) {
		memcpy(&pageData_[offset], &colTypes[i], sizeof(int));
		offset = offset + sizeof(int);
	}
	for (int i = 0; i < numOfColumns; i++) {
		memcpy(&pageData_[offset], &colSizes[i], sizeof(int));
		offset = offset + sizeof(int);
	}
	//	DEBUG_B("OFFSET "<<offset)
	//	DEBUG_B("FAN OUT"<<indexHeader_.fanout);
	//	DEBUG_B("LEAF FAN OUT"<<indexHeader_.maxNoOfRecordsInLeaf);
	initColInfo();
	return SUCCESS;
}

void IndexHeaderPage::initColInfo() {
	int offset = sizeof(IndexHeaderStruct);
	columnTypes_ = new int[indexHeader_.noOfcolumns];
	//	DEBUG_B("NUMBER OF COLUMNS "<<indexHeader_.noOfcolumns)
	//	DEBUG_B("OFFSET "<<offset)
	for (int i = 0; i < indexHeader_.noOfcolumns; i++) {
		memcpy(&columnTypes_[i], &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
	}
	//	DEBUG_B("I AM HERE")
	columnSizes_ = new int[indexHeader_.noOfcolumns];
	for (int i = 0; i < indexHeader_.noOfcolumns; i++) {
		memcpy(&columnSizes_[i], &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
	}
	//	indexPageNumbers.clear();
	//	leafPageNumbers.clear();
}

int IndexHeaderPage::getMaxNoOfRecordsInLeaf() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.maxNoOfRecordsInLeaf;
}

void IndexHeaderPage::updateMaxNoOfRecordsInLeaf(int maxNoOfRecordsInLeaf) {
	indexHeader_.maxNoOfRecordsInLeaf = maxNoOfRecordsInLeaf;
	memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
}
int IndexHeaderPage::getRootPage() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.rootPageNumber;
}
int IndexHeaderPage::getFanout() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.fanout;
}
int IndexHeaderPage::getHeightOfTheTree() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.heightOfTheTree;
}
int IndexHeaderPage::getNumberOfKeys() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.numberOfKeys;
}
void IndexHeaderPage::setNumberOfKeys(int numberOfKeys) {
	indexHeader_.numberOfKeys = numberOfKeys;
	memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
}
void IndexHeaderPage::updateHeightOfTheTree(int newHeight) {
	indexHeader_.heightOfTheTree = newHeight;
	memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
}

void IndexHeaderPage::setRootPage(int pageNumber) {
	indexHeader_.rootPageNumber = pageNumber;
	memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
}

int IndexHeaderPage::getPageNumber() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.generalPageHeaderStruct.pageNumber;
}
void IndexHeaderPage::setFanout(int newfanout) {
	indexHeader_.fanout = newfanout;
	memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
}

int IndexHeaderPage::getNumOfColumns() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.noOfcolumns;
}

int IndexHeaderPage::getKeySize() {
	memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
	return indexHeader_.keySize;
}
/*
 void IndexHeaderPage::pushLeafPageNumbers(int pageNumber) {
 indexHeader_.leafPageNumbers.push_back(pageNumber);
 memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
 }
 void IndexHeaderPage::pushIndexPageNumbers(int pageNumber) {
 indexHeader_.indexPageNumbers.push_back(pageNumber);
 memcpy(pageData_, &indexHeader_, sizeof(IndexHeaderStruct));
 }
 int IndexHeaderPage::popLeafPageNumbers(int i) {
 memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
 int pageNumber;
 if (!indexHeader_.leafPageNumbers.empty()) {
 pageNumber = indexHeader_.leafPageNumbers.at(i);
 //		indexHeader_.leafPageNumbers.pop_back();
 } else {
 pageNumber = -1;
 }
 return pageNumber;
 }
 int IndexHeaderPage::popIndexPageNumbers(int i) {
 memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
 int pageNumber;
 if (!indexHeader_.indexPageNumbers.empty()) {
 pageNumber = indexHeader_.indexPageNumbers.at(i);
 //		indexHeader_.indexPageNumbers.pop_back();
 } else {
 pageNumber = -1;
 }
 return pageNumber;
 }
 int IndexHeaderPage::leafPageNumbersSize() {
 memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
 return indexHeader_.leafPageNumbers.size();
 }
 int IndexHeaderPage::indexPageNumbersSize() {
 memcpy(&indexHeader_, pageData_, sizeof(IndexHeaderStruct));
 return indexHeader_.indexPageNumbers.size();
 }
 */
void IndexHeaderPage::UIIndexHeaderPage() {
	cout
			<< "\n\t********************-INDEX HEADER PAGE-***********************"
			<< endl;
	cout << "\tINDEX HEADER PAGE NUMBER------------:" << getPageNumber()
			<< endl;
	cout << "\tRoot page number--------------------:" << getRootPage() << endl;
	cout << "\tIndexNode fanout--------------------:" << getFanout() << endl;
	cout << "\tMaximum records in leaf page--------:"
			<< getMaxNoOfRecordsInLeaf() << endl;
	cout << "\tHeight of tree----------------------:" << getHeightOfTheTree()
			<< endl;
	cout << "\tNumber of columns in Index----------:" << getNumOfColumns()
			<< endl;
	cout << "\tKey size----------------------------:" << getKeySize() << endl;
	cout << "\tNumber of Keys----------------------:" << getNumberOfKeys()
			<< endl;
	for (int i = 0; i < getNumOfColumns(); i++) {
		char columnT[10];
		memset(columnT, '\0', 10);
		switch (columnTypes_[i]) {
		case 1:
			memcpy(columnT, "INTEGER", sizeof("INTEGER"));
			break;
		case 2:
			memcpy(columnT, "FLOAT", sizeof("FLOAT"));
			break;
		case 3:
			memcpy(columnT, "DOUBLE", sizeof("DOUBLE"));
			break;
		case 4:
			memcpy(columnT, "STRING", sizeof("STRING"));
			break;
		case 5:
			memcpy(columnT, "LONG", sizeof("LONG"));
			break;
		}
		cout << "\tColumn Types [field " << i + 1 << "]--------------:"
				<< columnT << endl;
	}
	/*	cout << "\tNumber of Leaf Pages----------------:" << leafPageNumbersSize()
	 << endl;
	 cout << "\tNumber of Index Pages---------------:"
	 << indexPageNumbersSize() << endl;
	 */
	/*	if (leafPageNumbersSize() > 0 && indexPageNumbersSize() > 0) {
	 cout << "Leaf page numbers :";
	 for (int i = 0; i < leafPageNumbersSize() - 1; i++) {
	 cout << popLeafPageNumbers(i) << ",";
	 }
	 cout << popLeafPageNumbers(leafPageNumbersSize() - 1) << endl;
	 cout << "Index page numbers :";
	 for (int i = 0; i < indexPageNumbersSize() - 1; i++) {
	 cout << popIndexPageNumbers(i) << ",";
	 }
	 cout << popIndexPageNumbers(indexPageNumbersSize() - 1) << endl;
	 }
	 */
	cout << "\t**************************************************************"
			<< endl;
}
std::string IndexHeaderPage::toString() {
	DEBUG("INSIDE INDEX HEADER PAGE --  TO STRING");
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "INDEX HEADER PAGE NO: " << getPageNumber() << "\n";
	mystream
			<< "\n\t********************-INDEX HEADER PAGE-***********************"
			<< endl;
	mystream << "\tINDEX HEADER PAGE NUMBER------------:" << getPageNumber()
			<< "\n";
	mystream << "\tRoot page number--------------------:" << getRootPage()
			<< "\n";
	mystream << "\tIndexNode fanout--------------------:" << getFanout()
			<< "\n";
	mystream << "\tMaximum records in leaf page--------:"
			<< getMaxNoOfRecordsInLeaf() << "\n";
	mystream << "\tHeight of tree----------------------:"
			<< getHeightOfTheTree() << "\n";
	mystream << "\tNumber of columns in Index----------:" << getNumOfColumns()
			<< "\n";
	mystream << "\tKey size----------------------------:" << getKeySize()
			<< "\n";
	mystream << "\tNumber of Keys----------------------:" << getNumberOfKeys()
			<< "\n";
	for (int i = 0; i < getNumOfColumns(); i++) {
		char columnT[10];
		memset(columnT, '\0', 10);
		switch (columnTypes_[i]) {
		case 1:
			memcpy(columnT, "INTEGER", sizeof("INTEGER"));
			break;
		case 2:
			memcpy(columnT, "FLOAT", sizeof("FLOAT"));
			break;
		case 3:
			memcpy(columnT, "DOUBLE", sizeof("DOUBLE"));
			break;
		case 4:
			memcpy(columnT, "STRING", sizeof("STRING"));
			break;
		case 5:
			memcpy(columnT, "LONG", sizeof("LONG"));
			break;
		}
		mystream << "\tColumn Types [field " << i + 1 << "]--------------:"
				<< columnT << "\n";
	}
	return mystream.str();

}
