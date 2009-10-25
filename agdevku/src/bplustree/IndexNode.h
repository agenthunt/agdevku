/*
 * IndexNode.h
 *
 *  Created on: 22-Oct-2009
 *      Author: ravindra
 */

#ifndef INDEXNODE_H_
#define INDEXNODE_H_
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"
class IndexNode {
private:
	int pagenumber;
	typedef struct IndexNodePage{
		char** key;
		void** pointerstoNextLevel;
		int noOfKeys;
		int isRoot;
		struct IndexNodePage* parent;
		struct IndexNodePage* next;
	}IndexNodeStruct;
public:
	IndexNode();
	STATUS_CODE createNewNode(int &pagenumber,char* pageData);
	STATUS_CODE inserIntoIndexNode(int pagenumber,char* key,char*data);
	STATUS_CODE deletefromIndexNode(int pagenumber,char* key);
	STATUS_CODE reorganizeIndexNode(int pagenumber);
	virtual ~IndexNode();
};

#endif /* INDEXNODE_H_ */
