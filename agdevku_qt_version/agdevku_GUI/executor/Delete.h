/*
 * Delete.h
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#ifndef DELETE_H_
#define DELETE_H_
#include "../executor/ExecuteStructures.h"
#include "../global/GlobalStructures.h"
class Delete {
public:
	Delete();
	virtual ~Delete();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
	std::vector<RIDStruct> toDeleteList;
};

#endif /* DELETE_H_ */
