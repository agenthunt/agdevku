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

public:
	LeafNode();
	virtual ~LeafNode();
	STATUS_CODE createLeafPage(int &pagenumber, char*pagedata);
	STATUS_CODE insertIntoLeafPage(int pagenumber, char* key, int rid);
	STATUS_CODE deleteFromLeafPage(int pagenumber, char* key);
	STATUS_CODE redistributeLeafPage(int pagenumber);

private:

	typedef struct LeafNodePageStructure {
		GeneralPageHeaderStruct generalPageHeaderStruct;
		char** key;
		int* data;
		struct LeafNodePage* leftpointer;
		struct LeafNodePage* rightpointer;
		int level;
		int noOfRecords;
	} LeafNodePageStruct;
	int pageNumber_;
	char* pageData_;
};

#endif /* LEAFNODE_H_ */
