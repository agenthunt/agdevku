/*
 * SimpleDiskFileAccessor.h
 *
 *  Created on: 11-Oct-2009
 *      Author: shailesh
 */

#ifndef SIMPLEDISKFILEACCESSOR_H_
#define SIMPLEDISKFILEACCESSOR_H_
#include "DiskFileAccessor.h"
class SimpleDiskFileAccessor: public DiskFileAccessor {
public:
	SimpleDiskFileAccessor();
	virtual ~SimpleDiskFileAccessor();
	virtual STATUS_CODE createDiskFile(const char *diskFileName,int numOfPages,int pageSize);
	virtual STATUS_CODE openDiskFile(const char *diskFileName);
	virtual STATUS_CODE closeDiskFile();
	virtual STATUS_CODE deleteDiskFile(const char *diskFileName);
	virtual STATUS_CODE readPage(int pageNumber,int pageSize, char *pageData);
	virtual STATUS_CODE writePage(int pageNumber,int pageSize, char *pageData);
private :
	int fd_;// file descriptor
	int pageSize_;
};

#endif /* SIMPLEDISKFILEACCESSOR_H_ */
