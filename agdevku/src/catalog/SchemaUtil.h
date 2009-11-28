/*
 * SchemaUtil.h
 *
 *  Created on: 18-Nov-2009
 *      Author: shailesh
 */

#ifndef SCHEMAUTIL_H_
#define SCHEMAUTIL_H_
#include "Schema.h"
#include "CatalogConstants.h"
class SchemaUtil {
public:
	static void getInitialSysTableSchema(Schema& schema) {
		std::stringstream str;
		str << "systable,4," << "TABLE_ID," << Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH) << "," << "TABLE_NAME,"
				<< Util::int_to_string(TYPE_VARCHAR) << ","
				<< Util::int_to_string(DEFAULT_LENGTH) << "," << "NO_OF_COLS,"
				<< Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH) << "," << "HDR_PG_NO,"
				<< Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH);

		schema.prepareSchemaObj(str.str());

		//check out what this guy is claiming
		//ost.rdbuf()->freeze(0); //WITHOUT THIS, YOU HAVE A MEMORY LEAK
	}

	static void getInitialSysColSchema(Schema& schema) {
		std::stringstream str;
		str << "syscol,5," << "COL_ID," << Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH) << "," << "COL_NAME,"
				<< Util::int_to_string(TYPE_VARCHAR) << ","
				<< Util::int_to_string(DEFAULT_LENGTH) << "," << "COL_TYPE,"
				<< Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH) << "," << "COL_LENGTH,"
				<< Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH) << "," << "TAB_ID,"
				<< Util::int_to_string(TYPE_INTEGER) << ","
				<< Util::int_to_string(INTEGER_LENGTH);

		schema.prepareSchemaObj(str.str());

		//check out what this guy is claiming
		//ost.rdbuf()->freeze(0); //WITHOUT THIS, YOU HAVE A MEMORY LEAK
	}

private:
	//Not using anywhere, just for info
	typedef struct SysTableInitialStructure {
		int tableID;
		char tableName[32];
		int numOfColumns;
		int rowSize;
		int headerPageNumber;//directoryHeaderPagenumber
	} SysTableInitialStruct;
	SysTableInitialStruct sysTableInitialStruct_;
	typedef struct SysColInitialStructure {
		int columnID;
		char columnName[32];
		int tableID;
		int schemaColumnNumber;
		int nullable;//true or false
		int columnType;
		int columnSize;
	} SysColInitStruct;
	SysColInitStruct sysColInitStruct_;
};

#endif /* SCHEMAUTIL_H_ */
