/*
 * LeafNode.cpp
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#include "LeafNode.h"
#include "IndexNode.h"
#include "IndexHeaderPage.h"
#include "BPlusTree.h"
#include "BPlusTreeUtil.h"
#include "../utils/debug.h"
#include "../global/StatusCodes.h"
#include "../bufmgr/BufferManager.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../global/GlobalDefines.h"
#include <iostream>
#include <string.h>
#include <math.h>
#include <assert.h>
#include<stdlib.h>
using namespace std;
int LeafNode::testPinCount = 0;
LeafNode::LeafNode() {
	// TODO Auto-generated constructor stub
	indexHeaderObjCreatedHere = false;
}
LeafNode::LeafNode(int leafPageNumber) {
	BufferManager *bufMgr = BufferManager::getInstance();
	//		DEBUG_B("Pinning leaf page with with buffer manager")
	int error = bufMgr->pinAndGetPage(leafPageNumber, pageData_);
	if (error != SUCCESS) {
		DEBUG_B("ERROR IN CREATE LEAF PAGE");
		//			return error;
	}
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	int indexHeaderPageNumber = getIndexHeaderPageNumber();
	indexHeader_ = new IndexHeaderPage(indexHeaderPageNumber);
	indexHeaderObjCreatedHere = true;
}
int LeafNode::getIndexHeaderPageNumber() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.indexHeaderPageNumber;
}
LeafNode::LeafNode(IndexHeaderPage *indexHeaderPage, int leafPageNumber) {
	//	DEBUG_B("Creating leaf page with page number "<<leafPageNumber)
	// TODO Auto-generated constructor stub
	indexHeaderObjCreatedHere = false;
	indexHeader_ = indexHeaderPage;
	BufferManager *bufMgr = BufferManager::getInstance();
	//		DEBUG_B("Pinning leaf page with with buffer manager")
	int error = bufMgr->pinAndGetPage(leafPageNumber, pageData_);
	if (error != SUCCESS) {
		DEBUG_B("ERROR IN CREATE LEAF PAGE");
		//			return error;
	}
	//	DEBUG_B("ERROR "<<error)
	//	DEBUG_B("leaf page accessed "<<leafPageNumber)
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	//	testPinCount++;
}

LeafNode::~LeafNode() {
	// TODO Auto-generated destructor stub
	//	DEBUG_B("In LeafNode destructor")
	BufferManager *bufMgr = BufferManager::getInstance();
	//	DEBUG_B("Unpinning leaf page with buffer manager : "<<leafNodePageHeader_.generalPageHeader.pageNumber)
	bufMgr->unPinPage(leafNodePageHeader_.generalPageHeader.pageNumber, true);
	//	DEBUG_B("HERE UNPINNING LEAF PAGE"<<leafNodePageHeader_.generalPageHeader.pageNumber)
	//	testPinCount--;
	if(indexHeaderObjCreatedHere == true){
		delete indexHeader_;
	}
}

STATUS_CODE LeafNode::createLeafPage(IndexHeaderPage *indexHeaderPage) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int newPageNumber;
	int error = bufMgr->newPage(newPageNumber, pageData_);
	if (error != SUCCESS) {
		DEBUG_B("ERROR IN CREATE LEAF PAGE");
		return error;
	}
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	leafNodePageHeader_.generalPageHeader.pageType = TYPE_LEAF_PAGE;
	leafNodePageHeader_.level = 0;//All leaf nodes are at level 0
	leafNodePageHeader_.noOfRecords = 0;
	leafNodePageHeader_.parentPageNumber = -1;
	leafNodePageHeader_.leftPageNumber = -1;
	leafNodePageHeader_.rightPageNumber = -1;
	leafNodePageHeader_.indexHeaderPageNumber
			= indexHeaderPage->getPageNumber();
	memcpy(pageData_, &leafNodePageHeader_, sizeof(LeafNodePageHeaderStruct));
	//	indexHeaderPage->pushLeafPageNumbers(newPageNumber);
	indexHeader_ = indexHeaderPage;
	if (indexHeader_->getRootPage() == -1) {
		DEBUG_B("If root page is -1 add this page as root of tree")
		indexHeader_->setRootPage(newPageNumber);
	}
	return SUCCESS;
}

void LeafNode::getKey(int keyNumber, char* key1) {
	int offset = getLeafNodeHeaderSize() + keyNumber
			* (indexHeader_->getKeySize() + RID_LEN);
	memcpy(key1, &pageData_[offset], sizeof(indexHeader_->getKeySize()));
	//	DEBUG_B("IN CAMP FUNCTION "<<key1<<"  keynumber "<<keyNumber)
	//	display();
}
void LeafNode::getKeyAndRID(int keyNumber, char* key1, RIDStruct &rid) {
	int offset = getLeafNodeHeaderSize() + keyNumber
			* (indexHeader_->getKeySize() + RID_LEN);
	memcpy(key1, &pageData_[offset], sizeof(indexHeader_->getKeySize()));
	offset = offset + indexHeader_->getKeySize();
	memcpy(&rid, &pageData_[offset], RID_LEN);
	//	DEBUG_B("IAM LEAF NODE key ")
	//	BPlusTreeUtil::displayKey(key1,indexHeader_);
	//	cout<<endl;
}
int LeafNode::dummyKeyCompare(char* key1, char* key2) {
	return strcmp(key1, key2);
}
STATUS_CODE LeafNode::insertIntoLeafPage(const char* key, RIDStruct rid) {
	char tempKey[indexHeader_->getKeySize()];
	DEBUG_B("Inserting in Leaf node : "<<getPageNumber())
	DEBUG_B("Number of records in this leaf page : "<<getNoOfRecordsInNode())
	int i = 0;
	if (leafNodePageHeader_.noOfRecords
			== indexHeader_->getMaxNoOfRecordsInLeaf()) {
		//		cout<<"\n here "<<endl;
		//		cout<<"\n initial "<<getPageNumber()<<"  "<<getLeftPageNumber()<<"  "<<getRightPageNumber()<<endl;
		DEBUG_B("current number of records is already equal to MAXIMUM number of records in leaf page")
		DEBUG_B("So create new leaf page and distribute half keys to new page")
		int promoteLeafLeftPage, promoteLeafRightPage;
		char promoteKey[indexHeader_->getKeySize()];
		char
				tempKeys[leafNodePageHeader_.noOfRecords + 1][indexHeader_->getKeySize()];
		RIDStruct tempRIDS[leafNodePageHeader_.noOfRecords + 1];
		int tempCount = 0, found = 0;
		int offset = LeafNode::getLeafNodeHeaderSize();
		for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
			memcpy(tempKeys[tempCount], &pageData_[offset],
					indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			memcpy(&tempRIDS[tempCount], &pageData_[offset], RID_LEN);
			//			 dummyKeyCompare(tempKeys[tempCount], key);

			int comp = BPlusTreeUtil::keyCompare(tempKeys[tempCount], key,
					indexHeader_);
			if (comp == 1 && found == 0) {
				//DEBUG_B(" location found")
				char temp[indexHeader_->getKeySize()];
				RIDStruct temprid;
				memcpy(temp, tempKeys[tempCount], indexHeader_->getKeySize());
				temprid = tempRIDS[tempCount];
				memcpy(tempKeys[tempCount], key, indexHeader_->getKeySize());
				tempRIDS[tempCount] = rid;
				tempCount++;
				memcpy(tempKeys[tempCount], temp, indexHeader_->getKeySize());
				tempRIDS[tempCount] = temprid;
				found = 1;
			}
			offset = offset + RID_LEN;
			tempCount++;
		}

		if (found == 0) {
			//			DEBUG_B("TEMP COUNT "<<tempCount)
			memcpy(tempKeys[tempCount], key, indexHeader_->getKeySize());
			memcpy(&tempRIDS[tempCount], &rid, RID_LEN);
			tempCount++;
		}
		/*		for (int i = 0; i < tempCount; i++) {
		 DEBUG_B(" temp keys --------------"<<tempKeys[i])
		 }
		 */
		//	cout<<"\n initial "<<getPageNumber()<<"  "<<getLeftPageNumber()<<"  "<<getRightPageNumber()<<endl;
		offset = LeafNode::getLeafNodeHeaderSize();
		leafNodePageHeader_.noOfRecords = 0;
		DEBUG_B("Storing first of half keys(one by one) to current node :")
		for (int i = 0; i < tempCount / 2; i++) {
			memcpy(&pageData_[offset], tempKeys[i], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			memcpy(&pageData_[offset], &tempRIDS[i], RID_LEN);
			offset = offset + RID_LEN;
			leafNodePageHeader_.noOfRecords++;
		}
		//	cout<<"\n initial "<<getPageNumber()<<"  "<<getLeftPageNumber()<<"  "<<getRightPageNumber()<<endl;
		//		DEBUG_B("LEAF NODE RECORDS "<<leafNodePageHeader_.noOfRecords)
		//	cout<<"\n initial "<<getPageNumber()<<"  "<<getLeftPageNumber()<<"  "<<getRightPageNumber()<<endl;
		//		DEBUG_B(" LEAF NODE IS SPLITTING SO IT REQUIRES NEW LEAF PAGE NUMBER ")
		LeafNode newLeafNode;
		newLeafNode.createLeafPage(indexHeader_);
		DEBUG_B("New leaf page number is : "<<newLeafNode.getPageNumber())
		promoteLeafLeftPage = leafNodePageHeader_.generalPageHeader.pageNumber;
		promoteLeafRightPage = newLeafNode.getPageNumber();
		memcpy(promoteKey, tempKeys[tempCount / 2], indexHeader_->getKeySize());
		//	cout<<"\n right page number "<<leafNodePageHeader_.rightPageNumber<<endl;
		if (leafNodePageHeader_.rightPageNumber != -1) {
			//			DEBUG_B(" I AM HERE ")
			LeafNode rightLeafNode(indexHeader_,
					leafNodePageHeader_.rightPageNumber);
			rightLeafNode.setLeftPageNumber(newLeafNode.getPageNumber());
		}
		newLeafNode.setRightPageNumber(leafNodePageHeader_.rightPageNumber);
		newLeafNode.setLeftPageNumber(
				leafNodePageHeader_.generalPageHeader.pageNumber);
		memcpy(pageData_, &leafNodePageHeader_,
				sizeof(LeafNodePageHeaderStruct));
		//	cout<<"\n current "<<leafNodePageHeader_.leftPageNumber<<"  "<<leafNodePageHeader_.generalPageHeader.pageNumber<<"  "<<leafNodePageHeader_.rightPageNumber<<endl;

		// now create new leaf node

		/*		for(int i=0;i<tempCount;i++)
		 DEBUG_B("after alter "<<tempKeys[i])*/
		//bufMgr->unPinPage(newLeafNode.getPageNumber(),true);
		//LeafNode newLeafNode1(indexHeader_, newLeafNode.getPageNumber());
		DEBUG_B("Storing remaining half keys to new leaf node : ")
		for (int i = tempCount / 2; i < tempCount; i++) {
			newLeafNode.insertIntoLeafPage(tempKeys[i], tempRIDS[i]);
		}
		//NOW CREATE INDEX NODE and add key to it
		//		DEBUG_B("Leaf Node Parent page "<<getParentPageNumber());
		//		DEBUG_B("SAME-------------------"<<leafNodePageHeader_.parentPageNumber)

		if (leafNodePageHeader_.parentPageNumber == -1) {
DEBUG			("Current leaf page does not have any parent create new index node and make it as parent for leaf nodes")
			DEBUG_B("Creating new index node ")
			IndexNode indexNode;
			indexNode.createIndexNode(indexHeader_);
			int indexPageNumber = indexNode.getPageNumber();
			DEBUG_B("new index node page number "<<indexPageNumber);
			DEBUG_B("Values promoting to index page :")
			DEBUG_B("leaf page number : "<<promoteLeafLeftPage)
			DEBUG_B("key : ")
			if(BPLUSTREE_DEBUG==true)
			{
				BPlusTreeUtil::displayKey(promoteKey,indexHeader_);
				DEBUG_B(endl)
			}
			DEBUG_B("right page number : "<<promoteLeafRightPage)
			indexNode.insertIntoIndexNode(promoteKey, promoteLeafLeftPage,
					promoteLeafRightPage);
			leafNodePageHeader_.parentPageNumber = indexPageNumber;
			memcpy(pageData_, &leafNodePageHeader_,
					sizeof(LeafNodePageHeaderStruct));
			//			DEBUG_B(" here page number "<<leafNodePageHeader_.generalPageHeader.pageNumber);
			setRightPageNumber(newLeafNode.getPageNumber());
			//		cout<<"END "<<leafNodePageHeader_.leftPageNumber<<endl;
			setLeftPageNumber(leafNodePageHeader_.leftPageNumber);

			//	DEBUG_B(" its parent "<<leafNodePageHeader_.parentPageNumber);
			//	newLeafNode.setParentPageNumber(indexPageNumber);
		} else {
			DEBUG_B("Current leaf page has parent page so promote values to parent node")
			DEBUG_B("Parent page number : "<<leafNodePageHeader_.parentPageNumber)
			IndexNode indexNode(indexHeader_,
					leafNodePageHeader_.parentPageNumber);
			//		DEBUG_B(" here page number "<<leafNodePageHeader_.generalPageHeader.pageNumber);
			//			DEBUG_B(" ITS PARENT PAGE NUMBER "<<leafNodePageHeader_.parentPageNumber);
			//			DEBUG_B("PROMOTEING DATA +++++"<<promoteKey<<"  "<<promoteLeafLeftPage<<"  "<<promoteLeafRightPage)
			DEBUG_B("Values promoting to index page :")
			DEBUG_B("leaf page number : "<<promoteLeafLeftPage)
			DEBUG_B("key : ")
			if(BPLUSTREE_DEBUG==true)
			{
				BPlusTreeUtil::displayKey(promoteKey,indexHeader_);
				DEBUG_B(endl)
			}
			DEBUG_B("right page number : "<<promoteLeafRightPage)
			newLeafNode.setParentPageNumber(
					leafNodePageHeader_.parentPageNumber);
			//		DEBUG_B("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
			//		display();
			indexNode.insertIntoIndexNode(promoteKey, promoteLeafLeftPage,
					promoteLeafRightPage);
			memcpy(&leafNodePageHeader_, pageData_,
					sizeof(LeafNodePageHeaderStruct));
			//			DEBUG_B(" ITS PARENT PAGE NUMBER "<<leafNodePageHeader_.parentPageNumber);
			//		DEBUG_B("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
			//		display();
			setRightPageNumber(newLeafNode.getPageNumber());
			//		cout<<"END "<<leafNodePageHeader_.leftPageNumber<<endl;
			setLeftPageNumber(leafNodePageHeader_.leftPageNumber);
			//			DEBUG_B(" ITS PARENT PAGE NUMBER "<<leafNodePageHeader_.parentPageNumber);
			//		cout<<"\n current "<<leafNodePageHeader_.leftPageNumber<<"  "<<leafNodePageHeader_.generalPageHeader.pageNumber<<"  "<<leafNodePageHeader_.rightPageNumber<<endl;
			//		cout<<"\n current data "<<getLeftPageNumber()<<"  "<<getPageNumber()<<"  "<<getRightPageNumber()<<endl;
			//		display();
			//			DEBUG_B("PROMOTEING DATA +++++"<<promoteKey<<"  "<<promoteLeafLeftPage<<"  "<<promoteLeafRightPage)
			//			DEBUG_B("index page number"<<indexNode.getPageNumber())
			//			DEBUG_B("current page number "<<getPageNumber())
			//			DEBUG_B("right page number "<<getRightPageNumber())
			/*
			 if (getRightPageNumber() != -1) {
			 LeafNode leafNode100(indexHeader_, getPageNumber());
			 //			leafNode100.display();
			 LeafNode leafNode200(indexHeader_, getRightPageNumber());
			 //			leafNode200.display();
			 }
			 */
		}
		//	cout<<"\n current "<<leafNodePageHeader_.leftPageNumber<<"  "<<leafNodePageHeader_.generalPageHeader.pageNumber<<"  "<<leafNodePageHeader_.rightPageNumber<<endl;
		return SUCCESS;
	} else {
		//if number of records is less than maximum number of nodes in leaf
		DEBUG_B("NO OF RECORDS IN LEAF "<<leafNodePageHeader_.noOfRecords)
		DEBUG_B("This leaf node enough space to accomodate new records")
		for (i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
			getKey(i, tempKey);
			//BPlusTreeUtil::keyCompare(tempKey,key,indexHeader_);
			int compValue = -1;
			//			DEBUG_B("key campare "<<tempKey<<"---"<<key<<" i value---"<<i)
			//			dummyKeyCompare(tempKey, key);
			compValue = BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_);
			//			DEBUG_B(" comparision value "<<compValue)
			if (compValue == 1)
			break;
		}
		if (leafNodePageHeader_.noOfRecords == 0) {
			DEBUG_B("it has no records till now")
			int offset = getLeafNodeHeaderSize();
			memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			memcpy(&pageData_[offset], &rid, RID_LEN);
			leafNodePageHeader_.noOfRecords = leafNodePageHeader_.noOfRecords
			+ 1;
			memcpy(pageData_, &leafNodePageHeader_,
					sizeof(LeafNodePageHeaderStruct));
		} else {
			if (leafNodePageHeader_.noOfRecords == i) {
				//insert at the end
				DEBUG_B("Inserting new key at the end of leaf node");
				int offset = LeafNode::getLeafNodeHeaderSize()
				+ leafNodePageHeader_.noOfRecords
				* (indexHeader_->getKeySize() + RID_LEN);
				memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
				offset = offset + indexHeader_->getKeySize();
				memcpy(&pageData_[offset], &rid, RID_LEN);
				leafNodePageHeader_.noOfRecords
				= leafNodePageHeader_.noOfRecords + 1;
				memcpy(pageData_, &leafNodePageHeader_,
						sizeof(LeafNodePageHeaderStruct));
			} else {
				//insert at first or the middle
				DEBUG_B("INSERTING AT LOCATION "<<i)
				char previousFirstKey[indexHeader_->getKeySize()];
				char tempo[indexHeader_->getKeySize()];
				//				DEBUG_B("left page "<<getLeftPageNumber())
				/*			if(getParentPageNumber()!=-1)
				 {
				 IndexNode indexNode2(indexHeader_,getParentPageNumber());
				 indexNode2.getKeyNextToPageNumber(getLeftPageNumber(),previousFirstKey);
				 cout<<"ACTUAL KEY "<<previousFirstKey<<endl;
				 }
				 */
				getFirstKey(previousFirstKey);
				//				DEBUG_B("HERE")
				getParentKey(previousFirstKey);
				//			cout<<" tempo key "<<tempo<<endl;
				//				cout << " previous key " << previousFirstKey << endl;
				char tempChar[leafNodePageHeader_.noOfRecords
				* (indexHeader_->getKeySize() + RID_LEN)];
				int offset = LeafNode::getLeafNodeHeaderSize() + i
				* (indexHeader_->getKeySize() + RID_LEN);
				memcpy(tempChar, &pageData_[offset],
						(leafNodePageHeader_.noOfRecords - (i))
						* (indexHeader_->getKeySize() + RID_LEN));
				memcpy(&pageData_[offset], key, indexHeader_->getKeySize());
				offset = offset + indexHeader_->getKeySize();
				memcpy(&pageData_[offset], &rid, RID_LEN);
				offset = offset + RID_LEN;
				//			DEBUG_B("REMAINING TOTAL LENGTH "<<(leafNodePageHeader_.noOfRecords - (i))	* (indexHeader_->getKeySize() + RID_LEN))
				memcpy(&pageData_[offset], tempChar,
						(leafNodePageHeader_.noOfRecords - (i))
						* (indexHeader_->getKeySize() + RID_LEN));
				leafNodePageHeader_.noOfRecords
				= leafNodePageHeader_.noOfRecords + 1;
				DEBUG_B("UPDATED NUMBER OF RECORDS "<<leafNodePageHeader_.noOfRecords)
				memcpy(pageData_, &leafNodePageHeader_,
						sizeof(LeafNodePageHeaderStruct));
				//means inserting at first then to push to index node
				if (i == 0 && getParentPageNumber() != -1) {
					//				cout<<"\n parent number "<<getParentPageNumber()<<endl;
					IndexNode indexNode(indexHeader_, getParentPageNumber());
					//				cout<<" "<<previousFirstKey<<"   "<<key<<endl;
					//					dummyKeyCompare(previousFirstKey, key)

					if (BPlusTreeUtil::keyCompare(previousFirstKey, key,
									indexHeader_) != 0) {
						indexNode.replaceKey(previousFirstKey, key);
						//						indexNode.display();
					}
				}
			}
		}
	}
	return SUCCESS;
}
STATUS_CODE LeafNode::deleteFromLeafPage(const char* key, RIDStruct &givenRid,
		std::vector<int> &deletedPages) {
	//say given key is not found in any other indexNode
	int currentNoOfRecords = getNoOfRecordsInNode();
	DEBUG_B("In Leaf page : "<<getPageNumber())
	DEBUG_B("current number of nodes in this node : "<<currentNoOfRecords)
	//	DEBUG_B("CURRENT "<<currentNoOfRecords<<"new "<<ceil(indexHeader_->getMaxNoOfRecordsInLeaf()/2))
	if ((currentNoOfRecords - 1) >= ceil(
			indexHeader_->getMaxNoOfRecordsInLeaf() / 2)) {
		//delete key safely because it has enough number of keys
		DEBUG_B("Delete safely from this node it has enough number of keys ")
		int offset = LeafNode::getLeafNodeHeaderSize();
		char tempKey[indexHeader_->getKeySize()];
		RIDStruct tempRid;
		int found = 0, i;
		for (i = 0; i < getNoOfRecordsInNode(); i++) {
			memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			memcpy(&tempRid, &pageData_[offset], RID_LEN);
			offset = offset + RID_LEN;
			//			dummyKeyCompare(tempKey, key);
			if (BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_) == 0
					&& ridsCompare(tempRid, givenRid) == 0) {
				found = 1;
				break;
			}
		}
		if (found == 1 && i == getNoOfRecordsInNode() - 1) {
			setNoOfRecordsInNode(getNoOfRecordsInNode() - 1);
			return SUCCESS;
		}
		if (found == 1) {
			//			DEBUG_B("OFFSET "<<offset<<"  "<<i<<" "<<getNoOfRecordsInNode())
			int oldOffset = offset - (indexHeader_->getKeySize() + RID_LEN);
			char temp[(getNoOfRecordsInNode() - i - 1)
					* (indexHeader_->getKeySize() + RID_LEN)];
			memcpy(temp, &pageData_[offset], (getNoOfRecordsInNode() - i - 1)
					* (indexHeader_->getKeySize() + RID_LEN));
			memcpy(&pageData_[oldOffset], temp,
					(getNoOfRecordsInNode() - i - 1)
							* (indexHeader_->getKeySize() + RID_LEN));
			setNoOfRecordsInNode(getNoOfRecordsInNode() - 1);
			DEBUG_B("PARENT PAGE NUMBER "<<getParentPageNumber())
			int levelOfTree = indexHeader_->getHeightOfTheTree();
			int currentPage = getPageNumber();
			if (levelOfTree != 0) {
				IndexNode indexNode(indexHeader_, getParentPageNumber());
				char replaceKey[indexHeader_->getKeySize()];
				getFirstKey(replaceKey);
				indexNode.replaceKey(key, replaceKey);
			}
			return SUCCESS;
		}
	} else {
		//		DEBUG_B("BAD")
		//ohh leaf node does not have enough number of keys so do either rotate or merge
		DEBUG_B("This page does not have enough number of keys to do either rotate or merge")
		if (getParentPageNumber() == -1) {
			DEBUG_B("if it does not have any parent")
			if (getLevelOfNode() == indexHeader_->getHeightOfTheTree()) {
				DEBUG_B("If current node is root of the tree")
				int offset = LeafNode::getLeafNodeHeaderSize();
				char tempKey[indexHeader_->getKeySize()];
				RIDStruct tempRid;
				int found = 0, i;
				for (i = 0; i < getNoOfRecordsInNode(); i++) {
					memcpy(tempKey, &pageData_[offset],
							indexHeader_->getKeySize());
					offset = offset + indexHeader_->getKeySize();
					memcpy(&tempRid, &pageData_[offset], RID_LEN);
					offset = offset + RID_LEN;
					//					dummyKeyCompare(tempKey, key)
					if (BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_)
							== 0 && ridsCompare(tempRid, givenRid) == 0) {
						found = 1;
						break;
					}
				}
				if (found == 1 && i == getNoOfRecordsInNode() - 1) {
					setNoOfRecordsInNode(getNoOfRecordsInNode() - 1);
					if (getNoOfRecordsInNode() == 0) {
						deletedPages.push_back(getPageNumber());
						DEBUG_B("if number of records are 0 make root as -1")
						indexHeader_->setRootPage(-1);
						indexHeader_->updateHeightOfTheTree(0);
					}
					return SUCCESS;
				}
				if (found == 1) {
					//					DEBUG_B("OFFSET "<<offset<<"  "<<i<<" "<<getNoOfRecordsInNode())
					int oldOffset = offset - (indexHeader_->getKeySize()
							+ RID_LEN);
					char temp[(getNoOfRecordsInNode() - i - 1)
							* (indexHeader_->getKeySize() + RID_LEN)];
					memcpy(temp, &pageData_[offset], (getNoOfRecordsInNode()
							- i - 1) * (indexHeader_->getKeySize() + RID_LEN));
					memcpy(&pageData_[oldOffset], temp, (getNoOfRecordsInNode()
							- i - 1) * (indexHeader_->getKeySize() + RID_LEN));
					setNoOfRecordsInNode(getNoOfRecordsInNode() - 1);
					if (getNoOfRecordsInNode() == 0) {
						deletedPages.push_back(getPageNumber());
						DEBUG_B("if number of records are 0 make root as -1")
						indexHeader_->setRootPage(-1);
						indexHeader_->updateHeightOfTheTree(0);
					}

					return SUCCESS;
				}
			}
		} else {
			IndexNode indexNode(indexHeader_, getParentPageNumber());
			int leftSibling, rightSibling;
			DEBUG_B("current page number "<<getPageNumber())
			indexNode.getSiblings(getPageNumber(), leftSibling, rightSibling);
			DEBUG_B("left page number "<<leftSibling<<"  right page number "<<rightSibling)
			if (rightSibling != -1) {//if their is right sibling
				DEBUG_B("Checking with right page")
				LeafNode leafNode(indexHeader_, rightSibling);
				int noOfRecordsInRightPage = leafNode.getNoOfRecordsInNode();
				DEBUG_B("no of records in right page"<<noOfRecordsInRightPage)
				if ((noOfRecordsInRightPage - 1) >= ceil(
						indexHeader_->getMaxNoOfRecordsInLeaf() / 2)) {
					//rotate keys from right (right sibling) to left (current node)
					DEBUG_B("Right page more than half number of keys so rotate some keys from right page number")
					char firstKey[indexHeader_->getKeySize()];
					RIDStruct rid;
					leafNode.getFirstKey(firstKey, rid);
					//					DEBUG_B("FIRST KEY "<<firstKey<<" "<<rid.pageNumber<<"  "<<rid.slotNumber)
					leafNode.deleteFromLeafPage(firstKey, rid, deletedPages);
					insertIntoLeafPage(firstKey, rid);
					deleteFromLeafPage(key, givenRid, deletedPages);
					return SUCCESS;
				} else if (leftSibling != -1) {
					//right sibling does not have enough keys so check with left sibling
					DEBUG_B("right page is not available and left is available")
					DEBUG_B("checking with left page number")
					LeafNode leafNode1(indexHeader_, leftSibling);
					int noOfRecordsInLeftPage =
							leafNode1.getNoOfRecordsInNode();
					DEBUG_B("no of records in left page "<<noOfRecordsInLeftPage)
					if ((noOfRecordsInLeftPage - 1) >= ceil(
							indexHeader_->getMaxNoOfRecordsInLeaf() / 2)) {
						//rotate keys from left to right
						DEBUG_B("left page has more than enough number of keys so we can rotate some keys from left page to current page")
						char lastKey[indexHeader_->getKeySize()];
						RIDStruct rid;
						leafNode1.getLastKey(lastKey, rid);
						//						DEBUG_B("FIRST KEY "<<lastKey<<" "<<rid.pageNumber<<"  "<<rid.slotNumber)
						leafNode1.deleteFromLeafPage(lastKey, rid, deletedPages);
						//						DEBUG_B("INSERTING")
						insertIntoLeafPage(lastKey, rid);
						deleteFromLeafPage(key, givenRid, deletedPages);
						getLastKey(lastKey, rid);
						//						DEBUG_B("FIRST KEY "<<lastKey<<" "<<rid.pageNumber<<"  "<<rid.slotNumber)
						return SUCCESS;
					} else {
						//their is no enough number of keys in both left and right siblings
						//DO merging with one of the siblings (right)here
						DEBUG_B("IAM HERE-----TO MERGE WITH RIGHT PAGE NUMBER----IF INSUFFICIENT KEYS IN LEFT AND RIGHT--")
						DEBUG_B("Both left and right pages does not have enough number of keys to rotate")
						DEBUG_B("So merge with right page")
						deletedPages.push_back(getPageNumber());
						int offset = LeafNode::getLeafNodeHeaderSize();
						char
								tempKeys[indexHeader_->getMaxNoOfRecordsInLeaf()][indexHeader_->getKeySize()];
						RIDStruct rids[indexHeader_->getMaxNoOfRecordsInLeaf()];
						int tempCount = 0;
						char temp[indexHeader_->getKeySize()];
						RIDStruct tempRid;
						for (int i = 0; i < getNoOfRecordsInNode(); i++) {
							memcpy(temp, &pageData_[offset],
									indexHeader_->getKeySize());
							offset = offset + indexHeader_->getKeySize();
							memcpy(&tempRid, &pageData_[offset], RID_LEN);
							//IN FUTURE ADD THIS  && ridsCompare(givenRid,tempRid)!=0 in if condition
							//								dummyKeyCompare(temp, key);
							if (((BPlusTreeUtil::keyCompare(temp, key,
									indexHeader_) != 0) && (ridsCompare(
									givenRid, tempRid) != 0))
									|| ((BPlusTreeUtil::keyCompare(temp, key,
											indexHeader_) == 0)
											&& (ridsCompare(givenRid, tempRid)
													!= 0))) {

								memcpy(tempKeys[tempCount], temp,
										indexHeader_->getKeySize());
								memcpy(&rids[tempCount], &pageData_[offset],
										RID_LEN);
								tempCount++;
							}
							offset = offset + RID_LEN;
						}
						//						for (int i = 0; i < tempCount; i++)
						//							cout << "  " << tempKeys[i] << endl;
						DEBUG_B("Moving all keys except deleting key to right page")
						for (int i = 0; i < tempCount; i++) {
							leafNode.insertIntoLeafPage(tempKeys[i], rids[i]);
						}
						//						leafNode.display();
						DEBUG_B("deleting key from index node")
						IndexNode
								indexNode(indexHeader_, getParentPageNumber());
						char keyToDelete[indexHeader_->getKeySize()];
						indexNode.getKeyPreviousToPageNumber(getPageNumber(),
								keyToDelete);
						DEBUG_B("KEY TO DELETE ")
						if (BPLUSTREE_DEBUG == true) {
							BPlusTreeUtil::displayKey(keyToDelete, indexHeader_);
							DEBUG_B(endl)
						}
						char firstKey[indexHeader_->getKeySize()];
						leafNode.getFirstKey(firstKey);
						indexNode.replaceKeyPreviousToPageNumber(
								leafNode.getPageNumber(), key, firstKey);
						//					indexNode.indexNodeOnlyDisplay();
						indexNode.deleteKeyFromIndexNode(keyToDelete, false,
								deletedPages);
						//					char firstKey[indexHeader_->getKeySize()];
						//						indexNode.indexNodeOnlyDisplay();
						leafNode.getFirstKey(firstKey);
						//						DEBUG_B(firstKey<<"  "<<key)
						indexNode.replaceKey(key, firstKey);
						//						cout << "\n DELETION ENDS HERE" << endl;

					}
				} else {

					//their is no left leaf page number and right leaf page does not enough number of keys
					//DO merging with right page number
					DEBUG_B("Their is no left page number and right leaf page does not enough number of keys")
					DEBUG_B("Merging with right page number")
					//					DEBUG_B("IAM HERE-----TO MERGE WITH RIGHT PAGE NUMBER------")
					deletedPages.push_back(getPageNumber());
					int offset = LeafNode::getLeafNodeHeaderSize();
					char
							tempKeys[indexHeader_->getMaxNoOfRecordsInLeaf()][indexHeader_->getKeySize()];
					RIDStruct rids[indexHeader_->getMaxNoOfRecordsInLeaf()];
					int tempCount = 0;
					char temp[indexHeader_->getKeySize()];
					for (int i = 0; i < getNoOfRecordsInNode(); i++) {
						RIDStruct tempRid;
						memcpy(temp, &pageData_[offset],
								indexHeader_->getKeySize());
						offset = offset + indexHeader_->getKeySize();
						memcpy(&tempRid, &pageData_[offset], RID_LEN);
						// in future add this to  && ridsCompare(givenRid,tempRid)!=0 if condition
						//							dummyKeyCompare(temp, key);
						if (((BPlusTreeUtil::keyCompare(temp, key, indexHeader_)
								!= 0) && (ridsCompare(givenRid, tempRid) != 0))
								|| ((BPlusTreeUtil::keyCompare(temp, key,
										indexHeader_) == 0) && (ridsCompare(
										givenRid, tempRid) != 0))) {
							//							DEBUG_B("HERE ")
							//							BPlusTreeUtil::displayKey(temp, indexHeader_);
							//							cout<<"\n temp p :"<<tempRid.pageNumber<<"  "<<tempRid.slotNumber<<endl;
							//							BPlusTreeUtil::displayKey(key, indexHeader_);
							//							cout<<"\n p : "<<givenRid.pageNumber<<"  "<<givenRid.slotNumber<<endl;
							//							DEBUG_B("END")
							memcpy(tempKeys[tempCount], temp,
									indexHeader_->getKeySize());
							memcpy(&rids[tempCount], &pageData_[offset],
									RID_LEN);
							tempCount++;
						}
						offset = offset + RID_LEN;
					}
					/*					for (int i = 0; i < tempCount; i++) {
					 cout << "  ";
					 BPlusTreeUtil::displayKey(tempKeys[i], indexHeader_);
					 cout << endl;
					 }
					 */
					DEBUG_B("Moving all keys from current node to right node")
					for (int i = 0; i < tempCount; i++) {
						//						cout << "  " << tempKeys[i] << endl;
						leafNode.insertIntoLeafPage(tempKeys[i], rids[i]);
						//						DEBUG_B("key selected "<<key)
						//						DEBUG_B(" firstKey "<<tempKeys[0])
					}
					DEBUG_B("Deleting required key from index node (its parent node)")
					IndexNode indexNode(indexHeader_, getParentPageNumber());
					if (tempCount >= 0) {
						indexNode.replaceKey(key, tempKeys[0]);
					}
					char keyToDelete[indexHeader_->getKeySize()];
					indexNode.getKeyNextToPageNumber(getPageNumber(),
							keyToDelete);
					DEBUG_B("KEY TO DELETE "<<keyToDelete)
					char firstKey[indexHeader_->getKeySize()];
					leafNode.getFirstKey(firstKey);
					//					DEBUG_B("first key now "<<firstKey);
					//					DEBUG_B(" KEY "<<key<<"  FIRSTKEY "<<firstKey<<"PAGE NO "<<leafNode.getPageNumber())
					indexNode.replaceKeyPreviousToPageNumber(
							leafNode.getPageNumber(), key, firstKey);
					indexNode.deleteKeyFromIndexNode(keyToDelete, true,
							deletedPages);
					//				char firstKey[indexHeader_->getKeySize()];
					int pageNumber =
							indexNode.getFirstPageNumberPresentInIndexNode();
					LeafNode leafNode11(indexHeader_, pageNumber);
					leafNode11.getFirstKey(firstKey);
					//					DEBUG_B("FIRST KEY "<<firstKey)
					//					indexNode.indexNodeOnlyDisplay();
					indexNode.replaceKey(key, firstKey);
					//					cout << "\n DELETION ENDS HERE" << endl;

				}
			} else if (leftSibling != -1) {//if their is no rightsibling but their is a left sibling
				//SO ROTATE WITH LEFT SIBLING IF POSSIBLE
				DEBUG_B("Their is no right node so check with left page for rotating or merging")
				LeafNode leafNode(indexHeader_, leftSibling);
				int noOfRecordsInLeftSibling = leafNode.getNoOfRecordsInNode();
				DEBUG_B("no of records in left node"<<noOfRecordsInLeftSibling)
				if ((noOfRecordsInLeftSibling - 1) >= ceil(
						indexHeader_->getMaxNoOfRecordsInLeaf() / 2)) {
					//left sibling has enough number of keys
					//rotate keys from left to right
					DEBUG_B("left node has more than half of the keys so rotate with left page number")
					char lastKey[indexHeader_->getKeySize()];
					RIDStruct rid;
					leafNode.getLastKey(lastKey, rid);
					DEBUG_B("FIRST KEY "<<lastKey<<"  "<<rid.pageNumber<<"  "<<rid.slotNumber)
					leafNode.deleteFromLeafPage(lastKey, rid, deletedPages);
					insertIntoLeafPage(lastKey, rid);
					deleteFromLeafPage(key, givenRid, deletedPages);
					getFirstKey(lastKey, rid);
					DEBUG_B("FIRST KEY iiiiii"<<lastKey<<"  "<<rid.pageNumber<<"  "<<rid.slotNumber)
					return SUCCESS;
				} else {
					//left sibling does not enough number of keys and we don't have right sibling so
					//do merging with left sibling
					DEBUG_B("left page does not havve enough number of keys ")
					DEBUG_B("merge with left page")
					//					DEBUG_B("IAM HERE--------for merging with left node---")
					deletedPages.push_back(getPageNumber());
					int offset = LeafNode::getLeafNodeHeaderSize();
					char
							tempKeys[indexHeader_->getMaxNoOfRecordsInLeaf()][indexHeader_->getKeySize()];
					RIDStruct rids[indexHeader_->getMaxNoOfRecordsInLeaf()];
					int tempCount = 0;
					char temp[indexHeader_->getKeySize()];
					for (int i = 0; i < getNoOfRecordsInNode(); i++) {
						RIDStruct tempRid;
						memcpy(temp, &pageData_[offset],
								indexHeader_->getKeySize());
						offset = offset + indexHeader_->getKeySize();
						memcpy(&tempRid, &pageData_[offset], RID_LEN);
						//in future add this to  && ridsCompare(givenRid,tempRid)!=0 if condition

						//							dummyKeyCompare(temp, key);
						if (((BPlusTreeUtil::keyCompare(temp, key, indexHeader_)
								!= 0) && (ridsCompare(givenRid, tempRid) != 0))
								|| ((BPlusTreeUtil::keyCompare(temp, key,
										indexHeader_) == 0) && (ridsCompare(
										givenRid, tempRid) != 0))) {
							memcpy(tempKeys[tempCount], temp,
									indexHeader_->getKeySize());
							memcpy(&rids[tempCount], &pageData_[offset],
									RID_LEN);
							tempCount++;
						}

						offset = offset + RID_LEN;
					}
					/*					for (int i = 0; i < tempCount; i++)
					 cout << "  " << tempKeys[i] << endl;
					 */
					DEBUG_B("moving all the keys from current page to left page")
					for (int i = 0; i < tempCount; i++) {
						//						cout << "  " << tempKeys[i] << endl;
						leafNode.insertIntoLeafPage(tempKeys[i], rids[i]);
					}
					DEBUG_B("delete corresponding key from index node (from its parent)")
					IndexNode indexNode(indexHeader_, getParentPageNumber());
					char keyToDelete[indexHeader_->getKeySize()];
					indexNode.getKeyNextToPageNumber(getPageNumber(),
							keyToDelete);
					DEBUG_B("KEY TO DELETE "<<keyToDelete)
					indexNode.deleteKeyFromIndexNode(keyToDelete, false,
							deletedPages);

					char firstKey[indexHeader_->getKeySize()];
					indexNode.indexNodeOnlyDisplay();
					//					DEBUG_B(firstKey<<"  "<<key)
					leafNode.getFirstKey(firstKey);
					indexNode.replaceKey(key, firstKey);
					//					cout << "\n DELETION ENDS HERE" << endl;
					//********************************************************************
				}
			}
		}
	}
}
int LeafNode::searchInLeafNode(const char* key) {
	int noOfRecords = getNoOfRecordsInNode();
	int offset = LeafNode::getLeafNodeHeaderSize();
	char tempKey[indexHeader_->getKeySize()];
	int status = 1;
	for (int i = 0; i < noOfRecords; i++) {
		memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
		//		DEBUG_B("keys compare "<<tempKey<<"  "<<key)
		//			dummyKeyCompare(tempKey, key);
		status = BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_);
		if (status == 0)
			return 0;
		offset = offset + indexHeader_->getKeySize() + RID_LEN;
	}
	return 1;
}
int LeafNode::searchInLeafNode(const char *key, RIDStruct rid) {
	int noOfRecords = getNoOfRecordsInNode();
	int offset = LeafNode::getLeafNodeHeaderSize();
	char tempKey[indexHeader_->getKeySize()];
	RIDStruct existedRID;
	int status = 1;
	for (int i = 0; i < noOfRecords; i++) {
		memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&existedRID, &pageData_[offset], RID_LEN);
		//		DEBUG_B("keys compare "<<tempKey<<"  "<<key)
		//			dummyKeyCompare(tempKey, key);
		//			status =BPlusTreeUtil::keyCompare(tempKey,key,indexHeader_);
		if ((BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_) == 0)
				&& ridsCompare(rid, existedRID) != 0) {
			status = 2;
		}

		if ((BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_) == 0)
				&& ridsCompare(rid, existedRID) == 0)
			return 0;
		offset += RID_LEN;
	}
	return status;

}
int LeafNode::getLevelOfNode() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.level;
}
void LeafNode::setLevelOfNode(int newLevel) {
	leafNodePageHeader_.level = newLevel;
	memcpy(pageData_, &leafNodePageHeader_, sizeof(LeafNodePageHeaderStruct));
}

