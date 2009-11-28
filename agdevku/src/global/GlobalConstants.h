/*
 * GlobalConstants.h
 *
 *  Created on: Oct 1, 2009
 *      Author: shailesh
 */


#ifndef GLOBALCONSTANTS_H_
#define GLOBALCONSTANTS_H_


/*data here means where your database files
created and stored */
//const char *DATABASES_FOLDER = "src/databases/";
const char *DATABASES_FOLDER = "/home/shailesh/workspace/agdevku/src/databases";
const int DEFAULT_PAGE_SIZE = 8192;
const int DEFAULT_NUM_OF_PAGES = 40;
const int FREE_FRAME = -1;
const int FILE_NAME_SIZE = 256;

/**
 * Just a note , first 128 bytes are reserved for storing header information
 * about a page.If all are integer, then 128/4 = 32 int elements can be stored in
 * the header. this has been dropped as of now
 */
const int DB_MAIN_HEADER_PAGE = 0;


const int RID_LEN = 6;// use this in memcpy and write because to take care of structure padding overwrites
#endif /* GLOBALCONSTANTS_H_ */
