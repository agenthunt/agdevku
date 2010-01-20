/*
 * ExecuteManager.cpp
 *
 *  Created on: 28-Nov-2009
 *      Author: shailesh
 */
#include "ExecuteManager.h"

#include "../global/StatusCodes.h"
#include "../executor/CreateTable.h"
#include "../executor/ShowDatabases.h"
#include "../executor/CreateDatabase.h"
#include "../executor/OpenDatabase.h"
#include "../executor/InsertTable.h"
#include "../executor/ShowTables.h"
#include "../executor/DropDatabase.h"
#include "../executor/CreateIndex.h"
#include "../executor/Describe.h"
#include "../utils/ErrorLookupTable.h"
#include "../executor/Select.h"
#include "../executor/Delete.h"
#include "../executor/Update.h"
#include "../executor/DropTable.h"

#include "../utils/debug.h"
#include <string.h>
ExecuteManager::ExecuteManager() {

}

ExecuteManager::~ExecuteManager() {

}

STATUS_CODE ExecuteManager::execute(ExecuteStructures *executeStructures) {
	int error = UNKNOWN_ERROR;
	switch (executeStructures->executeStatement) {

	case ExecuteStructures::CREATE_DATABASE_STATEMENT: {
		CreateDatabase createDatabase;
		error = createDatabase.execute(executeStructures);
		break;
	}
	case ExecuteStructures::SHOW_DATABASES_STATEMENT: {
		ShowDatabases showDatabases;
		error = showDatabases.execute(executeStructures);
		break;
	}
	case ExecuteStructures::OPEN_DATABASE_STATEMENT: {
		OpenDatabase openDatabase;
		error = openDatabase.execute(executeStructures);
		break;
	}
	case ExecuteStructures::DROP_DATABASE_STATEMENT: {
		DEBUG("drop database");
		DropDatabase dropDatabase;
		error = dropDatabase.execute(executeStructures);
		break;
	}
	case ExecuteStructures::SHOW_TABLES_STATEMENT: {
		ShowTables showTables;
		error = showTables.execute(executeStructures);
		break;
	}
	case ExecuteStructures::CREATE_TABLE_STATEMENT: {
		CreateTable createTable;
		error = createTable.execute(executeStructures);
		break;
	}
	case ExecuteStructures::INSERT_STATEMENT: {
		InsertTable insertTable;
		error = insertTable.execute(executeStructures);
		break;
	}
	case ExecuteStructures::DESCRIBE_STATEMENT: {
		Describe describe;
		error = describe.execute(executeStructures);
		break;
	}
	case ExecuteStructures::SELECT_STATEMENT: {
		Select *select = new Select();
		error = select->execute(executeStructures);
		executeStructures->select = select;
		break;
	}
	case ExecuteStructures::DELETE_STATEMENT: {
		Delete deleteStatement;
		error = deleteStatement.execute(executeStructures);
		break;
	}
	case ExecuteStructures::UPDATE_STATEMENT: {
		Update update;
		error = update.execute(executeStructures);
		break;
	}
	case ExecuteStructures::DROP_TABLE_STATEMENT: {
		DropTable dropTable;
		error = dropTable.execute(executeStructures);
		break;
	}
	case ExecuteStructures::CREATE_INDEX_STATEMENT:{
		CreateIndex createIndex;
		error = createIndex.execute(executeStructures);
		break;
	}
	case ExecuteStructures::DROP_INDEX_STATEMENT:
		break;
	default:
		strcpy(executeStructures->errorMessage, ErrorLookupTable::lookup(
				UNKNOWN_ERROR).c_str());

	}

	strcpy(executeStructures->errorMessage,
			ErrorLookupTable::lookup(error).c_str());
	return error;
}
