/*
 * Schema.cpp
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#include <iomanip>
#include "Schema.h"
#include "../utils/util.h"
#include "../utils/debug.h"
#include "../utils/DataTypeLookup.h"
#include "../utils/FieldLengthLookup.h"
Schema::Schema() {
	// TODO Auto-generated constructor stub
	numOfColumns = 0;
}

Schema::Schema(std::string strDescSchema) {
	prepareSchemaObj(strDescSchema);
}

void Schema::prepareSchemaObj(std::string strDescSchema) {
	std::vector<string> splitVals = Util::split(strDescSchema, ",");
	tableName = splitVals.at(0);
	numOfColumns = Util::string_to_int(splitVals.at(1));

	for (unsigned i = 2; i < splitVals.size();) {
		columnNames.push_back(splitVals.at(i));
		i++;
		fieldTypes.push_back(Util::string_to_int(splitVals.at(i)));
		i++;
		fieldLengths.push_back(Util::string_to_int(splitVals.at(i)));
		i++;
	}
}

Schema::~Schema() {
	// TODO Auto-generated destructor stub
}

std::string Schema::toString() {
	std::stringstream str;
	str << tableName << "," << numOfColumns << "\n";
	for (int i = 0; i < numOfColumns; i++) {
		str << columnNames.at(i) << "\t";
		str << DataTypeLookup::getTypeDesc(fieldTypes.at(i)) << "\t";
		str << fieldLengths.at(i) << "\n";
	}
	str << "\n";
	return str.str();
}

std::string Schema::describeSchema() {
	std::stringstream mystream;
	int len = strlen("CREATE TABLE ");
	len = len + tableName.length();
	mystream << "CREATE TABLE " << tableName << "(\n";
	for (int i = 0; i < numOfColumns; i++) {
		mystream << columnNames.at(i)<<"\t";
		mystream << DataTypeLookup::getTypeDesc(fieldTypes.at(i));
		mystream << FieldLengthLookup::getTypeDesc(fieldLengths.at(i)) << ",\n";
	}
	mystream <<");\n";
	return mystream.str();
}


int Schema::getColumnNum(std::string columnName){
	for (unsigned i = 0; i < columnNames.size(); i++) {
		if(columnName==columnNames[i]){
			return i;
		}
	}
}
