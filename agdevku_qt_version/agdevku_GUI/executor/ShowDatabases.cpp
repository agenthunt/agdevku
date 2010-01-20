/*
 * ShowDatabases.cpp
 *
 *  Created on: 17-Nov-2009
 *      Author: shailesh
 */

#include "ShowDatabases.h"
#include "../bufmgr/BufferManager.h"
#include "../global/StatusCodes.h"
ShowDatabases::ShowDatabases() {
	// TODO Auto-generated constructor stub

}

ShowDatabases::~ShowDatabases() {
	// TODO Auto-generated destructor stub
}


STATUS_CODE ShowDatabases::execute(ExecuteStructures *executeStructures){
		BufferManager *bufMgr = BufferManager::getInstance();
		executeStructures->listOfDatabases = bufMgr->getListOfDatabases();
		return SUCCESS;
}
