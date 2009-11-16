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
	virtual ~Schema();
	int numOfColumns;
	int recSize;
	std::vector<std::string> columnNames;
	std::vector<int> fieldLengths;
	std::vector<int> fieldTypes;
};

#endif /* SCHEMA_H_ */
