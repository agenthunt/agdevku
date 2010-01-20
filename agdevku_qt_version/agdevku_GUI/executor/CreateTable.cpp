/*
 * CreateTable.cpp
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#include "CreateTable.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
CreateTable::CreateTable() {
	// TODO Auto-generated constructor stub

}

CreateTable::~CreateTable() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE CreateTable::execute(ExecuteStructures *executeStructures) {

	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;
	return catalogUtil.addTableEntry(executeStructures->schema);
}
