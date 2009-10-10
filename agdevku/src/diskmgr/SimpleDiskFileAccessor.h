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
	virtual STATUS_CODE openDiskFile(const char *diskFileName);
	virtual STATUS_CODE closeDiskFile();
	virtual STATUS_CODE readPage(int pageNumber, char *pageData);
	virtual STATUS_CODE writePage(int pageNumber, char *pageData);
	virtual STATUS_CODE allocatePage(int& pageNumber);
	virtual STATUS_CODE deallocatePage(int pageNumber);
private :
	int fd_;// file descriptor
};

#endif /* SIMPLEDISKFILEACCESSOR_H_ */