int LeafNode::getNoOfRecordsInNode() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.noOfRecords;
}
void LeafNode::setNoOfRecordsInNode(int noOfRecords) {
	leafNodePageHeader_.noOfRecords = noOfRecords;
	memcpy(pageData_, &leafNodePageHeader_, sizeof(LeafNodePageHeaderStruct));
}

int LeafNode::getParentPageNumber() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.parentPageNumber;
}
void LeafNode::setParentPageNumber(int parentPageNumber) {
	leafNodePageHeader_.parentPageNumber = parentPageNumber;
	memcpy(pageData_, &leafNodePageHeader_, sizeof(LeafNodePageHeaderStruct));
}

int LeafNode::getLeftPageNumber() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.leftPageNumber;
}
void LeafNode::setLeftPageNumber(int newLeftPageNumber) {
	leafNodePageHeader_.leftPageNumber = newLeftPageNumber;
	memcpy(pageData_, &leafNodePageHeader_, sizeof(LeafNodePageHeaderStruct));
}

int LeafNode::getRightPageNumber() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.rightPageNumber;
}
void LeafNode::setRightPageNumber(int newRightPageNumber) {
	leafNodePageHeader_.rightPageNumber = newRightPageNumber;
	memcpy(pageData_, &leafNodePageHeader_, sizeof(LeafNodePageHeaderStruct));
}
int LeafNode::getPageNumber() {
	memcpy(&leafNodePageHeader_, pageData_, sizeof(LeafNodePageHeaderStruct));
	return leafNodePageHeader_.generalPageHeader.pageNumber;
}
int LeafNode::getLeafNodeHeaderSize() {
	return sizeof(LeafNodePageHeaderStruct);
}
STATUS_CODE LeafNode::getFirstKey(char* firstKey, RIDStruct &rid) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	memcpy(firstKey, &pageData_[offset], indexHeader_->getKeySize());
	offset = offset + indexHeader_->getKeySize();
	memcpy(&rid, &pageData_[offset], RID_LEN);
}
STATUS_CODE LeafNode::getLastKey(char* lastKey, RIDStruct& rid) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	offset += (getNoOfRecordsInNode() - 1) * (indexHeader_->getKeySize()
			+ RID_LEN);
	memcpy(lastKey, &pageData_[offset], indexHeader_->getKeySize());
	offset = offset + indexHeader_->getKeySize();
	memcpy(&rid, &pageData_[offset], RID_LEN);
}
STATUS_CODE LeafNode::getFirstKey(char* firstKey) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	memcpy(firstKey, &pageData_[offset], indexHeader_->getKeySize());
	DEBUG_B(" first key "<<firstKey)
}
void LeafNode::getParentKey(char* key) {
	int parent = getParentPageNumber();
	if (parent != -1) {
		IndexNode indexNode(indexHeader_, parent);
		indexNode.getKeyPreviousToPageNumber(getPageNumber(), key);
		//		cout << "key " << key << endl;
	}
}

