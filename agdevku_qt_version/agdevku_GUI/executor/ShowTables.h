/*
 * ShowTables.h
 *
 *  Created on: 12-Dec-2009
 *      Author: shailesh
 */

#ifndef SHOWTABLES_H_
#define SHOWTABLES_H_
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class ShowTables {
public:
	ShowTables();
	virtual ~ShowTables();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* SHOWTABLES_H_ */
