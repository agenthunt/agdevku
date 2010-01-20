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
#include "../executor/Delete.h"
#include "../executor/CreateTable.h"
#include "../executor/InsertTable.h"
#include "../executor/Select.h"
#include "../bplustree/IndexHeaderPage.h"
#include <string.h>
#include "../bufmgr/BufferManager.h"
#include "../executor/ExprInfo.h"
#include "../utils/ErrorLookupTable.h"
CatalogUtil::CatalogUtil() {
	// TODO Auto-generated constructor stub
	dbMainHeaderPage_ = new DBMainHeaderPage(DB_MAIN_HEADER_PAGE);
	sysTableHeapFile_
			= new HeapFile(dbMainHeaderPage_->getSysTablePageNumber());
	sysColHeapFile_ = new HeapFile(
			dbMainHeaderPage_->getSysColumnsHeaderPageNumber());
	sysIndexHeapFile_ = new HeapFile(
			dbMainHeaderPage_->getSysIndexHeaderPageNumber());
	sysIndexColHeapFile_ = new HeapFile(
			dbMainHeaderPage_->getSysIndexColumnsHeaderPageNumber());
}

CatalogUtil::~CatalogUtil() {
	// TODO Auto-generated destructor stub
	delete sysTableHeapFile_;
	delete sysColHeapFile_;
	delete dbMainHeaderPage_;
	delete sysIndexHeapFile_;
	delete sysIndexColHeapFile_;

}

void CatalogUtil::getNextTableID() {
	sysTableHeapFile_->getNextRecordNumber();
}

STATUS_CODE CatalogUtil::addInitialSysTableRecord() {
	Schema sysTableInitialSchema;
	SchemaUtil::getInitialSysTableSchema(sysTableInitialSchema);
	int sysTableID = sysTableHeapFile_->getNextRecordNumber();
	std::string values[] = { Util::int_to_string(sysTableID),
			sysTableInitialSchema.tableName.c_str(), Util::int_to_string(
					sysTableInitialSchema.numOfColumns), Util::int_to_string(
					dbMainHeaderPage_->getSysTablePageNumber()) };
	std::vector<std::string> vecs(values,
			&values[sysTableInitialSchema.numOfColumns]);
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
						sysTableInitialSchema.fieldTypes.at(i)),
				Util::int_to_string(sysTableInitialSchema.fieldLengths.at(i)),
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

	//updating statistical data
	dbMainHeaderPage_->updateNumberOfTables(
			dbMainHeaderPage_->getNumberOfTables() + 1);

	return SUCCESS;
}

