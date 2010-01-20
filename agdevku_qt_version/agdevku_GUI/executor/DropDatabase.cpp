/*
 * DropDatabase.cpp
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#include "DropDatabase.h"
#include "../bufmgr/BufferManager.h"
#include "../utils/debug.h"
DropDatabase::DropDatabase() {
	// TODO Auto-generated constructor stub

}

DropDatabase::~DropDatabase() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE DropDatabase::execute(ExecuteStructures *executeStructures){
	DEBUG("drop database="<<executeStructures->databaseName);
	BufferManager *bufMgr = BufferManager::getInstance();
	return bufMgr->dropDatabase(executeStructures->databaseName.c_str());
}
