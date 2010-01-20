/*
 * CreateDatabase.cpp
 *
 *  Created on: 17-Nov-2009
 *      Author: shailesh
 */

#include "CreateDatabase.h"
#include "../bufmgr/BufferManager.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../utils/debug.h"
CreateDatabase::CreateDatabase() {
	// TODO Auto-generated constructor stub

}

CreateDatabase::~CreateDatabase() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE CreateDatabase::execute(ExecuteStructures *executeStructures){
	DEBUG("execute createdatabase" << executeStructures->databaseName);
	BufferManager *bufMgr = BufferManager::getInstance();
	return bufMgr->createDatabase(executeStructures->databaseName.c_str(),DEFAULT_NUM_OF_PAGES);
}
