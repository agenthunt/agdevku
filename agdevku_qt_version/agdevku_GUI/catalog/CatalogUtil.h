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
#include "IndexStructure.h"
#include "../executor/ExecuteStructures.h"
#include <vector>
class CatalogUtil {
public:
	CatalogUtil();
	virtual ~CatalogUtil();
	STATUS_CODE addInitialSysTableRecord();
	STATUS_CODE addInitialSysColTableRecord();
	STATUS_CODE addInitialSysIndexTableRecord();
	STATUS_CODE addInitialSysIndexColTableRecord();
	STATUS_CODE addIndexEntry(ExecuteStructures *executeStructures);
	STATUS_CODE deleteIndexEntry(ExecuteStructures *executeStructures);
	STATUS_CODE addTableEntry(Schema& schema);
	STATUS_CODE deleteTableEntry(Schema& schema);
	STATUS_CODE addIndexEntry(IndexStructure& indexStruct);
	STATUS_CODE dropIndexEntry(IndexStructure& indexStruct);
	STATUS_CODE getListOfTables(std::vector<std::string>& tableList);
	void getNextTableID();
	STATUS_CODE getTableID(const char *tableName, int& tableID);
	STATUS_CODE getSchema(const char *tableName, Schema& schema);
	STATUS_CODE getSchema(int dirHeaderPageNumber, Schema& schema);
	int getHeaderPageNumber(const char *tableName, int& headerPageNumber);
	bool checkIfIndexExists(Schema& schema,std::string identifierValue,
			std::vector<int>& indexHeaderPageNumberList);
private:
	HeapFile *sysTableHeapFile_;
	HeapFile *sysColHeapFile_;
	HeapFile *sysIndexHeapFile_;
	HeapFile *sysIndexColHeapFile_;
	DBMainHeaderPage *dbMainHeaderPage_;
};

#endif /* SYSTABLEUTIL_H_ */
