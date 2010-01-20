/*
 * DataTypeLookup.h
 *
 *  Created on: 28-Nov-2009
 *      Author: shailesh
 */

#ifndef DATATYPELOOKUP_H_
#define DATATYPELOOKUP_H_
#include "../global/GlobalDefines.h"
#include "string.h"
class DataTypeLookup {
public:
	static int getType(char *dataTypeString) {
		if (strcmp(dataTypeString, "INTEGER") == 0) {
			return TYPE_INTEGER;
		}else if (strcmp(dataTypeString, "VARCHAR") == 0) {
			return TYPE_VARCHAR;
		}
		return -1;
	}

	static std::string getTypeDesc(int dataType) {
		switch (dataType) {
		case TYPE_INTEGER:
			return "INTEGER";
		case TYPE_VARCHAR:
			return "VARCHAR";
		default:
			break;
		}
		return "UNKNOWN_DATA_TYPE";
	}
};
#endif /* DATATYPELOOKUP_H_ */
