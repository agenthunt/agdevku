/*
 * ExecuteStructures.h
 *
 *  Created on: 28-Nov-2009
 *      Author: shailesh
 */

#ifndef EXECUTESTRUCTURES_H_
#define EXECUTESTRUCTURES_H_
#include "../catalog/Schema.h"
#include "ExprInfo.h"
class Select;
#include <vector>
#include <string>
class ExecuteStructures {
public:
	ExecuteStructures();
	virtual ~ExecuteStructures();
	char errorMessage[512];
	char queryString[1024];
	std::vector<std::string> listOfDatabases;
	std::vector<std::string> tableList;
	std::string databaseName;
	Schema schema;
	std::string tableName;
	std::string indexName;
	std::vector<std::string> indexColumnNames;
	std::vector<std::string> insertValues;
	std::vector<std::string> updateQueryColumnNames;
	std::vector<std::string> updateQueryColumnValues;
	std::vector<ExprInfo> whereConditionExprList;
	std::vector<std::string> resultSetColumnList;
	bool afterFromKeyWord;
	bool allColumns;
	int executeStatement;
	std::string schemaDescription;
	Select *select;
	static const int CREATE_DATABASE_STATEMENT = 0;
	static const int SHOW_DATABASES_STATEMENT = 1;
	static const int OPEN_DATABASE_STATEMENT = 2;
	static const int DROP_DATABASE_STATEMENT = 3;
	static const int CREATE_TABLE_STATEMENT = 4;
	static const int INSERT_STATEMENT = 5;
	static const int SELECT_STATEMENT = 6;
	static const int DELETE_STATEMENT = 7;
	static const int UPDATE_STATEMENT = 8;
	static const int DROP_TABLE_STATEMENT = 9;
	static const int CREATE_INDEX_STATEMENT = 10;
	static const int DROP_INDEX_STATEMENT = 11;
	static const int SHOW_TABLES_STATEMENT = 12;
	static const int DESCRIBE_STATEMENT = 13;
};

#endif /* EXECUTESTRUCTURES_H_ */
