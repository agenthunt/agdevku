/*
 * ShowTables.cpp
 *
 *  Created on: 12-Dec-2009
 *      Author: shailesh
 */

#include "ShowTables.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
ShowTables::ShowTables() {
	// TODO Auto-generated constructor stub

}

ShowTables::~ShowTables() {
	// TODO Auto-generated destructor stub
}


STATUS_CODE ShowTables::execute(ExecuteStructures *executeStructures){
	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;
	return catalogUtil.getListOfTables(executeStructures->tableList);
}
