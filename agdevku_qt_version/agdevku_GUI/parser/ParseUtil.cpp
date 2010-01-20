/*
 * ParseUtil.cpp
 *
 *  Created on: 30-Nov-2009
 *      Author: shailesh
 */

#include "ParseUtil.h"
#include <ctype.h>
// prototype of bison-generated parser function
extern int yyparse();
extern int yy_scan_string(const char *);
ParseUtil::ParseUtil() {
	// TODO Auto-generated constructor stub
	executeStructures = new ExecuteStructures();
}

ParseUtil::~ParseUtil() {
	// TODO Auto-generated destructor stub
	delete executeStructures;
}

//STATUS_CODE ParseUtil::parseQuery(const char *buf) {
STATUS_CODE ParseUtil::parseQuery(QString buf) {

	strcpy(executeStructures->queryString,buf.toUpper().toStdString().c_str());
	DEBUG("parsetuil query="<<executeStructures->queryString);
	yy_scan_string(executeStructures->queryString);
	if (yyparse() != 0) {
		return PARSE_FAILED;
	}
	return SUCCESS;
}
