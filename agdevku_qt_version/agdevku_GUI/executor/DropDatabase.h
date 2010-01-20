/*
 * DropDatabase.h
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#ifndef DROPDATABASE_H_
#define DROPDATABASE_H_
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class DropDatabase {
public:
	DropDatabase();
	virtual ~DropDatabase();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* DROPDATABASE_H_ */
