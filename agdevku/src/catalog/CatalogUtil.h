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
#include "Schema.h"
class CatalogUtil {
public:
	CatalogUtil();
	virtual ~CatalogUtil();
	STATUS_CODE addInitialSysTableRecord();
	STATUS_CODE addInitialSysColTableRecord();
	STATUS_CODE addInitialSysIndexTableRecord();
	STATUS_CODE addInitialSysIndexColTableRecord();
	void getNextTableID();
	int getTableID(char *tableName);
	STATUS_CODE getSchema(char *tableName,Schema& schema);
	int getHeaderPageNumber(const char *tableName);
private:
	HeapFile *sysTableHeapFile_;
	HeapFile *sysColHeapFile_;
	DBMainHeaderPage *dbMainHeaderPage_;
};

#endif /* SYSTABLEUTIL_H_ */
