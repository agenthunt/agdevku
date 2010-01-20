/*
 * CreateDatabase.h
 *
 *  Created on: 17-Nov-2009
 *      Author: shailesh
 */

#ifndef CREATEDATABASE_H_
#define CREATEDATABASE_H_
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class CreateDatabase {
public:
	CreateDatabase();
	virtual ~CreateDatabase();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
private:

};

#endif /* CREATEDATABASE_H_ */
