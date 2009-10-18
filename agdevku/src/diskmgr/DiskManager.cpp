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

DiskManager::DiskManager() {
	// TODO Auto-generated constructor stub
	diskFileAccessor = new SimpleDiskFileAccessor();
}

DiskManager::~DiskManager() {
	// TODO Auto-generated destructor stub
	delete diskFileAccessor;
}

void DiskManager::constructFullPathOfFile(char *fullPathOfFile,
		const char *databaseName) {
	strcpy(fullPathOfFile, DATABASES_FOLDER);
	strcat(fullPathOfFile, "/");
	strcat(fullPathOfFile, databaseName);
	strcat(fullPathOfFile, ".db");
}

STATUS_CODE DiskManager::createDatabase(const char *databaseName,
		int numOfPages, int pageSize) {
	char fullPathOfFile[64];
	constructFullPathOfFile(fullPathOfFile, databaseName);
	int error = diskFileAccessor->createDiskFile(fullPathOfFile, numOfPages,
			pageSize);
	if (SUCCESS != error) {
		return error;
	}
	error = diskFileAccessor->openDiskFile(fullPathOfFile);
	if (SUCCESS != error) {
		return error;
	}
	char dummy[pageSize];
	memset(dummy, 0, pageSize);
	diskFileAccessor->writePage(numOfPages - 1, pageSize, dummy);
	char pageData[pageSize];
	DBMainHeaderPage dbMainHeaderPage;
	error = dbMainHeaderPage.createHeaderPage(pageSize, pageData, 0);
	if (SUCCESS != error) {
		return error;
	}
	//0th page number is always the db main header page
	error = diskFileAccessor->writePage(0, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}

	/**
	 * create a page for the FreePageManager and write those details onto disk
	 *
	 */
	FreePageManager freePageManager;
	error = freePageManager.createFreePageManagerPage(pageSize, pageData, 1);
	if (SUCCESS != error) {
		return error;
	}
	// 1st page number is always the free page manager page
	error = diskFileAccessor->writePage(1, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}
	DirectoryHeaderPage sysTableHeaderPage;
	error = sysTableHeaderPage.createDirectoryHeaderPage(pageSize, pageData, 2);
	if (SUCCESS != error) {
		return error;
	}
	// 2nd page number is always the sys table's header page
	error = diskFileAccessor->writePage(2, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}
	DirectoryHeaderPage sysColHeaderPage;
	error = sysColHeaderPage.createDirectoryHeaderPage(pageSize, pageData, 3);
	if (SUCCESS != error) {
		return error;
	}
	// 3th page number is always the sys col's header page
	error = diskFileAccessor->writePage(3, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}

	error = diskFileAccessor->closeDiskFile();
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;

}

STATUS_CODE DiskManager::openDatabase(const char *databaseName,
		int *pageSizeOfDatabase) {
	char fullPathOfFile[64];
	constructFullPathOfFile(fullPathOfFile, databaseName);
	int error = diskFileAccessor->openDiskFile(fullPathOfFile);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::closeDatabase() {
	int error = diskFileAccessor->closeDiskFile();
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::dropDatabase(const char *databaseName) {
	char fullPathOfFile[64];
	constructFullPathOfFile(fullPathOfFile, databaseName);
	int error = diskFileAccessor->deleteDiskFile(fullPathOfFile);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::readPage(int pageNumber, char *pageData) {
	int error = diskFileAccessor->readPage(pageNumber, pageSize_, pageData);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;
}

STATUS_CODE DiskManager::writePage(int pageNumber, char *pageData) {
	int error = diskFileAccessor->writePage(pageNumber, pageSize_, pageData);
	if (SUCCESS != error) {
		return error;
	}
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
