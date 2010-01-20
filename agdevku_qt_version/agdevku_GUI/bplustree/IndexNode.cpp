/*
 * IndexNode.cpp
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#include "IndexNode.h"
#include "LeafNode.h"
#include "IndexHeaderPage.h"
#include "BPlusTree.h"
#include "BPlusTreeUtil.h"
#include "../utils/debug.h"
#include "../bufmgr/BufferManager.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../global/StatusCodes.h"
#include "../global/GlobalDefines.h"
#include "../global/GlobalStructures.h"
#include <iostream>
#include<math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
using namespace std;
IndexNode::IndexNode() {
	// TODO Auto-generated constructor stub
	indexHeaderObjCreatedHere = false;
}
int IndexNode::debugCount_ = 0;
IndexNode::IndexNode(int indexPageNumber)
{
	BufferManager *bufMgr = BufferManager::getInstance();
	//		DEBUG_B("Pinning leaf page with with buffer manager")
	int error = bufMgr->pinAndGetPage(indexPageNumber, pageData_);
	if (error != SUCCESS) {
		DEBUG_B("ERROR IN CREATE INDEX PAGE");
		//			return error;
	}
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	int indexHeaderPageNumber = getIndexHeaderPageNumber();
	indexHeader_ = new IndexHeaderPage(indexHeaderPageNumber);
	indexHeaderObjCreatedHere = true;

}
int IndexNode::getIndexHeaderPageNumber()
{
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	return indexNodePageHeader_.indexHeaderPageNumber;
}
IndexNode::IndexNode(IndexHeaderPage *indexHeaderPage, int indexPageNumber) {
	//	DEBUG_B("Creating index page with page number : "<<indexPageNumber)
	indexHeaderObjCreatedHere = false;
	indexHeader_ = indexHeaderPage;
	pageData_ = NULL;
	BufferManager *bufMgr = BufferManager::getInstance();
	//	DEBUG_B("Pinning index page with buffer manager")
	bufMgr->pinAndGetPage(indexPageNumber, pageData_);
	//	DEBUG_B("INDEX PAGE DATA "<<pageData_)
//	debugCount_++;
	//	DEBUG_B("DEBUG_COUNT="<<debugCount_);
	//	printf("pointer = %lu\n",pageData_);
	memcpy(&indexNodePageHeader_, pageData_,
			IndexNode::getIndexNodeHeaderSize());
}
IndexNode::~IndexNode() {
	// TODO Auto-generated destructor stub
	//	DEBUG_B("In Index node Destructor")
	BufferManager *bufMgr = BufferManager::getInstance();
	//	DEBUG_B("UNPINNGING PAGE NUMBER "<<indexNodePageHeader_.generalPageHeaderStruct.pageNumber)
	//	DEBUG_B("Unpinning index page number : "<<indexNodePageHeader_.generalPageHeaderStruct.pageNumber)
	bufMgr->unPinPage(indexNodePageHeader_.generalPageHeaderStruct.pageNumber,
			true);
	if(indexHeaderObjCreatedHere == true){
		delete indexHeader_;
	}
}

STATUS_CODE IndexNode::createIndexNode(IndexHeaderPage* indexHeaderPage) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int newPageNumber;
	int error = bufMgr->newPage(newPageNumber, pageData_);
	if (error != SUCCESS) {
		DEBUG_B("ERROR IN CREATE INDEX PAGE");
		return error;
	}
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	indexNodePageHeader_.generalPageHeaderStruct.pageType=TYPE_INDEX_PAGE;
	indexNodePageHeader_.leftPageNumber = -1;
	indexNodePageHeader_.rightPageNumber = -1;
	indexNodePageHeader_.level = 1;//this is minimum level
	indexNodePageHeader_.noOfKeys = 0;
	indexNodePageHeader_.parentPageNumber = -1;
	memcpy(pageData_, &indexNodePageHeader_, sizeof(IndexNodePageHeaderStruct));
	//	indexHeaderPage->pushIndexPageNumbers(newPageNumber);
	indexHeader_ = indexHeaderPage;
	return SUCCESS;
}
STATUS_CODE IndexNode::insertIntoIndexNode(const char* key, int leftPageNumber,
		int rightPageNumber) {
	//	DEBUG_B("INDEX NODE value "<<key);
	//	DEBUG_B("INDEX NODE value "<<leftPageNumber);
	//	DEBUG_B("INDEX NODE value "<<rightPageNumber);
	//	DEBUG_B("CURRENT PAGE NUMBER "<<indexNodePageHeader_.generalPageHeaderStruct.pageNumber)
	//	DEBUG_B("NUMBER OF RECORDS IN INDEX NODE "<<indexNodePageHeader_.noOfKeys)
	//	DEBUG_B("ROOT OF BPLUS TREE "<<indexHeader_->getRootPage());
	//	DEBUG_B("HEIGHT OF BPLUS TREE "<<indexHeader_->getHeightOfTheTree());
	DEBUG_B("Icurrent index page number : "<<getPageNumber())
	DEBUG_B("Received values :: ")
	DEBUG_B("leftPage number : "<<leftPageNumber)
	DEBUG_B("key : ")
	if (BPLUSTREE_DEBUG == true) {
		BPlusTreeUtil::displayKey(key, indexHeader_);
		DEBUG_B(endl)
	}
	DEBUG_B(" Right page number : "<<rightPageNumber)
	int levelOfRoot = indexHeader_->getHeightOfTheTree();
	if (levelOfRoot == 0) {
		//IF INDEX NODE IS CREATED AT FIRST
		DEBUG_B("Making this index node as root of tree and inserting in this index node ")
		indexHeader_->setRootPage(
				indexNodePageHeader_.generalPageHeaderStruct.pageNumber);
		indexHeader_->updateHeightOfTheTree(indexHeader_->getHeightOfTheTree()
				+ 1);
		int offset = IndexNode::getIndexNodeHeaderSize();
		memcpy(&pageData_[offset], &leftPageNumber, sizeof(int));
		offset = offset + sizeof(int);
		memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&pageData_[offset], &rightPageNumber, sizeof(int));
		indexNodePageHeader_.noOfKeys = indexNodePageHeader_.noOfKeys + 1;
		//		DEBUG_B("no of key intially "<<indexNodePageHeader_.noOfKeys)
		memcpy(pageData_, &indexNodePageHeader_,
				IndexNode::getIndexNodeHeaderSize());
		LeafNode leafNode(indexHeader_, leftPageNumber);
		leafNode.setParentPageNumber(getPageNumber());
		LeafNode leafNode1(indexHeader_, rightPageNumber);
		leafNode1.setParentPageNumber(getPageNumber());
		return SUCCESS;
	} else {
		//IF ALREADY AN INDEX NODE IS EXISTED
		DEBUG_B("Number of keys in this index node : "<<indexNodePageHeader_.noOfKeys);
		if (indexNodePageHeader_.noOfKeys != indexHeader_->getFanout()) {
			//IF INDEX NODE HAS ENOUGH SPACE TO ACCOMODATE NEW KEY
			DEBUG_B("This index node has enough space to accommodate new keys")
			DEBUG_B("So inserting new key in this node only")
			int tempLeftPage, tempRightPage;
			char tempKey[indexHeader_->getKeySize()];
			int offset = IndexNode::getIndexNodeHeaderSize();
			int count;
			for (count = 0; count < indexNodePageHeader_.noOfKeys; count++) {
				memcpy(&tempLeftPage, &pageData_[offset], sizeof(int));
				offset = offset + sizeof(int);
				memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
				offset = offset + indexHeader_->getKeySize();
				memcpy(&tempRightPage, &pageData_[offset], sizeof(int));
				//				dummyKeyCompare(tempKey, key);
				int compValue = BPlusTreeUtil::keyCompare(tempKey, key,
						indexHeader_);
				//				DEBUG_B("key compare in index node "<<tempKey<<"----"<<key)
				if (compValue == 1) {
					DEBUG_B(" inserting at location : "<<count)
					char
							tempKeys[indexNodePageHeader_.noOfKeys][indexHeader_->getKeySize()];
					int tempPageNumbers[indexNodePageHeader_.noOfKeys - count
							+ 1];
					int tempOffset =
							IndexNode::getIndexNodeHeaderSize()
									+ count * (sizeof(int)
											+ indexHeader_->getKeySize());
					int i;
					DEBUG_B("Shifting remaining keys from location "<<count<<" to right side")
					for (i = count; i < indexNodePageHeader_.noOfKeys; i++) {
						memcpy(&tempPageNumbers[i], &pageData_[tempOffset],
								sizeof(int));
						//						DEBUG_B("page no-----------"<<tempPageNumbers[i])
						tempOffset = tempOffset + sizeof(int);
						memcpy(&tempKeys[i], &pageData_[tempOffset],
								indexHeader_->getKeySize());
						//						DEBUG_B(" KEY----------------------"<<tempKeys[i])
						tempOffset += indexHeader_->getKeySize();
					}
					memcpy(&tempPageNumbers[i], &pageData_[tempOffset],
							sizeof(int));
					offset = IndexNode::getIndexNodeHeaderSize() + count
							* (sizeof(int) + indexHeader_->getKeySize());
					//					DEBUG_B("TEMP LEFT AND RIGHT "<<tempLeftPage<<"  "<<tempRightPage)
					//					DEBUG_B("after values "<<tempPageNumbers[count])
					memcpy(&pageData_[offset], &leftPageNumber, sizeof(int));
					offset = offset + sizeof(int);
					memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
					offset += indexHeader_->getKeySize();
					memcpy(&pageData_[offset], &rightPageNumber, sizeof(int));
					offset += sizeof(int);
					if (getLevelOfIndexNode() == 1) {
						//						DEBUG_B("left right parent "<<leftPageNumber<<"  "<<rightPageNumber)
						//						DEBUG_B("parent "<<getPageNumber())
						LeafNode leafNode1(indexHeader_, leftPageNumber);
						LeafNode leafNode2(indexHeader_, rightPageNumber);
						LeafNode leafNode3(indexHeader_, tempLeftPage);
						LeafNode leafNode4(indexHeader_, tempRightPage);
						leafNode1.setLeftPageNumber(
								leafNode3.getLeftPageNumber());
						leafNode1.setRightPageNumber(rightPageNumber);
						leafNode2.setLeftPageNumber(leftPageNumber);
						leafNode2.setRightPageNumber(tempRightPage);
						leafNode4.setLeftPageNumber(rightPageNumber);
						leafNode1.setParentPageNumber(getPageNumber());
						leafNode2.setParentPageNumber(getPageNumber());
						//						leafNode1.display();
						//						leafNode2.display();
					} else {
						DEBUG_B(leftPageNumber<<"  "<<rightPageNumber<<"  "<<getPageNumber())
						IndexNode indexNode1(indexHeader_, leftPageNumber);
						IndexNode indexNode2(indexHeader_, rightPageNumber);
						IndexNode indexNode3(indexHeader_, tempLeftPage);
						IndexNode indexNode4(indexHeader_, tempRightPage);
						indexNode1.setLeftPageNumberOfIndexNode(
								indexNode3.getLeftPageNumberOfIndexNode());
						indexNode1.setRightPageNumber(rightPageNumber);
						indexNode2.setLeftPageNumberOfIndexNode(leftPageNumber);
						indexNode2.setRightPageNumber(tempRightPage);
						indexNode4.setLeftPageNumberOfIndexNode(rightPageNumber);
						indexNode1.setParentOfIndexNode(getPageNumber());
						indexNode2.setParentOfIndexNode(getPageNumber());
					}
					for (int j = count; j < indexNodePageHeader_.noOfKeys; j++) {
						memcpy(&pageData_[offset], &tempKeys[j],
								indexHeader_->getKeySize());
						offset = offset + indexHeader_->getKeySize();
						memcpy(&pageData_[offset], &tempPageNumbers[j + 1],
								sizeof(int));
						offset = offset + sizeof(int);
					}
					indexNodePageHeader_.noOfKeys++;
					DEBUG_B("increase number of keys by 1  new count :  "<<indexNodePageHeader_.noOfKeys)
					memcpy(pageData_, &indexNodePageHeader_,
							IndexNode::getIndexNodeHeaderSize());
					offset = IndexNode::getIndexNodeHeaderSize();
					/*					int pageno;
					 //					DEBUG_B("IAM HEREs-----THIS IS FOR DISPLAY WHOLE INDEX NODE VALUES--- and to assign it as parent for all its childs---------")
					 char x[indexHeader_->getKeySize()];
					 for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
					 memcpy(&pageno, &pageData_[offset], sizeof(int));
					 //	cout << pageno << " ";
					 //						LeafNode leafNode(indexHeader_,pageno);
					 //						leafNode.setParentPageNumber(getPageNumber());
					 offset += sizeof(int);
					 memcpy(x, &pageData_[offset],
					 indexHeader_->getKeySize());
					 //cout << x << " ";
					 offset += indexHeader_->getKeySize();
					 }
					 memcpy(&pageno, &pageData_[offset], sizeof(int));
					 //					LeafNode leafNode2(indexHeader_,pageno);
					 //					leafNode2.setParentPageNumber(getPageNumber());
					 //					cout << pageno << " " << endl;
					 */
					/*					if (levelOfRoot == 1) {
					 offset = IndexNode::getIndexNodeHeaderSize();
					 int pageno;
					 for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
					 memcpy(&pageno, &pageData_[offset], sizeof(int));
					 LeafNode leafNode(indexHeader_, pageno);
					 leafNode.setParentPageNumber(getPageNumber());
					 offset = offset + sizeof(int)
					 + indexHeader_->getKeySize();
					 }
					 memcpy(&pageno, &pageData_[offset], sizeof(int));
					 LeafNode leafNode(indexHeader_, pageno);
					 leafNode.setParentPageNumber(getPageNumber());
					 } else {
					 DEBUG_B("I AM HERE")
					 offset = IndexNode::getIndexNodeHeaderSize();
					 int pageno;
					 int leftPageNumber=-1;
					 int rightPageNumber=-1;
					 for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
					 memcpy(&pageno, &pageData_[offset], sizeof(int));
					 DEBUG_B("HERE****************************************************************"<<pageno)
					 IndexNode indexNode(indexHeader_, pageno);
					 indexNode.setParentOfIndexNode(getPageNumber());
					 offset = offset + sizeof(int)
					 + indexHeader_->getKeySize();
					 memcpy(&rightPageNumber,&pageData_[offset],sizeof(int));
					 DEBUG_B("left---current-----right---------"<<leftPageNumber<<"   "<<pageno<<"   "<<rightPageNumber)
					 indexNode.setLeftPageNumberOfIndexNode(leftPageNumber);
					 indexNode.setRightPageNumber(rightPageNumber);
					 leftPageNumber=pageno;
					 }
					 memcpy(&pageno, &pageData_[offset], sizeof(int));
					 IndexNode indexNode(indexHeader_, pageno);
					 indexNode.setParentOfIndexNode(getPageNumber());
					 indexNode.setLeftPageNumberOfIndexNode(leftPageNumber);

					 }
					 */
					return SUCCESS;
				}
			}
			if (indexNodePageHeader_.noOfKeys == 0) {
				offset = IndexNode::getIndexNodeHeaderSize();
				//				DEBUG_B(leftPageNumber<<"  "<<"  "<<key<<"  "<<rightPageNumber)
				memcpy(&pageData_[offset], &leftPageNumber, sizeof(int));
				offset = offset + sizeof(int);
				memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
				offset = offset + indexHeader_->getKeySize();
				memcpy(&pageData_[offset], &rightPageNumber, sizeof(int));
				indexNodePageHeader_.noOfKeys++;
				memcpy(pageData_, &indexNodePageHeader_,
						IndexNode::getIndexNodeHeaderSize());
				LeafNode leafNode1(indexHeader_, leftPageNumber);
				leafNode1.setParentPageNumber(getPageNumber());
				LeafNode leafNode2(indexHeader_, rightPageNumber);
				leafNode2.setParentPageNumber(getPageNumber());
				return SUCCESS;
			}
			if (count == indexNodePageHeader_.noOfKeys) {

				offset = IndexNode::getIndexNodeHeaderSize()
						+ (indexNodePageHeader_.noOfKeys
								* (indexHeader_->getKeySize() + sizeof(int)));
				if (getLevelOfIndexNode() == 1) {
					//					DEBUG_B("HERE")
					//					DEBUG_B("left "<<leftPageNumber)
					//					DEBUG_B("right "<<rightPageNumber)
					LeafNode leafNode1(indexHeader_, leftPageNumber);
					LeafNode leafNode2(indexHeader_, rightPageNumber);
					leafNode2.setLeftPageNumber(leftPageNumber);
					leafNode1.setParentPageNumber(getPageNumber());
					leafNode2.setParentPageNumber(getPageNumber());
					//					DEBUG_B("lraf "<<leafNode1.getRightPageNumber())
					//					leafNode2.setRightPageNumber(leafNode1.getRightPageNumber());
					leafNode1.setRightPageNumber(rightPageNumber);
					if (leafNode2.getRightPageNumber() != -1) {
						LeafNode leafNode3(indexHeader_,
								leafNode2.getRightPageNumber());
						leafNode3.setLeftPageNumber(rightPageNumber);
					}
				} else {
					IndexNode indexNode1(indexHeader_, leftPageNumber);
					IndexNode indexNode2(indexHeader_, rightPageNumber);
					indexNode1.setParentOfIndexNode(getPageNumber());
					indexNode2.setParentOfIndexNode(getPageNumber());
					indexNode2.setLeftPageNumberOfIndexNode(leftPageNumber);
					//					indexNode2.setRightPageNumber(indexNode1.getRightPageNumber());
					indexNode1.setRightPageNumber(rightPageNumber);
					if (indexNode2.getRightPageNumber() != -1) {
						IndexNode indexNode3(indexHeader_,
								indexNode2.getRightPageNumber());
						indexNode3.setLeftPageNumberOfIndexNode(rightPageNumber);
					}
				}

				memcpy(&pageData_[offset], &leftPageNumber, sizeof(int));
				offset = offset + sizeof(int);
				memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
				offset = offset + indexHeader_->getKeySize();
				memcpy(&pageData_[offset], &rightPageNumber, sizeof(int));
				indexNodePageHeader_.noOfKeys++;
				memcpy(pageData_, &indexNodePageHeader_,
						IndexNode::getIndexNodeHeaderSize());

				//				return SUCCESS;
			}
			/*
			 if (levelOfRoot == 1) {
			 offset = IndexNode::getIndexNodeHeaderSize();
			 int pageno;
			 for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
			 memcpy(&pageno, &pageData_[offset], sizeof(int));
			 LeafNode leafNode(indexHeader_, pageno);
			 leafNode.setParentPageNumber(getPageNumber());
			 offset = offset + sizeof(int)
			 + indexHeader_->getKeySize();
			 }
			 memcpy(&pageno, &pageData_[offset], sizeof(int));
			 LeafNode leafNode(indexHeader_, pageno);
			 leafNode.setParentPageNumber(getPageNumber());
			 } else {
			 offset = IndexNode::getIndexNodeHeaderSize();
			 int pageno;
			 for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
			 memcpy(&pageno, &pageData_[offset], sizeof(int));
			 IndexNode indexNode(indexHeader_, pageno);
			 indexNode.setParentOfIndexNode(getPageNumber());
			 offset = offset + sizeof(int)
			 + indexHeader_->getKeySize();
			 }
			 memcpy(&pageno, &pageData_[offset], sizeof(int));
			 IndexNode indexNode(indexHeader_, pageno);
			 indexNode.setParentOfIndexNode(getPageNumber());
			 }
			 */
			return SUCCESS;
		} else {
			//nof of keys index node is equals to maximum number so split it into two nodes
			DEBUG_B("Index node does not enough capacity to insert new keys ")
			DEBUG_B("So distribute keys into two index nodes")
			//			DEBUG_B("over flow of index node##########################################################")
			char
					tempKeys[indexNodePageHeader_.noOfKeys + 1][indexHeader_->getKeySize()];
			int tempPageNos[indexNodePageHeader_.noOfKeys + 2];
			int tempCount = 0;
			int offset = IndexNode::getIndexNodeHeaderSize();
			int count;
			int found = 0;
			for (count = 0; count < indexNodePageHeader_.noOfKeys; count++) {
				char tempKey[indexHeader_->getKeySize()];
				int tempLeft, tempRight;
				memcpy(&tempLeft, &pageData_[offset], sizeof(int));
				offset += sizeof(int);
				memcpy(&tempKey, &pageData_[offset], indexHeader_->getKeySize());
				offset += indexHeader_->getKeySize();
				memcpy(&tempRight, &pageData_[offset], sizeof(int));
				//				dummyKeyCompare(tempKey, key);
				int compValue = BPlusTreeUtil::keyCompare(tempKey, key,
						indexHeader_);
				if (compValue == 1 && found == 0) {
					memcpy(tempKeys[tempCount], key, indexHeader_->getKeySize());
					tempPageNos[tempCount] = leftPageNumber;
					tempCount++;
					tempPageNos[tempCount] = rightPageNumber;
					found = 1;
				}

				memcpy(tempKeys[tempCount], tempKey, indexHeader_->getKeySize());
				if (found == 0) {
					tempPageNos[tempCount] = tempLeft;
				}
				tempCount++;
				tempPageNos[tempCount] = tempRight;
				//				DEBUG_B("TEMP COUNT "<<tempCount)

			}
			if (found == 0) {
				memcpy(tempKeys[tempCount], key, indexHeader_->getKeySize());
				tempCount++;
				tempPageNos[tempCount] = rightPageNumber;

			}
			//						DEBUG_B("$$$$$$$$$$$$$$$$$$ temp count $$$$$$$$$$$$$$$$$$"<<tempCount)
			/*			 int j;
			 for(j=0;j<tempCount;j++)
			 {
			 cout<<" "<<tempPageNos[j]<<" "<<tempKeys[j];
			 }
			 cout<<" "<<tempPageNos[j]<<"     "<<indexHeader_->getKeySize()<<endl;
			 */
			indexNodePageHeader_.noOfKeys = 0;
			offset = IndexNode::getIndexNodeHeaderSize();
			int i;
			DEBUG_B("Inserting half of the keys in one index node")
			for (i = 0; i < tempCount / 2; i++) {
				memcpy(&pageData_[offset], &tempPageNos[i], sizeof(int));
				offset += sizeof(int);
				if (indexNodePageHeader_.level == 1) {
					LeafNode leafNode(indexHeader_, tempPageNos[i]);
					leafNode.setParentPageNumber(getPageNumber());
					//					DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
				} else {
					IndexNode indexNode(indexHeader_, tempPageNos[i]);
					indexNode.setParentOfIndexNode(getPageNumber());
				}
				//				DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
				memcpy(&pageData_[offset], tempKeys[i],
						indexHeader_->getKeySize());
				offset += indexHeader_->getKeySize();
				indexNodePageHeader_.noOfKeys++;
				//				DEBUG_B("inserting keys "<<tempPageNos[i]<<"  "<<tempKeys[i]);
			}
			//			DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
			memcpy(&pageData_[offset], &tempPageNos[i], sizeof(int));
			if (indexNodePageHeader_.level == 1) {
				LeafNode leafNode(indexHeader_, tempPageNos[i]);
				leafNode.setParentPageNumber(getPageNumber());
			} else {
				IndexNode indexNode(indexHeader_, tempPageNos[i]);
				indexNode.setParentOfIndexNode(getPageNumber());
			}
			//			DEBUG_B("inserting keys "<<tempPageNos[i]);
			//			memcpy(pageData_, &indexNodePageHeader_,IndexNode::getIndexNodeHeaderSize());
			//			DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
			//NOW CREATE A NEW INDEX NODE AND COPY REMAINING KEYS INTO THAT NODE
			IndexNode indexNode;
			indexNode.createIndexNode(indexHeader_);
			//			cout<<"\nNEW PAGE NUMBER  "<<indexNode.getPageNumber()<<endl;
			DEBUG_B("new index page number :"<<indexNode.getPageNumber())
			indexNode.setLevelOfIndexNode(indexNodePageHeader_.level);
			if (indexNodePageHeader_.rightPageNumber != -1) {
				IndexNode indexNode1(indexHeader_,
						indexNodePageHeader_.rightPageNumber);
				indexNode1.setLeftPageNumberOfIndexNode(
						indexNode.getPageNumber());
			}
			//			DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
			indexNode.setLeftPageNumberOfIndexNode(getPageNumber());
			//			DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
			indexNode.setRightPageNumber(indexNodePageHeader_.rightPageNumber);
			setRightPageNumber(indexNode.getPageNumber());
			//			DEBUG_B("NEW INDEX PAGE NUMBER+++++++++"<<indexNode.getPageNumber())
			//			DEBUG_B("TEMP COUT "<<tempCount)
			DEBUG_B("Inserting remaining keys into new index node")
			for (i = tempCount / 2 + 1; i < tempCount; i++) {
				char shiftKey[indexHeader_->getKeySize()];
				int shiftLeft, shiftRight;
				memcpy(shiftKey, tempKeys[i], indexHeader_->getKeySize());
				memcpy(&shiftLeft, &tempPageNos[i], sizeof(int));
				memcpy(&shiftRight, &tempPageNos[i + 1], sizeof(int));

				//				DEBUG_B("INSERTING KEYS++++++++++"<<shiftKey<<"  "<<shiftLeft<<"   "<<shiftRight)
				indexNode.insertIntoIndexNode(shiftKey, shiftLeft, shiftRight);
				//				DEBUG_B("index node level "<<indexNodePageHeader_.level)
				//				DEBUG_B("Index node page number "<<indexNode.getPageNumber())
				if (indexNodePageHeader_.level == 1) {
					//					DEBUG_B("HEHRE")
					//					DEBUG_B("left "<<shiftLeft<<" right "<<shiftRight)
					//					DEBUG_B("current page "<<indexNode.getPageNumber());
					LeafNode leafNode(indexHeader_, shiftLeft);
					leafNode.setParentPageNumber(indexNode.getPageNumber());
					//					leafNode.display();
					LeafNode leafNode1(indexHeader_, shiftRight);
					leafNode1.setParentPageNumber(indexNode.getPageNumber());
					//					leafNode.display();
				} else {
					//					DEBUG_B("HERE")
					IndexNode tempIndexNode(indexHeader_, shiftLeft);
					tempIndexNode.setParentOfIndexNode(
							indexNode.getPageNumber());
					IndexNode tempIndexNode1(indexHeader_, shiftRight);
					tempIndexNode1.setParentOfIndexNode(
							indexNode.getPageNumber());
				}
			}

			//			DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
			char promoteKey[indexHeader_->getKeySize()];
			int promoteLeft, promoteRight;
			promoteLeft = getPageNumber();
			promoteRight = indexNode.getPageNumber();
			memcpy(promoteKey, tempKeys[tempCount / 2],
					indexHeader_->getKeySize());

			if (indexHeader_->getHeightOfTheTree()
					== indexNodePageHeader_.level) {
				//NOW CREATE A ROOT NODE AND ASSIGN IT AS A PARENT FOR ABOVE NODES
				DEBUG_B("Create new index node and make it as root of tree")
				IndexNode indexNode1;
				indexNode1.createIndexNode(indexHeader_);
				//				DEBUG_B("NEW PAGE NUMBER_________________________________________________________"<<indexNode1.getPageNumber());
				DEBUG_B("new index page number : "<<indexNode1.getPageNumber())
				indexHeader_->setRootPage(indexNode1.getPageNumber());
				indexHeader_->updateHeightOfTheTree(
						indexHeader_->getHeightOfTheTree() + 1);
				DEBUG_B("Increase height of tree by one  new level : "<<indexHeader_->getHeightOfTheTree())
				int level = indexHeader_->getHeightOfTheTree();
				//				DEBUG_B("NEW LEVEL_____________________________________________________"<<indexHeader_->getHeightOfTheTree());
				indexNode1.setLevelOfIndexNode(level);
				//				DEBUG_B("PROMTES___________________________________________"<<promoteLeft<<"  "<<promoteKey<<"   "<<promoteRight);
				DEBUG_B("Promoting keys to heigher level : ")
				DEBUG_B("left page number : "<<promoteLeft)
				DEBUG_B("Key : ")
				if (BPLUSTREE_DEBUG == true) {
					BPlusTreeUtil::displayKey(promoteKey, indexHeader_);
					DEBUG_B(endl)
				}
				DEBUG_B("right page number : "<<promoteRight)
				indexNode1.insertIntoIndexNode(promoteKey, promoteLeft,
						promoteRight);
				indexNodePageHeader_.parentPageNumber
						= indexNode1.getPageNumber();
				indexNodePageHeader_.leftPageNumber = -1;
				indexNodePageHeader_.rightPageNumber
						= indexNode.getPageNumber();
				DEBUG_B("no of records "<<indexNodePageHeader_.noOfKeys);
				memcpy(pageData_, &indexNodePageHeader_,
						IndexNode::getIndexNodeHeaderSize());
				indexNode.setParentOfIndexNode(indexNode1.getPageNumber());
				indexNode.setLeftPageNumberOfIndexNode(getPageNumber());
			} else {
				//PROMOTE KEY, LEFT, RIGHT PAGE NUMBERS TO PARENT OF THE INDEX NODE
				IndexNode indexNode(indexHeader_, getParentOfIndexNode());
				//				DEBUG_B("PROMTES___________________________________________"<<promoteLeft<<"  "<<promoteKey<<"   "<<promoteRight);
				DEBUG_B("Promoting values : ")
				DEBUG_B("left page number : "<<promoteLeft)
				DEBUG_B("key : ")
				if (BPLUSTREE_DEBUG == true) {
					BPlusTreeUtil::displayKey(promoteKey, indexHeader_);
					DEBUG_B(endl)
				}
				DEBUG_B("right page number : "<<promoteRight)
				indexNode.insertIntoIndexNode(promoteKey, promoteLeft,
						promoteRight);
				return SUCCESS;
				//				indexHeader_->updateHeightOfTheTree(indexHeader_->getHeightOfTheTree() + 1);
			}
		}
	}
	return SUCCESS;
}
STATUS_CODE IndexNode::insertAtTheEndOfIndexNode(const char* key,
		int rightPageNumber) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int lastPage = getLastPageNumberPresentInIndexNode();
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
	}
	offset = offset + sizeof(int);
	memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
	offset = offset + indexHeader_->getKeySize();
	memcpy(&pageData_[offset], &rightPageNumber, sizeof(int));
	setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() + 1);
	if (getLevelOfIndexNode() == 1) {
		LeafNode leafNode(indexHeader_, rightPageNumber);
		leafNode.setParentPageNumber(getPageNumber());
	} else {
		IndexNode indexNode(indexHeader_, rightPageNumber);
		indexNode.setParentOfIndexNode(getPageNumber());
	}
}
STATUS_CODE IndexNode::insertAtTheFrontOfIndexNode(const char* key,
		int leftPageNumber) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	char tempKeys[getNoOfKeysInIndexNode()][indexHeader_->getKeySize()];
	int tempPageNumbers[getNoOfKeysInIndexNode() + 1];
	int tempCount = 0;
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&tempPageNumbers[tempCount], &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(tempKeys[tempCount], &pageData_[offset],
				indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		tempCount++;
	}
	memcpy(&tempPageNumbers[tempCount], &pageData_[offset], sizeof(int));
	offset = IndexNode::getIndexNodeHeaderSize();
	memcpy(&pageData_[offset], &leftPageNumber, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
	offset = offset + indexHeader_->getKeySize();
	for (int i = 0; i < tempCount; i++) {
		memcpy(&pageData_[offset], &tempPageNumbers[i], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(&pageData_[offset], tempKeys[i], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
	}
	memcpy(&pageData_[offset], &tempPageNumbers[tempCount], sizeof(int));
	setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() + 1);
	if (getLevelOfIndexNode() == 1) {
		LeafNode leafNode(indexHeader_, leftPageNumber);
		leafNode.setParentPageNumber(getPageNumber());
	} else {
		IndexNode indexNode(indexHeader_, leftPageNumber);
		indexNode.setParentOfIndexNode(getPageNumber());
	}
	//	indexNodeOnlyDisplay();
}
STATUS_CODE IndexNode::deleteKeyFromIndexNode(const char* key, bool flag,
		std::vector<int> &deletedPages) {
	DEBUG_B("IN INDEX NODE")
	DEBUG_B("key to delete :")
	if (BPLUSTREE_DEBUG == true) {
		BPlusTreeUtil::displayKey(key, indexHeader_);
		DEBUG_B(endl)
	}
	//	DEBUG_B("FLAG "<<flag)
	DEBUG_B("number of keys in this node "<<getNoOfKeysInIndexNode())
	int offset = IndexNode::getIndexNodeHeaderSize();
	char tempKey[indexHeader_->getKeySize()];
	int leftPageNumber, rightPageNumber;
	int i;
	for (i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rightPageNumber, &pageData_[offset], sizeof(int));
		//		DEBUG_B("DATA "<<leftPageNumber<<" "<<tempKey<<"  "<<rightPageNumber)
		//		dummyKeyCompare(key, tempKey)
		if (BPlusTreeUtil::keyCompare(key, tempKey, indexHeader_) == 0) {
			break;
		}
	}
	char tempKeys[indexHeader_->getFanout()][indexHeader_->getKeySize()];
	int tempPageNumbers[indexHeader_->getFanout() + 1];
	int tempCount = 0;
	int newOffset = offset;
	for (int j = i + 1; j < getNoOfKeysInIndexNode(); j++) {
		memcpy(&tempPageNumbers[tempCount], &pageData_[newOffset], sizeof(int));
		newOffset = newOffset + sizeof(int);
		memcpy(&tempKeys[tempCount], &pageData_[newOffset],
				indexHeader_->getKeySize());
		newOffset = newOffset + indexHeader_->getKeySize();
		tempCount++;
	}
	memcpy(&tempPageNumbers[tempCount], &pageData_[newOffset], sizeof(int));
	//	DEBUG_B("TEMP COUNT "<<tempCount)
	/*
	 for (int i = 0; i < tempCount; i++) {
	 //		DEBUG_B("KEYS: "<<tempKeys[i]<<" "<<tempPageNumbers[i])
	 }
	 */
	//	DEBUG_B("REMAINING "<<tempPageNumbers[tempCount])
	DEBUG_B("delete key from index node ")
	if (flag == true) {
		offset = offset - indexHeader_->getKeySize() - sizeof(int);
		int entered = 0;
		for (int i = 0; i < tempCount; i++) {
			memcpy(&pageData_[offset], &tempPageNumbers[i], sizeof(int));
			offset = offset + sizeof(int);
			memcpy(&pageData_[offset], &tempKeys[i], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			entered = 1;
			if (i == 0) {
				if (getLevelOfIndexNode() == 1) {
					LeafNode leafNode(indexHeader_, leftPageNumber);
					LeafNode newLeafNode(indexHeader_, tempPageNumbers[i]);
					newLeafNode.setLeftPageNumber(leafNode.getLeftPageNumber());
					if (leafNode.getLeftPageNumber() != -1) {
						LeafNode oldRightNode(indexHeader_,
								leafNode.getLeftPageNumber());
						oldRightNode.setRightPageNumber(
								newLeafNode.getPageNumber());
					}
				} else {
					IndexNode indexNode(indexHeader_, leftPageNumber);
					IndexNode newIndexNode(indexHeader_, tempPageNumbers[i]);
					newIndexNode.setLeftPageNumberOfIndexNode(
							indexNode.getLeftPageNumberOfIndexNode());
					if (indexNode.getLeftPageNumberOfIndexNode() != -1) {
						IndexNode oldRightIndexNode(indexHeader_,
								indexNode.getLeftPageNumberOfIndexNode());
						oldRightIndexNode.setRightPageNumber(
								newIndexNode.getPageNumber());
					}
				}
			}
		}
		memcpy(&pageData_[offset], &tempPageNumbers[tempCount], sizeof(int));
		setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() - 1);
		DEBUG_B("decrease number of keys by 1")
		if (entered == 0) {
			if (getLevelOfIndexNode() == 1) {
				DEBUG_B("REMOVED PAGENUMBER "<<leftPageNumber)
				int newPageNumber = tempPageNumbers[tempCount];
				LeafNode leafNode(indexHeader_, leftPageNumber);
				LeafNode newLeafNode(indexHeader_, newPageNumber);
				newLeafNode.setLeftPageNumber(leafNode.getLeftPageNumber());
				if (leafNode.getLeftPageNumber() != -1) {
					LeafNode oldRightNode(indexHeader_,
							leafNode.getLeftPageNumber());
					oldRightNode.setRightPageNumber(newLeafNode.getPageNumber());
				}

			} else {
				IndexNode indexNode(indexHeader_, leftPageNumber);
				IndexNode
						newIndexNode(indexHeader_, tempPageNumbers[tempCount]);
				newIndexNode.setLeftPageNumberOfIndexNode(
						indexNode.getLeftPageNumberOfIndexNode());
				if (indexNode.getLeftPageNumberOfIndexNode() != -1) {
					IndexNode oldRightIndexNode(indexHeader_,
							indexNode.getLeftPageNumberOfIndexNode());
					oldRightIndexNode.setRightPageNumber(
							newIndexNode.getPageNumber());
				}
			}
		}
		//		DEBUG_B("flag is true ebds HERE")
	}
	/*		if (getNoOfKeysInIndexNode() >= ceil(indexHeader_->getFanout() / 2)) {
	 DEBUG_B("GOOD")
	 } else {
	 DEBUG_B("CHECK AND ROTATE ")
	 indexNodeOnlyDisplay();
	 int rightPage = getRightPageNumber();
	 int leftPage = getLeftPageNumberOfIndexNode();
	 if (rightPage != -1) {
	 IndexNode rightIndexNode(indexHeader_, rightPage);
	 if ((rightIndexNode.getNoOfKeysInIndexNode() - 1) >= ceil(
	 indexHeader_->getFanout() / 2)) {
	 //Rotate from right index Node to left index node
	 char seperatingKey[indexHeader_->getKeySize()];
	 char firstKeyInRIghtPage[indexHeader_->getKeySize()];
	 rightIndexNode.getFirstKeyPresentInIndexNode(
	 firstKeyInRIghtPage);
	 int
	 shiftingPageNumber =
	 rightIndexNode.getFirstPageNumberPresentInIndexNode();
	 DEBUG_B("first key "<<firstKeyInRIghtPage)
	 DEBUG_B("SHIFT "<<shiftingPageNumber)
	 if (getParentOfIndexNode() != -1) {
	 IndexNode parentNode(indexHeader_,
	 getParentOfIndexNode());
	 parentNode.getSeperatingKey(getPageNumber(), rightPage,
	 seperatingKey);
	 DEBUG_B("SEPERATING KEY "<<seperatingKey)
	 parentNode.replaceKey(seperatingKey,
	 firstKeyInRIghtPage);
	 }
	 insertAtTheEndOfIndexNode(seperatingKey, shiftingPageNumber);
	 rightIndexNode.deleteFirstKeyFromIndexNode();
	 DEBUG_B("INDEX NODE OPERATIONS ENDS HERE")
	 }
	 }
	 }
	 */
	else {
		//		DEBUG_B("I AM DEBUG_B FLAG FALSE")
		//		DEBUG_B("temp count "<<tempCount)
		if (tempCount == 0) {
			//			DEBUG_B("number of records "<<getNoOfKeysInIndexNode())
			setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() - 1);
			//		indexNodeOnlyDisplay();
			if (getLevelOfIndexNode() == 1) {
				//				DEBUG_B("last number "<<rightPageNumber);
				int newLastPageNumber = getLastPageNumberPresentInIndexNode();
				tempPageNumbers[tempCount] = newLastPageNumber;
				//				DEBUG_B("new last page "<<newLastPageNumber)
				//				DEBUG_B("TEMP COUNT "<<tempPageNumbers[tempCount])
				LeafNode leafNode(indexHeader_, rightPageNumber);
				LeafNode newLeafNode(indexHeader_, newLastPageNumber);
				newLeafNode.setRightPageNumber(leafNode.getRightPageNumber());
				if (leafNode.getRightPageNumber() != -1) {
					LeafNode rightLeafPage(indexHeader_,
							leafNode.getRightPageNumber());
					rightLeafPage.setLeftPageNumber(newLastPageNumber);
				}
			} else {
				//				DEBUG_B("last page number "<<rightPageNumber)
				IndexNode indexNode(indexHeader_, rightPageNumber);
				int newLastPageNumber = getLastPageNumberPresentInIndexNode();
				tempPageNumbers[tempCount] = newLastPageNumber;
				//				DEBUG_B("new last Page number "<<newLastPageNumber)
				IndexNode newIndexNode(indexHeader_, newLastPageNumber);
				newIndexNode.setRightPageNumber(indexNode.getRightPageNumber());
				if (getNoOfKeysInIndexNode() == 0) {
					tempPageNumbers[tempCount] = newLastPageNumber;
				}
				if (indexNode.getRightPageNumber() != -1) {
					IndexNode rightIndexNode(indexHeader_,
							indexNode.getRightPageNumber());
					rightIndexNode.setLeftPageNumberOfIndexNode(
							newLastPageNumber);
				}
			}
		} else {
			//tempcount not equal to 0
			//			DEBUG_B("HERE ONLY _________________")
			offset = offset - indexHeader_->getKeySize() - sizeof(int);
			memcpy(&tempPageNumbers[0], &pageData_[offset], sizeof(int));
			//			DEBUG_B("TEMP FIRST "<<tempPageNumbers[0])
			//			DEBUG_B("TEMP COUNT "<<tempCount)
			for (int i = 0; i < tempCount; i++) {
				memcpy(&pageData_[offset], &tempPageNumbers[i], sizeof(int));
				offset = offset + sizeof(int);
				memcpy(&pageData_[offset], &tempKeys[i],
						indexHeader_->getKeySize());
				//				DEBUG_B("TEMPs  "<<tempPageNumbers[i]<<"   "<<tempKeys[i])
				offset = offset + indexHeader_->getKeySize();
				//				leftPageNumber=tempPageNumbers[i];
				if (getLevelOfIndexNode() == 1) {
					if (i != 0) {
						LeafNode leafNode(indexHeader_, tempPageNumbers[i - 1]);
						LeafNode newLeafNode(indexHeader_, tempPageNumbers[i]);
						newLeafNode.setLeftPageNumber(leafNode.getPageNumber());
						newLeafNode.setRightPageNumber(tempPageNumbers[i + 1]);
					} else {
						LeafNode leafNode(indexHeader_, tempPageNumbers[i]);
						leafNode.setRightPageNumber(tempPageNumbers[i + 1]);
					}
				} else {
					if (i != 0) {
						IndexNode indexNode(indexHeader_,
								tempPageNumbers[i - 1]);
						IndexNode
								newIndexNode(indexHeader_, tempPageNumbers[i]);
						newIndexNode.setLeftPageNumberOfIndexNode(
								indexNode.getPageNumber());
						newIndexNode.setRightPageNumber(tempPageNumbers[i + 1]);
					} else {
						IndexNode indexNode(indexHeader_, tempPageNumbers[i]);
						indexNode.setRightPageNumber(tempPageNumbers[i + 1]);
					}
				}
			}
			memcpy(&pageData_[offset], &tempPageNumbers[tempCount], sizeof(int));
			setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() - 1);
			if (getLevelOfIndexNode() == 1) {
				LeafNode leafNode(indexHeader_, tempPageNumbers[tempCount]);
				leafNode.setLeftPageNumber(tempPageNumbers[tempCount - 1]);
			} else {
				IndexNode indexNode(indexHeader_, tempPageNumbers[tempCount]);
				indexNode.setLeftPageNumberOfIndexNode(
						tempPageNumbers[tempCount - 1]);
			}

			//			DEBUG_B("********************FLAG FALSE ENDS HERE**************************************")
		}
	}
	if (getNoOfKeysInIndexNode() == 0) {
		DEBUG_B("number of keys become 0")
		DEBUG_B("so delete this page")
		DEBUG_B("Make it child as root")
		deletedPages.push_back(getPageNumber());
		//		DEBUG_B("I AM HERE OK")
		//		DEBUG_B("TEMP COUNT "<<tempCount)
		//		DEBUG_B("TEMP PAGE NUMBER "<<tempPageNumbers[tempCount])
		if (getLevelOfIndexNode() == 1) {
			indexHeader_->setRootPage(tempPageNumbers[tempCount]);
			LeafNode leafNode(indexHeader_, tempPageNumbers[tempCount]);
			leafNode.setParentPageNumber(-1);
			indexHeader_->updateHeightOfTheTree(0);
		} else {
			indexHeader_->setRootPage(tempPageNumbers[tempCount]);
			IndexNode indexNode(indexHeader_, tempPageNumbers[tempCount]);
			indexNode.setParentOfIndexNode(-1);
			indexHeader_->updateHeightOfTheTree(indexNode.getLevelOfIndexNode());
		}
		return SUCCESS;
	}
	//TILL NOE DELETION DONE
	//FROM HERE CHECKING STARTS WHETHER TO ROTATE OR MERGING OR NOTHING
	if (getNoOfKeysInIndexNode() >= ceil(indexHeader_->getFanout() / 2)) {
		DEBUG_B("IN GOOD STATE NO NEED TO ROTATE ")
		DEBUG_B("Index node has enough number of keys to delete freely")
	} else {
		DEBUG_B("Check left and right pages for rotation or merging")
		//		indexNodeOnlyDisplay();
		int rightPage = -1;
		int leftPage = -1;
		if (getParentOfIndexNode() != -1) {
			IndexNode parentNode(indexHeader_, getParentOfIndexNode());
			parentNode.getSiblings(getPageNumber(), leftPage, rightPage);
			//			DEBUG_B("SIBLINGS "<<leftSibling<<"  "<<rightSibling)
		}

		DEBUG_B("Right page number : "<<rightPage<<"  left page number : "<<leftPage)
		if (rightPage != -1) {
			DEBUG_B("DO ROTATION WITH RIGHT PAGE IF POSSIBLE")
			//rotate from right to left
			IndexNode rightIndexNode(indexHeader_, rightPage);
			DEBUG_B("no of keys in right page "<<rightIndexNode.getNoOfKeysInIndexNode())
			if ((rightIndexNode.getNoOfKeysInIndexNode() - 1) >= ceil(
					indexHeader_->getFanout() / 2)) {
				DEBUG_B("right page has more than half of the keys ")
				DEBUG_B("so rotate keys from right page")
				char seperatingKey[indexHeader_->getKeySize()];
				char firstKeyPresentOnRightPage[indexHeader_->getKeySize()];
				rightIndexNode.getFirstKeyPresentInIndexNode(
						firstKeyPresentOnRightPage);
				//				DEBUG_B("first key "<<firstKeyPresentOnRightPage)
				int rotatingPageNumber;
				rotatingPageNumber
						= rightIndexNode.getFirstPageNumberPresentInIndexNode();
				//				DEBUG_B("right page number "<<rotatingPageNumber)
				if (getParentOfIndexNode() != -1) {
					IndexNode parentIndexNode(indexHeader_,
							getParentOfIndexNode());
					parentIndexNode.getSeperatingKey(getPageNumber(),
							rightPage, seperatingKey);
					//					DEBUG_B("seperating key "<<seperatingKey)
					//pushing parent key down to child index node(current page) along with left most page in right index page
					insertAtTheEndOfIndexNode(seperatingKey, rotatingPageNumber);
					if (getLevelOfIndexNode() == 1) {
						LeafNode rotateLeafNode(indexHeader_,
								rotatingPageNumber);
						rotateLeafNode.setParentPageNumber(getPageNumber());
					} else {
						IndexNode rotateIndexNode(indexHeader_,
								rotatingPageNumber);
						rotateIndexNode.setParentOfIndexNode(getPageNumber());
					}
					//pushing leftmost key in right node to parent node
					parentIndexNode.replaceKey(seperatingKey,
							firstKeyPresentOnRightPage);
					//deleteing first key after it is moved to parent and its left page
					rightIndexNode.deleteFirstKeyFromIndexNode();
				}
			} else if (leftPage != -1) {
				//if left page is available
				DEBUG_B("Right page does not enough number of keys to rotate check eith left page number ")
				//				DEBUG_B("I AM HERE in left page to rotate")
				//				DEBUG_B("DO ROTATION WITH LEFT PAGE ")
				IndexNode leftIndexNode(indexHeader_, leftPage);
				DEBUG_B("no of keys in left page "<<leftIndexNode.getNoOfKeysInIndexNode());
				if ((leftIndexNode.getNoOfKeysInIndexNode() - 1) >= ceil(
						indexHeader_->getFanout() / 2)) {
					//rotate from left to right
					DEBUG_B("left page has more than half number of keys so rotate with left page keys")
					char seperatingKey[indexHeader_->getKeySize()];
					char lastKeyInLeftPage[indexHeader_->getKeySize()];
					leftIndexNode.getLastKeyPresentInIndexNode(
							lastKeyInLeftPage);
					int shiftingPageNumber =
							leftIndexNode.getLastPageNumberPresentInIndexNode();
					//					DEBUG_B("last key "<<lastKeyInLeftPage<<" last page no "<<shiftingPageNumber)
					if (getParentOfIndexNode() != -1) {
						IndexNode parentNode(indexHeader_,
								getParentOfIndexNode());
						//						parentNode.indexNodeOnlyDisplay();
						parentNode.getSeperatingKey(leftPage, getPageNumber(),
								seperatingKey);
						//						DEBUG_B("seperating key "<<seperatingKey)
						parentNode.replaceKey(seperatingKey, lastKeyInLeftPage);
					}
					insertAtTheFrontOfIndexNode(seperatingKey,
							shiftingPageNumber);
					if (getLevelOfIndexNode() == 1) {
						LeafNode rotateLeafNode(indexHeader_,
								shiftingPageNumber);
						rotateLeafNode.setParentPageNumber(getPageNumber());
					} else {
						IndexNode rotateIndexNode(indexHeader_,
								shiftingPageNumber);
						rotateIndexNode.setParentOfIndexNode(getPageNumber());
					}
					leftIndexNode.deleteLastKeyFromIndexNode();
					//					DEBUG_B("ROTATION ENDS HERE DELETION ENDS HERE")
				} else {
					//					DEBUG_B(" MERGING HERE first")
					DEBUG_B("both left and right pages has does not have enough number of keys so merge with right page number")
					//					DEBUG_B("MERGING HERE IF RIGHT AND LEFT PAGES DOES NOT HAVE ENOUGH NUMEBR OF KEY TO ROTATE")
					deletedPages.push_back(getPageNumber());
					//					DEBUG_B("curr "<<getPageNumber())
					//					DEBUG_B("left "<<getLeftPageNumberOfIndexNode())
					//					DEBUG_B("right "<<getRightPageNumber())
					char seperatingKey[indexHeader_->getKeySize()];
					if (getParentOfIndexNode() != -1) {
						IndexNode parentNode(indexHeader_,
								getParentOfIndexNode());
						parentNode.getSeperatingKey(getPageNumber(),
								getRightPageNumber(), seperatingKey);
					}
					//					DEBUG_B("seperating key "<<seperatingKey);
					int
							attachedPageNumber =
									rightIndexNode.getFirstPageNumberPresentInIndexNode();
					//					DEBUG_B("attched number "<<attachedPageNumber)
					char
							tempKeys[getNoOfKeysInIndexNode() + 1][indexHeader_->getKeySize()];
					int tempPageNumbers[getNoOfKeysInIndexNode() + 1];
					int tempCount = 0;
					int offset = IndexNode::getIndexNodeHeaderSize();
					DEBUG_B("Moving all keys from current node to right node ")
					DEBUG_B("deleting current node")
					for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
						memcpy(&tempPageNumbers[tempCount], &pageData_[offset],
								sizeof(int));
						offset += sizeof(int);
						memcpy(tempKeys[tempCount], &pageData_[offset],
								indexHeader_->getKeySize());
						offset = offset + indexHeader_->getKeySize();
						tempCount++;
					}
					memcpy(&tempPageNumbers[tempCount], &pageData_[offset],
							sizeof(int));
					memcpy(tempKeys[tempCount], seperatingKey,
							indexHeader_->getKeySize());
					for (int i = tempCount; i >= 0; i--) {
						//							DEBUG_B("values "<<tempPageNumbers[i]<<"  "<<tempKeys[i]<<"  "<<sendingNumber)
						rightIndexNode.insertAtTheFrontOfIndexNode(tempKeys[i],
								tempPageNumbers[i]);
					}
					if (getParentOfIndexNode() != -1) {
						IndexNode parentNode(indexHeader_,
								getParentOfIndexNode());
						parentNode.deleteKeyFromIndexNode(seperatingKey, true,
								deletedPages);
					}
					//					DEBUG_B("MERGING IS OVER ")
				}

				//				DEBUG_B("IT ENDS HERE*********************************************")
			} else {
				//do merging with right page here
				//				DEBUG_B("DO MERGE WITH RIGHT PAGE second")
				//				DEBUG_B("MERGE HERE IF RIGHT PAGE EXISTED AND LEFT PAGE NOT EXISTED SO MERGE WITH RIGHT PAGE")
				//				DEBUG_B("MERGING STARTS ********************************************************************")
				DEBUG_B("merging with right page number because left page does not exist")
				deletedPages.push_back(getPageNumber());
				//				DEBUG_B("curr "<<getPageNumber())
				//				DEBUG_B("left "<<getLeftPageNumberOfIndexNode())
				//				DEBUG_B("right "<<getRightPageNumber())
				char seperatingKey[indexHeader_->getKeySize()];
				if (getParentOfIndexNode() != -1) {
					IndexNode parentNode(indexHeader_, getParentOfIndexNode());
					parentNode.getSeperatingKey(getPageNumber(),
							getRightPageNumber(), seperatingKey);
				}
				//				DEBUG_B("seperating key "<<seperatingKey);
				int attachedPageNumber =
						rightIndexNode.getFirstPageNumberPresentInIndexNode();
				//				DEBUG_B("attched number "<<attachedPageNumber)
				char
						tempKeys[getNoOfKeysInIndexNode() + 1][indexHeader_->getKeySize()];
				int tempPageNumbers[getNoOfKeysInIndexNode() + 1];
				int tempCount = 0;
				int offset = IndexNode::getIndexNodeHeaderSize();
				DEBUG_B("Moving all keys from current node to right node")
				for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
					memcpy(&tempPageNumbers[tempCount], &pageData_[offset],
							sizeof(int));
					offset += sizeof(int);
					memcpy(tempKeys[tempCount], &pageData_[offset],
							indexHeader_->getKeySize());
					offset = offset + indexHeader_->getKeySize();
					tempCount++;
				}
				memcpy(&tempPageNumbers[tempCount], &pageData_[offset],
						sizeof(int));
				memcpy(tempKeys[tempCount], seperatingKey,
						indexHeader_->getKeySize());
				for (int i = tempCount; i >= 0; i--) {
					//							DEBUG_B("values "<<tempPageNumbers[i]<<"  "<<tempKeys[i]<<"  "<<sendingNumber)
					rightIndexNode.insertAtTheFrontOfIndexNode(tempKeys[i],
							tempPageNumbers[i]);
				}
				if (getParentOfIndexNode() != -1) {
					IndexNode parentNode(indexHeader_, getParentOfIndexNode());
					parentNode.deleteKeyFromIndexNode(seperatingKey, true,
							deletedPages);
				}
				//				DEBUG_B("MERGING IS OVER ")

				//				DEBUG_B("MERGING ENDS ***********************************************************************")
			}

		} else if (leftPage != -1) {
			DEBUG_B("checking left page values")
			IndexNode leftIndexNode(indexHeader_, leftPage);
			if ((leftIndexNode.getNoOfKeysInIndexNode() - 1) >= ceil(
					indexHeader_->getFanout() / 2)) {
				//rotate from left to right
				DEBUG_B("left page has more than enough number of keys so rotate keys from left page")
				char seperatingKey[indexHeader_->getKeySize()];
				char lastKeyInLeftPage[indexHeader_->getKeySize()];
				leftIndexNode.getLastKeyPresentInIndexNode(lastKeyInLeftPage);
				int shiftingPageNumber =
						leftIndexNode.getLastPageNumberPresentInIndexNode();
				//				DEBUG_B("last key "<<lastKeyInLeftPage<<" last page no "<<shiftingPageNumber)
				if (getParentOfIndexNode() != -1) {
					IndexNode parentNode(indexHeader_, getParentOfIndexNode());
					parentNode.getSeperatingKey(leftPage, getPageNumber(),
							seperatingKey);
					//					DEBUG_B("seperating key "<<seperatingKey)
					parentNode.replaceKey(seperatingKey, lastKeyInLeftPage);
				}
				insertAtTheFrontOfIndexNode(seperatingKey, shiftingPageNumber);
				if (getLevelOfIndexNode() == 1) {
					LeafNode rotateLeafNode(indexHeader_, shiftingPageNumber);
					rotateLeafNode.setParentPageNumber(getPageNumber());
				} else {
					IndexNode rotateIndexNode(indexHeader_, shiftingPageNumber);
					rotateIndexNode.setParentOfIndexNode(getPageNumber());
				}
				leftIndexNode.deleteLastKeyFromIndexNode();
				//				DEBUG_B("ROTATION ENDS HERE DELETION ENDS HERE")
			} else {
				//				DEBUG_B(" MERGING WITH LEFT PAGE HERE")
				//				DEBUG_B("MERGING HERE WITH LEFT PAGE NUMBER ")
				DEBUG_B("Left page does not have enough number of keys so merge with left page number")
				deletedPages.push_back(getPageNumber());
				//				DEBUG_B("curr "<<getPageNumber())
				//				DEBUG_B("left "<<getLeftPageNumberOfIndexNode())
				char seperatingKey[indexHeader_->getKeySize()];
				if (getParentOfIndexNode() != -1) {
					IndexNode parentNode(indexHeader_, getParentOfIndexNode());
					parentNode.getSeperatingKey(getLeftPageNumberOfIndexNode(),
							getPageNumber(), seperatingKey);
				}
				//				DEBUG_B("seperating key "<<seperatingKey);
				int attachedPageNumber = getFirstPageNumberPresentInIndexNode();
				DEBUG_B("attched number "<<attachedPageNumber)
				char
						tempKeys[getNoOfKeysInIndexNode() + 1][indexHeader_->getKeySize()];
				int tempPageNumbers[getNoOfKeysInIndexNode() + 1];
				int tempCount = 0;
				int offset = IndexNode::getIndexNodeHeaderSize();
				memcpy(tempKeys[tempCount], seperatingKey,
						indexHeader_->getKeySize());
				DEBUG_B("moving all keys from current node to left node")
				for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
					memcpy(&tempPageNumbers[tempCount], &pageData_[offset],
							sizeof(int));
					offset = offset + sizeof(int);
					memcpy(tempKeys[tempCount], &pageData_[offset],
							indexHeader_->getKeySize());
					offset = offset + indexHeader_->getKeySize();
					tempCount++;
				}
				memcpy(&tempPageNumbers[tempCount], &pageData_[offset],
						sizeof(int));
				//				leftIndexNode.indexNodeOnlyDisplay();
				//				DEBUG_B("values "<<seperatingKey<<"  "<<tempPageNumbers[0])
				leftIndexNode.insertAtTheEndOfIndexNode(seperatingKey,
						tempPageNumbers[0]);
				for (int i = 0; i < tempCount; i++) {
					//					DEBUG_B("values "<<tempKeys[i]<<"  "<<tempPageNumbers[i+1])
					leftIndexNode.insertAtTheEndOfIndexNode(tempKeys[i],
							tempPageNumbers[i + 1]);
				}
				if (getParentOfIndexNode() != -1) {
					IndexNode parentNode(indexHeader_, getParentOfIndexNode());
					parentNode.deleteKeyFromIndexNode(seperatingKey, false,
							deletedPages);
				}
				//				DEBUG_B("MERGING IS OVER ")

				//				DEBUG_B(" I AM HERE IF I AM CORRECT")
				//				int x;
				//				cin >> x;
			}
		}
	}

	return SUCCESS;
}
void IndexNode::deleteFirstKeyFromIndexNode() {
	int offset = IndexNode::getIndexNodeHeaderSize();
	char tempKeys[getNoOfKeysInIndexNode() - 1][indexHeader_->getKeySize()];
	int tempPageNumbers[getNoOfKeysInIndexNode()];
	int tempCount = 0;
	offset = offset + sizeof(int) + indexHeader_->getKeySize();
	for (int i = 1; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&tempPageNumbers[tempCount], &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(&tempKeys[tempCount], &pageData_[offset],
				indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		tempCount++;
	}
	memcpy(&tempPageNumbers[tempCount], &pageData_[offset], sizeof(int));
	offset = IndexNode::getIndexNodeHeaderSize();
	for (int i = 0; i < tempCount; i++) {
		memcpy(&pageData_[offset], &tempPageNumbers[i], sizeof(int));
		offset += sizeof(int);
		memcpy(&pageData_[offset], tempKeys[i], indexHeader_->getKeySize());
		offset += indexHeader_->getKeySize();
	}
	memcpy(&pageData_[offset], &tempPageNumbers[tempCount], sizeof(int));
	setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() - 1);
}
void IndexNode::deleteLastKeyFromIndexNode() {
	setNoOfKeysInIndexNode(getNoOfKeysInIndexNode() - 1);
	//if possible delete all page numbers
}
int IndexNode::isKeyFound(char* key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	offset = offset + sizeof(int);
	char temp[indexHeader_->getKeySize()];
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(temp, &pageData_[offset], indexHeader_->getKeySize());
		//		dummyKeyCompare(temp, key)
		if (BPlusTreeUtil::keyCompare(temp, key, indexHeader_) == 0)
			return 0;
		offset = indexHeader_->getKeySize() + sizeof(int);
	}
	return 1;
}
STATUS_CODE IndexNode::replaceKey(const char* oldKey, const char* newKey) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	//	DEBUG_B("ITS PAGE NUMBER FOR REPLACING"<<getPageNumber())
	//	cout<<" given keys "<<oldKey<<"  "<<newKey<<endl;
	offset = offset + sizeof(int);
	char temp[indexHeader_->getKeySize()];
	int found = 0;
	//	cout<<" num of keys "<<getNoOfKeysInIndexNode()<<endl;
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(temp, &pageData_[offset], indexHeader_->getKeySize());
		//		DEBUG_B("KEY COMPARE "<<temp<<"  "<<oldKey<<"  "<<i)
		//		dummyKeyCompare(temp, oldKey)
		if (BPlusTreeUtil::keyCompare(temp, oldKey, indexHeader_) == 0) {
			memcpy(&pageData_[offset], newKey, indexHeader_->getKeySize());
			found = 1;
			break;
		}
		offset += indexHeader_->getKeySize() + sizeof(int);
	}
	if (found == 0) {
		if (indexHeader_->getHeightOfTheTree() >= getLevelOfIndexNode()
				&& getParentOfIndexNode() != -1) {
			IndexNode indexNode(indexHeader_, getParentOfIndexNode());
			indexNode.replaceKey(oldKey, newKey);
		}
	}
	return SUCCESS;
}