STATUS_CODE CatalogUtil::addInitialSysColTableRecord() {
	Schema sysTableSchema;
	SchemaUtil::getInitialSysTableSchema(sysTableSchema);
	Schema sysColSchema;
	SchemaUtil::getInitialSysColSchema(sysColSchema);
	int sysColTableID = sysTableHeapFile_->getNextRecordNumber();
	std::string values[] = { Util::int_to_string(sysColTableID),
			sysColSchema.tableName.c_str(), Util::int_to_string(
					sysColSchema.numOfColumns), Util::int_to_string(
					dbMainHeaderPage_->getSysColumnsHeaderPageNumber()) };
	std::vector<std::string> vecs(values, &values[sysTableSchema.numOfColumns]);
	Tuple tuple(sysTableSchema, vecs);
	char *record = tuple.prepRecordForInsertion();
	DEBUG("record="<<values[1]<<(int)*record);
	RIDStruct rid;
	int error = sysTableHeapFile_->insertRecord(record, tuple.getLength(), rid);
	if (SUCCESS != error) {
		return error;
	}

	//add into sysCols
	for (int i = 0; i < sysColSchema.numOfColumns; i++) {
		std::string values[] = { Util::int_to_string(
				sysColHeapFile_->getNextRecordNumber()),
				sysColSchema.columnNames.at(i), Util::int_to_string(
						sysColSchema.fieldTypes.at(i)), Util::int_to_string(
						sysColSchema.fieldLengths.at(i)), Util::int_to_string(
						sysColTableID) };
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

	//updating statistical data
	dbMainHeaderPage_->updateNumberOfTables(
			dbMainHeaderPage_->getNumberOfTables() + 1);

	return SUCCESS;
}

STATUS_CODE CatalogUtil::getSchema(const char *tableName, Schema& schema) {
	if (strcasecmp(tableName, "systable") == 0) {
		SchemaUtil::getInitialSysTableSchema(schema);
		return SUCCESS;
	} else if (strcasecmp(tableName, "syscol") == 0) {
		SchemaUtil::getInitialSysColSchema(schema);
		return SUCCESS;
	} else {
		Schema sysTableSchema;
		SchemaUtil::getInitialSysTableSchema(sysTableSchema);
		DEBUG("systable schema="<<sysTableSchema.toString());
		TableScan tableScan(sysTableHeapFile_);
		char *record;
		unsigned recordLen;
		int tableID;
		int error = tableScan.getNextRecord(record, recordLen);
		bool tableFound = false;
		while (SCAN_OVER != error) {
			if (SUCCESS != error) {
				return error;
			}
			Tuple tuple(sysTableSchema, record, recordLen);
			if (tuple.fieldMatch(TABLE_NAME_POS, tableName) == true) {
				tuple.getField(TABLE_ID_POS, &tableID);
				char tableName[sysTableSchema.fieldLengths.at(TABLE_NAME_POS)];
				tuple.getField(TABLE_NAME_POS, tableName);
				std::string temp(tableName);
				schema.tableName = temp;
				int numOfCols;
				tuple.getField(NO_OF_COLS_POS, &numOfCols);
				schema.numOfColumns = numOfCols;
				tableFound = true;
				break;
			}
			error = tableScan.getNextRecord(record, recordLen);
		}

		if (tableFound == false) {
			return TABLE_NOT_FOUND;
		}

		Schema sysColSchema;
		SchemaUtil::getInitialSysColSchema(sysColSchema);
		TableScan sysColTableScan(sysColHeapFile_);
		char *sysColRecord;
		unsigned sysColRecordLen;
		while (SCAN_OVER != sysColTableScan.getNextRecord(sysColRecord,
				sysColRecordLen)) {
			Tuple tuple(sysColSchema, sysColRecord, sysColRecordLen);
			if (tuple.fieldMatch(TAB_ID_POS, &tableID) == true) {
				char colName[sysColSchema.fieldLengths.at(COL_NAME_POS)];
				tuple.getField(COL_NAME_POS, colName);
				std::string str(colName);
				schema.columnNames.push_back(str);
				int fieldType;
				tuple.getField(COL_TYPE_POS, &fieldType);
				schema.fieldTypes.push_back(fieldType);
				int fieldLength;
				tuple.getField(COL_LENGTH_POS, &fieldLength);
				schema.fieldLengths.push_back(fieldLength);
				//schema.numOfColumns++;// not needed here right?
			}
		}
	}
	return TABLE_EXISTS;
}

STATUS_CODE CatalogUtil::getSchema(int dirHeaderPageNumber, Schema& schema) {
	DEBUG ("getSchema for dirHeaderPageNumber="<<dirHeaderPageNumber);
	Schema sysTableSchema;
	SchemaUtil::getInitialSysTableSchema(sysTableSchema);
	DEBUG("systable schema="<<sysTableSchema.toString());
	TableScan tableScan(sysTableHeapFile_);
	char *record;
	unsigned recordLen;
	int tableID;
	int error = tableScan.getNextRecord(record, recordLen);
	bool tableFound = false;
	while (SCAN_OVER != error) {
		if (SUCCESS != error) {
			return error;
		}
		Tuple tuple(sysTableSchema, record, recordLen);
		if (tuple.fieldMatch(HDR_PG_NO_POS, &dirHeaderPageNumber) == true) {
			tuple.getField(TABLE_ID_POS, &tableID);
			char tableName[sysTableSchema.fieldLengths.at(TABLE_NAME_POS)];
			tuple.getField(TABLE_NAME_POS, tableName);
			std::string temp(tableName);
			schema.tableName = temp;
			int numOfCols;
			tuple.getField(NO_OF_COLS_POS, &numOfCols);
			schema.numOfColumns = numOfCols;
			tableFound = true;
			break;
		}
		error = tableScan.getNextRecord(record, recordLen);
	}

	if (tableFound == false) {
		return TABLE_NOT_FOUND;
	}

	Schema sysColSchema;
	SchemaUtil::getInitialSysColSchema(sysColSchema);
	TableScan sysColTableScan(sysColHeapFile_);
	char *sysColRecord;
	unsigned sysColRecordLen;
	while (SCAN_OVER != sysColTableScan.getNextRecord(sysColRecord,
			sysColRecordLen)) {
		Tuple tuple(sysColSchema, sysColRecord, sysColRecordLen);
		if (tuple.fieldMatch(TAB_ID_POS, &tableID) == true) {
			char colName[sysColSchema.fieldLengths.at(COL_NAME_POS)];
			tuple.getField(COL_NAME_POS, colName);
			std::string str(colName);
			schema.columnNames.push_back(str);
			int fieldType;
			tuple.getField(COL_TYPE_POS, &fieldType);
			schema.fieldTypes.push_back(fieldType);
			int fieldLength;
			tuple.getField(COL_LENGTH_POS, &fieldLength);
			schema.fieldLengths.push_back(fieldLength);
			//schema.numOfColumns++;// not needed here right?
		}
	}

	return TABLE_EXISTS;
}

STATUS_CODE CatalogUtil::getHeaderPageNumber(const char *tableName,
		int& headerPageNumber) {
	DEBUG("CatalogUtil::getHeaderPageNumber");
	Schema sysTableSchema;
	getSchema("SYSTABLE", sysTableSchema);
	TableScan tableScan(sysTableHeapFile_);
	char *record;
	unsigned recordLen;
	while (SCAN_OVER != tableScan.getNextRecord(record, recordLen)) {
		Tuple tuple(sysTableSchema, record, recordLen);
		if (tuple.fieldMatch(TABLE_NAME_POS, tableName) == true) {
			tuple.getField(HDR_PG_NO_POS, &headerPageNumber);
			DEBUG ("getHEaderPageNumber"<<headerPageNumber);
			return SUCCESS;
		}
	}
	return TABLE_NOT_FOUND;
}

STATUS_CODE CatalogUtil::addTableEntry(Schema& schema) {
	DEBUG("in addTableEntry"<<schema.tableName);
	Schema tempSchema;
	int error = getSchema(schema.tableName.c_str(), tempSchema);
	if (error == TABLE_EXISTS) {
		//table already exists;
		return error;
	}

	DirectoryHeaderPage dirHeaderPage;
	dirHeaderPage.createDirectoryHeaderPage();
	int newDirectoryHeaderPageNumber = dirHeaderPage.getPageNumber();

	int tableID = sysTableHeapFile_->getNextRecordNumber();
	std::string values[] = { Util::int_to_string(tableID), schema.tableName,
			Util::int_to_string(schema.numOfColumns), Util::int_to_string(
					newDirectoryHeaderPageNumber) };

	//add into sysTable
	Schema sysTableSchema;
	error = getSchema("systable", sysTableSchema);
	std::vector<std::string> vecs(values, &values[sysTableSchema.numOfColumns]);
	if (SUCCESS != error) {
		return error;
	}

	Tuple tuple(sysTableSchema, vecs);
	char *record = tuple.prepRecordForInsertion();
	DEBUG("record="<<values[1]<<(int)*record);
	RIDStruct rid;
	error = sysTableHeapFile_->insertRecord(record, tuple.getLength(), rid);
	if (SUCCESS != error) {
		return error;
	}

	//add into sysCols
	Schema sysColSchema;
	SchemaUtil::getInitialSysColSchema(sysColSchema);
	for (int i = 0; i < schema.numOfColumns; i++) {
		std::string values[] = { Util::int_to_string(
				sysColHeapFile_->getNextRecordNumber()), schema.columnNames.at(
				i), Util::int_to_string(schema.fieldTypes.at(i)),
				Util::int_to_string(schema.fieldLengths.at(i)),
				Util::int_to_string(tableID) };
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

	//updating statistical data
	dbMainHeaderPage_->updateNumberOfTables(
			dbMainHeaderPage_->getNumberOfTables() + 1);

	return SUCCESS;
}

/**
 * get the heap file, release all the pages referred by it.
 * and check all indices if available, release all those pages
 * delete entry from sysCol and sysTable
 */
STATUS_CODE CatalogUtil::deleteTableEntry(Schema& schema) {
	BufferManager *bufMgr = BufferManager::getInstance();
	//call heapFile->deleteFile . it will take care of freeing
	//up all the pages. but before that release index stuff.
	DEBUG("in deleteTableEntry" <<schema.tableName.c_str());

	//first free all the pages used by the table
	HeapFile toBeDeleteHeapFile(schema.tableName.c_str());
	toBeDeleteHeapFile.deleteFile();

	//delete entry from SysCol
	int tableID;
	int error = getTableID(schema.tableName.c_str(), tableID);
	if (SUCCESS != error) {
		return error;
	}
	ExecuteStructures executeStructures_1;
	executeStructures_1.tableName = "SYSCOL";
	executeStructures_1.schema = schema;
	executeStructures_1.executeStatement = ExecuteStructures::DELETE_STATEMENT;
	ExprInfo exprInfo1(ExprInfo::IDENTIFIER_TYPE, "TAB_ID");
	ExprInfo exprInfo2(ExprInfo::LITERAL_TYPE, Util::int_to_string(tableID));
	ExprInfo exprInfo3(ExprInfo::OPERATOR_TYPE, "=");
	executeStructures_1.whereConditionExprList.push_back(exprInfo1);
	executeStructures_1.whereConditionExprList.push_back(exprInfo2);
	executeStructures_1.whereConditionExprList.push_back(exprInfo3);
	Delete deleteStatement1;
	error = deleteStatement1.execute(&executeStructures_1);
	if (SUCCESS != error) {
		DEBUG("error"<<ErrorLookupTable::lookup(error).c_str());
		return error;
	}

	//delete entry from systable
	ExecuteStructures executeStructures_2;
	executeStructures_2.tableName = "SYSTABLE";
	executeStructures_2.schema = schema;
	executeStructures_2.executeStatement = ExecuteStructures::DELETE_STATEMENT;
	ExprInfo exprInfo11(ExprInfo::IDENTIFIER_TYPE, "TABLE_NAME");
	ExprInfo exprInfo21(ExprInfo::LITERAL_TYPE, schema.tableName);
	ExprInfo exprInfo31(ExprInfo::OPERATOR_TYPE, "=");
	executeStructures_2.whereConditionExprList.push_back(exprInfo11);
	executeStructures_2.whereConditionExprList.push_back(exprInfo21);
	executeStructures_2.whereConditionExprList.push_back(exprInfo31);
	Delete deleteStatement2;
	error = deleteStatement2.execute(&executeStructures_2);
	if (SUCCESS != error) {
		DEBUG("error"<<ErrorLookupTable::lookup(error).c_str());
		return error;
	}

	//updating statistical data
	dbMainHeaderPage_->updateNumberOfTables(
			dbMainHeaderPage_->getNumberOfTables() - 1);
	return error;
}

STATUS_CODE CatalogUtil::getListOfTables(std::vector<std::string>& tableList) {
	DEBUG("indise CatalogUtil::getListOfTables");
	Schema sysTableSchema;
	getSchema("systable", sysTableSchema);
	TableScan tableScan(sysTableHeapFile_);
	char *record;
	unsigned recordLen;
	while (SCAN_OVER != tableScan.getNextRecord(record, recordLen)) {
		Tuple tuple(sysTableSchema, record, recordLen);
		char tableName[sysTableSchema.fieldLengths.at(TABLE_NAME_POS)];
		tuple.getField(TABLE_NAME_POS, &tableName);
		std::string str(tableName);
		tableList.push_back(str);
	}
	return SUCCESS;
}

STATUS_CODE CatalogUtil::getTableID(const char *tableName, int& tableID) {
	DEBUG("CatalogUtil::getTableID");
	Schema sysTableSchema;
	getSchema("SYSTABLE", sysTableSchema);
	TableScan tableScan(sysTableHeapFile_);
	char *record;
	unsigned recordLen;
	while (SCAN_OVER != tableScan.getNextRecord(record, recordLen)) {
		Tuple tuple(sysTableSchema, record, recordLen);
		if (tuple.fieldMatch(TABLE_NAME_POS, tableName) == true) {
			tuple.getField(TABLE_ID_POS, &tableID);
			DEBUG ("getTableID"<<tableID);
			return SUCCESS;
		}
	}
	return TABLE_NOT_FOUND;
}

STATUS_CODE CatalogUtil::addInitialSysIndexTableRecord() {
	CreateTable createTable;
	ExecuteStructures executeStructures;
	SchemaUtil::getInitialSysIndexSchema(executeStructures.schema);
	int error = createTable.execute(&executeStructures);
	return error;
}

STATUS_CODE CatalogUtil::addInitialSysIndexColTableRecord() {
	CreateTable createTable;
	ExecuteStructures executeStructures;
	SchemaUtil::getInitialSysIndexColSchema(executeStructures.schema);
	int error = createTable.execute(&executeStructures);
	return error;
}

STATUS_CODE CatalogUtil::addIndexEntry(ExecuteStructures *executeStructures) {

	Schema schema;
	getSchema(executeStructures->tableName.c_str(), schema);
	IndexHeaderPage indexHeaderPage;
	int numOfColumns = executeStructures->indexColumnNames.size();
	int colTypes[numOfColumns];
	int colSizes[numOfColumns];
	int totalKeySize = 0;
	for (int i = 0; i < numOfColumns; i++) {
		int schemaColNum = schema.getColumnNum(
				executeStructures->indexColumnNames[i]);
		if (schema.fieldTypes[schemaColNum] == TYPE_INTEGER) {
			colTypes[i] = COL_INTEGER;
			colSizes[i] = schema.fieldLengths[schemaColNum];
			totalKeySize = totalKeySize + colSizes[i];
		}
		if (schema.fieldTypes[schemaColNum] == TYPE_VARCHAR) {
			colTypes[i] = COL_VARCHAR;
			colSizes[i] = schema.fieldLengths[schemaColNum];
			totalKeySize = totalKeySize + colSizes[i];
		}

	}
	indexHeaderPage.createIndexHeaderPage(numOfColumns, colTypes, colSizes,
			totalKeySize);
	DEBUG("add index , index header page="<< indexHeaderPage.getPageNumber());

	int tableID;
	getTableID(schema.tableName.c_str(), tableID);
	InsertTable insertTable;
	ExecuteStructures toAddRecordInSysIndex;
	toAddRecordInSysIndex.tableName = "SYSINDEX";
	int indexID = sysIndexHeapFile_->getNextRecordNumber();
	toAddRecordInSysIndex.insertValues.push_back(Util::int_to_string(indexID));
	toAddRecordInSysIndex.insertValues.push_back(executeStructures->indexName);
	toAddRecordInSysIndex.insertValues.push_back(Util::int_to_string(
			executeStructures->indexColumnNames.size()));
	toAddRecordInSysIndex.insertValues.push_back(Util::int_to_string(
			indexHeaderPage.getPageNumber()));
	toAddRecordInSysIndex.insertValues.push_back(Util::int_to_string(tableID));

	int error = insertTable.execute(&toAddRecordInSysIndex);

	for (int i = 0; i < numOfColumns; i++) {
		InsertTable toInsertTableInSysIndexCol;
		ExecuteStructures toAddRecordInSysIndexCol;
		toAddRecordInSysIndexCol.tableName = "SYSINDEXCOL";
		toAddRecordInSysIndexCol.insertValues.push_back(Util::int_to_string(
				sysIndexColHeapFile_->getNextRecordNumber()));
		toAddRecordInSysIndexCol.insertValues.push_back(Util::int_to_string(
				indexID));
		int schemaColNum = schema.getColumnNum(
				executeStructures->indexColumnNames[i]);
		toAddRecordInSysIndexCol .insertValues.push_back(Util::int_to_string(
				schemaColNum));
		error = toInsertTableInSysIndexCol.execute(&toAddRecordInSysIndexCol);
	}
	return error;
}

STATUS_CODE CatalogUtil::deleteIndexEntry(ExecuteStructures *executeStructures) {

}

bool CatalogUtil::checkIfIndexExists(Schema& schema,
		std::string identifierValue,
		std::vector<int>& indexHeaderPageNumberList) {
	if (schema.tableName == "SYSTABLE" || schema.tableName == "SYSCOL"
			|| schema.tableName == "SYSINDEX" || schema.tableName
			== "SYSINDEXCOL") {
		return false;
	}

	//go through sysindexcol to indexids
	Select select;
	ExecuteStructures executeStructures_1;
	executeStructures_1.tableName = "SYSINDEXCOL";
	executeStructures_1.resultSetColumnList.push_back("INDEX_ID");
	Schema sysIndexColSchema;
	getSchema(executeStructures_1.tableName.c_str(), sysIndexColSchema);
	executeStructures_1.schema = sysIndexColSchema;
	ExprInfo exprInfo1(ExprInfo::IDENTIFIER_TYPE, "SYS_COL_ID");
	ExprInfo exprInfo2(ExprInfo::LITERAL_TYPE, Util::int_to_string(
			schema.getColumnNum(identifierValue)));
	ExprInfo exprInfo3(ExprInfo::OPERATOR_TYPE, "=");
	executeStructures_1.whereConditionExprList.push_back(exprInfo1);
	executeStructures_1.whereConditionExprList.push_back(exprInfo2);
	executeStructures_1.whereConditionExprList.push_back(exprInfo3);
	select.execute(&executeStructures_1);

	//for each index_id, get index header page numbers from sysindex and add to the list
	for (unsigned i = 0; i < select.resultSet.size(); i++) {
		Select selectQuerytoGetFromSysIndex;
		ExecuteStructures toGetFromSysIndex;
		toGetFromSysIndex.resultSetColumnList.push_back("INDEX_HDR_PG_NO");
		toGetFromSysIndex.tableName = "SYSINDEX";
		ExprInfo exprInfo1(ExprInfo::IDENTIFIER_TYPE, "INDEX_ID");
		ExprInfo exprInfo2(ExprInfo::LITERAL_TYPE,
				select.resultSet[i].getFieldInStringFormat("INDEX_ID"));
		ExprInfo exprInfo3(ExprInfo::OPERATOR_TYPE, "=");
		toGetFromSysIndex.whereConditionExprList.push_back(exprInfo1);
		toGetFromSysIndex.whereConditionExprList.push_back(exprInfo2);
		toGetFromSysIndex.whereConditionExprList.push_back(exprInfo3);
		int error = selectQuerytoGetFromSysIndex.execute(&toGetFromSysIndex);
		if (SUCCESS != error) {
			return error;
		}
		for (unsigned j = 0; j < selectQuerytoGetFromSysIndex.resultSet.size(); j++) {
			int
					indexHeaderPageNum =
							Util::string_to_int(
									selectQuerytoGetFromSysIndex.resultSet[j].getFieldInStringFormat(
											"INDEX_HDR_PG_NO"));
			indexHeaderPageNumberList.push_back(indexHeaderPageNum);
		}
		return true;
	}
	return false;
}
