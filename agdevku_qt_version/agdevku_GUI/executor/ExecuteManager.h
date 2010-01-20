/*
 * ExecutorInterface.h
 *
 *  Created on: 28-Nov-2009
 *      Author: shailesh
 */

#ifndef EXECUTEMANAGER_H_
#define EXECUTEMANAGER_H_
#include "../global/GlobalStructures.h"
#include "../executor/CreateTable.h"
#include "../catalog/Schema.h"
#include "ExecuteStructures.h"
class ExecuteManager{
public:
	ExecuteManager();
	~ExecuteManager();
	STATUS_CODE execute(ExecuteStructures *executeStructure);
};
#endif /* EXECUTORINTERFACE_H_ */
