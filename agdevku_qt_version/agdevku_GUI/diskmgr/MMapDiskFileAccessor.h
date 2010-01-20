/*
 * MMapDiskFileAccessor.h
 *
 *  Created on: 09-Jan-2010
 *      Author: shailesh
 */

#ifndef MMAPDISKFILEACCESSOR_H_
#define MMAPDISKFILEACCESSOR_H_
#include "DiskFileAccessor.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
class MMapDiskFileAccessor: public DiskFileAccessor {
public:
	MMapDiskFileAccessor();
	virtual ~MMapDiskFileAccessor();
	virtual STATUS_CODE createDiskFile(const char *diskFileName,
			int numOfPages, int pageSize);
	virtual STATUS_CODE openDiskFile(const char *diskFileName);
	virtual STATUS_CODE closeDiskFile();
	virtual STATUS_CODE deleteDiskFile(const char *diskFileName);
	virtual STATUS_CODE readPage(int pageNumber, int pageSize, char *pageData);
	virtual STATUS_CODE writePage(int pageNumber, int pageSize, char *pageData);
private:
	int fd_;// file descriptor
	int pageSize_;
	char *addr;
	struct stat sb;
};

#endif /* MMAPDISKFILEACCESSOR_H_ */
