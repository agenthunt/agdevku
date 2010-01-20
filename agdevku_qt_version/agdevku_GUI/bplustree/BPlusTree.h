/*
 * BPlusIndex.h
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#ifndef BPLUSTREE_H_
#define BPLUSTREE_H_
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"
#include "../bplustree/IndexHeaderPage.h"
#include<vector>
#include<string>
#include<string.h>
using namespace std;
//std::vector<int> deletedPages;
class BPlusTree {
public:
	BPlusTree();
	BPlusTree(int indexHeaderPageNumber);//FOR HEAP (OPEN INDEX)
	virtual ~BPlusTree();
	STATUS_CODE insertIntoBPlusTree(const char* key, RIDStruct &rid);//FOR HEAP (INSERT KEY)
	STATUS_CODE deleteFromBPlusTree(const char* key, RIDStruct &rid);//FOR HEAP (DELETE KEY)
	STATUS_CODE searchInBPlusTree(const char* key);//(a)FOR HEAP (SEARCHING WILL GIVE LEAF PAGE NUMBER)
	STATUS_CODE searchKeyInBPlusTree(const char* key,
			std::vector<RIDStruct> &RIDVector);
	STATUS_CODE userInterface(int indexHeaderPageNumber);
	void UIIndexOrLeafNode();
	void bplusTreeUILeafNode(int pageNumber);
	void bplusTreeUIIndexNode(int pageNumber);
	void UIInsertRecord();
	void UISearching();
	void searchingWithOperator(const char* key, int searchOp);
	void UISetFanoutOfTree();
	void UIInsertRange();
	void UIDeleteKey();
	void UIDeleteRange();
	void UISetDebugFlag();
	STATUS_CODE display();
	void UIDisplayTree();
	void insertIntRange(int range1, int range2, int incrementFactor);
	void insertAllRanges(char* range1, char* range2, char* incrementFactor);
	void insertFloatRange(float range1, float range2, float incrementFactor);
	void
	insertDoubleRange(double range1, double range2, double incrementFactor);
	void insertLongRange(long range1, long range2, long incrementFactor);
	void deleteIntRange(int range1, int range2);
	void deleteFloatRange(float range1, float range2);
	void deleteDoubleRange(double range1, double range2);
	void deleteLongRange(long range1, long range2);
	void deleteAllRanges(char* range1, char* range2);
	void insertVarCharRange(int totalNumOfKeys);
	void storeTillRequiredKey(int startLeafPage, const char* key1,
			const char* key2, std::vector<RIDStruct> &RIDVector, std::vector<
					string> &keyVector);
	void tempKeyCompare(char* key1, char* key2);
	//(b)FOR HEAP SEARCH KEY WITH OPERATOR AND LEAF PAGE PAGE NUMBER
	void bplusTreeSearchKeyInLeafNodeWithOp(int leafPageNumber,
			const char* totalKey, int op, std::vector<RIDStruct> &RIDVector,
			std::vector<string> &keyVector);
	void bplusTreeSearchKeyInLeafNodeWithRangeSearch(int leafPageNumber,
			const char* key1, const char* key2,
			std::vector<RIDStruct> &RIDVector, std::vector<string> &keyVector);

	void deleteIndex(int indexPageNumber);//FOR HEAP ALSO
	void findLeafIndexPages(std::vector<int> &leafPages,
			std::vector<int> &indexPages);
	void displayIndexPages(std::vector<int> indexPageVector);
	void displayLeafPages(std::vector<int> leafPageVector);
	int findInIndexPages(std::vector<int> indexPageVector, int pageNumber);
	int findInLeafPages(std::vector<int> leafPageVector, int pageNumber);
	//	static std::vector<int> deletedPages;
	void getAllRIDS(std::vector<RIDStruct> &RIDVector,std::vector<string> &keyVector);
private:
	IndexHeaderPage *indexHeaderPage_;
};

#endif /* BPLUSINDEX_H_ */
