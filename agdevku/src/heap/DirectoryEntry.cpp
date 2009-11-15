/*
 * DirectoryEntry.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DirectoryEntry.h"
#include "string.h"
DirectoryEntry::DirectoryEntry() {
	// TODO Auto-generated constructor stub

}

DirectoryEntry::~DirectoryEntry() {
	// TODO Auto-generated destructor stub
}

int DirectoryEntry::getSizeOfDirectoryEntry(){
	return sizeof(DirectoryEntryStruct);
}

unsigned DirectoryEntry::getFreeSpace(){
	return directoryEntry_.freeSpace;
}

int DirectoryEntry::getPageNumber(){
	return directoryEntry_.pageNumber;
}
