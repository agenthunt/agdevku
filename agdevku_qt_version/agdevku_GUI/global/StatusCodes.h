/*
 * StatusCodes.h
 *
 *  Created on: Oct 6, 2009
 *      Author: shailesh
 */

#ifndef STATUSCODES_H_
#define STATUSCODES_H_

#define TRUE true
#define FALSE false
#define SUCCESS  true
#define FAILURE	 false
#define INVALID_PAGE  -1
#define PAGE_NOT_FOUND 3
#define DISK_FILE_CREATE_ERROR 4
#define DISK_FILE_DELETE_ERROR 5
#define DISK_FILE_OPEN_ERROR 6
#define DISK_FILE_CLOSE_ERROR 7
#define DISK_PAGE_READ_ERROR 8
#define DISK_PAGE_WRITE_ERROR 9
#define BUFFER_FULL 10
#define DATABASE_NOT_OPEN 11

#define SCAN_OVER 12
#define TABLE_NOT_FOUND 13
#define DATABASE_ALREADY_EXISTS 14
#define TABLE_EXISTS 15
#define UNKNOWN_ERROR 16
#define PARSE_FAILED 17
#define DATABASE_DOES_NOT_EXIST 18
#define CLOSE_OPENED_DATABASE 19


#endif /* STATUSCODES_H_ */
