/*
 * SysTableUtil.cpp
 *
 *  Created on: 15-Nov-2009
 *      Author: shailesh
 */

#include "CatalogUtil.h"
#include "../global/GlobalDefines.h"
#include "../utils/util.h"
#include "../heap/Tuple.h"
#include "../utils/debug.h"
#include "Schema.h"
#include "SchemaUtil.h"
#include "../heap/TableScan.h"
#include "CatalogConstants.h"
#include <string.h>
CatalogUtil::CatalogUtil() {
	// TODO Auto-generated constructor stub
	dbMainHeaderPage_ = new DBMainHeaderPage(DB_MAIN_HEADER_PAGE);
	sysTableHeapFile_
			= new HeapFile(dbMainHeaderPage_->getSysTablePageNumber());
	sysColHeapFile_ = new HeapFile(
			dbMainHeaderPage_->getSysColumnsHeaderPageNumber());
}

CatalogUtil::~CatalogUtil() {
	// TODO Auto-generated destructor stub
	delete sysTableHeapFile_;
	delete sysColHeapFile_;
	delete dbMainHeaderPage_;

}

void CatalogUtil::getNextTableID() {
	sysTableHeapFile_->getNextRecordNumber();
}

STATUS_CODE CatalogUtil::addInitialSysTableRecord() {
	Schema sysTableInitialSchema;
	SchemaUtil::getInitialSysTableSchema(sysTableInitialSchema);
	int sysTableID = sysTableHeapFile_->getNextRecordNumber();
	std::string values[] = { Util::int_to_string(sysTableID), sysTableInitialSchema.tableName.c_str(),
			Util::int_to_string(sysTableInitialSchema.numOfColumns),
			Util::int_to_string(dbMainHeaderPage_->getSysTablePageNumber()) };
	std::vector<std::string> vecs(values, &values[sysTableInitialSchema.numOfColumns]);
	Tuple tuple(sysTableInitialSchema, vecs);
	char *record = tuple.prepRecordForInsertion();
	DEBUG("record="<<values[1]<<(int)*record);
	RIDStruct rid;
	int error = sysTableHeapFile_->insertRecord(record, tuple.getLength(), rid);
	if (SUCCESS != error) {
		return error;
	}
	//add into sysCols
	Schema sysColSchema;
	SchemaUtil::getInitialSysColSchema(sysColSchema);
	for (int i = 0; i < sysTableInitialSchema.numOfColumns; i++) {
		std::string values[] = { Util::int_to_string(
				sysColHeapFile_->getNextRecordNumber()),
				sysTableInitialSchema.columnNames.at(i), Util::int_to_string(
						sysTableInitialSchema.fieldTypes.at(i)), Util::int_to_string(
						sysTableInitialSchema.fieldLengths.at(i)),
				Util::int_to_string(sysTableID) };
		std::vector<std::string> vecs(values,
				&values[sysColSchema.numOfColumns]);
		Tuple tuple(sysColSchema, vecs);
		char *record = tuple.prepRecordForInsertion();
		DEBUG("record="<<values[1]<<(int)*record);
		RIDStruct rid;
		int error = sysColHeapFile_->insertRecord(record, tuple.getLength(),
				rid);
		if (SUCCESS != error) {
			return error;
		}
	}
	return SUCCESS;
}

