/*
 * DropTable.h
 *
 *  Created on: 11-Dec-2009
 *      Author: shailesh
 */

#ifndef DROPTABLE_H_
#define DROPTABLE_H_
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class DropTable {
public:
	DropTable();
	virtual ~DropTable();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* DROPTABLE_H_ */
