/*
 * DiskFile.h
 *
 *  Created on: 11-Oct-2009
 *      Author: shailesh
 */

#ifndef DISKFILEACCESSOR_H_
#define DISKFILEACCESSOR_H_
#include "../global/GlobalStructures.h"

/**
 * This is an interface for disk file. It can have different implementations
 * such as C-fileI/O or a system calls or mmap etc.
 */
class DiskFileAccessor{
public:
	virtual STATUS_CODE createDiskFile(const char *diskFileName,int numOfPages,int pageSize)=0;
	virtual STATUS_CODE openDiskFile(const char *diskFileName) = 0;
	virtual STATUS_CODE closeDiskFile() = 0;
	virtual STATUS_CODE deleteDiskFile(const char *diskFileName) = 0;
	virtual STATUS_CODE readPage(int pageNumber,int pageSize,char *pageData) = 0;
	virtual STATUS_CODE writePage(int pageNumber,int pageSize,char *pageData) = 0;

};

#endif /* DISKFILEACCESSOR_H_ */
