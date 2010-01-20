/*
 * Delete.cpp
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#include "Delete.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
#include "../global/StatusCodes.h"
#include "../heap/Tuple.h"
#include "../utils/debug.h"
#include "../utils/PostFixEvaluator.h"
#include "../global/ExternDefOfGlobalVariables.h"
#include "../heap/TableScan.h"
Delete::Delete() {
	// TODO Auto-generated constructor stub

}

Delete::~Delete() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE Delete::execute(ExecuteStructures *executeStructures) {
	DEBUG("inside deleteTable execute");
	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;
	Schema schema;

	error = catalogUtil.getSchema(executeStructures->tableName.c_str(), schema);
	DEBUG("tableName="<<schema.tableName.c_str());
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
				DEBUG("to delete rows="<<tuple.toString());
				RIDStruct rid = tableScan.getCurrentRID();
				toDeleteList.push_back(rid);
				count++;
			}
		} else {//no where conditions
			//add all to resultset
			DEBUG("to delete rows="<<tuple.toString());
			RIDStruct rid = tableScan.getCurrentRID();
			toDeleteList.push_back(rid);
			count++;
		}

	}

	for (unsigned i = 0; i < toDeleteList.size(); i++) {
		DEBUG("to delete RID={"<<toDeleteList[i].pageNumber<<","<<toDeleteList[i].slotNumber<<"}");
		heapFile.deleteRecord(toDeleteList[i]);
	}

	return SUCCESS;
}