STATUS_CODE LeafNode::searchKeyInLeafNode(const char* key, std::vector<
		RIDStruct> &RIDVector) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	char tempKey[indexHeader_->getKeySize()];
	int found = 0;
	RIDStruct tempRid;
	for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
		memcpy(tempKey, &pageData_[offset], indexHeader_->getKeySize());
		//			dummyKeyCompare(tempKey, key);
		int comp = BPlusTreeUtil::keyCompare(tempKey, key, indexHeader_);
		if (comp == 0) {
			found = 1;
		}
		offset = offset + indexHeader_->getKeySize();
		if (found == 1) {
			memcpy(&tempRid, &pageData_[offset], RID_LEN);
			RIDVector.push_back(tempRid);
			found = 0;
		}
		offset = offset + RID_LEN;
	}

}
int LeafNode::ridsCompare(RIDStruct rid1, RIDStruct rid2) {
	if (rid1.pageNumber == rid2.pageNumber && rid1.slotNumber
			== rid2.slotNumber)
		return 0;
	else
		return 1;
}
void LeafNode::display() {
	int offset = LeafNode::getLeafNodeHeaderSize();
	cout << "CURRENT LEAF PAGE\t\t" << getPageNumber() << endl;
	cout << "ITS PARENT PAGE NUMBER\t\t" << getParentPageNumber() << endl;
	cout << "NO OF RECORDS IN LEAF NODE\t" << getNoOfRecordsInNode() << endl;
	cout << "LEFT PAGE NUMBER\t\t" << getLeftPageNumber() << endl;
	cout << "RIGHT PAGE NUMBER\t\t" << getRightPageNumber() << endl;
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rid, &pageData_[offset], RID_LEN);
		cout << "\t\t" << "key " << i << "--" << key << "\tpageNumber--"
				<< rid.pageNumber << "\tslot number " << rid.slotNumber << endl;
		;
		offset = offset + RID_LEN;
	}
}
void LeafNode::UILeafNode() {

	//	cout<<" Want to check left leaf page or right page number (y/n)"<<endl;
	cout << endl;
	cout << "\tCurrent Leaf page number-----------------: " << getPageNumber()
			<< endl;
	cout << "\tParent Page number-----------------------: "
			<< getParentPageNumber() << endl;
	cout << "\tNumber of records in Leaf Node-----------: "
			<< getNoOfRecordsInNode() << endl;
	cout
			<< "\t***********************RECORDS IN LEAF NODE *********************** "
			<< endl;
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	int offset = LeafNode::getLeafNodeHeaderSize();
	for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
		cout << "\t";
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rid, &pageData_[offset], RID_LEN);
		cout << "key : ";
		BPlusTreeUtil::displayKey(key, indexHeader_);
		cout << "\tPage number : " << rid.pageNumber << "\tSlot number :"
				<< rid.slotNumber << endl;
		offset = offset + RID_LEN;
	}
	cout
			<< "\t*********************** END OF RECORDS IN LEAF NODE *************** "
			<< endl;
	cout << "\tLeft Leaf Page Number--------------------: "
			<< getLeftPageNumber() << endl;
	cout << "\tRIght Leaf Page Number-------------------: "
			<< getRightPageNumber() << endl;
}
void LeafNode::UIDisplayOfLeafNode(int numOfTabs) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	for (int k = 0; k < numOfTabs; k++) {
		cout << "\t";
	}
	cout << "Leaf Page Number : " << getPageNumber() << endl;
	for (int i = 0; i < getNoOfRecordsInNode(); i++) {
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rid, &pageData_[offset], RID_LEN);
		offset = offset + RID_LEN;
		for (int k = 0; k < numOfTabs; k++) {
			cout << "\t";
		}
		cout << "k:";
		BPlusTreeUtil::displayKey(key, indexHeader_);
		cout << "\t" << "p:" << rid.pageNumber << "\t" << "s:"
				<< rid.slotNumber << endl;
	}
}
void LeafNode::DisplayKeyAt(int keyPosition) {
	int offset = LeafNode::getLeafNodeHeaderSize() + keyPosition
			* (indexHeader_->getKeySize() + RID_LEN);
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	memcpy(&key, &pageData_[offset], indexHeader_->getKeySize());
	offset = offset + indexHeader_->getKeySize();
	memcpy(&rid, &pageData_[offset], RID_LEN);
	cout << "k :" << key << "  p:" << rid.pageNumber << "  s:"
			<< rid.slotNumber;
}
int LeafNode::searchKeyInLeafNodeWithOp(const char* key, int op, std::vector<
		RIDStruct> &RIDVector, std::vector<string> &keyVector) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	char insideKey[indexHeader_->getKeySize()];
	RIDStruct rid;
	int found = 0;
	switch (op) {
	case 1:
		for (int i = 0; i < getNoOfRecordsInNode(); i++) {
			memcpy(insideKey, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			if (BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_) == 0) {
				found = 1;
				memcpy(&rid, &pageData_[offset], RID_LEN);
				RIDVector.push_back(rid);
				keyVector.push_back(insideKey);
			} else if (i == (getNoOfRecordsInNode() - 1)) {
				found = 0;
				break;
			}
			offset = offset + RID_LEN;
		}
		break;
	case 2:
		for (int i = 0; i < getNoOfRecordsInNode(); i++) {
			memcpy(insideKey, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			if (BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_) == 1) {
				found = 1;
				memcpy(&rid, &pageData_[offset], RID_LEN);
				offset += RID_LEN;
				RIDVector.push_back(rid);
				string str(insideKey, indexHeader_->getKeySize());
				keyVector.push_back(str);
				string dummy = keyVector.back();
				str.clear();
			} else {
				offset = offset + RID_LEN;
				found = 1;
			}
		}
		break;
	case 3:
		for (int i = 0; i < getNoOfRecordsInNode(); i++) {
			memcpy(insideKey, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			if ((BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_) == 1)
					|| (BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_)
							== 0)) {
				found = 1;
				memcpy(&rid, &pageData_[offset], RID_LEN);
				offset += RID_LEN;
				RIDVector.push_back(rid);
				string str(insideKey, indexHeader_->getKeySize());
				keyVector.push_back(str);
				string dummy = keyVector.back();
				str.clear();
			} else {
				offset = offset + RID_LEN;
				found = 1;
			}
		}
		break;
	case 4:
		for (int i = 0; i < getNoOfRecordsInNode(); i++) {
			memcpy(insideKey, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			if (BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_) == -1) {
				found = 1;
				memcpy(&rid, &pageData_[offset], RID_LEN);
				offset += RID_LEN;
				RIDVector.push_back(rid);
				string str(insideKey, indexHeader_->getKeySize());
				keyVector.push_back(str);
				string dummy = keyVector.back();
				str.clear();
			} else {
				offset = offset + RID_LEN;
				found = 1;
			}
		}
		break;
	case 5:
		for (int i = 0; i < getNoOfRecordsInNode(); i++) {
			memcpy(insideKey, &pageData_[offset], indexHeader_->getKeySize());
			offset = offset + indexHeader_->getKeySize();
			if ((BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_) == -1)
					|| (BPlusTreeUtil::keyCompare(insideKey, key, indexHeader_)
							== 0)) {
				found = 1;
				memcpy(&rid, &pageData_[offset], RID_LEN);
				offset += RID_LEN;
				RIDVector.push_back(rid);
				string str(insideKey, indexHeader_->getKeySize());
				keyVector.push_back(str);
				string dummy = keyVector.back();
				str.clear();
			} else {
				offset = offset + RID_LEN;
				found = 1;
			}
		}
		break;

	}
	return found;
}