STATUS_CODE CatalogUtil::addInitialSysColTableRecord(){
	Schema sysColInitialSchema;
	SchemaUtil::getInitialSysColSchema(sysColInitialSchema);
	int sysTableID = sysTableHeapFile_->getNextRecordNumber();
	std::string values[] = { Util::int_to_string(sysTableID), sysColInitialSchema.tableName.c_str(),
			Util::int_to_string(sysColInitialSchema.numOfColumns),
			Util::int_to_string(dbMainHeaderPage_->getSysTablePageNumber()) };
	std::vector<std::string> vecs(values, &values[sysColInitialSchema.numOfColumns]);
	Tuple tuple(sysColInitialSchema, vecs);
	char *record = tuple.prepRecordForInsertion();
	DEBUG("record="<<values[1]<<(int)*record);
	RIDStruct rid;
	int error = sysTableHeapFile_->insertRecord(record, tuple.getLength(), rid);
	if (SUCCESS != error) {
		return error;
	}
	//add into sysCols
	Schema sysColSchema;
	SchemaUtil::getInitialSysColSchema(sysColSchema);
	for (int i = 0; i < sysColInitialSchema.numOfColumns; i++) {
		std::string values[] = { Util::int_to_string(
				sysColHeapFile_->getNextRecordNumber()),
				sysColInitialSchema.columnNames.at(i), Util::int_to_string(
						sysColInitialSchema.fieldTypes.at(i)), Util::int_to_string(
						sysColInitialSchema.fieldLengths.at(i)),
				Util::int_to_string(sysTableID) };
		std::vector<std::string> vecs(values,
				&values[sysColSchema.numOfColumns]);
		Tuple tuple(sysColSchema, vecs);
		char *record = tuple.prepRecordForInsertion();
		DEBUG("record="<<values[1]<<(int)*record);
		RIDStruct rid;
		int error = sysColHeapFile_->insertRecord(record, tuple.getLength(),
				rid);
		if (SUCCESS != error) {
			return error;
		}
	}
	return SUCCESS;
}

STATUS_CODE CatalogUtil::getSchema(char *tableName, Schema& schema) {
	if (strcmp(tableName, "systable") == 0) {
		SchemaUtil::getInitialSysTableSchema(schema);
	} else if (strcmp(tableName, "syscol") == 0) {
		SchemaUtil::getInitialSysColSchema(schema);
	} else {
		Schema sysTableSchema;
		SchemaUtil::getInitialSysTableSchema(sysTableSchema);
		TableScan tableScan(sysTableHeapFile_);
		char *record;
		unsigned recordLen;
		int tableID;
		while (SCAN_OVER != tableScan.getNextRecord(record, recordLen)) {
			Tuple tuple(sysTableSchema, record, recordLen);
			if (tuple.fieldMatch(TABLE_NAME_POS, tableName) == true) {
				tuple.getField(TABLE_ID_POS, &tableID);
				char tableName[sysTableSchema.fieldLengths.at(TABLE_NAME_POS)];
				tuple.getField(TABLE_NAME_POS,tableName);
				std::string temp(tableName);
				schema.tableName = temp;
				int numOfCols;
				tuple.getField(NO_OF_COLS_POS,&numOfCols);
				schema.numOfColumns = numOfCols;
				break;
			}
			return TABLE_NOT_FOUND;
		}

		Schema sysColSchema;
		SchemaUtil::getInitialSysColSchema(sysColSchema);
		TableScan sysColTableScan(sysColHeapFile_);
		char *sysColRecord;
		unsigned sysColRecordLen;
		while (SCAN_OVER != sysColTableScan.getNextRecord(sysColRecord,
				sysColRecordLen)) {
			Tuple tuple(sysColSchema,sysColRecord,sysColRecordLen);
			if (tuple.fieldMatch(TAB_ID_POS,&tableID) == true) {
				char colName[sysColSchema.fieldLengths.at(COL_NAME_POS)];
				tuple.getField(COL_NAME_POS,colName);
				std::string str(colName);
				schema.columnNames.push_back(str);
				int fieldType;
				tuple.getField(COL_TYPE_POS,&fieldType);
				schema.fieldTypes.push_back(fieldType);
				int fieldLength;
				tuple.getField(COL_LENGTH_POS,&fieldLength);
				schema.fieldLengths.push_back(fieldLength);
			}
		}
	}
	return SUCCESS;
}

int CatalogUtil::getHeaderPageNumber(const char *tableName) {

}

