/*
 * Schema.cpp
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#include "Schema.h"
#include "../utils/util.h"
#include "../utils/debug.h"
Schema::Schema() {
	// TODO Auto-generated constructor stub

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
