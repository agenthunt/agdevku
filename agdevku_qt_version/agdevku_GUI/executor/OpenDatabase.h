/*
 * OpenDatabase.h
 *
 *  Created on: 11-Dec-2009
 *      Author: shailesh
 */

#ifndef OPENDATABASE_H_
#define OPENDATABASE_H_
#include "../executor/ExecuteStructures.h"
#include "../global/GlobalStructures.h"
class OpenDatabase {
public:
	OpenDatabase();
	virtual ~OpenDatabase();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* OPENDATABASE_H_ */
