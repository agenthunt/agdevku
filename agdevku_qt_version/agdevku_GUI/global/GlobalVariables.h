

#include "../executor/ExecuteStructures.h"

//parser related structure
ExecuteStructures *executeStructures;
char currentlyOpenedDatabase[256];
char executionTime[512];
char numberOfRowsReturned[512];

//cannot return error codes in constructor,
//hack to get error code from there.
int constructorErrorStatus;

bool BPLUSTREE_DEBUG = false;
bool HEAP_VERBOSE = false;
bool COMMON_DEBUG = false;
bool USING_INDEX = false;
bool  USE_CLIENT_SIDE_SORT = false;
bool USE_MMAP = false;
//Qt relate variables
int ROWS_PER_PAGE = 250;
//struct timeval start, end;
//long mtime, seconds, useconds;
