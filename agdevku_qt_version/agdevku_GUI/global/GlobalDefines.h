/*
 * GlobalDefines.h
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#ifndef GLOBALDEFINES_H_
#define GLOBALDEFINES_H_

#define INTEGER_LENGTH 4
#define TYPE_VARCHAR 0
#define TYPE_INTEGER 1
#define DEFAULT_LENGTH 32


//defining pageTypes here
#define TYPE_DB_MAIN_HEADER_PAGE 0
#define TYPE_DIRECTORY_HEADER_PAGE 1
#define TYPE_DIRECTORY_PAGE 2
#define TYPE_DATA_PAGE 3
#define TYPE_INDEX_PAGE 4
#define TYPE_LEAF_PAGE 5
#define TYPE_UNKNOWN_PAGE 6
#define TYPE_INDEX_HEADER_PAGE 7

//declares column type for bplus tree
#define COL_INTEGER 1
#define COL_FLOAT 2
#define COL_DOUBLE 3
#define COL_VARCHAR 4
#define COL_LONG 5

#define EQUAL_TO 1
#define GREATER_THAN 2
#define GREATER_THAN_OR_EQUAL_TO 3
#define LESS_THAN 4
#define LESS_THAN_OR_EQUAL_TO 5

#endif /* GLOBALDEFINES_H_ */
