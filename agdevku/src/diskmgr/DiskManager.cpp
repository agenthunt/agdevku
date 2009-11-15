/*
 * DiskManager.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include <string.h>
#include "DiskManager.h"
#include "DiskFileAccessor.h"
#include "SimpleDiskFileAccessor.h"
#include "DBMainHeaderPage.h"
#include "FreePageManager.h"
#include "../heap/DirectoryHeaderPage.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "LinkedListFreePageManager.h"
#include "../bufmgr/BufferManager.h"
DiskManager::DiskManager() {
	// TODO Auto-generated constructor stub
	diskFileAccessor_ = new SimpleDiskFileAccessor();
}

DiskManager::~DiskManager() {
	// TODO Auto-generated destructor stub
	delete diskFileAccessor_;
}

void DiskManager::constructFullPathOfFile(char *fullPathOfFile,
		const char *databaseName) {
	strcpy(fullPathOfFile, DATABASES_FOLDER);
	strcat(fullPathOfFile, "/");
	strcat(fullPathOfFile, databaseName);
	strcat(fullPathOfFile, ".db");
}

/*STATUS_CODE DiskManager::createDatabase(const char *databaseName,
 int numOfPages, int pageSize) {
 char fullPathOfFile[64];
 constructFullPathOfFile(fullPathOfFile, databaseName);
 int error = diskFileAccessor_->createDiskFile(fullPathOfFile, numOfPages,
 pageSize);
 if (SUCCESS != error) {
 return error;
 }
 error = diskFileAccessor_->openDiskFile(fullPathOfFile);
 if (SUCCESS != error) {
 return error;
 }
 char dummy[pageSize];
 memset(dummy, 0, pageSize);
 diskFileAccessor_->writePage(numOfPages - 1, pageSize, dummy);
 char pageData[pageSize];
 DBMainHeaderPage dbMainHeaderPage;
 error = dbMainHeaderPage.createHeaderPage(pageSize, pageData, 0);
 if (SUCCESS != error) {
 return error;
 }
 //0th page number is always the db main header page
 error = diskFileAccessor_->writePage(0, pageSize, pageData);
 if (SUCCESS != error) {
 return error;
 }


 // create a page for the FreePageManager and write those details onto disk

 FreePageManager freePageManager;
 error = freePageManager.createFreePageManagerPage(pageSize, pageData, 1);
 if (SUCCESS != error) {
 return error;
 }
 // 1st page number is always the free page manager page
 error = diskFileAccessor_->writePage(1, pageSize, pageData);
 if (SUCCESS != error) {
 return error;
 }
 DirectoryHeaderPage sysTableHeaderPage;
 error = sysTableHeaderPage.createDirectoryHeaderPage(pageSize, pageData, 2);
 if (SUCCESS != error) {
 return error;
 }
 // 2nd page number is always the sys table's header page
 error = diskFileAccessor_->writePage(2, pageSize, pageData);
 if (SUCCESS != error) {
 return error;
 }
 DirectoryHeaderPage sysColHeaderPage;
 error = sysColHeaderPage.createDirectoryHeaderPage(pageSize, pageData, 3);
 if (SUCCESS != error) {
 return error;
 }
 // 3th page number is always the sys col's header page
 error = diskFileAccessor_->writePage(3, pageSize, pageData);
 if (SUCCESS != error) {
 return error;
 }

 error = diskFileAccessor_->closeDiskFile();
 if (SUCCESS != error) {
 return error;
 }
 return SUCCESS;

 }*/

STATUS_CODE DiskManager::createDatabase(const char *databaseName,
		int numOfPages, int pageSize) {
	char fullPathOfFile[64];
	constructFullPathOfFile(fullPathOfFile, databaseName);
	int error = diskFileAccessor_->createDiskFile(fullPathOfFile, numOfPages,
			pageSize);
	if (SUCCESS != error) {
		return error;
	}
	error = diskFileAccessor_->openDiskFile(fullPathOfFile);
	if (SUCCESS != error) {
		return error;
	}

	LinkedListFreePageManager linkedListFreePageManager;
	linkedListFreePageManager.createLinkedListOfFreePages(1, numOfPages - 1,
			diskFileAccessor_, pageSize);

	char pageData[pageSize];

	error
			= DBMainHeaderPage::createHeaderPage(pageSize, pageData, 0,
					numOfPages);
	if (SUCCESS != error) {
		return error;
	}

	//0th page number is always the db main header page
	error = diskFileAccessor_->writePage(0, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}

	error = diskFileAccessor_->closeDiskFile();
	delete diskFileAccessor_;
	diskFileAccessor_ = NULL;
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::openDatabase(const char *databaseName) {
	char fullPathOfFile[64];
	constructFullPathOfFile(fullPathOfFile, databaseName);
	diskFileAccessor_ = getDiskFileAccessor();
	int error = diskFileAccessor_->openDiskFile(fullPathOfFile);
	if (SUCCESS != error) {
		return error;
	}

	return SUCCESS;
}

STATUS_CODE DiskManager::closeDatabase() {
	int error = diskFileAccessor_->closeDiskFile();
	if (SUCCESS != error) {
		return error;
	}
	delete diskFileAccessor_;
	diskFileAccessor_ = NULL;
	return SUCCESS;
}

STATUS_CODE DiskManager::dropDatabase(const char *databaseName) {
	char fullPathOfFile[64];
	constructFullPathOfFile(fullPathOfFile, databaseName);
	int error = diskFileAccessor_->deleteDiskFile(fullPathOfFile);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::readPage(int pageNumber, int pageSize, char *pageData) {
	if (NULL == diskFileAccessor_) {
		return DISK_FILE_OPEN_ERROR;
	}

	int error = diskFileAccessor_->readPage(pageNumber, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::writePage(int pageNumber, int pageSize, char *pageData) {
	if (NULL == diskFileAccessor_) {
		return DISK_FILE_OPEN_ERROR;
	}
	int error = diskFileAccessor_->writePage(pageNumber, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

DiskFileAccessor* DiskManager::getDiskFileAccessor() {
	diskFileAccessor_ = new SimpleDiskFileAccessor();
}

STATUS_CODE DiskManager::resizeDatabase(int numberOfPages,int pageSize){
	DBMainHeaderPage dbMainHeaderPage(DB_MAIN_HEADER_PAGE);
	int totalNumberofPagesInDB = dbMainHeaderPage.getTotalNumberOfPages();
	LinkedListFreePageManager freePageManager;
	int error = freePageManager.createLinkedListOfFreePages(
			totalNumberofPagesInDB, DEFAULT_NUM_OF_PAGES - 1, diskFileAccessor_,
			pageSize);
	if (SUCCESS != error) {
		return error;
	}
	dbMainHeaderPage.updateFreePagePointer(
			dbMainHeaderPage.getTotalNumberOfPages());
	dbMainHeaderPage.updateTotalNumberOfPages(totalNumberofPagesInDB
			+ DEFAULT_NUM_OF_PAGES);
	return SUCCESS;
}



STATUS_CODE DiskManager::allocatePage(int& pageNumber) {
	return allocatePages(pageNumber, 1);
}

STATUS_CODE DiskManager::deallocatePage(int pageNumber) {
	return deallocatePages(pageNumber, 1);
}

STATUS_CODE DiskManager::allocatePages(int& firstPageNumber, int howMany) {

}

STATUS_CODE DiskManager::deallocatePages(int& firstPageNumber, int howMany) {

}
