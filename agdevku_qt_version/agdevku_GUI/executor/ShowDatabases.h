/*
 * ShowDatabases.h
 *
 *  Created on: 17-Nov-2009
 *      Author: shailesh
 */

#ifndef SHOWDATABASES_H_
#define SHOWDATABASES_H_
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class ShowDatabases {
public:
	ShowDatabases();
	virtual ~ShowDatabases();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* SHOWDATABASES_H_ */
