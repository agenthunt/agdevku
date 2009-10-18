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
#include "../global/ExternDefsOfGlobalConstants.h"

DiskManager::DiskManager() {
	// TODO Auto-generated constructor stub
	diskFileAccessor = new SimpleDiskFileAccessor();
}

DiskManager::~DiskManager() {
	// TODO Auto-generated destructor stub
	delete diskFileAccessor;
}

STATUS_CODE DiskManager::createDatabase(const char *databaseName,
		int numOfPages, int pageSize) {
	char fullPathOfFile[64];
	strcpy(fullPathOfFile, DATABASES_FOLDER);
	strcat(fullPathOfFile, "/");
	strcat(fullPathOfFile, databaseName);
	strcat(fullPathOfFile,".db");
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
	memset(dummy,0,pageSize);
	diskFileAccessor->writePage(numOfPages-1,pageSize,dummy);
	char pageData[pageSize];
	DBMainHeaderPage dbMainHeaderPage;
	error = dbMainHeaderPage.createHeaderPage(pageSize,pageData,0);
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
	 */
	FreePageManager freePageManager;
	error = freePageManager.createFreePageManagerPage(pageSize,pageData,1);
	if (SUCCESS != error) {
		return error;
	}
	// 1st page number is always the free page manager page
	error = diskFileAccessor->writePage(1, pageSize, pageData);
	if (SUCCESS != error) {
		return error;
	}
	return SUCCESS;

}

STATUS_CODE DiskManager::openDatabase(const char *databaseName,
		int *pageSizeOfDatabase) {

}

void DiskManager::closeDatabase() {

}

STATUS_CODE DiskManager::dropDatabase(const char *databaseName) {

}

STATUS_CODE DiskManager::readPage(int pageNumber, char *pageData) {

}

STATUS_CODE DiskManager::writePage(int pageNumber, char *pageData) {

}

STATUS_CODE DiskManager::allocatePage(int& pageNumber) {

}

STATUS_CODE DiskManager::deallocatePage(int pageNumber) {

}
