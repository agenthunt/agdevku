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
class BPlusTree {
public:
	BPlusTree();
	virtual ~BPlusTree();
	STATUS_CODE insertIntoBPlusTree(int pagenumber,char* key,int data);
	STATUS_CODE deleteFromBPlusTree(int pagenumber,char* key);
	STATUS_CODE searchInBPlusTree(char* key);
};

#endif /* BPLUSINDEX_H_ */
