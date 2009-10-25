/*
 * LinkedListFreePageManager.h
 *
 *  Created on: 20-Oct-2009
 *      Author: shailesh
 */

#ifndef LINKEDLISTFREEPAGEMANAGER_H_
#define LINKEDLISTFREEPAGEMANAGER_H_
#include "../global/StatusCodes.h"
#include "../global/GlobalStructures.h"
#include "DiskMgrRelatedStructures.h"
#include "DiskFileAccessor.h"
class LinkedListFreePageManager {
public:
	LinkedListFreePageManager();
	virtual ~LinkedListFreePageManager();
	STATUS_CODE createLinkedListOfFreePages(int rootPageNumber,
			int numberOfPages, DiskFileAccessor *diskFileAccessor,int pageSize);
	STATUS_CODE getFreePageNumber(int& freePageNumber);

};

#endif /* LINKEDLISTFREEPAGEMANAGER_H_ */
