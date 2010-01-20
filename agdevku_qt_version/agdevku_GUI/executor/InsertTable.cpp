/*
 * InsertTable.cpp
 *
 *  Created on: 02-Dec-2009
 *      Author: shailesh
 */

#include "InsertTable.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
#include "../global/StatusCodes.h"
#include "../heap/Tuple.h"
#include "../utils/debug.h"
#include "../utils/util.h"
#include "../utils/timer_util.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include "../bplustree/IndexHeaderPage.h"
#include "../bplustree/BPlusTree.h"
#include "../executor/Select.h"
InsertTable::InsertTable() {
	// TODO Auto-generated constructor stub

}

InsertTable::~InsertTable() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE InsertTable::execute(ExecuteStructures *executeStructures) {
	TimerUtil timerUtil;
	timerUtil.startTimer();
	DEBUG("inside insertTable execute");
	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;
	Schema schema;
	error = catalogUtil.getSchema(executeStructures->tableName.c_str(), schema);
	if (error == TABLE_NOT_FOUND) {
		return error;
	}
	//insert into first values (10); queries like this col is not specified
	if (executeStructures->insertValues.size() != schema.numOfColumns) {
		return PARSE_FAILED;
	}
	HeapFile heapFile(executeStructures->tableName.c_str());
	for (int i = 0; i < executeStructures->insertValues.size(); i++) {
		DEBUG("insertValue["<<i<<"]="<<executeStructures->insertValues[i]);
	}

	Tuple tuple(schema, executeStructures->insertValues);
	char *record = tuple.prepRecordForInsertion();
	for (int i = 0; i < tuple.getLength(); i++) {
		DEBUG("record after prep"<<record[i]);
	}
	RIDStruct rid;
	error = heapFile.insertRecord(record, tuple.getLength(), rid);
	if (SUCCESS != error) {
		return error;
	}
	int numOfRecords = heapFile.getNumOfRecords();
	if (numOfRecords % 5000 == 0) {
		std::cout << "NUM OF RECORDS INSERTED=" << numOfRecords << endl;
	}
	if (USING_INDEX == true) {
		DEBUG("using index")
		int tableID;
		catalogUtil.getTableID(schema.tableName.c_str(), tableID);
		Select select;
		ExecuteStructures toGetFromSysIndex;
		toGetFromSysIndex.resultSetColumnList.push_back("INDEX_ID");
		toGetFromSysIndex.resultSetColumnList.push_back("INDEX_HDR_PG_NO");
		toGetFromSysIndex.tableName = "SYSINDEX";
		ExprInfo exprInfo1(ExprInfo::IDENTIFIER_TYPE, "TAB_ID");
		ExprInfo
				exprInfo2(ExprInfo::LITERAL_TYPE, Util::int_to_string(tableID));
		ExprInfo exprInfo3(ExprInfo::OPERATOR_TYPE, "=");
		toGetFromSysIndex.whereConditionExprList.push_back(exprInfo1);
		toGetFromSysIndex.whereConditionExprList.push_back(exprInfo2);
		toGetFromSysIndex.whereConditionExprList.push_back(exprInfo3);
		error = select.execute(&toGetFromSysIndex);
		if (SUCCESS != error) {
			return error;
		}
		for(unsigned i=0;i<select.resultSet.size();i++){
			Select selectQuerytoGetFromSysIndexCol;
			ExecuteStructures toGetFromSysIndexCol;
			toGetFromSysIndexCol.resultSetColumnList.push_back("SYS_COL_ID");
			toGetFromSysIndexCol.tableName = "SYSINDEXCOL";
			ExprInfo exprInfo1(ExprInfo::IDENTIFIER_TYPE, "INDEX_ID");
			ExprInfo
					exprInfo2(ExprInfo::LITERAL_TYPE, select.resultSet[i].getFieldInStringFormat("INDEX_ID"));
			ExprInfo exprInfo3(ExprInfo::OPERATOR_TYPE, "=");
			toGetFromSysIndexCol.whereConditionExprList.push_back(exprInfo1);
			toGetFromSysIndexCol.whereConditionExprList.push_back(exprInfo2);
			toGetFromSysIndexCol.whereConditionExprList.push_back(exprInfo3);
			error = selectQuerytoGetFromSysIndexCol.execute(&toGetFromSysIndexCol);
			if (SUCCESS != error) {
				return error;
			}
			int indexHeaderPageNum = Util::string_to_int(select.resultSet[i].getFieldInStringFormat("INDEX_HDR_PG_NO"));
			IndexHeaderPage indexHeaderPage(indexHeaderPageNum);
			int keySize = indexHeaderPage.getKeySize();
			char *keyData = new char[keySize];
			tuple.getKeyData(selectQuerytoGetFromSysIndexCol.resultSetColumnList_,keyData);
			BPlusTree bplusTree(indexHeaderPageNum);
			bplusTree.insertIntoBPlusTree(keyData,rid);
			delete keyData;
		}
	}
	timerUtil.updateNumberOfRows(numOfRecords);
	timerUtil.endTimer();
	std::string elapsedTime = timerUtil.getElapsedTimeInMilliSeconds();
	elapsedTime.append("  milliseconds");
	timerUtil.updateExecutionTimeMessage(elapsedTime);
	return SUCCESS;
}
