/*
 * TableScan.h
 *
 *  Created on: 17-Nov-2009
 *      Author: shailesh
 */

#ifndef TABLESCAN_H_
#define TABLESCAN_H_
#include "HeapFile.h"

#include "../global/GlobalStructures.h"
#include "../global/StatusCodes.h"

class TableScan {
public:
	TableScan(HeapFile *heapFile);
	virtual ~TableScan();
	STATUS_CODE getNextRecord(char*& record,unsigned& recordLen);
	RIDStruct getCurrentRID();
private:
	HeapFile *heapFile_;
	int curDirPageNum_;
	int curDirEntryNum_;
	int curDataPageNum_;
	int curSlotNum_;
	RIDStruct curRID;
};

#endif /* TABLESCAN_H_ */
