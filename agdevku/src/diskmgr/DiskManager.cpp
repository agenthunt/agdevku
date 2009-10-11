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
	strcpy(fullPathOfFile,DATABASES_FOLDER);
	strcat(fullPathOfFile,"/");
	strcat(fullPathOfFile,databaseName);
	return diskFileAccessor->createDiskFile(fullPathOfFile,numOfPages,pageSize);
}

