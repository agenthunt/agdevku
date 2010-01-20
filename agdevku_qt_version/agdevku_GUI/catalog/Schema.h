/*
 * Schema.h
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#ifndef SCHEMA_H_
#define SCHEMA_H_
#include <string>
#include <vector>
class Schema {
public:
	Schema();
	Schema(std::string strDescSchema);
	virtual ~Schema();
	int numOfColumns;
	std::string tableName;
	std::vector<std::string> columnNames;
	std::vector<int> fieldTypes;
	std::vector<int> fieldLengths;
    void prepareSchemaObj(std::string strDescSchema);
    int getColumnNum(std::string columnName);
    std::string toString();
    std::string describeSchema();

};

#endif /* SCHEMA_H_ */
