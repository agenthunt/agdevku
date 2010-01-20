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
#include "../global/ExternDefOfGlobalVariables.h"
#include "../utils/debug.h"
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

STATUS_CODE TableScan::getNextRecord(char*& record, unsigned& recordLen) {
	while (curDirPageNum_ != -1) {
		DirectoryPage dirPage(curDirPageNum_);
		if (SUCCESS != constructorErrorStatus) {
			DEBUG("dirPageNum="<<dirPage.getPageNumber());
			DEBUG("constructorErrorStatus="<<constructorErrorStatus<<"pagenumber="<<curDirPageNum_);
			return constructorErrorStatus;
		}
		int numOfDEs = dirPage.getNumberOfDirectoryEntries();
		while (curDirEntryNum_ < numOfDEs) {
			DirectoryEntry dirEntry;
			dirPage.getDirectoryEntry(curDirEntryNum_, dirEntry);
			curDataPageNum_ = dirEntry.getPageNumber();
			DataPage dataPage(curDataPageNum_);
			while (curSlotNum_ < dataPage.getNumOfSlots()) {
				if (dataPage.isValidSlotNum(curSlotNum_) == true) {
					const RIDStruct rid = { curDataPageNum_, curSlotNum_ };
					dataPage.getRecord(rid, record, recordLen);
					curRID.pageNumber = curDataPageNum_;
					curRID.slotNumber = curSlotNum_;
					curSlotNum_++;
					return SUCCESS;
				}
				curSlotNum_++;
			}
			curDirEntryNum_++;
			curSlotNum_ = 0;
		}
		curDirPageNum_ = dirPage.getNextDirectoryPageNumber();
		curDirEntryNum_ = 0;
	}

	return SCAN_OVER;
}

RIDStruct TableScan::getCurrentRID() {
	DEBUG("curRID="<<curRID.pageNumber<<","<<curRID.slotNumber);
	return curRID;
}
