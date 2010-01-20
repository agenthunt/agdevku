/*
 * Describe.cpp
 *
 *  Created on: 18-Dec-2009
 *      Author: shailesh
 */

#include "Describe.h"
#include "../catalog/CatalogUtil.h"
#include "../catalog/Schema.h"
#include "../utils/ValidatorUtil.h"
#include "../global/StatusCodes.h"
#include "../utils/debug.h"
Describe::Describe() {
	// TODO Auto-generated constructor stub

}

Describe::~Describe() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE Describe::execute(ExecuteStructures *executeStructures) {
	DEBUG("inside describeTable execute");
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
	executeStructures->schemaDescription = schema.describeSchema();
	DEBUG("schema Desc"<<executeStructures->schemaDescription);
	return SUCCESS;
}
