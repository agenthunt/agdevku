/*
 * MMapDiskFileAccessor.cpp
 *
 *  Created on: 09-Jan-2010
 *      Author: shailesh
 */

#include "MMapDiskFileAccessor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "DiskFileAccessor.h"
#include "../global/StatusCodes.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../utils/debug.h"
#include <stdio.h>

MMapDiskFileAccessor::MMapDiskFileAccessor() {
	// TODO Auto-generated constructor stub
	fd_ = -1;
}

MMapDiskFileAccessor::~MMapDiskFileAccessor() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE MMapDiskFileAccessor::createDiskFile(const char *diskFileName,
		int numOfPages, int pageSize) {
	fd_ = creat(diskFileName, S_IRUSR | S_IWUSR);
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

STATUS_CODE MMapDiskFileAccessor::openDiskFile(const char *diskFileName) {
	DEBUG("disk File Name"<<diskFileName);
	fd_ = open(diskFileName, O_RDWR);

	DEBUG("file desc = "<<fd_);
	if (SYS_DEBUG == TRUE) {
		perror("open");
	}
	if (fd_ == -1) {
		if (SYS_DEBUG == TRUE) {
			perror("open");
		}
		return DISK_FILE_OPEN_ERROR;
	}

	if (fstat(fd_, &sb) == -1) {
		if (SYS_DEBUG == TRUE) {
			perror("fstat");
		}
		return DISK_FILE_OPEN_ERROR;
	}

	if (!S_ISREG(sb.st_mode)) {
		return DISK_FILE_OPEN_ERROR;
	}

	DEBUG("file Size"<<sb.st_size);
	addr = (char*) mmap(NULL, 8192 * 1000, PROT_READ | PROT_WRITE, MAP_SHARED,
			fd_, 0);
	if (addr == MAP_FAILED) {
		if (SYS_DEBUG == TRUE) {
			perror("mmap");
		}
		return DISK_FILE_OPEN_ERROR;
	}

	if (-1 == fd_) {
		return DISK_FILE_OPEN_ERROR;
	}
	return SUCCESS;
}

STATUS_CODE MMapDiskFileAccessor::closeDiskFile() {
	if (fstat(fd_, &sb) == -1) {
		if (SYS_DEBUG == TRUE) {
			perror("fstat");
		}
		return DISK_FILE_OPEN_ERROR;
	}
	int error = close(fd_);
	if (munmap(addr, sb.st_size) == -1) {
		if (SYS_DEBUG == true) {
			perror("munmap");
			return DISK_FILE_CLOSE_ERROR;
		}
	}
	fd_ = -1;
	if (-1 == error) {
		return DISK_FILE_CLOSE_ERROR;
	}
	return SUCCESS;
}
STATUS_CODE MMapDiskFileAccessor::deleteDiskFile(const char *diskFileName) {
	DEBUG("deleting file"<<diskFileName);
	int status = unlink(diskFileName);
	if (-1 == status) {
		return DISK_FILE_DELETE_ERROR;
	}
	return SUCCESS;
}

STATUS_CODE MMapDiskFileAccessor::readPage(int pageNumber, int pageSize,
		char *pageData) {
	DEBUG("using mmap");
	if (fstat(fd_, &sb) == -1) {
		if (SYS_DEBUG == TRUE) {
			perror("fstat");
		}
		return DISK_FILE_OPEN_ERROR;
	}

	int offset = pageNumber * pageSize;
	if (offset < sb.st_size) {
		DEBUG("using mmap");
		for (int i = 0; i < pageSize; i++) {
			pageData[i] = addr[offset + i];
		}
	} else {
		int error = lseek(fd_, pageNumber * pageSize, SEEK_SET);
		if (-1 == error) {
			return DISK_PAGE_READ_ERROR;
		}
		error = read(fd_, pageData, pageSize);
		if (error < pageSize) {
			if (SYS_DEBUG) {
				perror("read");
			}
			return DISK_PAGE_READ_ERROR;
		}
	}

	return SUCCESS;
}

STATUS_CODE MMapDiskFileAccessor::writePage(int pageNumber, int pageSize,
		char *pageData) {

	if (fstat(fd_, &sb) == -1) {
		if (SYS_DEBUG == TRUE) {
			perror("fstat");
		}
		return DISK_FILE_OPEN_ERROR;
	}

	int offset = pageNumber * pageSize;
	if (offset < sb.st_size) {
		DEBUG("using mmap");
		for (int i = 0; i < pageSize; i++) {
			addr[offset + i] = pageData[i];
		}
//		int error = msync(addr, sb.st_size, MS_SYNC);
//		if (error == -1) {
//			return DISK_PAGE_WRITE_ERROR;
//		}
	} else {
		int error = lseek(fd_, pageNumber * pageSize, SEEK_SET);
		if (-1 == error) {
			return DISK_PAGE_WRITE_ERROR;
		}
		error = write(fd_, pageData, pageSize);
		if (error < pageSize) {
			return DISK_PAGE_WRITE_ERROR;
		}
	}

	if (fstat(fd_, &sb) == -1) {
		if (SYS_DEBUG == TRUE) {
			perror("fstat");
		}
		return DISK_FILE_OPEN_ERROR;
	}

	return SUCCESS;
}

