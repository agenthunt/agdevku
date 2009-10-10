/*
 * DiskManager.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DISKMANAGER_H_
#define DISKMANAGER_H_
#include "../global/StatusCodes.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/GlobalStructures.h"
class DiskManager {
public:
	DiskManager();
	virtual ~DiskManager();
	/**
	 * add an extension ".db" and assign to databaseFileName
	 *  create a file and the initial structures like headerpage,systable,syscolumn etc
	 * and writes to it.
	 */
	bool createDatabase(const char *databaseName,int pageSize = DEFAULT_PAGE_SIZE);
	bool createDatabase(const char *databaseName,int numOfPages = DEFAULT_NUM_OF_PAGES,int pageSize = DEFAULT_PAGE_SIZE);
	void closeDatabase();
	bool openDatabase(const char *databaseName,int *pageSizeOfDatabase);
	STATUS_CODE readPage(int pageNumber,char *pageData);
	STATUS_CODE writePage(int pageNumber, char *pageData);
	STATUS_CODE allocatePage(int& pageNumber);
	STATUS_CODE deallocatePage(int pageNumber);


private:
	int fd_;//database file descriptor
	int pageSize_;// using this different databases might have different pagesize
	              //also for demo purpose
	char *databaseFileName; //this will be with extension .db
};

#endif /* DISKMANAGER_H_ */
