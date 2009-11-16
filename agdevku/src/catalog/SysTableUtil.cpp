/*
 * SysTableUtil.cpp
 *
 *  Created on: 15-Nov-2009
 *      Author: shailesh
 */

#include "SysTableUtil.h"
#include "../global/GlobalDefines.h"
#include "../utils/util.h"
#include "../heap/Tuple.h"

SysTableUtil::SysTableUtil() {
	// TODO Auto-generated constructor stub
	dbMainHeaderPage_ = new DBMainHeaderPage(DB_MAIN_HEADER_PAGE);
	sysTableHeapFile_
			= new HeapFile(dbMainHeaderPage_->getSysTablePageNumber());
}

SysTableUtil::~SysTableUtil() {
	// TODO Auto-generated destructor stub
	delete dbMainHeaderPage_;
	delete sysTableHeapFile_;
}

void SysTableUtil::getNextTableID() {
	sysTableHeapFile_->getNextRecordNumber();
}

STATUS_CODE SysTableUtil::addInitialSysTableRecord() {
	sysTableInitialStruct_.headerPageNumber
			= dbMainHeaderPage_->getSysTablePageNumber();
	sysTableInitialStruct_.numOfColumns = 5;
	sysTableInitialStruct_.rowSize = sizeof(SysTableInitialStruct);
	sysTableInitialStruct_.tableID = sysTableHeapFile_->getNextRecordNumber();

	Schema schema;
	schema.numOfColumns = 5;
	schema.recSize = sysTableInitialStruct_.rowSize;
	schema.columnNames.push_back("TABLE_ID");
	schema.columnNames.push_back("TABLE_NAME");
	schema.columnNames.push_back("NO OF COLS");
	schema.columnNames.push_back("ROW_SIZE");
	schema.columnNames.push_back("HEADER PG NO");
	schema.fieldLengths.push_back(INTEGER_LENGTH);
	schema.fieldLengths.push_back(32);
	schema.fieldLengths.push_back(INTEGER_LENGTH);
	schema.fieldLengths.push_back(INTEGER_LENGTH);
	schema.fieldLengths.push_back(INTEGER_LENGTH);
	schema.fieldTypes.push_back(TYPE_INTEGER);
	schema.fieldTypes.push_back(TYPE_VARCHAR);
	schema.fieldTypes.push_back(TYPE_INTEGER);
	schema.fieldTypes.push_back(TYPE_INTEGER);
	schema.fieldTypes.push_back(TYPE_INTEGER);
	std::string values[] = { "0",
			"systable",
			Util::int_to_string(sysTableInitialStruct_.numOfColumns),
			Util::int_to_string(sysTableInitialStruct_.rowSize),
			Util::int_to_string(sysTableInitialStruct_.headerPageNumber) };
	Tuple tuple(schema, values);
	char *record = tuple.prepRecordForInsertion();
	RIDStruct rid;
	int error = sysTableHeapFile_->insertRecord(record,tuple.getLength(),rid);
	if(SUCCESS!=error){
		return error;
	}
	return SUCCESS;
}

