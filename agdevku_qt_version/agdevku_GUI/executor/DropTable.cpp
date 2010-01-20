/*
 * DropTable.cpp
 *
 *  Created on: 11-Dec-2009
 *      Author: shailesh
 */

#include "DropTable.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
DropTable::DropTable() {
	// TODO Auto-generated constructor stub

}

DropTable::~DropTable() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE DropTable::execute(ExecuteStructures *executeStructures) {

	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;
	catalogUtil.getSchema(executeStructures->tableName.c_str(),
			executeStructures->schema);
	return catalogUtil.deleteTableEntry(executeStructures->schema);
}
