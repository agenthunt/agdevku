/*
 * SysTableUtil.h
 *
 *  Created on: 15-Nov-2009
 *      Author: shailesh
 */

#ifndef SYSTABLEUTIL_H_
#define SYSTABLEUTIL_H_
#include "../heap/HeapFile.h"
#include "../diskmgr/DBMainHeaderPage.h"
#include "../global/GlobalStructures.h"
#include "../global/StatusCodes.h"

class SysTableUtil {
public:
	SysTableUtil();
	virtual ~SysTableUtil();
	STATUS_CODE addInitialSysTableRecord();
	void getNextTableID();
private:
	typedef struct SysTableInitialStructure {
		int tableID;
		char tableName[32];
		int numOfColumns;
		int rowSize;
		int headerPageNumber;//directoryHeaderPagenumber
	} SysTableInitialStruct;
	SysTableInitialStruct sysTableInitialStruct_;
	HeapFile *sysTableHeapFile_;
	DBMainHeaderPage *dbMainHeaderPage_;
};

#endif /* SYSTABLEUTIL_H_ */
