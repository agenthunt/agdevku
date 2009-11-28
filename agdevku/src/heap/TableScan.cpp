/*
 * TableScan.cpp
 *
 *  Created on: 17-Nov-2009
 *      Author: shailesh
 */

#include "TableScan.h"
#include "DirectoryEntry.h"
#include "DataPage.h"
#include "DirectoryPage.h"

TableScan::TableScan(HeapFile *heapFile) {
	// TODO Auto-generated constructor stub
	heapFile_ = heapFile;
	curDirPageNum_ = heapFile_->dirHeaderPage_->getFirstDirectoryPageNumber();
	curDirEntryNum_ = 0;
	curSlotNum_ = 0;
}

TableScan::~TableScan() {
	// TODO Auto-generated destructor stub

}



STATUS_CODE TableScan::getNextRecord(char *record, unsigned& recordLen) {
	while (curDirPageNum_ != -1) {
		DirectoryPage dirPage(curDirPageNum_);
		int numOfDEs = dirPage.getNumberOfDirectoryEntries();
		while (curDirEntryNum_ < numOfDEs) {
			DirectoryEntry dirEntry;
			dirPage.getDirectoryEntry(curDirEntryNum_, dirEntry);
			curDataPageNum_ = dirEntry.getPageNumber();
			DataPage dataPage(curDataPageNum_);
			while (curSlotNum_ < dataPage.getNumOfSlots()) {
				const RIDStruct rid = { curDataPageNum_, curSlotNum_ };
				dataPage.getRecord(rid, record, recordLen);
				curSlotNum_++;
				return SUCCESS;
			}
			curDirEntryNum_++;
		}
		curDirPageNum_ = dirPage.getNextDirectoryPageNumber();
	}
	return SCAN_OVER;
}
