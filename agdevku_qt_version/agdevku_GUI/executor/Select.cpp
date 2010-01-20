/*
 * Select.cpp
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#include "Select.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
#include "../global/StatusCodes.h"
#include "../heap/Tuple.h"
#include "../utils/debug.h"
#include "../utils/util.h"
#include "../utils/PostFixEvaluator.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include "../global/GlobalDefines.h"
#include "../bplustree/BPlusTree.h"
#include "../bplustree/IndexHeaderPage.h"
#include <ctime>
#include <algorithm>
Select::Select() {
	// TODO Auto-generated constructor stub
	tableScanOpened = false;
	allColumns_ = false;
}

Select::~Select() {
	// TODO Auto-generated destructor stub
	if (tableScanOpened == true) {
		delete tableScan;
	}
}

STATUS_CODE Select::execute(ExecuteStructures *executeStructures) {
	totalNumOfRecords = 0;
	time(&start);
	timerUtil.startTimer();
	resultSetColumnList_ = executeStructures->resultSetColumnList;
	allColumns_ = executeStructures->allColumns;
	whereConditionExprList_.assign(
			executeStructures->whereConditionExprList.begin(),
			executeStructures->whereConditionExprList.end());
	DEBUG("inside selectTable execute");
	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;

	error = catalogUtil.getSchema(executeStructures->tableName.c_str(), schema);
	if (error == TABLE_NOT_FOUND) {
		return error;
	}
	heapFile = new HeapFile(schema.tableName.c_str());

	bool indexExists = false;
	std::vector<int> indexHeaderPageNumberList;
	for (unsigned i = 0; i < whereConditionExprList_.size(); ++i) {
		if (whereConditionExprList_[i].type_ == ExprInfo::IDENTIFIER_TYPE) {
			bool tmp = catalogUtil.checkIfIndexExists(schema,
					whereConditionExprList_[i].identifierValue,
					indexHeaderPageNumberList);
			if (indexExists == false && tmp == true) {
				indexExists = true;
			}
		}
	}

	if (indexExists == true && USING_INDEX == true) {
		//get from index and return;
		for (unsigned i = 0; i < indexHeaderPageNumberList.size(); i++) {
			DEBUG("indexHeaderPageNumberList["<<i<<"]="<<indexHeaderPageNumberList[i]);
		}
		//tmp hack, pick the first index only
		int indexHeaderPageNum = indexHeaderPageNumberList[0];
		IndexHeaderPage indexHeaderPage(indexHeaderPageNum);
		BPlusTree bplusTree(indexHeaderPageNum);
		int keySize = indexHeaderPage.getKeySize();

		std::vector<std::string> columnList;
		std::vector<std::string> literalList;
		int operatorType;
		for (unsigned i = 0; i < whereConditionExprList_.size(); i++) {
			if (whereConditionExprList_[i].type_ == ExprInfo::IDENTIFIER_TYPE) {
				columnList.push_back(whereConditionExprList_[i].identifierValue);
			} else if (whereConditionExprList_[i].type_
					== ExprInfo::OPERATOR_TYPE) {
				if (whereConditionExprList_[i].operatorValue == "=") {
					operatorType = EQUAL_TO;
				} else if (whereConditionExprList_[i].operatorValue == "<") {
					operatorType = LESS_THAN;
				} else if (whereConditionExprList_[i].operatorValue == ">") {
					operatorType = GREATER_THAN;
				} else if (whereConditionExprList_[i].operatorValue == "<=") {
					operatorType = LESS_THAN_OR_EQUAL_TO;
				} else if (whereConditionExprList_[i].operatorValue == ">=") {
					operatorType = GREATER_THAN_OR_EQUAL_TO;
				}
			} else if (whereConditionExprList_[i].type_
					== ExprInfo::LITERAL_TYPE) {
				literalList.push_back(whereConditionExprList_[i].literalValue);
			}
		}

		char *keyData = new char[keySize];
		int offset = 0;
		for (unsigned i = 0; i < columnList.size(); i++) {
			int fieldNum = schema.getColumnNum(columnList[i]);
			if (schema.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
				int recordData = Util::string_to_int(literalList[i]);
				DEBUG("keyData"<<recordData);
				memcpy(&keyData[offset], &recordData,
						schema.fieldLengths[fieldNum]);
				offset = offset + schema.fieldLengths[fieldNum];
			} else if (schema.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
				const char *recordData = literalList[i].c_str();
				memcpy(&keyData[offset], recordData,
						schema.fieldLengths[fieldNum]);
				offset = offset + schema.fieldLengths[fieldNum];
			}
		}

		int leafPageNum = bplusTree.searchInBPlusTree(keyData);
		bplusTree.bplusTreeSearchKeyInLeafNodeWithOp(leafPageNum, keyData,
				operatorType, ridList, keyList);
		DEBUG("keyList from ridList"<<ridList.size());
		delete keyData;
		if (USE_CLIENT_SIDE_SORT == true) {
			sort(ridList.begin(), ridList.end());
		}
		for (unsigned j = 0; j < ridList.size(); j++) {
			char *record;
			unsigned recordLen;
			heapFile->getRecord(ridList[j], record, recordLen);
			Tuple tuple(schema, record, recordLen);
			resultSet.push_back(tuple);
		}
		timerUtil.endTimer();
		timerUtil.updateNumberOfRows(resultSet.size());
		std::string elapsedTime = timerUtil.getElapsedTimeInMilliSeconds();
		elapsedTime.append("  milliseconds");
		timerUtil.updateExecutionTimeMessage(elapsedTime);

		return SUCCESS;
	}

	tableScan = new TableScan(heapFile);
	tableScanOpened = true;
	char *record;
	unsigned recordLen;
	int count = 0;
	while (tableScan->getNextRecord(record, recordLen) != SCAN_OVER) {
		Tuple tuple(schema, record, recordLen);
		if (executeStructures->whereConditionExprList.size() > 0) {
			PostFixEvaluator postFixEvaluator(tuple);
			if (postFixEvaluator.evaluate(
					executeStructures->whereConditionExprList) == true) {
				//add to result set;
				DEBUG("tuple"<<tuple.toString());
				resultSet.push_back(tuple);
				count++;
			}
		} else {//no where conditions
			//add all to resultset
			DEBUG("tuple"<<tuple.toString());
			resultSet.push_back(tuple);
			count++;
		}

		totalNumOfRecords = count;
		if (count == ROWS_PER_PAGE) {
			break;
		}
	}
	time(&end);
	timerUtil.endTimer();
	diff = difftime(end, start);
	timerUtil.updateExecutionTime(diff);
	timerUtil.updateNumberOfRows(totalNumOfRecords);
	std::string elapsedTime = timerUtil.getElapsedTimeInMilliSeconds();
	elapsedTime.append("  milliseconds");
	timerUtil.updateExecutionTimeMessage(elapsedTime);
	return SUCCESS;
}

void Select::loadFirstPage() {
	timerUtil.startTimer();
	totalNumOfRecords = 0;
	if (tableScanOpened == true) {
		delete tableScan;
		tableScan = new TableScan(heapFile);
	}
	resultSet.clear();
	char *record;
	unsigned recordLen;
	int count = 0;
	while (tableScan->getNextRecord(record, recordLen) != SCAN_OVER) {
		Tuple tuple(schema, record, recordLen);
		if (whereConditionExprList_.size() > 0) {
			PostFixEvaluator postFixEvaluator(tuple);
			if (postFixEvaluator.evaluate(whereConditionExprList_) == true) {
				//add to result set;
				DEBUG("tuple"<<tuple.toString());
				resultSet.push_back(tuple);
				count++;
			}
		} else {//no where conditions
			//add all to resultset
			DEBUG("tuple"<<tuple.toString());
			resultSet.push_back(tuple);
			count++;
		}

		totalNumOfRecords = count;
		if (count == ROWS_PER_PAGE) {
			break;
		}
	}
	DEBUG("count of rows="<<count);
	timerUtil.updateNumberOfRows(totalNumOfRecords);
	timerUtil.endTimer();
	std::string elapsedTime = timerUtil.getElapsedTimeInMilliSeconds();
	elapsedTime.append("  milliseconds");
	timerUtil.updateExecutionTimeMessage(elapsedTime);
}

void Select::loadNextPage() {
	resultSet.clear();
	char *record;
	unsigned recordLen;
	int count = 0;
	timerUtil.startTimer();
	while (tableScan->getNextRecord(record, recordLen) != SCAN_OVER) {
		Tuple tuple(schema, record, recordLen);
		if (whereConditionExprList_.size() > 0) {
			PostFixEvaluator postFixEvaluator(tuple);
			if (postFixEvaluator.evaluate(whereConditionExprList_) == true) {
				//add to result set;
				DEBUG("tuple"<<tuple.toString());
				resultSet.push_back(tuple);
				count++;
			}
		} else {//no where conditions
			//add all to resultset
			DEBUG("tuple"<<tuple.toString());
			resultSet.push_back(tuple);
			count++;
		}

		if (count == ROWS_PER_PAGE) {
			break;
		}
	}
	totalNumOfRecords = totalNumOfRecords + count;
	timerUtil.updateNumberOfRows(totalNumOfRecords);
	DEBUG("count of rows="<<count);
	timerUtil.endTimer();
	std::string elapsedTime = timerUtil.getElapsedTimeInMilliSeconds();
	elapsedTime.append("  milliseconds");
	timerUtil.updateExecutionTimeMessage(elapsedTime);
	DEBUG("totalNumOfRecords"<<totalNumOfRecords);
}

void Select::loadPrevPage() {

}

void Select::loadLastPage() {
	timerUtil.startTimer();
	time(&start);
	resultSet.clear();
	char *record;
	unsigned recordLen;
	int count = 0;
	while (tableScan->getNextRecord(record, recordLen) != SCAN_OVER) {
		Tuple tuple(schema, record, recordLen);
		if (whereConditionExprList_.size() > 0) {
			PostFixEvaluator postFixEvaluator(tuple);
			if (postFixEvaluator.evaluate(whereConditionExprList_) == true) {
				//add to result set;
				DEBUG("tuple"<<tuple.toString());
				resultSet.push_back(tuple);
				count++;
			}
		} else {//no where conditions
			//add all to resultset
			DEBUG("tuple"<<tuple.toString());
			resultSet.push_back(tuple);
			count++;
		}

		if (count > (ROWS_PER_PAGE)) {
			resultSet.clear();
			totalNumOfRecords = totalNumOfRecords + count;
			count = 0;
		}
	}
	DEBUG("count of rows="<<count);
	time(&end);
	timerUtil.endTimer();
	diff = difftime(end, start);
	timerUtil.updateExecutionTime(diff);
	totalNumOfRecords = totalNumOfRecords + count;
	timerUtil.updateNumberOfRows(totalNumOfRecords);
	//timerUtil.updateNumberOfRows(resultSet.size());
	std::string elapsedTime = timerUtil.getElapsedTimeInMilliSeconds();
	elapsedTime.append("  milliseconds");
	timerUtil.updateExecutionTimeMessage(elapsedTime);
	DEBUG("count of rows="<<count);
	DEBUG("totalNumOfRecords"<<totalNumOfRecords);
}