/*int IndexNode::foundInIndexNode(char* key)
 {
 int offset=IndexNode::getIndexNodeHeaderSize();
 offset=offset+sizeof(int);
 char tempKey[indexHeader_->getKeySize()];
 int status=-1;
 for(int i=0;i<indexNodePageHeader_.noOfKeys;i++)
 {
 memcpy(tempKey,&pageData_[offset],indexHeader_->getKeySize());
 status=dummyKeyCompare(tempKey,key);
 }
 }
 */
int IndexNode::dummyKeyCompare(char* key1, char* key2) {
	return strcmp(key1, key2);
}
STATUS_CODE IndexNode::replaceKeyPreviousToPageNumber(int pageNumber,
		const char *oldKey, const char *newKey) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int leftPage;
	char tempKey[indexHeader_->getKeySize()];
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&leftPage, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
		offset += indexHeader_->getKeySize();
		if (leftPage == pageNumber) {
			//			dummyKeyCompare(tempKey, oldKey)

			if (BPlusTreeUtil::keyCompare(tempKey, oldKey, indexHeader_) == 0) {
				offset = offset - indexHeader_->getKeySize();
				memcpy(&pageData_[offset], newKey, indexHeader_->getKeySize());
				return SUCCESS;
			}
		}
	}
}
int IndexNode::searchInIndexNode(const char* key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	char tempKey[indexHeader_->getKeySize()];
	int leftPageNumber, rightPageNumber;
	for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
		memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rightPageNumber, &pageData_[offset], sizeof(int));
		//		dummyKeyCompare(tempKey, key)
		int compValue = BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_);
		//		DEBUG_B(" temp key "<<tempKey<<" new key "<<key)
		//		DEBUG_B("left page number "<<leftPageNumber)
		//		DEBUG_B("comparision return value "<<compValue<<endl)
		if (compValue == 0)
			return rightPageNumber;
		if (compValue == 1)
			return leftPageNumber;
		//DEBUG_B("found left page Number "<<leftPageNumber);
	}
	memcpy(&rightPageNumber, &pageData_[offset], sizeof(int));
	offset = offset + sizeof(int);
	return rightPageNumber;
}
void IndexNode::getSeperatingKey(int leftPage, int rightPage, char *key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int lpno, rpno;
	char temp[indexHeader_->getKeySize()];
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&lpno, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(temp, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rpno, &pageData_[offset], sizeof(int));
		if (leftPage == lpno && rightPage == rpno) {
			memcpy(key, temp, indexHeader_->getKeySize());
			break;
		}
	}
}
int IndexNode::getLevelOfIndexNode() {
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	return indexNodePageHeader_.level;
}
void IndexNode::setLevelOfIndexNode(int level) {
	indexNodePageHeader_.level = level;
	memcpy(pageData_, &indexNodePageHeader_, sizeof(IndexNodePageHeaderStruct));
}
int IndexNode::getNoOfKeysInIndexNode() {
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	return indexNodePageHeader_.noOfKeys;
}
void IndexNode::setNoOfKeysInIndexNode(int noOfKeys) {
	indexNodePageHeader_.noOfKeys = noOfKeys;
	memcpy(pageData_, &indexNodePageHeader_, sizeof(IndexNodePageHeaderStruct));
}
int IndexNode::getParentOfIndexNode() {
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	return indexNodePageHeader_.parentPageNumber;
}
void IndexNode::setParentOfIndexNode(int parentPageNumber) {
	indexNodePageHeader_.parentPageNumber = parentPageNumber;
	memcpy(pageData_, &indexNodePageHeader_, sizeof(IndexNodePageHeaderStruct));
}
int IndexNode::getLeftPageNumberOfIndexNode() {
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	return indexNodePageHeader_.leftPageNumber;
}
void IndexNode::setLeftPageNumberOfIndexNode(int leftPageNumber) {
	indexNodePageHeader_.leftPageNumber = leftPageNumber;
	memcpy(pageData_, &indexNodePageHeader_, sizeof(IndexNodePageHeaderStruct));
}
int IndexNode::getRightPageNumber() {
	memcpy(&indexNodePageHeader_, pageData_, sizeof(IndexNodePageHeaderStruct));
	return indexNodePageHeader_.rightPageNumber;
}
void IndexNode::setRightPageNumber(int rightPageNumber) {
	indexNodePageHeader_.rightPageNumber = rightPageNumber;
	memcpy(pageData_, &indexNodePageHeader_, sizeof(IndexNodePageHeaderStruct));
}
int IndexNode::getPageNumber() {
	return indexNodePageHeader_.generalPageHeaderStruct.pageNumber;
}
void IndexNode::getFirstKeyPresentInIndexNode(char* key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	offset = offset + sizeof(int);
	memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
}
void IndexNode::getLastKeyPresentInIndexNode(char* key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	offset = offset + (getNoOfKeysInIndexNode() * (sizeof(int)
			+ indexHeader_->getKeySize()));
	offset = offset - indexHeader_->getKeySize();
	memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
}
int IndexNode::getFirstPageNumberPresentInIndexNode() {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int pageNumber;
	memcpy(&pageNumber, &pageData_[offset], sizeof(int));
	return pageNumber;
}
int IndexNode::getLastPageNumberPresentInIndexNode() {
	int offset = IndexNode::getIndexNodeHeaderSize();
	offset = offset + indexNodePageHeader_.noOfKeys * (sizeof(int)
			+ indexHeader_->getKeySize());
	int pageNumber;
	memcpy(&pageNumber, &pageData_[offset], sizeof(int));
	return pageNumber;
}
int IndexNode::getIndexNodeHeaderSize() {
	return sizeof(IndexNodePageHeaderStruct);
}
STATUS_CODE IndexNode::getSiblings(int givenPageNumber, int &leftSibling,
		int &rightSibling) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int currentLeftPageNumber = -1;
	int currentRightPageNumber = -1;
	int previousLeftPageNumber = -1;
	int found = 0;
	//	DEBUG_B("no of keys "<<getNoOfKeysInIndexNode())
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&currentLeftPageNumber, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
		memcpy(&currentRightPageNumber, &pageData_[offset], sizeof(int));
		//		DEBUG_B("curr left "<<currentLeftPageNumber<<"  curr right "<<currentRightPageNumber);
		if (currentLeftPageNumber == givenPageNumber) {
			//			DEBUG_B("prev"<<previousLeftPageNumber<<" curr "<<currentRightPageNumber);
			leftSibling = previousLeftPageNumber;
			rightSibling = currentRightPageNumber;
			return SUCCESS;
		}
		previousLeftPageNumber = currentLeftPageNumber;
	}
	leftSibling = previousLeftPageNumber;
	rightSibling = -1;
	return SUCCESS;

}
void IndexNode::getKeyNextToPageNumber(int pageNumber, char* key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int pno;
	char temp[indexHeader_->getKeySize()];
	int i;
	for (i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&pno, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(temp, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		if (pno == pageNumber) {
			memcpy(key, temp, indexHeader_->getKeySize());
			break;
		}
	}
	if (i == getNoOfKeysInIndexNode()) {
		memcpy(key, temp, indexHeader_->getKeySize());
	}
}
void IndexNode::getKeyPreviousToPageNumber(int pageNumber, char* key) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int pno;
	char temp[indexHeader_->getKeySize()];
	memcpy(&pno, &pageData_[offset], sizeof(int));
	offset = offset + sizeof(int);
	if (pno == pageNumber) {
		key = NULL;
	} else {
		for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
			memcpy(temp, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			memcpy(&pno, &pageData_[offset], sizeof(int));
			offset = offset + sizeof(int);
			if (pno == pageNumber) {
				memcpy(key, temp, indexHeader_->getKeySize());
				break;
			}
		}
	}
}
int IndexNode::getPageNumberNextToGivenPageNumber(int givenPageNumber) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int pageno;
	int resultPageNo;
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&pageno, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
		memcpy(&resultPageNo, &pageData_[offset], sizeof(int));
		if (pageno == givenPageNumber) {
			return pageno;
		}
	}
	return -1;
}
int IndexNode::getPageNumberPreviousToGivenPageNumber(int givenPageNumber) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int pageno, resultPageNo;
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&pageno, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
		memcpy(&resultPageNo, &pageData_[offset], sizeof(int));
		if (resultPageNo == givenPageNumber) {
			return pageno;
		}
	}
	return -1;
}
void IndexNode::indexNodeOnlyDisplay() {
	int offset = IndexNode::getIndexNodeHeaderSize();
	char tp[indexHeader_->getKeySize()];
	int leftPageNumber, rightPageNumber;
	for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
		memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(tp, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		//		cout << leftPageNumber << "  " << tp << "   ";
	}
	memcpy(&rightPageNumber, &pageData_[offset], sizeof(int));
	//	cout << rightPageNumber << endl;
}
void IndexNode::display() {
	cout << "CURRENT INDEX PAGE NUMBER\t" << getPageNumber() << endl;
	cout << "THIS PARENT PARE NUMBER\t\t" << getParentOfIndexNode() << endl;
	cout << "LEFT PAGE NUMBER\t\t" << getLeftPageNumberOfIndexNode() << endl;
	cout << "RIGHT PAGE NUMBER\t\t" << getRightPageNumber() << endl;
	cout << "NO OF KEYS IN INDEX NODE\t" << indexNodePageHeader_.noOfKeys
			<< endl;
	cout << "LEVEL OF NODE\t\t\t" << getLevelOfIndexNode() << endl;
	int leftPageNumber;
	int rightPageNumber;
	char existingKey[indexHeader_->getKeySize()];
	int level = getLevelOfIndexNode();
	DEBUG_B("CONTENTS OF INDEX NODE ")
	indexNodeOnlyDisplay();
	int offset = IndexNode::getIndexNodeHeaderSize();
	for (int i = 0; i < indexNodePageHeader_.noOfKeys; i++) {
		memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
		//		DEBUG_B("left page"<<leftPageNumber);
		offset = offset + sizeof(int);
		memcpy(existingKey, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		//		DEBUG_B("KEY "<<existingKey);
		memcpy(&rightPageNumber, &pageData_[offset], sizeof(int));
		//		DEBUG_B(" right page "<<rightPageNumber)
		//		offset=offset+sizeof(int);
		cout << leftPageNumber << "  " << existingKey << "   "
				<< rightPageNumber << endl;
		if (level == 1) {
			LeafNode leafNode(indexHeader_, leftPageNumber);
			//			leafNode.display();
			cout << "----------NEXT LEAF PAGE -------------------------------"
					<< endl;
			LeafNode leafNode1(indexHeader_, rightPageNumber);
			leafNode1.display();
		} else {
			IndexNode indexNode(indexHeader_, leftPageNumber);
			indexNode.display();
			IndexNode indexNode1(indexHeader_, rightPageNumber);
			indexNode1.display();
		}

	}

	//			LeafNode leafNode1(indexHeader_,rightPageNumber);
	//	 		leafNode1.display();
}
void IndexNode::UIIndexNode() {
	//	cout << "\n I AM IN UI OF INDEX NODE " << endl;
	cout<<endl;
	cout << "\tCurrent Index Page Number ------------------------: "
			<< getPageNumber() << endl;
	cout << "\tCurrent Level of Index Page ----------------------: "
			<< getLevelOfIndexNode() << endl;
	cout << "\tParent page number -------------------------------: "
			<< getParentOfIndexNode() << endl;
	cout << "\tNumber of keys in Index Page ---------------------: "
			<< getNoOfKeysInIndexNode() << endl;
	cout
			<< "\t***********************DATA IN INDEX NODE **************************"
			<< endl;
	cout<<"\t";
	int offset = IndexNode::getIndexNodeHeaderSize();
	int leftPageNumber;
	char key[indexHeader_->getKeySize()];
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		cout << " [" << leftPageNumber << "]  ";
		BPlusTreeUtil::displayKey(key, indexHeader_);
	}
	memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
	cout << " [" << leftPageNumber << "]" << endl;
	cout
			<< "\t********************* END OF DATA IN INDEX NODE ******************** "
			<< endl;
	cout << "\tLeft Index Page Number ---------------------------:"
			<< getLeftPageNumberOfIndexNode() << endl;
	cout << "\tRight Index Page Number --------------------------:"
			<< getRightPageNumber() << endl;

}
void IndexNode::UIDisplayOfIndexNode(int numberOfTabs) {
	int offset = IndexNode::getIndexNodeHeaderSize();
	int pageNumbers[getNoOfKeysInIndexNode() + 1];
	char keys[getNoOfKeysInIndexNode()][indexHeader_->getKeySize()];
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&pageNumbers[i], &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(&keys[i], &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
	}
	memcpy(&pageNumbers[getNoOfKeysInIndexNode()], &pageData_[offset],
			sizeof(int));
	for (int tab = 0; tab < numberOfTabs; tab++) {
		cout << "\t";
	}
	cout << "Index Page Number : " << getPageNumber() << endl;
	for (int tab = 0; tab < numberOfTabs; tab++) {
		cout << "\t";
	}

	cout << "Level of Index Node : " << getLevelOfIndexNode() << endl;
	for (int tab = 0; tab < numberOfTabs; tab++) {
		cout << "\t";
	}
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		cout << "[" << pageNumbers[i] << "]" << "|";
		BPlusTreeUtil::displayKey(keys[i], indexHeader_);
		cout << "|";
	}
	cout << "[" << pageNumbers[getNoOfKeysInIndexNode()] << "]" << endl;
	int updateTabs = numberOfTabs + indexHeader_->getFanout();//getNoOfKeysInIndexNode();
	if (getLevelOfIndexNode() == 1) {
		for (int k2 = 0; k2 <= getNoOfKeysInIndexNode(); k2++) {
			LeafNode leafNode(indexHeader_, pageNumbers[k2]);
			leafNode.UIDisplayOfLeafNode(updateTabs);
		}
	} else {
		for (int k1 = 0; k1 <= getNoOfKeysInIndexNode(); k1++) {
			IndexNode indexNode(indexHeader_, pageNumbers[k1]);
			indexNode.UIDisplayOfIndexNode(updateTabs);
		}
	}
}
void IndexNode::deleteChilds(std::vector<int> &allPageNumbers) {
	int level = getLevelOfIndexNode();
	int offset = getIndexNodeHeaderSize();
	int pageNumber1;

	for (int i = 0; i < getNoOfKeysInIndexNode() + 1; i++) {
		memcpy(&pageNumber1, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
		if (level > 1) {
			//			cout<<"\n\tdelete index nodes : "<<pageNumber1<<endl;
			IndexNode indexNode(indexHeader_, pageNumber1);
			indexNode.deleteChilds(allPageNumbers);
			allPageNumbers.push_back(pageNumber1);
		}
		if (level == 1) {
			//			cout<<"\n\tdelete leaf nodes : "<<pageNumber1<<endl;
			allPageNumbers.push_back(pageNumber1);
		}
	}
}
void IndexNode::findPages(std::vector<int> &leafPages,
		std::vector<int> &indexPages) {
	int level = getLevelOfIndexNode();
	int offset = getIndexNodeHeaderSize();
	int pageNumber1;
	for (int i = 0; i < getNoOfKeysInIndexNode() + 1; i++) {
		memcpy(&pageNumber1, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
		if (level > 1) {
			IndexNode indexNode(indexHeader_, pageNumber1);
			indexNode.findPages(leafPages, indexPages);
			indexPages.push_back(pageNumber1);
		}
		if (level == 1) {
			leafPages.push_back(pageNumber1);
		}
	}
}
std::string IndexNode::toString()
{
	DEBUG("INSIDE INDEX PAGE --  TO STRING");
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "\tINDEX HEADER PAGE NO: " << getIndexHeaderPageNumber() << "\n";

	mystream << "\tLevel of Index Page ----------------------: "
			<< getLevelOfIndexNode() << "\n";
	mystream << "\tParent page number -------------------------------: "
			<< getParentOfIndexNode() << "\n";
	mystream << "\tNumber of keys in Index Page ---------------------: "
			<< getNoOfKeysInIndexNode() << "\n";
	mystream
			<< "\t***********************DATA IN INDEX NODE **************************"
			<< "\n";
	mystream<<"\t";
	int offset = IndexNode::getIndexNodeHeaderSize();
	int leftPageNumber;
	char key[indexHeader_->getKeySize()];
	for (int i = 0; i < getNoOfKeysInIndexNode(); i++) {
		memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int);
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		mystream << " [" << leftPageNumber << "]  ";
		mystream << BPlusTreeUtil::KeytoString(key, indexHeader_);
	}
	memcpy(&leftPageNumber, &pageData_[offset], sizeof(int));
	mystream << " [" << leftPageNumber << "]" << "\n";
	mystream
			<< "\t********************* END OF DATA IN INDEX NODE ******************** "
			<< "\n";
	mystream << "\tLeft Index Page Number ---------------------------:"
			<< getLeftPageNumberOfIndexNode() << "\n";
	mystream << "\tRight Index Page Number --------------------------:"
			<< getRightPageNumber() << "\n";
	return mystream.str();
}


void IndexNode::findFirstLeafPage(std::vector<int> &leafPages)
{
	int level = getLevelOfIndexNode();
	int offset = getIndexNodeHeaderSize();
	int pageNumber1;
	for (int i = 0; i < getNoOfKeysInIndexNode() + 1; i++) {
		memcpy(&pageNumber1, &pageData_[offset], sizeof(int));
		offset = offset + sizeof(int) + indexHeader_->getKeySize();
		if (level > 1) {
			IndexNode indexNode(indexHeader_, pageNumber1);
			indexNode.findFirstLeafPage(leafPages);
		}
		if (level == 1) {
			leafPages.push_back(pageNumber1);
		}
	}

}
