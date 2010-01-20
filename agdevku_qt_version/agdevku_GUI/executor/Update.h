/*
 * Update.h
 *
 *  Created on: 07-Jan-2010
 *      Author: shailesh
 */

#ifndef UPDATE_H_
#define UPDATE_H_
#include "../executor/ExecuteStructures.h"
#include "../global/GlobalStructures.h"
#include <vector>
class Update {
public:
	Update();
	virtual ~Update();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
	std::vector<RIDStruct> toUpdateList;
};

#endif /* UPDATE_H_ */
