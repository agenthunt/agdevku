/*
 * DirectoryEntry.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DIRECTORYENTRY_H_
#define DIRECTORYENTRY_H_
#include <string>
class DirectoryEntry {
public:
	DirectoryEntry();
	virtual ~DirectoryEntry();
	int getSizeOfDirectoryEntry();
	int getDirectoryEntryNumber();
	unsigned getFreeSpace();
	int getPageNumber();
	std::string toString();
	typedef struct DirectoryEntryStructure{
		int entryNumber;
		int pageNumber;
		int freeSpace;
	} DirectoryEntryStruct;
	DirectoryEntryStruct directoryEntry_;
};

#endif /* DIRECTORYENTRY_H_ */
