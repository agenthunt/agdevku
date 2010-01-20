/*
 * Update.cpp
 *
 *  Created on: 07-Jan-2010
 *      Author: shailesh
 */

#include "Update.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
#include "../global/StatusCodes.h"
#include "../utils/PostFixEvaluator.h"
#include "../heap/TableScan.h"
#include "../heap/Tuple.h"
#include "../utils/debug.h"
Update::Update() {
	// TODO Auto-generated constructor stub

}

Update::~Update() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE Update::execute(ExecuteStructures *executeStructures) {
	DEBUG("inside update execute");
	DEBUG("tableName"<<executeStructures->tableName.c_str())
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

	HeapFile heapFile(schema.tableName.c_str());
	TableScan tableScan(&heapFile);

	char *record;
	unsigned recordLen;
	int count = 0;
	DEBUG("starting table scan");
	while (tableScan.getNextRecord(record, recordLen) != SCAN_OVER) {
		Tuple tuple(schema, record, recordLen);
		if (executeStructures->whereConditionExprList.size() > 0) {
			PostFixEvaluator postFixEvaluator(tuple);
			if (postFixEvaluator.evaluate(
					executeStructures->whereConditionExprList) == true) {
				//add to result set;
				DEBUG("to update rows="<<tuple.toString());
				RIDStruct rid = tableScan.getCurrentRID();
				toUpdateList.push_back(rid);
				count++;
			}
		} else {//no where conditions
			//add all to resultset
			DEBUG("to update rows="<<tuple.toString());
			RIDStruct rid = tableScan.getCurrentRID();
			toUpdateList.push_back(rid);
			count++;
		}

	}

	for (unsigned i = 0; i < toUpdateList.size(); i++) {
		DEBUG("to delete RID={"<<toUpdateList[i].pageNumber<<","<<toUpdateList[i].slotNumber<<"}");
		//get the record from heap file
		char *record;
		unsigned recordLen;
		heapFile.getRecord(toUpdateList[i], record, recordLen);
		Tuple tuple(schema, record, recordLen);
		//set the relevant fields
		for (unsigned j = 0; j
				< executeStructures->updateQueryColumnNames.size(); j++) {
			tuple.setFieldByConversion(
					executeStructures->updateQueryColumnNames[i],
					executeStructures->updateQueryColumnValues[i]);
		}
		//updateRecord
		DEBUG("recordLen="<<recordLen);
		heapFile.updateRecord(toUpdateList[i], record, recordLen);
	}
	return SUCCESS;
}
