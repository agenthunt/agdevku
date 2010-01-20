/*
 * ExecuteStructures.cpp
 *
 *  Created on: 28-Nov-2009
 *      Author: shailesh
 */

#include "ExecuteStructures.h"
#include <string.h>
ExecuteStructures::ExecuteStructures() {
	// TODO Auto-generated constructor stub
	executeStatement = -1;
	afterFromKeyWord = false;
	allColumns = false;
	memset(queryString,'\0',1024);
	select = NULL;
}

ExecuteStructures::~ExecuteStructures() {
	// TODO Auto-generated destructor stub
}
