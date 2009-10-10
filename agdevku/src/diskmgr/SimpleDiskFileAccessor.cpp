/*
 * SimpleDiskFileAccessor.cpp
 *
 *  Created on: 11-Oct-2009
 *      Author: shailesh
 */

#include <unistd.h>
#include "SimpleDiskFileAccessor.h"
#include "../global/StatusCodes.h"

SimpleDiskFileAccessor::SimpleDiskFileAccessor() {
	// TODO Auto-generated constructor stub

}

SimpleDiskFileAccessor::~SimpleDiskFileAccessor() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE SimpleDiskFileAccessor::openDiskFile(const char *diskFileName) {

}
STATUS_CODE SimpleDiskFileAccessor::closeDiskFile() {
	int error = close(fd_);
	if (-1 == error) {
		return DISK_FILE_CLOSE_ERROR;
	}
	return SUCCESS;
}
STATUS_CODE SimpleDiskFileAccessor::readPage(int pageNumber, char *pageData) {

}
STATUS_CODE SimpleDiskFileAccessor::writePage(int pageNumber, char *pageData) {

}
STATUS_CODE SimpleDiskFileAccessor::allocatePage(int& pageNumber) {

}
STATUS_CODE SimpleDiskFileAccessor::deallocatePage(int pageNumber) {

}
