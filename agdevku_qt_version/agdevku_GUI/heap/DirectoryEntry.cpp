/*
 * DirectoryEntry.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DirectoryEntry.h"
#include "string.h"
#include <sstream>
DirectoryEntry::DirectoryEntry() {
	// TODO Auto-generated constructor stub

}

DirectoryEntry::~DirectoryEntry() {
	// TODO Auto-generated destructor stub
}

int DirectoryEntry::getSizeOfDirectoryEntry() {
	return sizeof(DirectoryEntryStruct);
}

unsigned DirectoryEntry::getFreeSpace() {
	return directoryEntry_.freeSpace;
}

int DirectoryEntry::getPageNumber() {
	return directoryEntry_.pageNumber;
}

int DirectoryEntry::getDirectoryEntryNumber() {
	return directoryEntry_.entryNumber;
}

std::string DirectoryEntry::toString() {
	std::ostringstream mystream;
	mystream << "DIRECTORY ENTRY NUMBER: " << getDirectoryEntryNumber() << "\n";
	mystream << "PAGE NUMBER: " << getPageNumber() << "\n";
	mystream << "FREE SPACE:  " << getFreeSpace() << "\n";
	return mystream.str();
}
