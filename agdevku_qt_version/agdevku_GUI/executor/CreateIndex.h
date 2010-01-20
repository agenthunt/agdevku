/*
 * CreateIndex.h
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#ifndef CREATEINDEX_H_
#define CREATEINDEX_H_
#include "../catalog/Schema.h"
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class CreateIndex {
public:
	CreateIndex();
	virtual ~CreateIndex();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* CREATEINDEX_H_ */
