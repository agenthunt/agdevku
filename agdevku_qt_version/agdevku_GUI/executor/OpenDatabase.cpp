/*
 * OpenDatabase.cpp
 *
 *  Created on: 11-Dec-2009
 *      Author: shailesh
 */

#include "OpenDatabase.h"
#include "../utils/ValidatorUtil.h"

OpenDatabase::OpenDatabase() {
	// TODO Auto-generated constructor stub

}

OpenDatabase::~OpenDatabase() {
	// TODO Auto-generated destructor stub
}

STATUS_CODE OpenDatabase::execute(ExecuteStructures *executeStructures) {


	BufferManager *bufMgr = BufferManager::getInstance();
	const char *databaseName = executeStructures->databaseName.c_str();
	return bufMgr->openDatabase(databaseName);

}