std::string LeafNode::toString() {
	DEBUG("INSIDE LEAF PAGE --  TO STRING");
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "\tINDEX HEADER PAGE NO: " << getIndexHeaderPageNumber() << "\n";
	mystream << "\tParent Page number-----------------------: "
			<< getParentPageNumber() << "\n";
	mystream << "\tNumber of records in Leaf Node-----------: "
			<< getNoOfRecordsInNode() << "\n";
	mystream
			<< "\t***********************RECORDS IN LEAF NODE *********************** "
			<< "\n";
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	int offset = LeafNode::getLeafNodeHeaderSize();
	for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
		mystream << "\t";
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rid, &pageData_[offset], RID_LEN);
		mystream << "key : ";
		mystream << BPlusTreeUtil::KeytoString(key, indexHeader_);
		mystream << "\tPage number : " << rid.pageNumber << "\tSlot number :"
				<< rid.slotNumber << "\n";
		offset = offset + RID_LEN;
	}
	mystream
			<< "\t*********************** END OF RECORDS IN LEAF NODE *************** "
			<< "\n";
	mystream << "\tLeft Leaf Page Number--------------------: "
			<< getLeftPageNumber() << "\n";
	mystream << "\tRIght Leaf Page Number-------------------: "
			<< getRightPageNumber() << "\n";
	return mystream.str();
}


