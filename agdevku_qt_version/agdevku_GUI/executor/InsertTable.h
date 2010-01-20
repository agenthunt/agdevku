/*
 * InsertTable.h
 *
 *  Created on: 02-Dec-2009
 *      Author: shailesh
 */

#ifndef INSERTTABLE_H_
#define INSERTTABLE_H_
#include "../executor/ExecuteStructures.h"
#include "../global/GlobalStructures.h"
class InsertTable {
public:
	InsertTable();
	virtual ~InsertTable();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* INSERTTABLE_H_ */
