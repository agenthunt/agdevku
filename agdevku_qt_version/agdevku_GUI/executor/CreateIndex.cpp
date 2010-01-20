/*
 * CreateIndex.cpp
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#include "CreateIndex.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/ValidatorUtil.h"
CreateIndex::CreateIndex() {
	// TODO Auto-generated constructor stub

}

CreateIndex::~CreateIndex() {
	// TODO Auto-generated destructor stub
}


STATUS_CODE CreateIndex::execute(ExecuteStructures *executeStructures) {

	int error = ValidatorUtil::basicValidation();
	if (SUCCESS != error) {
		return error;
	}
	CatalogUtil catalogUtil;
	return catalogUtil.addIndexEntry(executeStructures);
}