int LeafNode::searchFirstKeyToSecondKey(const char* firstKey,
		const char* secondKey, std::vector<RIDStruct> &RIDVector, std::vector<
				string> &keyVector) {
	int stop = 1;
	int offset = LeafNode::getLeafNodeHeaderSize();
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rid, &pageData_[offset], RID_LEN);
		if ((BPlusTreeUtil::keyCompare(key, firstKey, indexHeader_) == 0
				|| BPlusTreeUtil::keyCompare(key, firstKey, indexHeader_) == 1)
				&& (BPlusTreeUtil::keyCompare(key, secondKey, indexHeader_)
						== 0 || BPlusTreeUtil::keyCompare(key, secondKey,
						indexHeader_) == -1)) {
			RIDVector.push_back(rid);
			string str(key, indexHeader_->getKeySize());
			keyVector.push_back(str);
			str.clear();
			offset = offset + RID_LEN;
			stop = 1;
		} else {
			offset = offset + RID_LEN;
			stop = 0;
		}
	}
	return stop;
}
void LeafNode::getKeysAndRIDs(std::vector<RIDStruct> &RIDVector, std::vector<
		string> &keyVector) {
	int offset = LeafNode::getLeafNodeHeaderSize();
	char key[indexHeader_->getKeySize()];
	RIDStruct rid;
	for (int i = 0; i < leafNodePageHeader_.noOfRecords; i++) {
		memcpy(key, &pageData_[offset], indexHeader_->getKeySize());
		offset = offset + indexHeader_->getKeySize();
		memcpy(&rid, &pageData_[offset], RID_LEN);
		offset = offset + RID_LEN;
		RIDVector.push_back(rid);
		string str(key, indexHeader_->getKeySize());
		keyVector.push_back(str);
		str.clear();
	}
}
