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

class LeafNode {
private:
	int pagenumber;
	char* pagedata;
	typedef struct LeafNodePage{
		GeneralPageHeaderStruct generalPageHeaderStruct;
		char** key;
		int* data;
		struct LeafNodePage* leftpointer;
		struct LeafNodePage* rightpointer;
		int level;
		int noOfRecords;
	};
public:
	LeafNode();
	STATUS_CODE createLeafPage(int &pagenumber,char*pagedata);
	STATUS_CODE insertIntoLeafPage(int pagenumber,char* key,int rid);
	STATUS_CODE deleteFromLeafPage(int pagenumber,char* key);
	STATUS_CODE redistributeLeafPage(int pagenumber);
	virtual ~LeafNode();
};

#endif /* LEAFNODE_H_ */
