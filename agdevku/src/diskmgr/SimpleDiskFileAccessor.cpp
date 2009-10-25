/*
 * SimpleDiskFileAccessor.cpp
 *
 *  Created on: 11-Oct-2009
 *      Author: shailesh
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "DiskFileAccessor.h"
#include "SimpleDiskFileAccessor.h"
#include "../global/StatusCodes.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../utils/debug.h"
#include <stdio.h>
SimpleDiskFileAccessor::SimpleDiskFileAccessor() {
	// TODO Auto-generated constructor stub
	fd_ = -1;
}

SimpleDiskFileAccessor::~SimpleDiskFileAccessor() {
	// TODO Auto-generated destructor stub

}

STATUS_CODE SimpleDiskFileAccessor::createDiskFile(const char *diskFileName,
		int numOfPages, int pageSize) {
	fd_ = creat(diskFileName,S_IRUSR|S_IWUSR);
	DEBUG("file desc = "<<fd_);
	//fd_ = open(diskFileName, O_RDWR | O_CREAT, S_IRWXU);
	//write(fd_,"created",7);
	if (SYS_DEBUG == TRUE) {
		perror("creat");
	}
	if (-1 == fd_) {
		close(fd_);
		return DISK_FILE_CREATE_ERROR;
	}
	close(fd_);
	return SUCCESS;
}

STATUS_CODE SimpleDiskFileAccessor::openDiskFile(const char *diskFileName) {
	fd_ = open(diskFileName, O_RDWR);
	DEBUG("file desc = "<<fd_);
	if (SYS_DEBUG == TRUE) {
		perror("open");
	}
	if (-1 == fd_) {
		return DISK_FILE_OPEN_ERROR;
	}
	return SUCCESS;
}

STATUS_CODE SimpleDiskFileAccessor::closeDiskFile() {
	int error = close(fd_);
	fd_ = -1;
	if (-1 == error) {
		return DISK_FILE_CLOSE_ERROR;
	}
	return SUCCESS;
}
STATUS_CODE SimpleDiskFileAccessor::deleteDiskFile(const char *diskFileName){
	int status = unlink(diskFileName);
	if( -1 == status){
		return DISK_FILE_DELETE_ERROR;
	}
	return SUCCESS;
}

STATUS_CODE SimpleDiskFileAccessor::readPage(int pageNumber, int pageSize,
		char *pageData) {
	int error = lseek(fd_, pageNumber * pageSize, SEEK_SET);
	if (-1 == error) {
		return DISK_PAGE_READ_ERROR;
	}
	error = read(fd_, pageData, pageSize);
	if (error < pageSize) {
		if(SYS_DEBUG){
			perror("read");
		}
		return DISK_PAGE_READ_ERROR;
	}
	return SUCCESS;
}

STATUS_CODE SimpleDiskFileAccessor::writePage(int pageNumber, int pageSize,
		char *pageData) {
	int error = lseek(fd_, pageNumber * pageSize, SEEK_SET);
	if (-1 == error) {
		return DISK_PAGE_WRITE_ERROR;
	}
	error = write(fd_, pageData, pageSize);
	if (error < pageSize) {
		return DISK_PAGE_WRITE_ERROR;
	}
	return SUCCESS;
}

