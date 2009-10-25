/*
 * BufferManager.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_
#define FRAME_NOT_FOUND -1
#include <map>
#include <vector>
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../diskmgr/LinkedListFreePageManager.h"
#include "../global/GlobalStructures.h"
#include "../global/StatusCodes.h"
#include "../diskmgr/DiskManager.h"

#include "Frame.h"
class BufferManager {
public:

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
	 * read pageSize from mainheader and set the variable pageSize_ = pageSize
	 */
	STATUS_CODE openDatabase(const char *databaseName);
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
	 * Call LinkedListFreePageManager to get freepagenumber,
	 * if buffer free , just find empty frames for the added pages
	 * no need to read anything from disk and store, because it is a new page
	 * if buffer full, return BUFFER_FULL_ERROR
	 */
	STATUS_CODE
	newPage(int& pageNumber, char*& pageData);

	/**
	 * call LinkedListFreePageManager to add the freed page to list
	 * also if this pageNumber in bufferPool, reset pinCount = 0 , so that
	 * it can be replaced
	 */
	STATUS_CODE freePage(int pageNumber);


	/**
	 * check if page exists in buffer pool, if yes return
	 * pageData, else load page from disk into frame and return
	 * pageData, update frameLookupTable_
	 * increment pincount
	 */
	STATUS_CODE pinAndGetPage(int pageNumber, char*& pageData);

	/**
	 *decrement the pinCount of the frame corresponding to page
	 *also if dirty == true , mark frame as dirty
	 */
	void unPinPage(int pageNumber, bool dirty);

	int getCurrentlyUsingPageSize();
	static BufferManager* getInstance();
private:
	BufferManager();
	/**
	 * increment the pinCount of the frame corresponding to page
	 */
	//void pinPage(int pageNumber);
	int getFreeFrame();
	void insertIntoLookupTable(int pageNumber,int frameNumber);
	int getFrame(int pageNumber);


	/**
	 * get the frame number,
	 * update the pageData
	 * mark dirty
	 * not sure if this is needed
	 */
	STATUS_CODE writePage(int pageNumber, char *pageData);


	static BufferManager *bufferManagerInstance_;
	Frame **bufferPool_;
	int pageSize_;
	int numOfFrames_;
	std::map<int, int> frameLookupTable_;
	DiskManager diskManager_;
	LinkedListFreePageManager freePageManager_;
	int bufferSize_;//in MB
	bool isDatabaseOpen_ ;
    int dummy;

};

#endif /* BUFFERMANAGER_H_ */
