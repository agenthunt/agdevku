/*
 * ValidatorUtil.h
 *
 *  Created on: 04-Dec-2009
 *      Author: shailesh
 */

#ifndef VALIDATORUTIL_H_
#define VALIDATORUTIL_H_
#include "../global/GlobalStructures.h"
#include "../global/StatusCodes.h"
#include "../bufmgr/BufferManager.h"
class ValidatorUtil {
public:
	static STATUS_CODE basicValidation() {
		BufferManager *bufMgr = BufferManager::getInstance();
		if (bufMgr->isDatabaseOpen_ == false) {
			return DATABASE_NOT_OPEN;
		}
		return SUCCESS;
	}


};
#endif /* VALIDATORUTIL_H_ */
