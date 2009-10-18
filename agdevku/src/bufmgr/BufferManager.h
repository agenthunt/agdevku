/*
 * BufferManager.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_
#include <map>
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/GlobalStructures.h"
#include "../global/StatusCodes.h"
#include "../diskmgr/DiskManager.h"
#include "Frame.h"
class BufferManager {
public:
	BufferManager();
	virtual ~BufferManager();
	/**
	 * when use database command is given,
	 * numOfFrames_ = sizeInMB/pageSize
	 * allocate numOfFrames_ in bufferPool
	 * set the private variable pageSize_ = pageSize
	 */
	STATUS_CODE
			initializeBuffer(int sizeInMB, int pageSize = DEFAULT_PAGE_SIZE);

	/**
	 * delegates the call to the Diskmanager.
	 */
	STATUS_CODE createDatabase(const char *name, int numOfPages =
			DEFAULT_NUM_OF_PAGES);
	/**
	 * delegates the call to the Diskmanager.
	 */
	STATUS_CODE dropDatabase(const char *name);
	/**
	 * delegates the call to the Diskmanager,
	 * calls initializeBuffer with appropriate
	 * if initializeBuffer has been called before then check if
	 * pageSizeOfDatabase == pageSize_
	 */
	bool openDatabase(const char *databaseName, int *pageSizeOfDatabase);
	/**
	 *delegate to diskManager
	 */
	STATUS_CODE closeDatabase();
	/**
	 * flush a specific page to disk
	 */
	STATUS_CODE flushPageToDisk(int pageNumber);
	/**
	 * flush all dirty pages to disk
	 */
	STATUS_CODE flushAllPagesToDisk();

	/**
	 * Call Diskmanager to allocate "howMany" number of pages and
	 * if buffer free , just find empty frames for the added pages
	 * no need to read anything from disk and store, because it is a new page
	 * if buffer full, deallocate the pages and return error
	 */
	STATUS_CODE
			newPage(int& firstPageNumber, char*& firstPage, int howMany = 1);
	/**
	 * call Diskmanager to deallocate page,
	 * also if this pageNumber in bufferPool, reset pinCount = 0 , so that
	 * it can be replaced
	 */
	STATUS_CODE freePage(int pageNumber);
	/**
	 * check if page exists in buffer pool, if yes return
	 * pageData, else load page from disk into frame and return
	 * pageData, update frameLookupTable_
	 */
	STATUS_CODE getPage(int pageNumber, char *pageData);
	/**
	 * get the frame number,
	 * update the pageData
	 * mark dirty
	 */
	STATUS_CODE writePage(int pageNumber, char *pageData);

	/**
	 * increment the pinCount of the frame corresponding to page
	 */
	void pinPage(int pageNumber);
	/**
	 *decrement the pinCount of the frame corresponding to page
	 *also if dirty == true , mark frame as dirty
	 */
	void unPinPage(int pageNumber,bool dirty);

	int getCurrentlyUsingPageSize();
private:
	Frame **bufferPool_;
	int pageSize_;
	int numOfFrames_;
	std::map<int,int> frameLookupTable_;
	DiskManager diskManager_;
};

#endif /* BUFFERMANAGER_H_ */
